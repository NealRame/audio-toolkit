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
	int status = 0;
	try {

		// std::shared_ptr<audio::codec::decoder> decoder 
		// 	= audio::codec::decoder::get(".wav");

		// std::shared_ptr<audio::buffer> audio_buffer 
		// 	= decoder->decode("input.wav");

		std::shared_ptr<audio::codec::decoder> decoder 
			= audio::codec::decoder::get(".mp3");

		std::shared_ptr<audio::codec::coder> coder
			= audio::codec::coder::get(".wav");

		std::shared_ptr<audio::buffer> audio_buffer 
			= decoder->decode("input2.mp3");
		coder->encode("output.wav", audio_buffer->sequence());

		audio::format format(2, 44100);

		audio::generator<audio::noise> noise_generator(format, 0.5);
		audio::generator<audio::sine> sine_generator(format, 1.f, 440.);
		audio::generator<audio::square> square_generator(format, 1.f, 440.);
		audio::generator<audio::sawtooth> sawtooth_generator(format, 1.f, 440.);
		audio::generator<audio::triangle> triangle_generator(format, 1.f, 440.);

		std::shared_ptr<audio::buffer> audio_noise_buffer
			= noise_generator.buffer(2.);

		std::shared_ptr<audio::buffer> audio_sine_buffer 
			= sine_generator.buffer(2.);

		std::shared_ptr<audio::buffer> audio_square_buffer 
			= square_generator.buffer(2.);

		std::shared_ptr<audio::buffer> audio_sawtooth_buffer
			= sawtooth_generator.buffer(2.);

		std::shared_ptr<audio::buffer> audio_triangle_buffer
			= triangle_generator.buffer(2.);

		coder->encode("noise.wav", audio_noise_buffer->sequence());
		coder->encode("sine.wav", audio_sine_buffer->sequence());
		coder->encode("square.wav", audio_square_buffer->sequence());
		coder->encode("sawtooth.wav", audio_sawtooth_buffer->sequence());
		coder->encode("triangle.wav", audio_triangle_buffer->sequence());

	} catch (audio::error aerr) {
		status = 1;
		std::cerr << aerr.message << std::endl;
	}

	return status;
}
