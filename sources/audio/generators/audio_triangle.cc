/*
 * audio_generator_engine_triangle.cpp
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */

#include "audio_triangle.h"

#include <cmath>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::triangle;

struct triangle::impl {
	impl(format fmt) :
		sequence(fmt, format::size_type(1)) {
	}
	class sequence sequence;
	float half_period;
	float step;
};

triangle::~triangle () {
}

triangle::triangle (class format fmt, float init, float a, float f) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	t_(init),
	pimpl_(new impl(fmt)) {
	pimpl_->half_period = 1./(2*frequency);
	pimpl_->step = 1./fmt.sample_rate();
}

sequence::const_frame triangle::operator() () {
	int floor_part = floor(2*t_*frequency + 0.5);
	float v = 4*frequency
			*(t_ - pimpl_->half_period*floor_part)
			*(floor_part % 2 ? -1 : 1);
	sequence::frame f = pimpl_->sequence.at(0);
	for (float &sample: f) {
		sample = v;
	}
	t_ += pimpl_->step;
	return f;
}