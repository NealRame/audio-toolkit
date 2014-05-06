// audio_generator_engine_sine.cpp
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_sine.h"

#include <cmath>
#include <boost/math/constants/constants.hpp>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::sine;

struct sine::impl {
	impl (class format format) :
		sequence(format, format::size_type(1)) {
	}
	class sequence sequence;
	float constant;
	float step;
};

sine::sine (class format fmt, float init, float a, float f) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	t_(init),
	pimpl_(new impl(fmt)) {
	pimpl_->constant = boost::math::constants::two_pi<float>()*frequency;
	pimpl_->step = 1./fmt.sample_rate();
}

sine::~sine () {
}

sequence::const_frame sine::operator() () {
	float v = amplitude*sinf(pimpl_->constant*t_);
	sequence::frame f = pimpl_->sequence.at(0);
	t_ += pimpl_->step;
	for (float &sample: f) {
		sample = v;
	}
	return f;
}
