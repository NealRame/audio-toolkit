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
	buffer (class format, com::nealrame::utils::buffer &);
	virtual ~buffer ();
public:
	class format format () const;
	double duration () const;
	unsigned int frame_count () const;
public:
	/// Append the given `const_frame` to this buffer, increasing its
	/// capacity if needed.
	///
	/// If the specified frame have a different channel count than this
	/// `dynamic_buffer` a `error` exception with status 
	/// `FormatDoesNotMatch` will be raised.
	void append (const_frame &) throw(error);
	/// Append the given `sequence` to this buffer, increasing its capacity
	/// if needed.
	/// 
	/// If the specified sequence have a different format than this
	/// `dynamic_buffer` a `error` exception with status 
	/// `FormatDoesNotMatch` will be raised.
	void append (const sequence &) throw(error);
public:
	/// Get the current capacity of this `buffer`.
	size_t capacity () const;
	/// Adjust the buffer internal capacity so that it can contain the
	/// required count of frames for the given duration.
	/// If the specified duration is shortest than the current duration
	/// then all the frames beyond the given duration will be lost.
	void reserve (double duration);
	/// Adjust the buffer internal capacity so that it can contain the
	/// specified count of frames.
	/// If the specified frame count is lower than the current frame count
	/// then all the frames after the given frame count will be lost.
	void reserve (unsigned int frame_count);
	/// Increase the internal capacity of the buffer by the amount required
	/// to contain the number of frames required for the specified
	/// duration.
	/// It will not increase the current duration.
	void enlarge (double duration);
	/// Increase the internal capacity of the buffer by the amount required
	/// to contain the specified count of frames.
	/// It will not increase the current frame count.
	void enlarge (unsigned int frame_count);
	/// Decrease the internal capacity of the buffer by the amount needed
	/// to contain the number of frames required for the specified 
	/// duration.
	void shrink (double duration);
	/// Decrease the internal capacity of the buffer by the amount required
	/// to contain the specified number of frames.
	void shrink (unsigned int frame_count);
public:
	/// Return a reference on the raw data buffer of this `dynamic_buffer`
	com::nealrame::utils::buffer & raw_buffer ();
	/// Return a constant reference on the raw data buffer of this
	/// `dynamic_buffer`.
	const com::nealrame::utils::buffer & raw_buffer () const;
public:
	/// Return a reference on a sequence starting at the begening of this
	/// `dynamic_buffer` and ending at the last frame.
	class sequence & sequence();
	/// Return a constant reference on a sequence starting at the begening
	/// of this `dynamic_buffer` and ending at the last frame.
	const class sequence & sequence() const;
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */