/// audio_coder.cpp
///
/// Created on: March 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_coder.h"

#include <fstream>

using namespace com::nealrame::audio;
using com::nealrame::audio::codec::coder;

void coder::encode (const std::string &filename, const sequence &seq)
	const throw(error) {
	std::ofstream out(filename.data(), std::ofstream::binary);
	encode_(out, seq);
}

void coder::encode (std::ostream &stream, const sequence &seq) 
	const throw(error) {
	std::ostream out(stream.rdbuf());
	return encode_(out, seq);
}
