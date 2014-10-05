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
	impl(class format format, float t0, float c) :
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

square::square(class format fmt, float init, float a, float f) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	d_(new impl(fmt, t0, boost::math::constants::two_pi<float>()*frequency))
{ }

square::~square()
{ }

const sequence::frame square::operator()()
{
	auto f = d_->sequence.at(0);
	auto v = sinf(d_->constant*d_->t);
	auto s = v > 0 ? amplitude : -amplitude;

	for (auto &sample: f) {
		sample = s;
	}

	d_->t += d_->step;

	return f;
}