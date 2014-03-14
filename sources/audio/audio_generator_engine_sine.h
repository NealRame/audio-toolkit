/*
 * audio_error.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_SINE_GENERATOR_ENGINE_H_
#define AUDIO_SINE_GENERATOR_ENGINE_H_

#include <cmath>

#include <boost/math/constants/constants.hpp>

#include <audio/format>
#include <audio/frame>
#include <audio/buffer>

#include <iostream>

namespace com {
namespace nealrame {
namespace audio {
struct sine_engine {
	sine_engine (class format fmt, float amplitude, float frequency) :
		format(fmt),
		buffer(fmt, 1/frequency) {

		using namespace boost::math;

		unsigned int sample_rate = fmt.sample_rate();
		unsigned int frame_index = 0;
		float c = constants::two_pi<float>()*frequency/sample_rate;

		for (auto frame: buffer.sequence()) {
			float v = amplitude*sinf(frame_index*c);
			for (float &sample: frame) {
				sample = v;
			}
			frame_index++;
		}
		
		itr_ = buffer.sequence().cbegin();
		end_ = buffer.sequence().cend();
	}
	const_frame frame() {
		const_frame f = *itr_++;
		if (itr_ == end_) {
			itr_ = buffer.sequence().cbegin();
		}
		return f;
	}
	class format format;
	class buffer buffer;
private:
	sequence::const_iterator itr_;
	sequence::const_iterator end_;
};
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_SINE_GENERATOR_ENGINE_H_ */