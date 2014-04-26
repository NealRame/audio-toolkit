#include <iostream>
#include <sstream>
#include <type_traits>

#include <cmath>
#include <audio/codec>
#include <audio/buffer>

#include <audio/version>

#include <boost/math/constants/constants.hpp>

using namespace com::nealrame;

audio::buffer sine_generator(
	audio::format::size_type frame_rate, 
	audio::format::size_type channel_count, 
	float amplitude, float freq, double duration) {
	audio::buffer buf(audio::format(channel_count, frame_rate));

	auto it = buf.set_duration(1.);

	float step = 1./frame_rate;
	float t = 0;
	float c = boost::math::constants::two_pi<float>()*freq;

	std::for_each(it, buf.end(), [amplitude, c, step, &t](audio::buffer::frame frame) {
		std::fill(frame.begin(), frame.end(), amplitude*sinf(t*c));
		t += step;
	});

	return buf;
}

audio::buffer load_buffer(const std::string &filename) {
	std::shared_ptr<audio::codec::decoder> decoder = audio::get_decoder(".wav");
	return decoder->decode(filename);
}

void store_buffer(const std::string &filename, const audio::buffer &audio_buffer) {
	std::shared_ptr<audio::codec::coder> coder = audio::get_coder(".wav");
	coder->encode(filename, audio_buffer);
}

int main (int argc, char **argv) {

	std::cout << audio::version::full << std::endl;

	try {
		audio::buffer buf =sine_generator(44100, 2, 0.9, 110, 1.);
		store_buffer("output.wav", buf);
	} catch (audio::error &err) {
		std::cerr << err.status() << std::endl;
		std::cerr << err.what() << std::endl;
	}
	return 0;
}