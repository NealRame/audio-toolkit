/*
 * AudioCoder.cpp
 *
 * Created on: March 09, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#include <boost/algorithm/string.hpp>
#include "audio_coder.h"

namespace com {
namespace nealrame {
namespace audio {
namespace codec {

std::shared_ptr<coder> coder::get_coder (const std::string &extension) 
		throw(error) {
	std::shared_ptr<coder> coder;
	std::string ext = boost::to_lower_copy(extension);
	if (ext == ".wav") {
		coder = nullptr;
	} else {
		error::raise(audio::error::status::CodecNoSuitableCoderFound);
	}
	return coder;
}

coder::coder (enum quality quality) :
	quality_(quality) {
}

coder::coder () : 
	coder(quality::Default) {
}

void coder::encode (
		const com::nealrame::audio::buffer &buffer,
		const std::string &filename) const throw(error) {
	std::ofstream ofs(filename.data(), std::ofstream::binary);
	try {
		encode(buffer, ofs);
		ofs.close();
	} catch (com::nealrame::audio::error &e) {
		ofs.close();
		throw e;
	}
}

enum coder::quality coder::quality () const {
	return quality_;
}

void coder::set_quality (enum quality quality) {
	quality = quality_;
}

} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
