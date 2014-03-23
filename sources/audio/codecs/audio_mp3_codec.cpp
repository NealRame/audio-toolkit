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
			err.message = mpg123_plain_strerror(error_code);
			throw err;
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

	throw error {
		error::CodecUnexpectedError, 
		mpg123_strerror(mpgdata.handle)
	};
}

static inline sequence & process_sequence(sequence &seq, unsigned int seq_idx) {
	std::ostringstream oss;
	std::shared_ptr<audio::codec::coder> coder 
		= audio::codec::coder::get(".wav");

	// unsigned int channel_count = seq.format().channel_count();
	// float eps = 1.f/std::numeric_limits<int16_t>::max();
	// bool ok = false;
	// auto it_n0 = seq.begin();

	// for (auto it_n1 = it_n0+1, end = seq.end(); it_n1 != end; it_n0++, it_n1++) {
	// 	for (unsigned int i = 0; i < channel_count; ++i) {
	// 		float &sn0 = it_n0->sample_at(i);
	// 		float &sn1 = it_n1->sample_at(i);

	// 		if (fabs(fabs(sn1) - 1) < eps) {
	// 			std::cout << "|sn0| = " << sn0 << std::endl;
	// 			std::cout << "|sn1| = " << sn1 << std::endl;
	// 		}

	// 		ok = ok || (fabs(fabs(sn1) - 1) < eps);

	// 		if (ok && fabs(sn0 - sn1) > 1.f) {
	// 			std::cout << "-----------" << std::endl;
	// 			std::cout << "sn0: " << sn0 << std::endl;
	// 			std::cout << "sn1: " << sn1 << std::endl;
	// 			std::cout << "|sn0 - sn1|: " << fabs(sn0 - sn1) << std::endl;
	// 			std::cout << "-----------" << std::endl;
	// 			sn1 = sn0;
	// 		}
	// 	}
	// }

	unsigned int off = 0;

	if (seq_idx == 4191) {

		for (auto frame: seq) {
			std::cout << seq_idx << ":" << off++;
			for (auto sample: frame) {
				std::cout << ":" << sample;
			}
			std::cout << std::endl;
		}

		oss << "seqs/seq_#" << seq_idx << "-" << seq.frame_count() << ".wav";
		std::cout << "encode seq::" << seq_idx << "[" << seq.frame_count() << "]" << std::endl;

		coder->encode(oss.str(), seq);		
	}


	return seq;
}

void read_frames (decoder_data &mpgdata, std::ifstream &in, buffer &out) {
	auto &out_buffer(mpgdata.out_buffer); // MUST BE TESTED

	mpgdata.out_buffer.reserve(mpg123_outblock(mpgdata.handle));

	size_t done;
	int err;
	unsigned int seq_idx = 0;

	do {
		err = mpg123_read(mpgdata.handle,
			static_cast<unsigned char *>(out_buffer.data()),
			out_buffer.capacity(), &done);

		out_buffer.set_length(done);

		switch (err) {
		case MPG123_OK:
			// std::cout 
			// 	<< "       [OK] frame cout: " 
			// 	<< out.format().frame_count(done)
			// 	<< " (done = " << done << ")"
			// 	<< std::endl;
			{
				sequence seq = sequence(out.format(), out_buffer);
				out.append(process_sequence(seq, seq_idx++));
			}
			break;

		case MPG123_NEED_MORE:
			// std::cout
			// 	<< "[NEED_MORE] frame cout: " 
			// 	<< out.format().frame_count(done)
			// 	<< " (done = " << done << ")"
			// 	<< std::endl;
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

void 
codec::MP3_coder::encode(std::ofstream &, const audio::sequence &) const 
	throw(error) {

}

//////////////////////////////////////////////////////////////////////////////
// Coder /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// int lame_quality(Coder::Quality quality) {
// 	switch (quality) {
// 	case Coder::Quality::Best: return 0;
// 	case Coder::Quality::Good: return 2;
// 	case Coder::Quality::Acceptable: return 5;
// 	case Coder::Quality::Fastest: return 7;
// 	}
// 	return 0;
// }

// #define ERR_HANDLER_DEFAULT_BUFFER_SIZE 128
// #define MP3_ENCODE_INPUT_BUFFER_SIZE   1024

// void error_handler(const char *fmt, va_list ap) {
// 	char **buffer = nullptr;
// 	size_t buffer_size = ERR_HANDLER_DEFAULT_BUFFER_SIZE;
// 	bool again;
// 	do {
// 		size_t written;
// 		*buffer = (char *)realloc(*buffer, buffer_size);

// 		written = vsnprintf(*buffer, buffer_size, fmt, ap);

// 		if (written >= buffer_size) {
// 			again = true;
// 			buffer_size = written;
// 		} else {
// 			again = false;
// 		}
// 	} while (again);
// 	Error::raise(Error::Status::MP3CodecError, std::string(*buffer));
// }

// #if defined(DEBUG)
// void debug_handler(const char *, va_list ap) {
// 	fvprintf(stderr, "%s\n", ap);
// }

// void message_handler(const char *, va_list) {
// 	fvprintf(stderr, "%s\n", ap);
// }
// #else
// void debug_handler(const char *, va_list) {}
// void message_handler(const char *, va_list) {}
// #endif

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

// void MP3Coder::encode(const Buffer &buffer, std::ofstream &out) const {
// 	RAII_MP3CoderData encode_data(*this, buffer, out);

// 	unsigned int offset = 0;
// 	int nbytes;

// 	do {
// 		unsigned int count = buffer.read(offset, MP3_ENCODE_INPUT_BUFFER_SIZE, encode_data.mp3_input_buffer);

// 		nbytes = lame_encode_buffer_interleaved_ieee_float(
// 				encode_data.gfp, 
// 				encode_data.mp3_input_buffer, 
// 				count,
// 				(unsigned char *)encode_data.mp3_output_buffer,
// 				encode_data.mp3_output_buffer_size);

// 		if (nbytes >= 0) {
// 			out.write((char *)encode_data.mp3_output_buffer, nbytes);
// 		} else {
// 			Error::raise(Error::Status::MP3CodecError,
// 				(boost::format("Lame encode error code: %1%") % nbytes).str());
// 		}

// 		offset += count;
// 	} while (offset < buffer.frameCount());

// 	nbytes = lame_encode_flush(encode_data.gfp,
// 				(unsigned char *)encode_data.mp3_output_buffer,
// 				encode_data.mp3_output_buffer_size);

// 	if (nbytes >= 0) {
// 		out.write((char *)encode_data.mp3_output_buffer, nbytes);
// 	}
// }
