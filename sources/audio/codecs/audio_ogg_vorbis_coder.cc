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
	ogg_input_stream (std::istream &input) :
		input_(input) {

		ogg_sync_init(&sync_);

		try {
			ogg_page page;
			read_page(page);

			int stream_serial = ogg_page_serialno(&page);

			if (ogg_stream_init(&state_, stream_serial) < 0) {
				error::raise(error::CodecUnexpectedError);
			}
			if (ogg_stream_pagein(&state_, &page) < 0) {
				error::raise(error::CodecUnexpectedError);
			}
		} catch (error &err) {
			error::raise(err.status(), 
					"Failed to init Ogg input stream");
		}
	}

	virtual ~ogg_input_stream () {
		ogg_sync_clear(&sync_);
		ogg_stream_clear(&state_);
	}

	void read_page (ogg_page &page) {
		char *buffer;
		int status;

		// while the page is not complete, read more data from stream
		// to fill in the page
		while ((status = ogg_sync_pageout(&sync_, &page)) == 0) {
			buffer = ogg_sync_buffer(&sync_, 8192);
			input_.read(buffer, 8192);

			std::streamsize bytes = input_.gcount();

			if (! (bytes > 0 
				&& ogg_sync_wrote(&sync_, bytes) == 0)) {
				error::raise(error::CodecUnexpectedError,
						"Failed to read Ogg page");
			}
		}

	}

	void read_packet (ogg_packet &packet) {
		if (ogg_sync_check(&sync_) != 0) {
			error::raise(error::CodecUnexpectedError,
					"Failed to read Ogg packet");
		}

		// if the packet is not complete, read more page
		if (ogg_stream_packetout(&state_, &packet) != 1) {
			ogg_page page;

			read_page(page);

			if (ogg_stream_pagein(&state_, &page) < 0) {
				error::raise(error::CodecUnexpectedError, 
						"Failed to read Ogg packet");
			}

			read_packet(packet);
		}
	}

private:
	std::istream &input_;
	ogg_sync_state sync_;
	ogg_stream_state state_;
};

// class decoder {
// public:
// 	decoder () {

// 	}
// private:
// 	vorbis_info v_state_;
// 	vorbis_comment v_comment_;
// 	vorbis_dsp_state v_dsp_;
// 	vorbis_block v_block_;
// };

}; // namespace ogg_vorbis__

buffer OGGVorbis_decoder::decode_ (std::istream &) const throw(error) {
	throw error(error::CodecUnexpectedError, "not implemented");
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
private:
	std::ostream &output_;
	ogg_stream_state state_;
};

class coder {
public:
	coder (const format &fmt) {
		if (ogg_stream_init(&state_, (int)time(nullptr)) < 0) {
			error::raise(error::CodecUnexpectedError,
					"Ogg internal error");
		}

		vorbis_info_init(&info_);

		int status;

		if ((status = vorbis_encode_init_vbr(&info_, 
				fmt.channel_count(), fmt.sample_rate(), 1.0)) < 0) {
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
	}

	virtual ~coder () {
		vorbis_comment_clear(&comment_);
		vorbis_dsp_clear(&dsp_);
		vorbis_info_clear(&info_);
		ogg_stream_clear(&state_);
	}

	void write_ogg_page (std::ostream &, ogg_page &);
	void write_packets (std::ostream &);
	void flush (std::ostream &);
	void encode_header (std::ostream &);
	void encode_frames (std::ostream &, buffer::const_frame_iterator, format::size_type);
	void encode_frames (std::ostream &, format::size_type);
private:
	ogg_stream_state state_;
	vorbis_block block_;
	vorbis_comment comment_;
	vorbis_dsp_state dsp_;	
	vorbis_info info_;
};

void coder::write_ogg_page (std::ostream &output, ogg_page &page) {
	output.write((const char *)page.header, page.header_len);
	output.write((const char *)page.body,   page.body_len);
}

void coder::write_packets (std::ostream &output) {
	ogg_packet packet;
	ogg_page page;
	int status;

	if ((status = vorbis_analysis(&block_, &packet)) < 0) {
		error::raise(error::CodecUnexpectedError,
				"Vorbis internal error");
	}

	if ((status = ogg_stream_packetin(&state_, &packet)) < 0) {
		error::raise(error::CodecUnexpectedError, 
				"Ogg internal error");
	}

	while (ogg_stream_pageout(&state_, &page)) {
		write_ogg_page(output, page);
	}
}

void coder::flush (std::ostream &output) {
	ogg_page page;
	while (ogg_stream_flush(&state_, &page)) {
		write_ogg_page(output, page);
	}
}

void coder::encode_header (std::ostream &output) {
	ogg_packet sid, comm, code;
	int status;
	
	// fill theses packet with:
	// - stream id -> sid
	// - comments -> comm
	// - codebook -> code
	if ((status = vorbis_analysis_headerout(&dsp_, &comment_, &sid, &comm, &code)) < 0) {
		error::raise(error::CodecUnexpectedError,
				"Vorbis internal error");
	}

	if ((status = ogg_stream_packetin(&state_, &sid)) < 0) {
		error::raise(error::CodecUnexpectedError, 
				"Ogg internal error");
	}

	if ((status = ogg_stream_packetin(&state_, &comm)) < 0) {
		error::raise(error::CodecUnexpectedError, 
				"Ogg internal error");
	}
	
	if ((status = ogg_stream_packetin(&state_, &code)) < 0) {
		error::raise(error::CodecUnexpectedError,
				"Ogg internal error");
	}

	flush(output);
}

void coder::encode_frames (std::ostream &output, buffer::const_frame_iterator it, format::size_type frame_count) {
	float **samples = vorbis_analysis_buffer(&dsp_, frame_count);

	for (format::size_type j = 0; j < frame_count; ++j, ++it) {
		for (int i = 0; i < it->channel_count(); ++i) {
			samples[i][j] = it->at(i);
		}
	}
	
	encode_frames(output, frame_count);
}

void coder::encode_frames (std::ostream &output, format::size_type frame_count) {
	int status;

	if ((status = vorbis_analysis_wrote(&dsp_, frame_count)) < 0) {
		error::raise(error::CodecUnexpectedError,
				"Vorbis internal error");
	}

	while ((status = vorbis_analysis_blockout(&dsp_, &block_)) > 0) {
		write_packets(output);
	}

	if (status < 0) {
		error::raise(error::CodecUnexpectedError,
				"Vorbis internal error");
	}	
}

}; // namespace ogg_vorbis__

void OGGVorbis_coder::encode_ (std::ostream &output, const buffer &buffer) const
	throw(error)
{
	ogg_vorbis__::coder ov_coder(buffer.format());
	
	ov_coder.encode_header(output);
	
	auto it = buffer.begin(), end = buffer.end();
	while (it != end) {
		format::size_type frame_count =
			std::min<ptrdiff_t>(1024, end - it);
		
		ov_coder.encode_frames(output, it, frame_count);
		it += frame_count;
	}
	
	ov_coder.encode_frames(output, 0);
	ov_coder.flush(output);
}
