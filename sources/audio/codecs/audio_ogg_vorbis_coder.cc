/// audio_mp3_codec.cc
/// Created on: April 26, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_ogg_vorbis_coder.h"
#include "audio_ogg_vorbis_decoder.h"

#include <audio/buffer>
#include <audio/format>

#include <algorithm>
#include <cstdio>

extern "C" {
#       include <ogg/ogg.h>
#       include <vorbis/codec.h>
#	include <vorbis/vorbisenc.h>
}

using namespace com::nealrame::audio;
using com::nealrame::audio::codec::OGGVorbis_coder;
using com::nealrame::audio::codec::OGGVorbis_decoder;

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace ogg_vorbis__ {

class ogg_input_stream {
public:
	ogg_input_stream (std::istream &input, std::streamsize read_count = 8192) :
		input_(input),
		read_count_(read_count),
		eof_(false) {

		ogg_sync_init(&sync_);

		try {
			ogg_page page;
			
			if (! read_page_(page)) {
				error::raise(error::CodecUnexpectedError);
			}

			int stream_serial = ogg_page_serialno(&page);

			if (ogg_stream_init(&state_, stream_serial) < 0) {
				error::raise(error::CodecUnexpectedError);
			}
			
			if (ogg_stream_pagein(&state_, &page) < 0) {
				error::raise(error::CodecUnexpectedError);
			}
		} catch (error &err) {
			error::raise(err.status(), 
					"Failed to initialize Ogg input stream");
		}
	}

	virtual ~ogg_input_stream () {
		ogg_sync_clear(&sync_);
		ogg_stream_clear(&state_);
	}
	
	bool read_packet (ogg_packet &packet) {
		if (ogg_sync_check(&sync_) != 0) {
			error::raise(error::CodecUnexpectedError,
					"Failed to read Ogg packet");
		}

		// if the packet is not complete, read more page
		if (ogg_stream_packetout(&state_, &packet) != 1) {
			ogg_page page;

			if (! read_page_(page)) {
				return false;
			}

			if (ogg_stream_pagein(&state_, &page) < 0) {
				error::raise(error::CodecUnexpectedError,
						"Failed to read Ogg packet");
			}
			
			return read_packet(packet);
		}
		
		return true;
	}

	bool eof () const {
		return eof_;
	}

private:
	bool read_page_ (ogg_page &page) {
		char *buffer;
		int status;
		
		// while the page is not complete, read more data from stream
		// to fill in the page
		while ((status = ogg_sync_pageout(&sync_, &page)) == 0) {
			
			if (input_.eof()) {
				eof_ = true;
				return false;
			}
			
			buffer = ogg_sync_buffer(&sync_, read_count_);
			input_.read(buffer, read_count_);
			
			std::streamsize bytes = input_.gcount();
			
			if (ogg_sync_wrote(&sync_, bytes) < 0) {
				error::raise(error::CodecUnexpectedError,
						"Failed to read Ogg page");
			}
		}
		
		return true;
	}
	
private:
	std::istream &input_;
	std::streamsize read_count_;
	ogg_sync_state sync_;
	ogg_stream_state state_;
	bool eof_;
};

class vorbis_input_stream {
public:
	vorbis_input_stream (std::istream &input) :
		ogg_stream_(input) {

		vorbis_info_init(&state_);
		vorbis_comment_init(&comment_);

		read_header_();
	}

	virtual ~vorbis_input_stream () {
		vorbis_block_clear(&block_);
		vorbis_dsp_clear(&dsp_);
		vorbis_info_clear(&state_);
		vorbis_comment_clear(&comment_);
	}

	// Returns the audio `format` of this vorbis input stream.
	format get_format() {
		return format(state_.channels, state_.rate);
	}

	// Reads at most the specified count of frames from this input vorbis
	// stream and append them to the given `buffer`.
	void read (buffer &buf, format::size_type frame_count) {
		if (get_format() != buf.format()) {
			error::raise(error::CodecFormatError,
					"Vorbis stream format differs from buffer format");
		}
		do {
			float **pcm;
			int count = vorbis_synthesis_pcmout(&dsp_, &pcm);

			if (count > 0) {
				count = std::min<format::size_type>(count, frame_count);

				vorbis_synthesis_read(&dsp_, count);
				buf.append(pcm, count);

				frame_count -= count;
			} else {
				read_ogg_packet_();
			}
		} while (! (frame_count == 0 || eof()));
	}

	// Reads all data from this vorbis input stream to the given buffer.
	void read (buffer &buf) {
		while (! ogg_stream_.eof()) {
			read(buf, 1024);
		}
	}

	// Returns true iff there is no pcm data to be read and ogg stream has
	// reach the end of stream.
	bool eof () {
		return vorbis_synthesis_pcmout(&dsp_, nullptr) == 0
			&& ogg_stream_.eof(); 
	}

private:
	void read_header_ () {
		ogg_packet packet;
		int status;

		for (int i = 0; i < 3; ++i) {
			ogg_stream_.read_packet(packet);
			if ((status = vorbis_synthesis_headerin(&state_, &comment_, &packet)) < 0) {
				error::raise(error::CodecUnexpectedError, 
						"Failed to read vorbis header");
			}
		}

		if (vorbis_synthesis_init(&dsp_, &state_) != 0) {
			error::raise(error::CodecUnexpectedError, 
					"Vorbis internal error");
		}

		if (vorbis_block_init(&dsp_, &block_) != 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}
	}

	void read_ogg_packet_ () {
		ogg_packet packet;

		if (! ogg_stream_.read_packet(packet)) {
			return ;
		}

		int status;

		if ((status = vorbis_synthesis (&block_, &packet)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis decode error");
		}

		if ((status = vorbis_synthesis_blockin(&dsp_, &block_)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis decode error");
		}
	}

private:
	ogg_input_stream ogg_stream_;
	vorbis_info state_;
	vorbis_comment comment_;
	vorbis_dsp_state dsp_;
	vorbis_block block_;
};

}; // namespace ogg_vorbis__

buffer OGGVorbis_decoder::decode_ (std::istream &input) const throw(error) {
	ogg_vorbis__::vorbis_input_stream ov_decoder(input);

	buffer buf(ov_decoder.get_format());
	ov_decoder.read(buf);

	return buf;
}

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace ogg_vorbis__ {

class ogg_output_stream {
public:
	ogg_output_stream (std::ostream &output) :
		output_(output) {
		if (ogg_stream_init(&state_, (int)time(nullptr)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Ogg internal error");
		}
	}

	virtual ~ogg_output_stream () {
		ogg_stream_clear(&state_);
	}
	
	void write_page (ogg_page &page) {
		output_.write(reinterpret_cast<const char *>(page.header), page.header_len);
		output_.write(reinterpret_cast<const char *>(page.body), page.body_len);
	}

	void write_packet (ogg_packet &packet) {
		if (ogg_stream_packetin(&state_, &packet) < 0) {
			error::raise(error::CodecUnexpectedError, 
					"Ogg internal error");
		}

		ogg_page page;

		while (ogg_stream_pageout(&state_, &page)) {
			write_page(page);
		}
	}

	void flush () {
		ogg_page page;

		while (ogg_stream_flush(&state_, &page)) {
			write_page(page);
		}
	}

private:
	std::ostream &output_;
	ogg_stream_state state_;
};

class vorbis_output_stream {
public:
	vorbis_output_stream (std::ostream &output, const format &fmt, float quality) :
		ogg_stream_(output) {
		vorbis_info_init(&info_);

		int status;

		if ((status = vorbis_encode_init_vbr(
				&info_,
				fmt.channel_count(), fmt.sample_rate(),
				quality)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}

		if (vorbis_analysis_init(&dsp_, &info_) != 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}

		if (vorbis_block_init(&dsp_, &block_) != 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}

		vorbis_comment_init(&comment_);
		vorbis_comment_add_tag(&comment_, "ENCODER", "audio-toolkit");
		vorbis_comment_add_tag(&comment_, "VENDOR", "nealrame.com");

		encode_header_();
	}

	virtual ~vorbis_output_stream () {
		// flush vorbis stream
		encode_frames_(0);
		flush();
		// cleaning up
		vorbis_comment_clear(&comment_);
		vorbis_dsp_clear(&dsp_);
		vorbis_info_clear(&info_);
	}

	format get_format() {
		return format(info_.channels, info_.rate);
	}

	void flush () {
		ogg_stream_.flush();
	}

	void write (const buffer &buf) {
		if (get_format() != buf.format()) {
			error::raise(error::CodecFormatError,
					"Vorbis stream format differs from buffer format");
		}

		format::size_type remaining_frame_count = buf.frame_count();
		format::size_type frame_index = 0;

		while (remaining_frame_count > 0) {
			auto frame_count = std::min(remaining_frame_count, format::size_type(1024));

			buf.copy(vorbis_analysis_buffer(&dsp_, frame_count), frame_count, frame_index);
			encode_frames_(frame_count);

			remaining_frame_count -= frame_count;
			frame_index += frame_count;
		}
	}
private:
	void write_packet_ () {
		ogg_packet packet;
		int status;

		if ((status = vorbis_analysis(&block_, &packet)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}

		ogg_stream_.write_packet(packet);
	}

	void encode_header_ () {
		int status;
		ogg_packet sid, comm, code;

		// fill theses packet with:
		// - stream id -> sid
		// - comments -> comm
		// - codebook -> code
		if ((status = vorbis_analysis_headerout(
				&dsp_, &comment_, &sid, &comm, &code)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}

		// Write packet to ogg output stream

		ogg_stream_.write_packet(sid);
		ogg_stream_.write_packet(comm);
		ogg_stream_.write_packet(code);

		flush();
	}

	void encode_frames_ (format::size_type frame_count) {
		int status;

		if ((status = vorbis_analysis_wrote(&dsp_, frame_count)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}

		while ((status = vorbis_analysis_blockout(&dsp_, &block_)) > 0) {
			write_packet_();
		}

		if (status < 0) {
			error::raise(error::CodecUnexpectedError,
					"Vorbis internal error");
		}
	}

private:
	ogg_output_stream ogg_stream_;
	vorbis_block block_;
	vorbis_comment comment_;
	vorbis_dsp_state dsp_;	
	vorbis_info info_;
};

}; // namespace ogg_vorbis__

void OGGVorbis_coder::encode_ (std::ostream &output, const buffer &buffer) const
	throw(error) {
	ogg_vorbis__::vorbis_output_stream ov_coder(output, buffer.format(), 1.0);
	ov_coder.write(buffer);
}
