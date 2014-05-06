// audio_generator_engine_noise.h
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_NOISE_GENERATOR_ENGINE_H_
#define AUDIO_NOISE_GENERATOR_ENGINE_H_

#include <audio/format>
#include <audio/sequence>

#include <memory>

namespace com {
namespace nealrame {
namespace audio {
namespace generators {
struct noise {
	noise (class format fmt, float amplitude);
	virtual ~noise ();
	void reset () {}
	sequence::const_frame operator() ();
	const float amplitude;
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace generators
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_NOISE_GENERATOR_ENGINE_H_ */