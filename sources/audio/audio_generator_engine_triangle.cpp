/*
 * audio_generator_engine_triangle.cpp
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */

#include "audio_generator_engine_triangle.h"

#include <cmath>

#include <audio/buffer>

using namespace com::nealrame::audio;

struct triangle::impl {
	impl(class format format) :
		buffer(format, 1u) {
	}
	class buffer buffer;
	float half_period;
	float frequency;
	float step;
};

triangle::triangle (class format fmt, float amplitude, float frequency) :
	format(fmt),
	amplitude(std::fmin(amplitude, 1.)),
	t(0),
	pimpl_(new impl(fmt)) {
	pimpl_->frequency = frequency;
	pimpl_->half_period = 1./(2*frequency);
	pimpl_->step = 1./fmt.sample_rate();
}

triangle::~triangle () {
}

const_frame triangle::frame () {
	class frame f = pimpl_->buffer.sequence().frame_at(0);

	int floor_part = floor(2*t*pimpl_->frequency + 0.5);
	float v = 4*pimpl_->frequency
			*(t - pimpl_->half_period*floor_part)
			*(floor_part % 2 ? -1 : 1);

	for (float &sample: f) {
		sample = v;
	}

	t += pimpl_->step;

	return f;
}
