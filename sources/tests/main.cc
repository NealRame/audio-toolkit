#include <iostream>
#include <iomanip>
#include <sstream>

#include <cmath>

#include <audio/codec>
#include <audio/sequence>

#include <audio_toolkit_version>

#include <audio/generator>
#include <audio/generators/noise>
#include <audio/generators/triangle>
#include <audio/generators/sawtooth>
#include <audio/generators/square>
#include <audio/generators/sine>

#include <utils/buffer>

using namespace com::nealrame;

int main (int argc, char **argv) {
	std::cout << audio::version::full << std::endl;

	// try {
		// audio::generator<audio::generators::noise> noise(audio::format(2, 44100), 0.8);
		// audio::generator<audio::generators::sawtooth> sawtooth(audio::format(2, 44100), 0., 0.8, 110.);
		audio::generator<audio::generators::sine> sine(audio::format(2, 44100), 0., 0.8, 110.);
		// audio::generator<audio::generators::square> square(audio::format(2, 44100), 0., 0.8, 110.);
		// audio::generator<audio::generators::triangle> triangle(audio::format(2, 44100), 0., 0.8, 110);

		// audio::store_buffer("noise.wav", noise.sequence(2.));
		// audio::store_buffer("sawtooth.wav", sawtooth.sequence(2.));
		audio::store_buffer("sine.mp3", sine.sequence(2.));
		audio::store_buffer("sine.ogg", sine.sequence(2.));
		// audio::store_buffer("square.wav", square.sequence(2.));
		// audio::store_buffer("triangle.wav", triangle.sequence(2.));
		
	// } catch (audio::error &err) {
	// 	std::cerr << err.status() << std::endl;
	// 	std::cerr << err.what() << std::endl;
	// 	return 1;
	// }

	// try {
	// 	if (argc > 1) {
	// 		audio::sequence seq = audio::load_buffer(argv[1]);
	// 		store_buffer("output.wav", seq);
	// 	}
	// } catch (audio::error &err) {
	// 	std::cerr << err.status() << std::endl;
	// 	std::cerr << err.what() << std::endl;
	// 	return 1;
	// }

	return 0;
}
