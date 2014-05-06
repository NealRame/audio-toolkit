// audio_generator_engine_sawtooth.cpp
//
// Created on: March 08, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_sawtooth.h"

#include <cmath>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::sawtooth;

struct sawtooth::impl {
	impl(class format format) :
		sequence(format, format::size_type(1)) {
	}
	class sequence sequence;
	float step;
};

sawtooth::sawtooth (class format fmt, float init, float a, float f) :
	format(fmt),
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	t_(init),
	pimpl_(new impl(fmt)) {
	pimpl_->step = 1./fmt.sample_rate();
}

sawtooth::~sawtooth () {
}

sequence::const_frame sawtooth::operator() () {
	float floor_part = floor(t_*frequency + 0.5);
	float s = 2*amplitude*(t_*frequency - floor_part);
	sequence::frame f = pimpl_->sequence.at(0);

	t_ += pimpl_->step;

	for (float &sample: f) {
		sample = s;
	}
	return f;
}
