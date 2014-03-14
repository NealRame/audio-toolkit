/*
 * audio_generator.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_GENERATOR_H_
#define AUDIO_GENERATOR_H_

#include <memory>

#include <audio/format>
#include <audio/frame>
#include <audio/buffer>

namespace com {
namespace nealrame {
namespace audio {

/// com::nealrame::audio::generator
///
template <class Engine>
class generator {
public:
	template<typename ...Args> 
	generator(const class format &fmt, Args... args) :
		engine_(fmt, std::forward<Args>(args)...) {
	}
public:
	void reset () { engine_.reset(); }
	const_frame frame () {
		return engine_.frame();
	}
	std::shared_ptr<class buffer> buffer (unsigned int frame_count) {
		std::shared_ptr<class buffer> buf =
			std::make_shared<class buffer>(engine_.format, frame_count);
		audio::sequence seq = buf->sequence();
		for (class frame f: seq) {
			f = frame();
		};
		return buf;
	}
	std::shared_ptr<class buffer> buffer (double duration) {
		std::shared_ptr<class buffer> buf =
			std::make_shared<class buffer>(engine_.format, duration);
		audio::sequence seq = buf->sequence();
		for (class frame f: seq) {
			auto g = frame();
			f = g;
		};
		return buf;
	}
private:
	Engine engine_;
};
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_GENERATOR_H_ */