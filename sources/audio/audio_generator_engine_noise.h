/*
 * audio_error.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_NOISE_GENERATOR_ENGINE_H_
#define AUDIO_NOISE_GENERATOR_ENGINE_H_

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <audio/format>
#include <audio/frame>
#include <audio/buffer>

namespace com {
namespace nealrame {
namespace audio {
struct noise_engine {
	noise_engine (class format fmt) :
		format(fmt),
		buffer(fmt, 1U),
		uni(rand, uni_dist) {
	}
	const_frame frame() {
		class frame f = buffer.sequence().frame_at(0);
		for (auto &sample: f) {
			sample = uni();
		}
		return f;
	}
	class format format;
	class buffer buffer;
	boost::minstd_rand rand;
	boost::uniform_real<> uni_dist;
	boost::variate_generator<boost::minstd_rand&, boost::uniform_real<>> uni;
};
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_NOISE_GENERATOR_ENGINE_H_ */