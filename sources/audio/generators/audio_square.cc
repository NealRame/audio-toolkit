// audio_generator_engine_square.cpp
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_square.h"
#include <audio/sequence>

#include <cmath>
#include <boost/math/constants/constants.hpp>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::square;

struct square::impl {
	impl (class format format) :
		sequence(format, format::size_type(1)) {
	}
	class sequence sequence;
	float constant;
	float step;
};

square::square (class format fmt, float init, float a, float f) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	t_(init),
	pimpl_(new impl(fmt)) {
	pimpl_->constant = boost::math::constants::two_pi<float>()*frequency;
	pimpl_->step = 1./fmt.sample_rate();
}

square::~square () {
}

sequence::const_frame square::operator() () {
	sequence::frame f = pimpl_->sequence.at(0);

	float v = sinf(pimpl_->constant*t_);
	float s = v > 0 ? amplitude : -amplitude;

	t_ += pimpl_->step;

	for (float &sample: f) {
		sample = s;
	}

	return f;
}