/*
 * audio_decoder.cpp
 *
 * Created on: March 09, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#include <boost/algorithm/string.hpp>
#include "audio_decoder.h"
#include "audio_pcm_decoder.h"

// #include "audio_MP3_decoder.h"
// #include "audio_PCM_decoder.h"
// #include "audio_OggVorbis_decoder.h"

namespace com {
namespace nealrame {
namespace audio {
namespace codec {

std::shared_ptr<decoder> decoder::get (const std::string &extension) throw(error) {
	std::shared_ptr<decoder> decoder;
	std::string ext = boost::to_lower_copy(extension);
	if (ext == ".wav") {
		decoder = std::make_shared<PCM_decoder>();
	} else {
		error::raise(audio::error::status::CodecNoSuitableDecoderFound);
	}
	return decoder;
}

std::shared_ptr<audio::buffer> decoder::decode(const std::string &filename) const throw(error) {
	std::shared_ptr<audio::buffer> buffer;
	std::ifstream in(filename, std::fstream::in|std::fstream::binary);

	in.exceptions(std::fstream::failbit|std::fstream::badbit);
	try {
		buffer = decode(in);
		in.close();
	} catch (audio::error &err) {
		in.close();
		throw err;
	}
	return buffer;
}

} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */