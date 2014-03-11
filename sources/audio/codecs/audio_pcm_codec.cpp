/**
 * audio_pcm_coder.cpp
 *
 * Created on: March 09, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>

#include <audio/buffer>
#include <audio/frame>
#include <audio/error>

#include "audio_pcm_coder.h"
#include "audio_pcm_decoder.h"

using namespace com::nealrame;
using com::nealrame::audio::codec::PCM_decoder;

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

#if defined (DEBUG)

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
	<< "\n================================" << std::endl;                 \
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
	<< "\n================================" << std::endl;                 \
} while (0)
#define debug_wave_data_chunk(CHUNK)                                          \
do {                                                                          \
	std::cerr                                                             \
	<< "WAVE data chunk:"                                                 \
	<< "\n           size: " << (CHUNK).size                              \
	<< "\n================================" << std::endl;                 \
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
		audio::error::raise(audio::error::status::CodecFormatError);
	}
}

template <>
inline void check_data<WaveFormatChunk> (WaveFormatChunk &data) {
	debug_wave_format_chunk(data);
	if (strncmp(data.id, "fmt ", 4) != 0) {
		audio::error::raise(audio::error::status::CodecFormatError);
	}
}

template <>
inline void check_data<WaveDataChunk> (WaveDataChunk &data) {
	debug_wave_data_chunk(data);
	if (strncmp(data.id, "data", 4) != 0) {
		audio::error::raise(audio::error::status::CodecFormatError);
	}
}

template <typename T>
inline void read (std::ifstream &in, T &data) {
	in.read(reinterpret_cast<char *>(&data), sizeof(T));
	if (in.gcount() != sizeof(T)) {
		audio::error::raise(audio::error::status::CodecIOError);
	}
	check_data<T>(data);
}

template <>
inline void read (std::ifstream &in, utils::dynamic_buffer &buffer) {
	in.read(static_cast<char *>(buffer.data()), buffer.capacity());
	buffer.set_length(in.gcount());
	if (buffer.length() < buffer.capacity()) {
		audio::error::raise(audio::error::status::CodecIOError);
	}
}

template<typename T>
inline void fill_audio_buffer (
	utils::buffer &data, audio::sequence sequence, 
	unsigned int count, unsigned int offset)  {

#if defined (DEBUG)
	std::cerr << "Fill audio buffer with " << count << " frames" << std::endl;
#endif

	unsigned int channel_count = sequence.format().channel_count();
	auto sample_iterator = data.begin<T>();
	auto frame_iterator = sequence.begin() + offset;

	std::for_each(frame_iterator, frame_iterator + count, 
		[channel_count, &sample_iterator](audio::frame frame) {
		frame.write(sample_iterator, sample_iterator + channel_count);
		sample_iterator += channel_count;
	});
}

std::shared_ptr<audio::buffer> 
PCM_decoder::decode(std::ifstream &in) const throw(audio::error) {
	try {
		RIFFHeaderChunk header_chunk; read(in, header_chunk);
		WaveFormatChunk format_chunk; read(in, format_chunk);
		WaveDataChunk data_chunk;     read(in, data_chunk);

		class format format(
			format_chunk.channelCount,
			format_chunk.sampleRate);

		unsigned int frame_offset = 0;
		unsigned int remaining_frames =
			format.frame_count(
				static_cast<size_t>(data_chunk.size));

		std::shared_ptr<audio::buffer> audio_buffer =
			std::make_shared<buffer>(format, remaining_frames);

		utils::dynamic_buffer buffer;

#if defined (DEBUG)
		std::cerr << "---" << std::endl;
		std::cerr << "Frame count " << remaining_frames << std::endl;
		std::cerr << "Audio buffer length " << audio_buffer->raw_buffer().length() << std::endl;
		std::cerr << "---" << std::endl;
#endif

		while (remaining_frames > 0 && ! in.eof()) {
			unsigned int frame_count =
				std::min<unsigned int>(1024, remaining_frames);

#if defined (DEBUG)
			std::cerr << "---" << std::endl;
			std::cerr << "Buffer length " << buffer.length() << std::endl;
			std::cerr << "Read " << frame_count << " frames" << std::endl;
#endif

			buffer.reserve(format_chunk.bytePerFrame*frame_count);
			read(in, buffer);

#if defined (DEBUG)
			std::cerr << "Buffer length " << buffer.length() << std::endl;
			std::cerr << "---" << std::endl;
#endif

			switch (format_chunk.bitPerSample) {
			case 8:
				fill_audio_buffer< int8_t>(
					buffer, audio_buffer->sequence(),
					frame_count, frame_offset);
				break;

			case 16:
				fill_audio_buffer<int16_t>(
					buffer, audio_buffer->sequence(),
					frame_count, frame_offset);
				break;

			default:
				break;
			}

			frame_offset += frame_count;
			remaining_frames -= frame_count;
		}

		return audio_buffer;
	} catch (std::ifstream::failure ioerr) {
		error::raise(error::status::CodecIOError, ioerr.what());
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
// Coder /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// struct RAII_PCMCoderData {
// 	std::ofstream &output;
// 	std::ofstream::iostate output_state;

// 	RAII_PCMCoderData(std::ofstream &out) :
// 		output(out) {
// 		output_state = output.exceptions();
// 		output.exceptions(std::ifstream::failbit | std::ifstream::badbit);
// 	}

// 	virtual ~RAII_PCMCoderData() {
// 		output.exceptions(output_state);
// 	}
// };

// void PCMCoder::encode(const Buffer &buffer, std::ofstream &out) const {
// 	RIFFHeaderChunk header_chunk;

// 	try {
// 		Format fmt = buffer.format();

// 		memcpy(header_chunk.id,     "RIFF", 4);
// 		memcpy(header_chunk.format, "WAVE", 4);
// 		header_chunk.size = 4 + sizeof(WaveFormatChunk) + sizeof(WaveDataChunk) + fmt.sizeForFrameCount(buffer.frameCount());
// 		debug_riff_header_chunk(header_chunk);
// 		out.write((const char *)&header_chunk, sizeof(RIFFHeaderChunk));

// 		WaveFormatChunk wave_format;
// 		memcpy(wave_format.id, "fmt ", 4);
// 		wave_format.size = sizeof(WaveFormatChunk) - sizeof(wave_format.id) - sizeof(wave_format.size);
// 		wave_format.audioFormat = 1;
// 		wave_format.channelCount = fmt.channelCount();
// 		wave_format.sampleRate = fmt.sampleRate();
// 		wave_format.byteRate = fmt.sizeForFrameCount(fmt.sampleRate());
// 		wave_format.bytePerFrame = fmt.sizeForFrameCount(1);
// 		wave_format.bitPerSample = fmt.bitDepth();
// 		debug_wave_format_chunk(wave_format);
// 		out.write((const char *)&wave_format, sizeof(WaveFormatChunk));

// 		WaveDataChunk wave_data;
// 		memcpy(wave_data.id, "data", 4);
// 		wave_data.size = fmt.sizeForFrameCount(buffer.frameCount());
// 		debug_wave_data_chunk(wave_data);
// 		out.write((const char *)&wave_data, sizeof(WaveDataChunk));
// 		out.write((const char *)buffer.data(), fmt.sizeForFrameCount(buffer.frameCount()));
// 	} catch (std::ofstream::failure ioerr) {
// 		Error::raise(Error::Status::IOError, ioerr.what());
// 	}
// }
