#include <iostream>
#include <sstream>
#include <type_traits>

#include <cmath>
#include <audio/codec>
#include <audio/sequence>

#include <audio/version>

#include <boost/math/constants/constants.hpp>

using namespace com::nealrame;

audio::sequence sine_generator(
	audio::format::size_type frame_rate, 
	audio::format::size_type channel_count, 
	float amplitude, float freq, double duration) {
	audio::sequence seq(audio::format(channel_count, frame_rate));

	auto it = seq.set_duration(1.);

	float step = 1./frame_rate;
	float t = 0;
	float c = boost::math::constants::two_pi<float>()*freq;

	std::for_each(it, seq.end(), [amplitude, c, step, &t](audio::sequence::frame frame) {
		std::fill(frame.begin(), frame.end(), amplitude*sinf(t*c));
		t += step;
	});

	return seq;
}

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
	// std::cout << audio::version::full << std::endl;

	// try {
	// 	audio::sequence seq =sine_generator(44100, 2, 0.9, 110, 1.);
	// 	store_buffer("output.ogg", seq);
	// } catch (audio::error &err) {
	// 	std::cerr << err.status() << std::endl;
	// 	std::cerr << err.what() << std::endl;
	// 	return 1;
	// }

	try {
		if (argc > 1) {
			audio::sequence seq = load_buffer(argv[1]);
			store_buffer("output.wav", seq);
		}
	} catch (audio::error &err) {
		std::cerr << err.status() << std::endl;
		std::cerr << err.what() << std::endl;
		return 1;
	}

	return 0;
}
