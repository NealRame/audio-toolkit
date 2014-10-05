// audio_generator_engine_noise.cpp
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#include "audio_noise.h"

#include <random>
#include <ctime>

using namespace com::nealrame::audio;
using com::nealrame::audio::generators::noise;

struct noise::impl {
	impl(class format format) :
		sequence(format, format::size_type(1)),
		distribution(0, 1),
		rand(std::bind(distribution, std::default_random_engine()))
	{ }
	class sequence sequence;
    std::uniform_int_distribution<> distribution;
    std::function<float()> rand;
};

noise::noise(class format fmt, float a) :
	amplitude(std::fabs(std::fmin(a, 1.))),
	d_(std::make_unique<impl>(fmt))
{ }

noise::~noise()
{ }

const sequence::frame noise::operator()()
{
	auto f = d_->sequence.at(0);
	for (auto &sample: f) {
		sample = amplitude*d_->rand();
	}
	return f;
}
