// audio_generator_engine_triangle.h
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#pragma once

#include <audio/format>
#include <audio/sequence>

#include <utils/pimpl>

namespace com {
namespace nealrame {
namespace audio {
namespace generators {
struct triangle {
	triangle(class format fmt, float init, float amplitude, float frequency);
	virtual ~triangle();
	void reset();
	const sequence::frame operator()();
	const float amplitude;
	const float frequency;
	const float t0;
private:
	PIMPL;
};
} // namespace generators
} // namespace audio
} // namespace nealrame
} // namespace com
