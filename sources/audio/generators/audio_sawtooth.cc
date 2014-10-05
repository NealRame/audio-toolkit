// audio_generator_engine_sawtooth.cpp
//
// Created on: March 08, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_sawtooth.h"

#include <cmath>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::sawtooth;

struct sawtooth::impl {
	impl(class format format, float init) :
		sequence(format, format::size_type(1)),
		step(1./format.sample_rate()),
		t(init)
	{ }
	class sequence sequence;
	float step;
	float t;
};

sawtooth::sawtooth(class format fmt, float init, float a, float f) :
	format(fmt),
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	d_(new impl(fmt, init))
{ }

sawtooth::~sawtooth()
{ }

void sawtooth::reset()
{ d_->t = t0; }

const sequence::frame sawtooth::operator()()
{
	auto floor_part = floor(d_->t*frequency + 0.5);
	auto s = 2*amplitude*(d_->t*frequency - floor_part);
	auto f = d_->sequence.at(0);

	for (float &sample: f) {
		sample = s;
	}
	
	d_->t += d_->step;

	return f;
}
