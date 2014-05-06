// audio_generator_engine_sawtooth.h
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_SAWTOOTH_GENERATOR_ENGINE_H_
#define AUDIO_SAWTOOTH_GENERATOR_ENGINE_H_

#include <audio/format>
#include <audio/sequence>

#include <memory>

namespace com {
namespace nealrame {
namespace audio {
namespace generators {
struct sawtooth {
	sawtooth (class format fmt, float init, float amplitude, float frequency);
	virtual ~sawtooth ();
	void reset () 
	{ t_ = t0; }
	sequence::const_frame operator() ();
	class format format;
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
# endif /* AUDIO_SAWTOOTH_GENERATOR_ENGINE_H_ */