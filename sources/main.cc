#include <iostream>
#include <sstream>
#include <type_traits>

#include <cmath>
#include <audio/buffer>

#include <boost/math/constants/constants.hpp>

namespace com {
namespace nealrame {
namespace audio {
	std::ostream & operator<< (std::ostream &stream, audio::buffer::const_frame frame) {
		std::ostream output(stream.rdbuf());
		output.setf(
			std::ios_base::dec
				| std::ios_base::fixed
				| std::ios_base::showpoint
				| std::ios_base::showpos
				| std::ios_base::right
		);
		output.precision(8);
		for (unsigned int i = 0, count = frame.channel_count(); i < count; ++i) {
			stream << "[" << i << "]=";
			output << frame[i];
			stream << ", ";
		}
		return stream;
	}
};
};
};

using namespace com::nealrame;

const audio::format::size_type frame_rate = 44100;
const audio::format::size_type channel_count = 2;

int main (int argc, char **argv) {
	audio::buffer buf(audio::format(channel_count, frame_rate));

	std::cout << "frame count: " << buf.frame_count() << std::endl;

	auto it = buf.set_duration(1.);

	for (auto frame: buf) {
		std::cout << frame << std::endl;
	}

	float amplitude = 0.8f;
	float frequency = 440.f;
	float step = 1./frame_rate;
	float t = 0;
	float c = boost::math::constants::two_pi<float>()*frequency;

	std::for_each(it, buf.end(), [amplitude, c, step, &t](audio::buffer::frame frame) {
		std::fill(frame.begin(), frame.end(), amplitude*sinf(t*c));
		t += step;
	});

	// std::cout << "frame count: " << buf.frame_count() << std::endl;

	// for (auto frame: buf) {
	// 	std::cout << frame << std::endl;
	// }

	return 0;
}
