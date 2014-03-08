#ifndef AUDIO_FRAME_CONTAINER_H_
#define AUDIO_FRAME_CONTAINER_H_

#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>

#include <utils/abstract_buffer>
#include <audio/format>
#include <audio/frame>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/reverse_iterator.hpp>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::frame_container
/// 
/// The `buffer` class provides representation of an audio buffer. It is 
/// nothing more than a sample container with an assign format descriptor.
class frame_container {
public:
	virtual format format () const = 0;
	virtual double duration () const = 0;
	virtual unsigned int frame_count () const = 0;
	virtual frame frame_at(unsigned int frame_index) = 0;
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
		using frame_ptr = com::nealrame::utils::abstract_buffer::base_iterator<U>;
	private:
		unsigned int channel_count_;
		typename frame_iterator<T>::type ptr_;

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
				typename std::enable_if<std::is_convertible<U *, typename frame_iterator<T>::type::value_type *>::value, enabler>::type = enabler()) :
			channel_count_(channel_count),
			ptr_(ptr) { }
		template <typename U>
		base_iterator (const U &other,
				typename std::enable_if<std::is_convertible<U *, T *>::value, enabler>::type = enabler()) :
			channel_count_(other.channel_count_),
			ptr_(other.ptr_) { }
	};
	using iterator = base_iterator<frame>;
	using const_iterator = base_iterator<frame const>;
public:
	virtual iterator begin () = 0;
	virtual iterator end () = 0;
	virtual const_iterator begin () const = 0;
	virtual const_iterator end () const = 0;
	virtual const_iterator cbegin () const final { return begin(); }
	virtual const_iterator cend () const final { return end(); }
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */