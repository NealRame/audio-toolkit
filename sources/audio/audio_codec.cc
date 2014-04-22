/// audio_codec.cpp
/// Created on: March 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
  
#include "audio_codec.h"

#include "codecs/audio_wave_coder.h"
#include "codecs/audio_wave_decoder.h"

#include <boost/algorithm/string.hpp>

namespace com {
namespace nealrame {
namespace audio {

std::shared_ptr<codec::coder> get_coder(const std::string &ext) {
	if (boost::to_lower_copy(ext) == ".wav") {
		return std::make_shared<codec::WAVE_coder>();
	}
	throw error(error::CoderNotFound);
}

std::shared_ptr<codec::decoder> get_decoder(const std::string &ext) {
	if (boost::to_lower_copy(ext) == ".wav") {
		return std::make_shared<codec::WAVE_decoder>();
	}
	throw error(error::DecoderNotFound);
}

}
}
}
