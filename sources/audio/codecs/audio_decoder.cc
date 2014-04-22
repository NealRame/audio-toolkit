/// audio_decoder.cc
/// 
/// Created on: April 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_decoder.h"
#include "../audio_buffer.h"

using namespace com::nealrame::audio;

buffer codec::decoder::decode (const std::string &filename) const throw(error) {
	std::ifstream in(filename, std::fstream::in|std::fstream::binary);
	in.exceptions(std::fstream::failbit|std::fstream::badbit);
	try {
		return decode(in);
	} catch (audio::error &err) {
		throw err;
	}
}