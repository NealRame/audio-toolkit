/*
 * audio_generator_engine_triangle.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_TRIANGLE_GENERATOR_ENGINE_H_
#define AUDIO_TRIANGLE_GENERATOR_ENGINE_H_

#include <memory>

#include <audio/format>
#include <audio/frame>

namespace com {
namespace nealrame {
namespace audio {
struct triangle {
	triangle (class format fmt, float amplitude, float frequency);
	virtual ~triangle ();
	void reset () { t = 0; }
	const_frame frame ();
	class format format;
	float amplitude;
	float t;
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_TRIANGLE_GENERATOR_ENGINE_H_ */