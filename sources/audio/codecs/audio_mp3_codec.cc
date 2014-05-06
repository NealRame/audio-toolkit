/// audio_mp3_codec.cc
/// Created on: April 26, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_mp3_coder.h"
#include "audio_mp3_decoder.h"

#include <audio/buffer>
#include <audio/format>

#include <algorithm>
#include <cstdio>

extern "C" {
#	include <mpg123.h>
#	include <lame/lame.h>
}

using namespace com::nealrame::audio;
using com::nealrame::audio::codec::MP3_coder;
using com::nealrame::audio::codec::MP3_decoder;

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

struct decoder_data {
	explicit decoder_data (std::istream &stream) :
		input(stream) {
		int error_code;

		try {
			if ((error_code = mpg123_init()) != MPG123_OK) {
				error::raise(error::CodecUnexpectedError);
			}
			if ((handle = mpg123_new(nullptr, &error_code)) == nullptr) {
				error::raise(error::CodecUnexpectedError);
			}
			// Force the floating point output
			mpg123_param(handle,
				MPG123_ADD_FLAGS, MPG123_FORCE_FLOAT, 0.);
			if ((error_code = mpg123_open_feed(handle)) != MPG123_OK) {
				error::raise(error::CodecUnexpectedError);
			}
		} catch (error &err) {
			error::raise(
				err.status(),
				mpg123_plain_strerror(error_code)
			);
		}
	}
	~decoder_data () {
		mpg123_close(handle);
		mpg123_delete(handle);
		mpg123_exit();
	}
	char in_buffer[4096];
	std::istream &input;
	mpg123_handle *handle;
};

size_t feed (decoder_data &mpgdata) {
	size_t len = 0;
	if (mpgdata.input.good()) {
		mpgdata.input.read(mpgdata.in_buffer, sizeof(mpgdata.in_buffer));
		len = mpgdata.input.gcount();
		mpg123_feed(mpgdata.handle, (unsigned char *)(mpgdata.in_buffer), len);
	}
	return len;
}

format read_format (decoder_data &mpgdata) {
	long sampling_rate;
	int channel_count, encoding;
	int err;

	do {
		feed(mpgdata);
		err = mpg123_getformat(
			mpgdata.handle,
			&sampling_rate, &channel_count, &encoding
		);
	} while (err == MPG123_NEED_MORE && mpgdata.input.good());

	if (mpgdata.input.eof()) {
		throw error(error::CodecFormatError);
	}

	if (mpgdata.input.bad() || mpgdata.input.fail()) {
		throw error(error::IOError);
	}

	if (err < 0) {
		throw error(
			error::CodecUnexpectedError,
			mpg123_strerror(mpgdata.handle)
		);
	}

	mpg123_format_none(mpgdata.handle);
	mpg123_format(mpgdata.handle, sampling_rate, channel_count, MPG123_ENC_FLOAT);

	return format(channel_count, sampling_rate);
}

buffer read_frames (decoder_data &mpgdata, const format &fmt) {
	int len = mpg123_length(mpgdata.handle);
	size_t outblock_size = mpg123_outblock(mpgdata.handle);

	format::size_type frame_count = 0;
	format::size_type max_outgoing_frame_count
		= outblock_size/(sizeof(float)*fmt.channel_count());

	buffer buf(fmt);

	if (len > 0) {
		buf.reserve(static_cast<format::size_type>(len));
	}
	
	int err;
	size_t done;
	
	do {
		// resize the buffer so it can receive new frames
		buf.set_frame_count(frame_count + max_outgoing_frame_count);

		err = mpg123_read(mpgdata.handle,
			(unsigned char *)(buf.data(frame_count)),
			outblock_size, &done);

		frame_count += done/(sizeof(float)*fmt.channel_count());
		buf.set_frame_count(frame_count);
		
		if (err == MPG123_NEED_MORE) {
			if (! (feed(mpgdata) > 0)) {
				err = MPG123_DONE;
			}
		}
	} while (err == MPG123_OK || err == MPG123_NEED_MORE);

	if (err != MPG123_DONE) {
		error::raise(error::CodecUnexpectedError,
				mpg123_strerror(mpgdata.handle));
	}

	return buf;
}

buffer MP3_decoder::decode_(std::istream &input) const
	throw(error) {
	decoder_data mpgdata(input);
	format fmt = read_format(mpgdata);
	return read_frames(mpgdata, fmt);
}

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#if defined(DEBUG)
void coder_debug_handler (const char *, va_list ap) {
	vfprintf(stderr, "%s\n", ap);
}
void coder_message_handler (const char *, va_list ap) {
	vfprintf(stdout, "%s\n", ap);
}
#else
void coder_debug_handler (const char *, va_list) {}
void coder_message_handler (const char *, va_list) {}
#endif

static void coder_error_handler (const char *fmt, va_list ap) {
	size_t msg_size = 128u;
	struct deleter { void operator() (char *a) { free(a); } };
	std::unique_ptr<char [], deleter> msg(static_cast<char *>(malloc(msg_size)));
	do {
		size_t size = vsnprintf(msg.get(), msg_size, fmt, ap);
		if (size > msg_size) {
			msg_size = size;
			msg.reset(static_cast<char *>(realloc(msg.get(), msg_size)));
		} else {
			error::raise(error::CodecUnexpectedError, msg.get());
		}
	} while (true);
}

struct encode_data {
	static const size_t frame_count = 1024;

	encode_data (const format &fmt) {
		if ((gfp = lame_init()) == nullptr) {
			error::raise(error::CodecUnexpectedError);
		}

		lame_set_errorf(gfp, coder_error_handler);
		lame_set_debugf(gfp, coder_debug_handler);
		lame_set_msgf(gfp, coder_message_handler);

		lame_set_num_channels(gfp, fmt.channel_count());
		lame_set_in_samplerate(gfp, fmt.sample_rate());
		lame_set_mode(gfp, JOINT_STEREO);
		// lame_set_bWriteVbrTag(gfp, 0);
		lame_set_quality(gfp, 2); // TODO: change that ASAP

		if (lame_init_params(gfp) < 0) {
			error::raise(error::CodecUnexpectedError);
		}
	}

	~encode_data () {
		lame_close(gfp);
	}

	lame_t gfp;
	unsigned char mp3buffer[5*frame_count/4 + 7200];
};

const size_t encode_data::frame_count;

void MP3_coder::encode_ (std::ostream &output, const buffer &buffer) const throw(error) {
	encode_data lamedata(buffer.format());

	format::size_type total_frame_count = buffer.frame_count();
	format::size_type frame_index = 0;

	int n;

	while (frame_index < total_frame_count) {
		format::size_type frame_count =
			std::min(encode_data::frame_count,
					total_frame_count - frame_index);

		n = lame_encode_buffer_interleaved_ieee_float(
			lamedata.gfp,
			buffer.data(frame_index), frame_count,
			lamedata.mp3buffer, sizeof(lamedata.mp3buffer)
		);

		if (n > 0) {
			output.write((char *)lamedata.mp3buffer, n);
		}

		frame_index += frame_count;
	}

	n = lame_encode_flush(
		lamedata.gfp, 
		lamedata.mp3buffer, sizeof(lamedata.mp3buffer));

	if (n > 0) {
		output.write((char *)lamedata.mp3buffer, n);
	}
}
