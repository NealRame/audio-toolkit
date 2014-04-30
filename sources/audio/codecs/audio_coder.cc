/// audio_coder.cpp
///
/// Created on: March 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_coder.h"

#include <fstream>

using namespace com::nealrame::audio;
using com::nealrame::audio::codec::coder;

void coder::encode (const std::string &filename, const buffer &buffer)
	const throw(error) {
	std::ofstream out(filename.data(), std::ofstream::binary);
	encode_(out, buffer);
}

void coder::encode (std::ostream &stream, const buffer &buffer) 
	const throw(error) {
	std::ostream out(stream.rdbuf());
	return encode_(out, buffer);
}
