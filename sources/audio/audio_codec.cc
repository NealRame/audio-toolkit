/// audio_codec.cpp
/// Created on: March 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
  
#include "audio_codec.h"

#include <boost/algorithm/string.hpp>

// Wave PCM codec
#include "codecs/audio_wave_coder.h"
#include "codecs/audio_wave_decoder.h"

// MP3 codec
#include "codecs/audio_mp3_coder.h"
#include "codecs/audio_mp3_decoder.h"

// Ogg Vorbis codec
#include "codecs/audio_ogg_vorbis_coder.h"

namespace com {
namespace nealrame {
namespace audio {

std::shared_ptr<codec::coder> get_coder(const std::string &ext) {
	std::string extension = boost::to_lower_copy(ext);

	if (extension == ".wav") {
		return std::make_shared<codec::WAVE_coder>();
	}
	if (extension == ".mp3") {
		return std::make_shared<codec::MP3_coder>();
	}
	if (extension == ".ogg") {
		return std::make_shared<codec::OGGVorbis_coder>();
	}

	throw error(error::CoderNotFound);
}

std::shared_ptr<codec::decoder> get_decoder(const std::string &ext) {
	std::string extension = boost::to_lower_copy(ext);

	if (extension == ".wav") {
		return std::make_shared<codec::WAVE_decoder>();
	}
	if (extension == ".mp3") {
		return std::make_shared<codec::MP3_decoder>();
	}

	throw error(error::DecoderNotFound);
}

}
}
}
