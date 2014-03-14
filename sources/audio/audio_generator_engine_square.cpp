/*
 * audio_generator_engine_square.cpp
 *
 * Created on: March 14, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#include "audio_generator_engine_square.h"

#include <cmath>

#include <boost/math/constants/constants.hpp>

#include <audio/buffer>

using namespace com::nealrame::audio;

struct square::impl {
	impl (class format format) :
		buffer(format, 1u) {
	}
	class buffer buffer;
	float constant;
	float step;
};

square::square (class format fmt, float amplitude, float frequency) :
	format(fmt),
	amplitude(std::fabs(std::fmin(amplitude, 1.))),
	t(0),
	pimpl_(new impl(fmt)) {
	pimpl_->constant = boost::math::constants::two_pi<float>()*frequency;
	pimpl_->step = 1./format.sample_rate();
}

square::~square () {
}

const_frame square::frame () {
	class frame f = pimpl_->buffer.sequence().frame_at(0);

	float v = sinf(pimpl_->constant*t);
	float s = v > 0 ? amplitude : -amplitude;

	t += pimpl_->step;

	for (float &sample: f) {
		sample = s;
	}

	return f;
}
