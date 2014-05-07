#include <iostream>
#include <sstream>
#include <type_traits>

#include <cmath>
#include <audio/codec>
#include <audio/sequence>

#include <audio/version>

#include <audio/generators/noise>
#include <audio/generators/triangle>
#include <audio/generators/sawtooth>
#include <audio/generators/square>
#include <audio/generators/sine>

#include <audio/generator>

using namespace com::nealrame;

audio::sequence load_buffer(const std::string &filename) {
	std::string extension = filename.substr(filename.length() - 4);
	std::shared_ptr<audio::codec::decoder> decoder =
		audio::get_decoder(extension);
	return decoder->decode(filename);
}

void store_buffer(const std::string &filename, const audio::sequence &seq) {
	std::string extension = filename.substr(filename.length() - 4);
	std::shared_ptr<audio::codec::coder> coder =
		audio::get_coder(extension);
	coder->encode(filename, seq);
}

int main (int argc, char **argv) {
	std::cout << audio::version::full << std::endl;

	try {
		audio::generator<audio::generators::noise> noise(audio::format(2, 44100), 0.8);
		audio::generator<audio::generators::sawtooth> sawtooth(audio::format(2, 44100), 0., 0.8, 110.);
		audio::generator<audio::generators::sine> sine(audio::format(2, 44100), 0., 0.8, 110.);
		audio::generator<audio::generators::square> square(audio::format(2, 44100), 0., 0.8, 110.);
		audio::generator<audio::generators::triangle> triangle(audio::format(2, 44100), 0., 0.8, 110);

		store_buffer("noise.wav", noise.sequence(2.));
		store_buffer("sawtooth.wav", sawtooth.sequence(2.));
		store_buffer("sine.wav", sine.sequence(2.));
		store_buffer("square.wav", square.sequence(2.));
		store_buffer("triangle.wav", triangle.sequence(2.));
		
	} catch (audio::error &err) {
		std::cerr << err.status() << std::endl;
		std::cerr << err.what() << std::endl;
		return 1;
	}

	// try {
	// 	if (argc > 1) {
	// 		audio::sequence seq = load_buffer(argv[1]);
	// 		store_buffer("output.wav", seq);
	// 	}
	// } catch (audio::error &err) {
	// 	std::cerr << err.status() << std::endl;
	// 	std::cerr << err.what() << std::endl;
	// 	return 1;
	// }

	return 0;
}
