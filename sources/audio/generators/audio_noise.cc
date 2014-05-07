// audio_generator_engine_noise.cpp
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_noise.h"

#include <random>
#include <ctime>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::noise;

static float random_value () {
	static std::default_random_engine engine(time(nullptr));
	static std::uniform_real_distribution<float> distribute;
	return distribute(engine);
}

struct noise::impl {
	impl (class format format) :
		sequence(format, format::size_type(1)) {
	}
	class sequence sequence;
};

noise::noise (class format fmt, float a) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	pimpl_(new impl(fmt)) {
}

noise::~noise () {
}

sequence::const_frame noise::operator() () {
	sequence::frame f = pimpl_->sequence.at(0);
	for (auto &sample: f) {
		sample = amplitude*random_value();
	}
	return f;
}
