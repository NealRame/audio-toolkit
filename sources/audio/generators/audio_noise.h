// audio_generator_engine_noise.h
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
struct noise {
	noise(class format fmt, float amplitude);
	virtual ~noise();
	void reset() {}
	const sequence::frame operator()();
	const float amplitude;
private:
	PIMPL;
};
} // namespace generators
} // namespace audio
} // namespace nealrame
} // namespace com
