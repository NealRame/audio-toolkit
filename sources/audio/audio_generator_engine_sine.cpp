/*
 * audio_generator_engine_sine.cpp
 *
 * Created on: March 14, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#include "audio_generator_engine_sine.h"

#include <cmath>

#include <boost/math/constants/constants.hpp>

#include <audio/buffer>

using namespace com::nealrame::audio;

struct sine::impl {
	impl (class format format) :
		buffer(format, 1u) {
	}
	class buffer buffer;
	float constant;
	float step;
};

sine::sine (class format fmt, float amplitude, float frequency) :
	format(fmt),
	amplitude(amplitude),
	t(0),
	pimpl_(new impl(fmt)) {
	pimpl_->constant = boost::math::constants::two_pi<float>()*frequency;
	pimpl_->step = 1./format.sample_rate();
}

sine::~sine () {
}

const_frame sine::frame () {
	class frame f = pimpl_->buffer.sequence().frame_at(0);

	float v = amplitude*sinf(pimpl_->constant*t);
	t += pimpl_->step;

	for (float &sample: f) {
		sample = v;
	}

	return f;
}
