#ifndef AUDIO_SEQUENCE_H_
#define AUDIO_SEQUENCE_H_

#include <functional>
#include <iterator>
#include <memory>

#include <utils/buffer>
#include <audio/format>
#include <audio/frame>

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
	sequence (const class format &format, com::nealrame::utils::buffer &);
	sequence (sequence &&);
	virtual ~sequence ();
public:
	virtual class format format () const;
	virtual double duration () const;
	virtual unsigned int frame_count () const;
	virtual size_t size () const;
public:
	const com::nealrame::utils::buffer & raw_buffer () const {
		return raw_buffer_;
	}
public:
	virtual frame frame_at (size_type frame_index);
	virtual const_frame frame_at (size_type frame_index) const;
public:
	virtual frame operator[] (size_type frame_index) {
		return frame_at(frame_index);
	}
	virtual const_frame operator[] (size_type frame_index) const {
		return frame_at(frame_index);
	}
public:
	template <typename T>
	class base_iterator : 
		public boost::iterator_facade<
			base_iterator<T>, T, boost::random_access_traversal_tag, T> {
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
				typename std::enable_if<std::is_convertible<U *, typename T::iterator::value_type *>::value, enabler>::type = enabler()) :
			channel_count_(channel_count),
			ptr_(ptr) { }
		template <typename U>
		base_iterator (const U &other,
				typename std::enable_if<std::is_convertible<U *, T *>::value, enabler>::type = enabler()) :
			channel_count_(other.channel_count_),
			ptr_(other.ptr_) { }
	};
	using iterator = base_iterator<frame>;
	using const_iterator = base_iterator<const_frame>;
public:
	virtual iterator begin ();
	virtual iterator end ();
	virtual const_iterator begin () const;
	virtual const_iterator end () const;
	virtual const_iterator cbegin () const final { return begin(); }
	virtual const_iterator cend () const final { return end(); }
private:
	class format format_;
	com::nealrame::utils::static_buffer raw_buffer_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */