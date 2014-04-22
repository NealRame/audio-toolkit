/*
 * audio_mp3_codec.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: jux
 */

#include "audio_mp3_coder.h"
#include "audio_mp3_decoder.h"

extern "C" {
#	include <mpg123.h>
#	include <lame/lame.h>
#	include <stdio.h>
}

#include <cstring>
#include <cstdint>

#include <cmath>    // REMOVE THIS ASAP
#include <fstream>  // REMOVE THIS ASAP
#include <iostream> // REMOVE THIS ASAP
#include <sstream>  // REMOVE THIS ASAP
#include <limits>   // REMOVE THIS ASAP

#include <boost/format.hpp>
#include <boost/detail/endian.hpp>

#include "../../utils/utils_dynamic_buffer.h"
#include "../../utils/utils_static_buffer.h"

#include "../audio_buffer.h"
#include "../audio_error.h"
#include "../audio_format.h"
#include "../audio_sequence.h"
#include "../audio_const_sequence.h"

using namespace com::nealrame;
using namespace com::nealrame::audio;

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

struct decoder_data {
	
	decoder_data () {
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
		in_buffer.reserve(4096);
	}

	~decoder_data () {
		mpg123_close(handle);
		mpg123_delete(handle);
		mpg123_exit();
	}

	mpg123_handle *handle;
	utils::dynamic_buffer  in_buffer;
	utils::dynamic_buffer out_buffer;
};

size_t feed (decoder_data &mpgdata, std::ifstream &in) {
	mpgdata.in_buffer.set_length(0);
	mpgdata.in_buffer.fill(in);

	unsigned char *data = static_cast<unsigned char *>(mpgdata.in_buffer.data());
	size_t len = mpgdata.in_buffer.length();

	mpg123_feed(mpgdata.handle, data, len);

	return len;
}

audio::format read_format (decoder_data &mpgdata, std::ifstream &in) {
	long sampling_rate;
	int channel_count, encoding;

	int err = mpg123_getformat(
		mpgdata.handle, &sampling_rate, &channel_count, &encoding);

	switch (err) {
	case MPG123_OK:
		mpg123_format_none(mpgdata.handle);
		mpg123_format(mpgdata.handle, sampling_rate, channel_count, MPG123_ENC_FLOAT);
		return audio::format(channel_count, sampling_rate);

	case MPG123_NEED_MORE:
		feed(mpgdata, in);
		return read_format(mpgdata, in);

	default:
		break;
	}

	throw error(
		error::CodecUnexpectedError, 
		mpg123_strerror(mpgdata.handle)
	);
}

void read_frames (decoder_data &mpgdata, std::ifstream &in, buffer &out) {
	auto &out_buffer(mpgdata.out_buffer); // MUST BE TESTED

	mpgdata.out_buffer.reserve(mpg123_outblock(mpgdata.handle));

	size_t done;
	int err;
	// unsigned int seq_idx = 0;

	do {
		err = mpg123_read(mpgdata.handle,
			static_cast<unsigned char *>(out_buffer.data()),
			out_buffer.capacity(), &done);

		out_buffer.set_length(done);

		switch (err) {
		case MPG123_OK:
			out.append(sequence(out.format(), out_buffer));
			break;

		case MPG123_NEED_MORE:
			if (! (feed(mpgdata, in) > 0)) {
				err = MPG123_DONE;
			}
			break;

		default:
			break;
		}

	} while (err == MPG123_OK || err == MPG123_NEED_MORE);

	if (err != MPG123_DONE) {
		error::raise(error::CodecUnexpectedError,
				mpg123_strerror(mpgdata.handle));
	}
}

std::shared_ptr<buffer>
codec::MP3_decoder::decode(std::ifstream &input) const throw(error) {
	input.exceptions(std::ifstream::badbit);

	decoder_data mpgdata;
	audio::format format = read_format(mpgdata, input);
	std::shared_ptr<buffer> audio_buffer = std::make_shared<buffer>(format);
	read_frames(mpgdata, input, *audio_buffer);

	return audio_buffer;
}

//////////////////////////////////////////////////////////////////////////////
// Coder /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static
int lame_quality(enum codec::coder::quality q) {
	switch (q) {
	case codec::coder::quality::Best:
		return 0;

	case codec::coder::quality::Good:
		return 2;

	case codec::coder::quality::Acceptable: 
		return 5;

	case codec::coder::quality::Fastest:
		return 7;
	}

	return 0;
}


#if defined(DEBUG)
void coder_debug_handler (const char *, va_list ap) {
	fvprintf(stderr, "%s\n", ap);
}
void coder_message_handler (const char *, va_list ap) {
	fvprintf(stdout, "%s\n", ap);
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

	encode_data (sequence &seq, enum codec::coder::quality quality) {
		if ((gfp = lame_init()) == nullptr) {
			error::raise(error::CodecUnexpectedError);
		}

		lame_set_errorf(gfp, coder_error_handler);
		lame_set_debugf(gfp, coder_debug_handler);
		lame_set_msgf(gfp, coder_message_handler);

		format fmt = seq.format();

		lame_set_num_channels(gfp, fmt.channel_count());
		lame_set_in_samplerate(gfp, fmt.sample_rate());
		lame_set_mode(gfp, JOINT_STEREO);
		// lame_set_bWriteVbrTag(gfp, 0);
		lame_set_quality(gfp, lame_quality(quality));

		if (lame_init_params(gfp) < 0) {
			error::raise(error::CodecUnexpectedError);
		}
	}

	~encode_data () {
		lame_close(gfp);
	}

	lame_t gfp;
};

// struct RAII_MP3CoderData {
// 	std::ofstream &output;
// 	std::ofstream::iostate output_state;
// 	lame_t gfp;
// 	int mp3_output_buffer_size, mp3_input_buffer_size;
// 	char *mp3_output_buffer;
// 	float *mp3_input_buffer;

// 	RAII_MP3CoderData(const MP3Coder &coder, const Buffer &buffer, std::ofstream &out) :
// 		output(out) {

// 		output_state = output.exceptions();
// 		output.exceptions(std::ofstream::failbit | std::ofstream::badbit );

// 		if ((gfp = lame_init()) == nullptr) {
// 			Error::raise(Error::Status::MP3CodecError, "Failed to init lame encoder.");
// 		}

// 		lame_set_errorf(gfp, error_handler);
// 		lame_set_debugf(gfp, debug_handler);
// 		lame_set_msgf  (gfp, message_handler);

// 		Format format = buffer.format();

// 		mp3_output_buffer_size = 1.25*buffer.frameCount() + 7200;
// 		mp3_output_buffer = 
// 			new char[mp3_output_buffer_size];

// 		mp3_input_buffer_size = format.channelCount()*MP3_ENCODE_INPUT_BUFFER_SIZE;
// 		mp3_input_buffer = 
// 			new float[mp3_input_buffer_size];

// 		lame_set_num_channels(gfp, format.channelCount());
// 		lame_set_in_samplerate(gfp, format.sampleRate());
// 		lame_set_quality(gfp, lame_quality(coder.quality()));
// 		lame_set_bWriteVbrTag(gfp, 0);

// 		lame_init_params(gfp);
// 	}

// 	virtual ~RAII_MP3CoderData( ) {
// 		if (gfp != nullptr) {
// 			lame_close(gfp);
// 			delete[] mp3_output_buffer;
// 			delete[] mp3_input_buffer;
// 		}
// 		output.exceptions(output_state);
// 	}
// };

void codec::MP3_coder::encode(std::ofstream &, const audio::sequence &seq) const throw(error) {
	static const unsigned int input_frame_count = 1024u;

	format fmt = seq.format();

	utils::dynamic_buffer out_buffer(1.25*input_frame_count + 7200);
	unsigned int frame_index = 0;

	do {
		const void * data = seq.subsequence(frame_index, input_frame_count).raw_buffer().data();
	} while (true);



	lame_encode_buffer_interleaved_ieee_float()


}
