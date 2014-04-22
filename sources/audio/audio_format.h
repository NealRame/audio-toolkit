/*
 * audio_format.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_FORMAT_H_
#define AUDIO_FORMAT_H_

#include <cstddef>

#include <audio/error>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::format
/// ============================
///
/// 
class format {
public:
	/// Initialize a frame container format given a channel count and 
	/// sample rate.
	format (unsigned int channel_count, unsigned int sample_rate)
		throw(audio::error);
public:
	/// Return true if and only if this format and the other one are
	/// equals.
	bool operator==(const format &other) const;
	/// Return true if and only if this format and the other one are not 
	/// equals.
	bool operator!=(const format &other) const { return !(*this == other);}
public:
	/// Return this format channel count.
	unsigned int channel_count () const
	{ return channel_count_; }
	/// Return this format sample rate.
	unsigned int sample_rate () const
	{ return sample_rate_; }
	/// Update this format sample rate.
	format & set_sample_rate (unsigned int rate) throw(audio::error);
	/// Return the duration of a frame container for a given frame count.
	double duration (unsigned int frame_count) const;
	/// Return the duration of a frame container for a given size.
	double duration (size_t size) const;
	/// Return the frame count of a frame container for a given size.
	unsigned int frame_count (size_t size) const;
	/// Return the frame count of frame container for a given duration.
	unsigned int frame_count (double duration) const;
	/// Return the size of a frame container for a given frame count.
	size_t size (unsigned int frame_count) const;
	/// Return the size of a frame container for a given duration.
	size_t size (double duration) const;
private:
	unsigned int channel_count_;
	unsigned int sample_rate_;
};
#if defined (TEST)
#include <ostream>
void PrintTo (const format &, ::std::ostream *os);
#endif
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */