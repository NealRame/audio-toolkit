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
	impl(format fmt, float f, float t0) :
		sequence(fmt, format::size_type(1)),
		half_period(1./(2*f)),
		step(1./fmt.sample_rate()),
		t(t0)
	{ }
	class sequence sequence;
	float half_period;
	float step;
	float t;
};

triangle::~triangle()
{ }

void triangle::reset()
{ d_->t = t0; }

triangle::triangle (class format fmt, float init, float a, float f) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	frequency(f),
	t0(init),
	d_(new impl(fmt, frequency, t0))
{ }

const sequence::frame triangle::operator()()
{
	int floor_part = floor(2*d_->t*frequency + 0.5);
	auto v = 4*frequency
			*(d_->t - d_->half_period*floor_part)
			*(floor_part%2 ? -1 : 1);
	auto f = d_->sequence.at(0);
	for (auto &sample: f) {
		sample = v;
	}
	
	d_->t += d_->step;

	return f;
}