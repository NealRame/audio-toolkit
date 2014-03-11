#include <algorithm>
#include <iostream>

#include <audio/buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/sequence>

#include <audio/codec>

#include <fstream>
#include <stdio.h>

using namespace com::nealrame;

int main(int argc, char **argv) {
	std::shared_ptr<audio::codec::decoder> decoder = audio::codec::decoder::get_decoder(".wav");
	std::shared_ptr<audio::buffer> audio_buffer = decoder->decode("input.wav");

	audio::sequence seq = audio_buffer->sequence();
	unsigned int count = 0;

	std::cout << seq.raw_buffer().length() << std::endl;
	std::cout << seq.frame_count() << std::endl;

	std::for_each (seq.begin(), seq.end(), [&count](audio::frame f) {
		std::cout << '[' << count++ <<"]: " << *(f.begin()) << std::endl;
	});

	return 0;
}
