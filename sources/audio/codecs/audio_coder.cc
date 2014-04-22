/// audio_coder.cpp
///
/// Created on: March 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_coder.h"

using namespace com::nealrame::audio;

codec::coder::coder (enum quality quality) :
	quality_(quality) {
}

codec::coder::coder () : 
	coder(quality::Default) {
}

void codec::coder::encode (const std::string &filename, const buffer &buffer)
	const throw(error) {
	std::ofstream ofs(filename.data(), std::ofstream::binary);
	try {
		encode(ofs, buffer);
		ofs.close();
	} catch (error &e) {
		ofs.close();
		throw e;
	}
}

enum codec::coder::quality codec::coder::quality () const {
	return quality_;
}

void codec::coder::set_quality (enum quality quality) {
	quality = quality_;
}
