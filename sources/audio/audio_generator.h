// audio_generator.h
//
// Created on: May 07, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_GENERATOR_H_
#define AUDIO_GENERATOR_H_

#include <memory>

#include <audio/format>
#include <audio/sequence>

namespace com {
namespace nealrame {
namespace audio {

/// class com::nealrame::audio::generator
/// =====================================
template <class Engine>
class generator {
public:
	/// Constructs a generator.
	template<typename ...Args>
	generator(const class format &fmt, Args... args) :
		format_(fmt),
		generate_(fmt, std::forward<Args>(args)...) {
	}

public:
	/// Returns the format of this `generator`.
	const format & format () const
	{ return format_; }

	/// Resets this `generator` to its initial value.
	void reset ()
	{ generate_.reset(); }

	/// Returns the next frame of this `generator`.
	sequence::const_frame operator() () 
	{ return generate_(); }

	///  Returns a `seqence` with a given count of frame.
	///
	/// *Parameters:*
	/// - `frame_count`
	///   The requested count of frame.
	class sequence sequence (unsigned int frame_count) {
		class sequence seq(format_, frame_count);
		for (auto f: seq) {
			f = generate_();
		};
		return seq;
	}

	/// Returns a `sequence` of a given duration.
	///
	/// *Parameters:*
	/// - `duration`
	///   The requested duration.
	class sequence sequence (double duration) {
		class sequence seq(format_, duration);
		for (auto f: seq) {
			f = generate_();
		};
		return seq;
	}
	
private:
	class format format_;
	Engine generate_;
};
} // namespace audio
} // namespace nealrame
} // namespace com
# endif /* AUDIO_GENERATOR_H_ */