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
	impl (class format format, float t0, float c) :
		sequence(format, format::size_type(1)),
		constant(c),
		step(1./format.sample_rate()),
		t(t0)
	{ }
	class sequence sequence;
	float constant;
	float step;
	float t;
};

sine::sine (class format fmt, float init, float a, float f) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	d_(new impl(fmt, t0, boost::math::constants::two_pi<float>()*frequency))
{ }

sine::~sine ()
{ }

void sine::reset()
{ d_->t = t0; }

const sequence::frame sine::operator()()
{
	auto v = amplitude*sinf(d_->constant*d_->t);
	auto f = d_->sequence.at(0);

	for (auto &sample: f) {
		sample = v;
	}

	d_->t += d_->step;
	
	return f;
}
