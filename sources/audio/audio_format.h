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
	/// Initializes an `audio::format` given a number of channel and a
	/// frame rate.
	///
	/// *Parameters*:
	/// - `channel_count`
	/// - `frame_rate`
	///
	/// *Exception:*
	/// - `error`
	///   If channel count or frame rate are not valid throw an error
	/// with status `FormatWrongSampleRateValueError` or
	/// `FormatWrongChannelCountValueError`.
	format (unsigned int channel_count, unsigned int sample_rate)
		throw(error);
public:
	/// Returns `true` if this `format` and the other are equals.
	bool operator== (const format &rhs) const noexcept;
	/// Returns `true` if this `format` and the other are not equals.
	bool operator!= (const format &rhs) const noexcept
	{ return ! (*this == rhs); }
public:
	/// Returns the number of channel of this `format`.
	size_type channel_count () const noexcept
	{ return channel_count_; }

	/// Returns the frame rate of this `format`.
	size_type sample_rate () const noexcept
	{ return frame_rate_; }

	/// Sets the frame rate of this `format`.
	///
	/// *Parameters:*
	/// - `sample_rate` the new frame rate
	///
	/// *Exeception:*
	/// - `error` if frame rate is not valid
	format & set_frame_rate (unsigned int frame_rate) throw(error);

	/// Returns the duration for the given frame count.
	///
	/// *Parameters:*
	/// - `frame_count`
	double duration (size_type frame_count) const noexcept;

	/// Returns the frame count for a given duration.
	///
	/// *Parameters:*
	/// - `duration`
	size_type frame_count (double duration) const noexcept;
private:
	size_type channel_count_;
	size_type frame_rate_;
};

std::ostream & operator<<(std::ostream &, format);

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_FORMAT_H_ */