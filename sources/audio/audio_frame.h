#ifndef AUDIO_FRAME_H_
#define AUDIO_FRAME_H_

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include <utils/buffer>
#include <audio/error>
#include <audio/format>
#include <audio/frame>
#include <audio/sample>

namespace com {
namespace nealrame {
namespace audio {

/// com::nealrame::audio::buffer
/// 
/// The `buffer` class provides representation of an audio buffer. It is 
/// nothing more than a sample container with an assign format descriptor.
class frame {
public:
	typedef com::nealrame::utils::buffer::iterator<float> iterator;
	typedef com::nealrame::utils::buffer::const_iterator<float> const_iterator;
public:
	frame (unsigned int channel_count, iterator first);
	frame () = delete;
public:
	size_t size () const { return channel_count*sizeof(float); }
public:
	iterator begin () { return first_; }
	iterator end () { return last_; }
	const_iterator begin () const { return first_; }
	const_iterator end () const { return last_; }
	const_iterator cbegin () const { return begin(); }
	const_iterator cend () const { return end(); }
public:
	template<typename Container>
	inline void write (Container c)
		throw(error) {
		write(c.begin(), c.end());
	}
	template<typename InputIterator>
	inline void write (InputIterator first, InputIterator last)
		throw(error) {
		typedef typename InputIterator::value_type value_type;
		auto it = first_;
		if ((last - first) != channel_count) {
			error::raise(error::FormatBadChannelCountValue);
		}
		::std::for_each(first, last, [&it](const value_type &v) {
			*it++ = value_to_sample<value_type>(v);
		});
	}
	template<typename Type>
	inline void write (const Type *first, const Type *last) 
		throw(error) {
		if ((last - first) != channel_count) {
			error::raise(error::FormatBadChannelCountValue);
		}
		auto it = first_;
		::std::for_each(first, last, [&it](const Type & v) {
			*it++ = value_to_sample<Type>(v);
		});
	}
	template<typename OutputIterator>
	inline void read (OutputIterator output) {
		typedef typename OutputIterator::value_type value_type;
		auto it = output;
		::std::for_each(first_, last_, [&it](const float &v) {
			*it++ = sample_to_value<value_type>(v);
		});
	}
	template<typename Type>
	inline void read (Type *a) {
		auto it = a;
		::std::for_each(first_, last_, [&it](const float &v) {
			*it++ = sample_to_value<Type>(v);
		});
	}
	template<typename T> frame & operator= (const std::vector<T> &v) {
		write<std::vector<T>>(v);
		return *this;
	}
	template<typename T> frame & operator= (const std::initializer_list<T> &v) {
		write<std::initializer_list<T>>(v);
		return *this;
	}
	frame & operator= (const frame &other) {
		write(other.begin(), other.end());
		return *this;
	}
	frame & operator= (const_frame &other) {
		write(other.begin(), other.end());
		return *this;
	}
public:
	const unsigned int channel_count;
private:
	iterator first_;
	iterator last_;
};

template<>
inline void frame::write<::std::initializer_list<float>::const_iterator> (
		::std::initializer_list<float>::const_iterator first,
		::std::initializer_list<float>::const_iterator last)
	throw(error) {
	if ((last - first) != channel_count) {
		error::raise(error::FormatBadChannelCountValue);
	}
	::std::copy(first, last, first_);
}

template<>
inline void frame::write<::std::vector<float>::const_iterator> (
		::std::vector<float>::const_iterator first,
		::std::vector<float>::const_iterator last)
	throw(error) {
	if ((last - first) != channel_count) {
		error::raise(error::FormatBadChannelCountValue);
	}
	::std::copy(first, last, first_);
}

template<>
inline void frame::write (float *first, float *last)
	throw(error) {
	if ((last - first) != channel_count) {
		error::raise(error::FormatBadChannelCountValue);
	}
	::std::copy(first, last, first_);
}

template<>
inline void frame::read<::std::vector<float>::iterator> (::std::vector<float>::iterator output) {
	::std::copy(first_, last_, output);
}

template<>
inline void frame::read<float *> (float *output) {
	::std::copy(first_, last_, output);
}

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_FRAME_H_ */