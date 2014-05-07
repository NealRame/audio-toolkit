// audio_generator_engine_sine.h
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_SINE_GENERATOR_ENGINE_H_
#define AUDIO_SINE_GENERATOR_ENGINE_H_

#include <audio/format>
#include <audio/sequence>

#include <memory>

namespace com {
namespace nealrame {
namespace audio {
namespace generators {
struct sine {
	sine (class format fmt, float init, float amplitude, float frequency);
	virtual ~sine ();
	void reset ()
	{ t_ = t0; }
	sequence::const_frame operator() ();
	const float amplitude;
	const float frequency;
	const float t0;
private:
	float t_;
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace generators
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_SINE_GENERATOR_ENGINE_H_ */