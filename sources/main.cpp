#include <algorithm>
#include <iostream>

#include <utils/buffer>

#include <audio/buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/sequence>

#include <audio/codec>

#include <fstream>
#include <stdio.h>

#include <type_traits>

using namespace com::nealrame;

int main(int argc, char **argv) {

	std::shared_ptr<audio::codec::decoder> decoder = audio::codec::decoder::get(".wav");
	std::shared_ptr<audio::buffer> audio_buffer = decoder->decode("input.wav");
	
	std::shared_ptr<audio::codec::coder> coder = audio::codec::coder::get(".wav");
	coder->encode("output.wav", audio_buffer->sequence());
	
	return 0;
}

