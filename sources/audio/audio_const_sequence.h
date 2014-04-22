/*
 * audio_const_sequence.h
 *
 * Created on: March 28, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_CONST_SEQUENCE_H_
#define AUDIO_CONST_SEQUENCE_H_

#include <functional>
#include <iterator>
#include <memory>

#include <utils/buffer>
#include <audio/format>
#include <audio/frame>

namespace com {
namespace nealrame {
namespace audio {
class sequence;
/// com::nealrame::audio::const_sequence
/// 
/// The `const_sequence` class provides representation of an audio buffer. That is
/// nothing more than a frame container with an assigned format descriptor to 
/// it.
class const_sequence {
public:
	typedef size_t size_type;
public:
	const_sequence () = delete;
	/// Initialize an audio `const_sequence` with the specified format and a 
	/// given data `utils::buffer`.
	const_sequence (const class format &format, const com::nealrame::utils::buffer &);
	const_sequence (const sequence &);
	const_sequence (const_sequence &&);
	virtual ~const_sequence ();
public:
	/// Return the audio `format` of this audio `const_sequence`.
	virtual class format format () const;
	/// Return the duration of this `const_sequence`.
	virtual double duration () const;
	/// Return the number of frame of this audio `const_sequence`.
	virtual unsigned int frame_count () const;
	/// Return the memory footprint of this audio `const_sequence`.
	virtual size_t size () const;
public:
	/// Return a pointer on the internal interleaved float buffer.
	const float * frames_buffer ();
	/// Return a pointer on the internal interleaved float buffer.
	const float * frames_buffer() const {
		return const_cast<const_sequence *>(this)->frames_buffer();
	}
	/// Return the internal raw data buffer
	const com::nealrame::utils::buffer & data ();
	/// Return the internal raw data buffer
	const com::nealrame::utils::buffer & data () const {
		return const_cast<const_sequence *>(this)->data();
	}
public:
	/// Return the `frame` af the given index.
	virtual const_frame frame_at (size_type frame_index);
	/// Return the `const_frame` af the given index.
	virtual const_frame frame_at (size_type frame_index) const {
		return const_cast<const_sequence *>(this)->frame_at(frame_index);
	}
public:
	/// Return the sub-part of this `const_sequence` starting at `frame_index`
	/// and holding at most `count` frames.
	const_sequence subsequence (size_type frame_index, size_type count);
	/// Return the sub-part of this `const const_sequence` starting at
	/// `frame_index` and holding at most `count` frames.
	const_sequence subsequence (size_type frame_index, size_type count) const {
		return const_cast<const_sequence *>(this)->subsequence(frame_index, count);
	}
public:
	/// Return the `frame` af the given index.
	virtual const_frame operator[] (size_type frame_index) {
		return frame_at(frame_index);
	}
	/// Return the `const_frame` af the given index.
	virtual const_frame operator[] (size_type frame_index) const {
		return frame_at(frame_index);
	}
public:
	class iterator :
		public boost::iterator_facade<
			iterator, const_frame, boost::random_access_traversal_tag, const_frame> {
	friend class const_sequence;
	private:
		friend boost::iterator_core_access;
		struct enabler {};
		template <typename U>
		using frame_ptr = com::nealrame::utils::buffer::iterator<U>;
	private:
		unsigned int channel_count_;
		typename const_frame::iterator ptr_;
	private:
		const_frame dereference () const {
			return const_frame(channel_count_, ptr_);
		}
		template <typename Iterator>
		bool equal (Iterator const &other) const {
			return ptr_ == other.ptr_;
		}
		ptrdiff_t distance_to (iterator const &other) const {
			return (ptr_ - other.ptr_)/channel_count_;
		}
		void advance (ptrdiff_t n) {
			ptr_ += n*channel_count_;
		}
		void increment () { advance( 1); }
		void decrement () { advance(-1); }
	public:
		iterator () :
			channel_count_(0) { }
		template <typename U>
		iterator (unsigned int channel_count,
				const frame_ptr<U> &ptr,
					typename std::enable_if<
						std::is_convertible<
							U &,
							typename const_frame::iterator::reference
							>::value, 
						enabler>::type = enabler()) :
			channel_count_(channel_count),
			ptr_(ptr) { }
		template <typename U>
		iterator (const U &other,
					typename std::enable_if<
						std::is_convertible<
							typename U::value_type::iterator::reference,
							typename const_frame::iterator::reference
							>::value,
						enabler>::type = enabler()) :
			channel_count_(other.channel_count_),
			ptr_(other.ptr_) { }
	};
public:
	/// Return an iterator on the first `frame` of this `const_sequence`.
	virtual iterator begin ();
	/// Return an iterator on the end of this `const_sequence`.
	virtual iterator end ();
	/// Return an const_iterator on the first `frame` of this `const_sequence`.
	virtual iterator begin () const {
		return const_cast<const_sequence *>(this)->begin();
	}
	/// Return an iterator on the end of this `const_sequence`.
	virtual iterator end () const {
		return const_cast<const_sequence *>(this)->end();
	}
	/// Return an const_iterator on the first `frame` of this `const_sequence`.
	virtual iterator cbegin () const final { return begin(); }
	/// Return an iterator on the end of this `const_sequence`.
	virtual iterator cend () const final { return end(); }
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_CONST_SEQUENCE_H_ */