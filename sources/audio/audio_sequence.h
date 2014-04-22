/*
 * audio_sequence.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_SEQUENCE_H_
#define AUDIO_SEQUENCE_H_

#include <memory>

#include <utils/buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/const_sequence>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::sequence
/// 
/// The `sequence` class provides representation of an audio buffer. That is
/// nothing more than a frame container with an assigned format descriptor to 
/// it.
class sequence {
public:
	typedef size_t size_type;
public:
	sequence () = delete;
	/// Initialize an audio `sequence` with the specified format and a 
	/// given data `utils::buffer`.
	sequence (const class format &format, com::nealrame::utils::buffer &);
	sequence (sequence &&);
	virtual ~sequence ();
public:
	/// Return the audio `format` of this audio `sequence`.
	virtual class format format () const;
	/// Return the duration of this `sequence`.
	virtual double duration () const;
	/// Return the number of frame of this audio `sequence`.
	virtual unsigned int frame_count () const;
	/// Return the memory footprint of this audio `sequence`.
	virtual size_t size () const;
public:
	/// Return a pointer on the internal interleaved float buffer.
	const float * frames_buffer ();
	/// Return a pointer on the internal interleaved float buffer.
	const float * frames_buffer() const {
		return const_cast<sequence *>(this)->frames_buffer();
	}
	/// Return the internal raw data buffer
	const com::nealrame::utils::buffer & data ();
	/// Return the internal raw data buffer
	const com::nealrame::utils::buffer & data () const {
		return const_cast<sequence *>(this)->data();
	}
public:
	/// Return the `frame` af the given index.
	virtual frame frame_at (size_type frame_index);
	/// Return the `const_frame` af the given index.
	virtual const_frame frame_at (size_type frame_index) const {
		return const_cast<sequence *>(this)->frame_at(frame_index);
	}
public:
	/// Return the sub-part of this `sequence` starting at `frame_index`
	/// and holding at most `count` frames.
	sequence subsequence (size_type frame_index, size_type count);
	/// Return the sub-part of this `const sequence` starting at
	/// `frame_index` and holding at most `count` frames.
	const_sequence subsequence (size_type frame_index, size_type count) const {
		return const_cast<sequence *>(this)->subsequence(frame_index, count);
	}
public:
	/// Return the `frame` af the given index.
	virtual frame operator[] (size_type frame_index) {
		return frame_at(frame_index);
	}
	/// Return the `const_frame` af the given index.
	virtual const_frame operator[] (size_type frame_index) const {
		return frame_at(frame_index);
	}
public:
	template <typename T>
	class base_iterator : 
		public boost::iterator_facade<
			base_iterator<T>, T, boost::random_access_traversal_tag, T> {
	friend class sequence;
	private:
		friend boost::iterator_core_access;
		template <class> friend class base_iterator;
		struct enabler {};
		template <typename U>
		using frame_ptr = com::nealrame::utils::buffer::base_iterator<U>;
	private:
		unsigned int channel_count_;
		typename T::iterator ptr_;
	private:
		T dereference () const {
			return T(channel_count_, ptr_);
		}
		template <class U>
		bool equal (base_iterator<U> const &other) const {
			return ptr_ == other.ptr_;
		}
		ptrdiff_t distance_to (base_iterator const &other) const {
			return (ptr_ - other.ptr_)/channel_count_;
		}
		void advance (ptrdiff_t n) {
			ptr_ += n*channel_count_;
		}
		void increment () { advance( 1); }
		void decrement () { advance(-1); }
	public:
		base_iterator () :
			channel_count_(0) { }
		template <typename U>
		base_iterator (unsigned int channel_count,
				const frame_ptr<U> &ptr,
					typename std::enable_if<
						std::is_convertible<
							U &,
							typename T::iterator::reference
							>::value, 
						enabler>::type = enabler()) :
			channel_count_(channel_count),
			ptr_(ptr) { }
		template <typename U>
		base_iterator (const U &other,
					typename std::enable_if<
						std::is_convertible<
							typename U::value_type::iterator::reference,
							typename T::iterator::reference
							>::value,
						enabler>::type = enabler()) :
			channel_count_(other.channel_count_),
			ptr_(other.ptr_) { }
	};
	using iterator = base_iterator<frame>;
	using const_iterator = base_iterator<const_frame>;
public:
	/// Return an iterator on the first `frame` of this `sequence`.
	virtual iterator begin ();
	/// Return an iterator on the end of this `sequence`.
	virtual iterator end ();
	/// Return an const_iterator on the first `frame` of this `sequence`.
	virtual const_iterator begin () const {
		return const_cast<sequence *>(this)->begin();
	}
	/// Return an iterator on the end of this `const_sequence`.
	virtual const_iterator end () const {
		return const_cast<sequence *>(this)->end();
	}
	/// Return an const_iterator on the first `frame` of this `sequence`.
	virtual const_iterator cbegin () const final { return begin(); }
	/// Return an iterator on the end of this `sequence`.
	virtual const_iterator cend () const final { return end(); }
private:
	struct impl;
	std::unique_ptr<impl> pimpl_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_CONST_SEQUENCE_H_ */