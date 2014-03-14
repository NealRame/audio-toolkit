/*
 * audio_generator_engine_noise.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_NOISE_GENERATOR_ENGINE_H_
#define AUDIO_NOISE_GENERATOR_ENGINE_H_

#include <memory>

#include <audio/format>
#include <audio/frame>

namespace com {
namespace nealrame {
namespace audio {
struct noise {
	noise (class format fmt, float amplitude);
	virtual ~noise();
	void reset () {}
	const_frame frame();
	class format format;
	float amplitude;
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_NOISE_GENERATOR_ENGINE_H_ */