#ifndef AUDIO_BUFFER_H_
#define AUDIO_BUFFER_H_

#include <functional>
#include <iterator>
#include <memory>
#include <vector>

#include <utils/buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/sequence>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::buffer
///
/// The `buffer` class provides representation of an audio buffer. That is a
/// frame container assigned a given format which manages its own memory area.
/// You can enlarge or reduce it as you want to.
class buffer {
public:
	typedef sequence::size_type size_type;
public:
	buffer () = delete;
	explicit buffer (class format);
	buffer (class format, unsigned int frame_count);
	buffer (class format, double duration);
	virtual ~buffer ();
public:
	class format format () const;
	double duration () const;
	unsigned int frame_count () const;
public:
	/// Add some frames to the end of this `buffer` to increase its
	/// duration by the given duration.
	/// If the parameter `clear` is set to `true`, set all the new sample
	/// to zero.
	void append (double duration, bool clear = false);
	/// Add the given count of frames to the end of this `buffer`.
	/// If the parameter `clear` is set to `true`, set all the new sample
	/// to zero.
	void append (unsigned int frame_count, bool clear = false);
	/// Append the given `sequence` to this `buffer`.
	///
	/// If the specified sequence have a different format than this
	/// `buffer` an `error` exception with status `FormatDoesNotMatch` will
	/// be raised.
	void append (const sequence &) throw(error);
	/// Removes some frames from the end of this buffer to decrease its 
	/// duration of the specified amount.
	void cutdown (double duration);
	/// Remove the given count of frames from the end of this `buffer`.
	void cutdown (unsigned int frame_count);
	/// Adjust the buffer internal capacity so that it can contain the
	/// required count of frames for the given duration.
	/// If the specified duration is shortest than the current duration
	/// then all the frames beyond the given duration will be lost.
	/// If `duration` is greater than this `buffer` current duration and 
	/// parameter `clear` is `true` then all the new samples are set to 
	/// zero.
	void set_duration (double duration, bool clear = false);
	/// Adjust the buffer internal capacity so that it can contain the
	/// specified count of frames.
	/// If the specified frame count is lower than the current frame count
	/// then all the frames after the given frame count will be lost.
	/// If `frame_count` is greater than this `buffer` current frame_count
	/// and parameter `clear` is `true` then all the new samples are set 
	/// to zero.
	void set_frame_count (unsigned int frame_count, bool clear = false);
public:
	/// Return a reference on the raw data buffer of this `dynamic_buffer`
	com::nealrame::utils::buffer & raw_buffer ();
	/// Return a constant reference on the raw data buffer of this
	/// `dynamic_buffer`.
	const com::nealrame::utils::buffer & raw_buffer () const;
public:
	/// Return a reference on a sequence starting at the begening of this
	/// `dynamic_buffer` and ending at the last frame.
	class sequence sequence();
	/// Return a constant reference on a sequence starting at the begening
	/// of this `dynamic_buffer` and ending at the last frame.
	const class sequence sequence() const;
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */