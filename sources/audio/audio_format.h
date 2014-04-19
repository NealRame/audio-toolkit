/// audio_format.h
///
/// Created on: April 14, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_FORMAT_H_
#define AUDIO_FORMAT_H_

#include "audio_error.h"

namespace com {
namespace nealrame {
namespace audio {
/// class com::nealrame::audio::format
/// ==================================
class format {
public:
	using size_type = size_t;
public:
	/// Constructs an `audio::format` given a number of channel and a
	/// sample rate.
	/// *parameters*:
	/// - `channel_count`
	/// - `sample_rate`
	/// *exception:*
	/// - `error` if channel count or sample rate is not valid
	format (unsigned int channel_count, unsigned int sample_rate)
		throw(error);
public:
	/// Returns `true` if this `format` and the other one are equals.
	bool operator== (const format &rhs) const noexcept;
	/// Returns `true` if this `format` and the other one are not equals.
	bool operator!= (const format &rhs) const noexcept
	{ return ! (*this == rhs); }
public:
	/// Returns the number of channel of this `format`.
	size_type channel_count () const noexcept
	{ return channel_count_; }
	/// Returns the sample rate of this `format`.
	size_type sample_rate () const noexcept
	{ return sample_rate_; }
	/// Sets the sample rate of this `format`.
	/// *parameters:*
	/// - `sample_rate` the new sample rate
	/// *exeception:*
	/// - `error` if sample rate is not valid
	format & set_sample_rate (unsigned int sample_rate) throw(error);
	/// Returns the duration for the given frame count.
	/// *parameters:*
	/// - `frame_count`
	double duration (size_type frame_count) const noexcept;
	/// Returns the frame count for a given duration.
	/// *parameters:*
	/// - `duration`
	size_type frame_count (double duration) const noexcept;
private:
	size_type channel_count_;
	size_type sample_rate_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_FORMAT_H_ */