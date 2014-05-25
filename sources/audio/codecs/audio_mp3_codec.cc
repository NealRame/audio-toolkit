/// audio_mp3_codec.cc
/// Created on: April 26, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_mp3_coder.h"
#include "audio_mp3_decoder.h"

#include "../audio_sequence.h"
#include "../audio_format.h"

#include "../../utils/utils_buffer.h"

#include <algorithm>
#include <cstdio>
#include <functional>

extern "C" {
#	include <mpg123.h>
#	include <lame/lame.h>
}

using namespace com::nealrame;
using namespace com::nealrame::audio;
using com::nealrame::audio::codec::MP3_coder;
using com::nealrame::audio::codec::MP3_decoder;

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace mp3_ {

class mpg123_lib {
	mpg123_lib () {
		int error_code;
		if ((error_code = mpg123_init()) != MPG123_OK) {
			error::raise(error::CodecUnexpectedError,
					mpg123_plain_strerror(error_code));
		}
	}

public:
	mpg123_lib (const mpg123_lib &) = delete;
	mpg123_lib & operator= (const mpg123_lib &) = delete;

	virtual ~mpg123_lib () {
		mpg123_exit();
	}

	static mpg123_lib & instance () {
		static std::once_flag flag;
		static std::unique_ptr<mpg123_lib> instance_;
		std::call_once(flag, [] {
			instance_.reset(new mpg123_lib());
		});
		return *(instance_.get());
	}

public:
	typedef std::unique_ptr<void, std::function<void (void *)>> handle;

	handle get_handle () {
		int error_code;
		mpg123_handle * hdl;

		if ((hdl = mpg123_new(nullptr, &error_code)) == nullptr) {
			error::raise(error::CodecUnexpectedError,
					mpg123_plain_strerror(error_code));
		}

		mpg123_param(hdl, MPG123_ADD_FLAGS, MPG123_FORCE_FLOAT, 0.);

		if ((error_code = mpg123_open_feed(hdl)) != MPG123_OK) {
			error::raise(error::CodecUnexpectedError,
					mpg123_plain_strerror(error_code));
		}

		return handle(hdl, [](void *h) {
			mpg123_close(reinterpret_cast<mpg123_handle *>(h));
			mpg123_delete(reinterpret_cast<mpg123_handle *>(h));
		});
	}

	void feed (handle &h, unsigned char *data, size_t size) {
		int error_code;
		if ((error_code = mpg123_feed(
			reinterpret_cast<mpg123_handle *>(h.get()), data, size)) < 0) {
			error::raise(error::CodecUnexpectedError,
					mpg123_plain_strerror(error_code));	
		}
	}

	std::unique_ptr<format> get_format (handle &h) {
		mpg123_handle *hdl = reinterpret_cast<mpg123_handle *>(h.get());

		int channels, encoding, error_code;
		long rate;

		error_code = mpg123_getformat(
			hdl, &rate, &channels, &encoding
		);

		std::unique_ptr<format> res;

		if (error_code == MPG123_OK) {
			mpg123_format_none(hdl);
			mpg123_format(hdl, rate, channels, MPG123_ENC_FLOAT);
			res.reset(new format(channels, rate));
		} else if (error_code != MPG123_NEED_MORE) {
			throw error(error::CodecUnexpectedError,
					mpg123_plain_strerror(error_code));
		}

		return res;
	}

	size_t read (handle &h, utils::buffer &output) {
		mpg123_handle *hdl = reinterpret_cast<mpg123_handle *>(h.get());

		// resize output buffer if its internal size is lower than
		// the maximum output block size
		size_t outblock_size = mpg123_outblock(hdl);
		if (output.size() < outblock_size) {
			output.resize(outblock_size);
		}

		int error_code;
		size_t done;

		error_code = mpg123_read(
			reinterpret_cast<mpg123_handle *>(h.get()),
			output.data<unsigned char>(), output.size(), &done
		);

		if (! (error_code == MPG123_OK
			|| error_code == MPG123_NEED_MORE)) {
			error::raise(error::CodecUnexpectedError,
					mpg123_plain_strerror(error_code));
		}

		return done;
	}
};

class input_stream {
	mpg123_lib::handle handle_;

	utils::buffer input_buffer_;
	std::istream &input_;

	utils::buffer output_buffer_;
	format::size_type output_frame_count_;
	format::size_type output_frame_index_;

	std::unique_ptr<format> format_;

private:
	format::size_type available_frames_ () const {
		return std::max<format::size_type>(
				0, output_frame_count_ - output_frame_index_
		);
	}

	float * frames_ () {
		return output_buffer_.data<float>()
			+ output_frame_index_*format_->channel_count();
	}

	void feed_ (mpg123_lib &lib) {
		input_.read(input_buffer_.data<char>(), input_buffer_.size());
		lib.feed(handle_, input_buffer_.data<unsigned char>(), input_.gcount());
	}

	void read_format_ (mpg123_lib &lib) {
		while (! (format_ = lib.get_format(handle_))) {
			if (input_.eof()) {
				error::raise(error::CodecUnexpectedError,
						"end of input stream");
			}
			feed_(lib);
		}
	}

	void read_ (mpg123_lib &lib) {
		if (available_frames_() == 0) {
			if (! format_) {
				read_format_(lib);
			}

			size_t size = 0;

			while (input_.good()
				&& (size = lib.read(handle_, output_buffer_)) == 0) {
				feed_(lib);
			}

			output_frame_count_ = size/(format_->channel_count()*sizeof(float));
			output_frame_index_ = 0;
		}
	}

public:
	input_stream (std::istream &stream, size_t input_buffer_size = 8192) :
		handle_(mpg123_lib::instance().get_handle()),
		input_buffer_(input_buffer_size),
		input_(stream),
		output_frame_count_(0),
		output_frame_index_(0) {
	}

	bool eof () const {
		return input_.eof() && available_frames_() == 0;
	}

	format & get_format () {
		if (! format_) {
			read_format_(mpg123_lib::instance());
		}
		return *format_;
	}

	format::size_type read (sequence &seq, format::size_type frame_count) {
		if (seq.format() != get_format()) {
			error::raise(error::FormatMismatchedError, 
					"format of provided sequence does not match");
		}

		mpg123_lib &lib = mpg123_lib::instance();

		format::size_type remaining_frame = frame_count;

		while (! eof() && remaining_frame > 0) {
			read_(lib);

			format::size_type count =
				std::min(remaining_frame, available_frames_());

			seq.append(frames_(), count);
			remaining_frame -= count;
			output_frame_index_ += count;
		}

		return frame_count - remaining_frame;
	}

	sequence read_all () {
		sequence seq(get_format());
		format::size_type n;
		do {
			n = read(seq, 1024);
		} while (n > 0);
		return seq;
	}
};
} /* namespace mp3_ */

sequence MP3_decoder::decode_(std::istream &input) const
	throw(error) {
	mp3_::input_stream mp3_istream(input);
	return mp3_istream.read_all();
}

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace mp3_ {

class output_stream {
	std::ostream &output_;
	lame_t lame_;
	format format_;

	const format::size_type input_frame_count_;
	utils::buffer mp3_buffer_;

	static void error_handler_ (const char *fmt, va_list ap) {
		size_t msg_size = 128u;
		std::unique_ptr<char, std::function<void(char *)>> 
			msg(static_cast<char *>(malloc(msg_size)),
				[](char *data){ delete(data); });
		do {
			size_t size = vsnprintf(msg.get(), msg_size, fmt, ap);
			if (size > msg_size) {
				msg_size = size;
				msg.reset((char *)realloc(msg.release(), size));
			} else {
				error::raise(error::CodecUnexpectedError, 
						msg.get());
			}
		} while (true);
	}

#if defined(DEBUG)
	static void debug_handler_ (const char *, va_list ap) {
		vfprintf(stderr, "%s\n", ap);
	}

	static void message_handler_ (const char *, va_list ap) {
		vfprintf(stdout, "%s\n", ap);
	}
#else
	static void debug_handler_ (const char *, va_list) {}
	static void message_handler_ (const char *, va_list) {}
#endif

public:
	output_stream (std::ostream &output, const format &fmt) :
		output_(output),
		format_(fmt),
		input_frame_count_(1024),
		mp3_buffer_(5*input_frame_count_/4 + 7200)
	{
		if ((lame_ = lame_init()) == nullptr) {
			error::raise(error::CodecUnexpectedError);
		}

		lame_set_errorf(lame_, error_handler_);
		lame_set_debugf(lame_, debug_handler_);
		lame_set_msgf(lame_, message_handler_);

		lame_set_num_channels(lame_, format_.channel_count());
		lame_set_in_samplerate(lame_, format_.sample_rate());
		lame_set_mode(lame_, JOINT_STEREO);

		// lame_set_bWriteVbrTag(lame_, 0);
		// TODO: change the quality parameter ASAP
		lame_set_quality(lame_, 2);

		if (lame_init_params(lame_) < 0) {
			error::raise(error::CodecUnexpectedError);
		}
	}

	virtual ~output_stream () {
		if (lame_ != nullptr) {
			flush();
			lame_close(lame_);
		}
	}

	void write (const sequence &seq) {

		if (seq.format() != format_) {
			error::raise(error::FormatMismatchedError,
					"format of provided sequence does not match");
		}

		format::size_type total_frame_count = seq.frame_count();
		format::size_type frame_index = 0;

		while (frame_index < total_frame_count) {
			int n;
			format::size_type frame_count;

			frame_count = std::min(
				input_frame_count_,
				total_frame_count - frame_index
			);

			n = lame_encode_buffer_interleaved_ieee_float(
				lame_,
				seq.data(frame_index), frame_count,
				mp3_buffer_.data<unsigned char>(),
				mp3_buffer_.size()
			);

			if (n > 0) output_.write(mp3_buffer_.data<char>(), n);

			frame_index += frame_count;
		}
	}

	void flush () {
		int n;
		n = lame_encode_flush(
			lame_,
			mp3_buffer_.data<unsigned char>(), mp3_buffer_.size()
		);
		if (n > 0) output_.write(mp3_buffer_.data<char>(), n);
	}
};
} /* namespace mp3_ */

void MP3_coder::encode_ (std::ostream &output, const sequence &seq) const 
	throw(error) {
	mp3_::output_stream mp3_ostream(output, seq.format());
	mp3_ostream.write(seq);
}
