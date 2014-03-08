/**
 * audio_format.h
 *
 * Created on: Dec 28, 2013
 *     Author: Neal.Rame. <contact@nealrame.com>
 */
#ifndef AUDIO_FORMAT_H_
#define AUDIO_FORMAT_H_

#include <cstddef>

#if defined(TEST)
#	include <ostream>
#endif

#include "audio_error.h"

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::format
/// ============================
///
/// 
class format {
public:
	///
	format (unsigned int channel_count, unsigned int sample_rate) throw(audio::error);
public:
	///
	bool operator==(const format &other) const;
public:
	///
	unsigned int channel_count () const
	{ return channel_count_; }
	///
	format & set_channel_count (unsigned int count) throw(audio::error);
	///
	unsigned int sample_rate () const
	{ return sample_rate_; }
	///
	format & set_sample_rate (unsigned int rate) throw(audio::error);
	///
	double duration (unsigned int frame_count) const;
	///
	double duration (size_t size) const;
	///
	unsigned int frame_count (size_t size) const;
	///
	unsigned int frame_count (double duration) const;
	///
	size_t size (unsigned int frame_count) const;
	///
	size_t size (double duration) const;
private:
	unsigned int channel_count_;
	unsigned int sample_rate_;
};

#if defined(TEST)
	void PrintTo (const format &, ::std::ostream *os);
#endif

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */