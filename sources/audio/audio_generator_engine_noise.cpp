/*
 * audio_generator_engine_noise.cpp
 *
 * Created on: March 14, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#include "audio_generator_engine_noise.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <audio/buffer>

using namespace com::nealrame::audio;

struct noise::impl {
	impl (class format format) :
		buffer(format, 1u),
		random(boost::minstd_rand(), boost::uniform_real<>()) {

	}
	class buffer buffer;
	boost::variate_generator<boost::minstd_rand, boost::uniform_real<>> random;
};

noise::noise (class format fmt, float amplitude) :
	format(fmt),
	amplitude(amplitude),
	pimpl_(new impl(format)) {
}

noise::~noise() {
}

const_frame noise::frame() {
	class frame f = pimpl_->buffer.sequence().frame_at(0);
	for (auto &sample: f) {
		sample = amplitude*pimpl_->random();
	}
	return f;
}
