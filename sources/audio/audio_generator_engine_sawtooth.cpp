/*
 * audio_generator_engine_sawtooth.cpp
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */

#include "audio_generator_engine_sawtooth.h"

#include <cmath>

#include <audio/buffer>

using namespace com::nealrame::audio;

struct sawtooth::impl {
	impl(class format format) :
		buffer(format, 1u) {
	}
	class buffer buffer;
	float two_frequency;
	float step;
};

sawtooth::sawtooth (class format fmt, float amplitude, float frequency) :
	format(fmt),
	amplitude(std::fmin(amplitude, 1.)),
	t(0),
	pimpl_(new impl(fmt)) {
	pimpl_->two_frequency = 2*frequency;
	pimpl_->step = 1./fmt.sample_rate();
}

sawtooth::~sawtooth () {
}

const_frame sawtooth::frame () {
	class frame f = pimpl_->buffer.sequence().frame_at(0);

	float floor_part = floor(t*pimpl_->two_frequency + 0.5);
	float s = 2*amplitude*(t*pimpl_->two_frequency - floor_part);

	for (float &sample: f) {
		sample = s;
	}

	t += pimpl_->step;

	return f;
}
