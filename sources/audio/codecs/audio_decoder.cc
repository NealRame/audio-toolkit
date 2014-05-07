/// audio_decoder.cc
/// 
/// Created on: April 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include <fstream>

#include "audio_decoder.h"
#include "../audio_sequence.h"

using namespace com::nealrame::audio;

sequence codec::decoder::decode (const std::string &filename) const throw(error) {
	std::ifstream in(filename, std::fstream::in|std::fstream::binary);
	return decode_(in);
}

sequence codec::decoder::decode (std::istream &stream) const throw(error) {
	std::istream in(stream.rdbuf());
	return decode_(in);
}
