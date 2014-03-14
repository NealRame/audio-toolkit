#include <algorithm>
#include <iostream>

#include <utils/buffer>

#include <audio/buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/generator>
#include <audio/sequence>

#include <audio/codec>

using namespace com::nealrame;

int main(int argc, char **argv) {

	// std::shared_ptr<audio::codec::decoder> decoder = audio::codec::decoder::get(".wav");
	// std::shared_ptr<audio::buffer> audio_buffer = decoder->decode("input.wav");
	
	audio::format format(2, 44100);
	// audio::generator<audio::noise_engine> noise(format);
	audio::generator<audio::sine_engine> sine(format, 0.5, 55.);

	// std::shared_ptr<audio::buffer> audio_buffer = noise.buffer(2.);
	std::shared_ptr<audio::buffer> audio_buffer = sine.buffer(2.);

	std::shared_ptr<audio::codec::coder> coder = audio::codec::coder::get(".wav");
	coder->encode("output.wav", audio_buffer->sequence());
	
	return 0;
}
