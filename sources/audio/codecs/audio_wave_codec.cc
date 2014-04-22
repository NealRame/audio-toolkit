/// audio_wave_coder.cpp
/// Created on: March 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_wave_coder.h"
#include "audio_wave_decoder.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

#include <audio/buffer>
#include <audio/error>
#include <audio/sample>

#if defined(DEBUG)
#	include <iostream>
#	include <iomanip>
#	include "../debug/audio_debug.h"
#	if defined(DEBUG_WAVE_CODEC)
#		defined DEBUG_WAVE_CODER
#		defined DEBUG_WAVE_DECODER
#	endif
#endif

using namespace com::nealrame::audio;
using com::nealrame::audio::codec::WAVE_coder;
using com::nealrame::audio::codec::WAVE_decoder;

struct RIFFHeaderChunk {
	char id[4];
	uint32_t size;
	char format[4];
} __attribute__((packed));

struct WaveFormatChunk {
	char id[4];
	uint32_t size;
	uint16_t audioFormat;
	uint16_t channelCount;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t bytePerFrame;
	uint16_t bitPerSample;
} __attribute__((packed));

struct WaveDataChunk {
	char id[4];
	uint32_t size;
} __attribute__((packed));

#if defined (DEBUG) && (defined(DEBUG_WAVE_CODER)||defined(DEBUG_WAVE_CODER))

template<int N>
void debug_array_of_char(char array[N]) {
	for (int i=0; i<N; ++i) {
		std::cerr << array[i];
	}
}

#define debug_riff_header_chunk(CHUNK)                                        \
do {                                                                          \
	std::cerr                                                             \
	<< "RIFF header chunk:"                                               \
	<< "\n             id: "; debug_array_of_char<4>((CHUNK).id);         \
	std::cerr                                                             \
	<< "\n           size: " << (CHUNK).size                              \
	<< "\n         format: "; debug_array_of_char<4>((CHUNK).format);     \
	std::cerr                                                             \
	<< "\n----" << std::endl;                                             \
} while (0)
#define debug_wave_format_chunk(CHUNK)                                        \
do {                                                                          \
	std::cerr                                                             \
	<< "WAVE format chunk:"                                               \
	<< "\n           size: " << (CHUNK).size                              \
	<< "\n   audio format: " << (CHUNK).audioFormat                       \
	<< "\n  channel count: " << (CHUNK).channelCount                      \
	<< "\n    sample rate: " << (CHUNK).sampleRate                        \
	<< "\n      byte rate: " << (CHUNK).byteRate                          \
	<< "\n byte per frame: " << (CHUNK).bytePerFrame                      \
	<< "\n bit per sample: " << (CHUNK).bitPerSample                      \
	<< "\n----" << std::endl;                                             \
} while (0)
#define debug_wave_data_chunk(CHUNK)                                          \
do {                                                                          \
	std::cerr                                                             \
	<< "WAVE data chunk:"                                                 \
	<< "\n           size: " << (CHUNK).size                              \
	<< "\n----" << std::endl;                                             \
} while (0)
#else
#define debug_riff_header_chunk(...)
#define debug_wave_format_chunk(...)
#define debug_wave_data_chunk(...)
#endif

//////////////////////////////////////////////////////////////////////////////
// Decoder ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void check_data (T &data);

template <>
inline void check_data<RIFFHeaderChunk> (RIFFHeaderChunk &data) {
	debug_riff_header_chunk(data);
	if (strncmp(data.id, "RIFF", 4) != 0
		&& strncpy(data.format, "WAVE", 4) != 0) {
		error::raise(error::CodecFormatError);
	}
}

template <>
inline void check_data<WaveFormatChunk> (WaveFormatChunk &data) {
	debug_wave_format_chunk(data);
	if (strncmp(data.id, "fmt ", 4) != 0) {
		error::raise(error::CodecFormatError);
	}
}

template <>
inline void check_data<WaveDataChunk> (WaveDataChunk &data) {
	debug_wave_data_chunk(data);
	if (strncmp(data.id, "data", 4) != 0) {
		error::raise(error::CodecFormatError);
	}
}

template <typename T>
inline void read (std::ifstream &in, T &data) {
	in.read(reinterpret_cast<char *>(&data), sizeof(T));
	if (in.gcount() != sizeof(T)) {
		error::raise(error::IOError);
	}
	check_data<T>(data);
}

template <typename T>
inline void read (std::ifstream &in, std::vector<T> &pcm_buf) {
#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "pre-read buffer size is " << pcm_buf.size() << " sample(s)" << std::endl;
#endif
	in.read(reinterpret_cast<char *>(pcm_buf.data()), sizeof(T)*pcm_buf.size());
	pcm_buf.resize(in.gcount()/sizeof(T));
#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "post-read buffer size is " << pcm_buf.size() << " sample(s)" << std::endl;
#endif
}

template <typename T>
inline 
format::size_type fill_audio_buffer (const std::vector<T> &pcm_buf, buffer &buffer) {
	format::size_type channel_count = buffer.format().channel_count();
	format::size_type frame_count = pcm_buf.size()/channel_count;
	format::size_type total_frame_count = buffer.frame_count() + frame_count;

	auto it = pcm_buf.begin();
	std::for_each(
		buffer.set_frame_count(total_frame_count), buffer.end(),
		[&it](buffer::frame frame) {
#if defined(DEBUG) && defined(DEBUG_WAVE_DECODER)
			std::cerr << "[0]=" << *it << ", [1]=" << *(it+1) << std::endl;
#endif
			for (float &sample: frame) {
				sample = value_to_sample<T>(*it++);
			}
#if defined(DEBUG) && defined(DEBUG_WAVE_DECODER)
			std::cerr << frame << std::endl;
#endif
		}
	);

#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "got " << frame_count << " audio frame(s)"<< std::endl;
#endif

	return frame_count;
}

template <typename T>
void read_data_chunk (std::ifstream &in, buffer &audio_buffer) {
	format::size_type channel_count = audio_buffer.format().channel_count();
	format::size_type frame_count = audio_buffer.capacity();

	std::vector<T> pcm_buffer;
	pcm_buffer.reserve(1024*channel_count);

#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "<read data chunk sizeof(sample)=" << sizeof(T) << ">" << std::endl;
#endif

	while (frame_count > 0 && ! in.eof()) {
#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "--------" << std::endl;
	std::cerr << "audio frame remaining " << frame_count << std::endl;
#endif
		format::size_type n = std::min<format::size_type>(frame_count, 1024);
#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "will read " << n << " audio frame(s)"<< std::endl;
#endif
		pcm_buffer.resize(n*channel_count);
		read(in, pcm_buffer);

		frame_count -= fill_audio_buffer<T>(pcm_buffer, audio_buffer);
#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
	std::cerr << "--------" << std::endl;
#endif
	}
}

buffer
WAVE_decoder::decode(std::ifstream &in) const throw(error) {
	try {
		RIFFHeaderChunk header_chunk;
		read(in, header_chunk);
		
		WaveFormatChunk format_chunk;
		read(in, format_chunk);
		
		WaveDataChunk data_chunk;
		read(in, data_chunk);

		format::size_type frame_count =
			data_chunk.size/format_chunk.bytePerFrame;

#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
		std::cerr << "audio frame count: " << frame_count << std::endl;
#endif

		buffer audio_buffer(format(
			format_chunk.channelCount,
			format_chunk.sampleRate));

#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
		std::cerr << "audio buffer created!" << std::endl;
#endif

		audio_buffer.reserve(frame_count);

		switch (format_chunk.bitPerSample) {
		case 8:
			read_data_chunk<int8_t>(in, audio_buffer);
			break;

		case 16:
#if defined (DEBUG) && defined(DEBUG_WAVE_DECODER)
		std::cerr << "16 bit per sample!" << std::endl;
#endif
			read_data_chunk<int16_t>(in, audio_buffer);
			break;

		default:
			throw error(error::FormatUnhandledSampleQuantificationValueError);
		}

		return audio_buffer;
	} catch (std::ifstream::failure ioerr) {
		throw error(error::IOError, ioerr.what());
	}
}

//////////////////////////////////////////////////////////////////////////////
// Coder /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void write (std::ofstream &, const buffer &);

template <>
inline void write<RIFFHeaderChunk> (std::ofstream &out, const buffer &audio_buffer) {
	format format = audio_buffer.format();

	unsigned int frame_count = audio_buffer.frame_count();
	unsigned int channel_count = format.channel_count();

	RIFFHeaderChunk chunk;

	memcpy(chunk.id,     "RIFF", 4);
	memcpy(chunk.format, "WAVE", 4);
	chunk.size = 4
		+ sizeof(WaveFormatChunk)
		+ sizeof(WaveDataChunk)
		+ (frame_count*channel_count*sizeof(int16_t));

	debug_riff_header_chunk(chunk);

	out.write(reinterpret_cast<char *>(&chunk), sizeof(RIFFHeaderChunk));
}

template <>
inline void write<WaveFormatChunk> (std::ofstream &out, const buffer &audio_buffer) {
	format format = audio_buffer.format();

	unsigned int channel_count = format.channel_count();
	unsigned int sample_rate = format.sample_rate();

	WaveFormatChunk chunk;
	memcpy(chunk.id, "fmt ", 4);
	chunk.size = sizeof(WaveFormatChunk) 
		- sizeof(chunk.id)
		- sizeof(chunk.size);
	chunk.audioFormat = 1;
	chunk.channelCount = channel_count;
	chunk.sampleRate = sample_rate;
	chunk.byteRate = channel_count*sample_rate*sizeof(int16_t);
	chunk.bytePerFrame = channel_count*sizeof(int16_t);
	chunk.bitPerSample = 8*sizeof(int16_t);

	debug_wave_format_chunk(chunk);

	out.write(reinterpret_cast<char *>(&chunk), sizeof(WaveFormatChunk));
}

template <>
inline void write<WaveDataChunk> (std::ofstream &out, const buffer &audio_buffer) {
	format format = audio_buffer.format();

	unsigned int frame_count = audio_buffer.frame_count();
	unsigned int channel_count = format.channel_count();

#if defined (DEBUG) && defined(DEBUG_WAVE_CODER)
	unsigned int frame_index = 0;
#endif

	WaveDataChunk chunk;
	memcpy(chunk.id, "data", 4);
	chunk.size = frame_count*channel_count*sizeof(int16_t);

	debug_wave_data_chunk(chunk);

	out.write(reinterpret_cast<char *>(&chunk), sizeof(WaveDataChunk));

	for (auto frame: audio_buffer) {

#if defined (DEBUG) && defined(DEBUG_WAVE_CODER)
		std::cerr << std::setw(6) << frame_index++ << ":" << frame << std::endl;
#endif
		// I wish i could use an input iterator here ...
		for (float sample: frame) {
			int16_t value = sample_to_value<int16_t>(sample);
			out.write(reinterpret_cast<char *>(&value), sizeof(value));
		}
#if defined (DEBUG) && defined(DEBUG_WAVE_CODEC)
		std::cerr << std::endl;
#endif
	}
}

void WAVE_coder::encode(std::ofstream &out, const buffer &buffer) const throw(error) {
	out.exceptions(std::ofstream::failbit|std::ofstream::badbit);

	try {
		write<RIFFHeaderChunk>(out, buffer);
		write<WaveFormatChunk>(out, buffer);
		write<WaveDataChunk>  (out, buffer);
	} catch (std::ofstream::failure ioerr) {
		error::raise(error::IOError, ioerr.what());
	}

}
