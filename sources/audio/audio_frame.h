#ifndef AUDIO_FRAME_H_
#define AUDIO_FRAME_H_

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include <utils/abstract_buffer>
#include <utils/buffer>
#include <audio/error>
#include <audio/format>

namespace com {
namespace nealrame {
namespace audio {

using samples = com::nealrame::utils::abstract_buffer;

/// com::nealrame::audio::buffer
/// 
/// The `buffer` class provides representation of an audio buffer. It is 
/// nothing more than a sample container with an assign format descriptor.
class frame {
public:
	const unsigned int channel_count;
public:
	typedef com::nealrame::utils::abstract_buffer::iterator<float> iterator;
	typedef com::nealrame::utils::abstract_buffer::const_iterator<float> const_iterator;
public:
	frame (const frame &);
	frame (unsigned int channel_count, samples &samples);
	frame (unsigned int channel_count, samples::iterator<float> first);
	frame () = delete;
public:
	iterator begin () { return first_; }
	iterator end () { return last_; }
	const_iterator begin () const { return first_; }
	const_iterator end () const { return last_; }
	const_iterator cbegin () const { return begin(); }
	const_iterator cend () const { return end(); }
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
			*it++ = v/::std::numeric_limits<value_type>::max();
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
			*it++ = v/::std::numeric_limits<Type>::max();
		});
	}
	template<typename OutputIterator>
	inline void read (OutputIterator output) {
		typedef typename OutputIterator::value_type value_type;
		auto it = output;
		::std::for_each(first_, last_, [&it](const float &v) {
			*it++ = v*std::numeric_limits<value_type>::max();
		});
	}
	template<typename Type>
	inline void read (Type *a) {
		auto it = a;
		::std::for_each(first_, last_, [&it](const float &v) {
			*it++ = v*std::numeric_limits<Type>::max();
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
private:
	iterator first_;
	iterator last_;
};

template <typename T>
struct frame_iterator;

template <>
struct frame_iterator<frame> {
	typedef frame::iterator type;
};

template <>
struct frame_iterator<frame const> {
	typedef frame::const_iterator type;
};

template<>
inline void frame::write<::std::initializer_list<uint8_t>::const_iterator> (
		::std::initializer_list<uint8_t>::const_iterator first,
		::std::initializer_list<uint8_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::initializer_list<uint16_t>::const_iterator> (
		::std::initializer_list<uint16_t>::const_iterator first,
		::std::initializer_list<uint16_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::initializer_list<int32_t>::const_iterator> (
		::std::initializer_list<int32_t>::const_iterator first,
		::std::initializer_list<int32_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::initializer_list<uint32_t>::const_iterator> (
		::std::initializer_list<uint32_t>::const_iterator first,
		::std::initializer_list<uint32_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::initializer_list<uint64_t>::const_iterator> (
		::std::initializer_list<uint64_t>::const_iterator first,
		::std::initializer_list<uint64_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::initializer_list<int64_t>::const_iterator> (
		::std::initializer_list<int64_t>::const_iterator first,
		::std::initializer_list<int64_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::vector<uint8_t>::const_iterator> (
		::std::vector<uint8_t>::const_iterator first,
		::std::vector<uint8_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::vector<uint16_t>::const_iterator> (
		::std::vector<uint16_t>::const_iterator first,
		::std::vector<uint16_t>::const_iterator last)
	throw(error) = delete;

template<>
inline void frame::write<::std::vector<uint32_t>::const_iterator> (
		::std::vector<uint32_t>::const_iterator first,
		::std::vector<uint32_t>::const_iterator last)
	throw(error) = delete;

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
inline void frame::write (uint8_t *first, uint8_t *last)
	throw(error) = delete;

template<>
inline void frame::write (uint16_t *first, uint16_t *last)
	throw(error) = delete;

template<>
inline void frame::write (int32_t *first, int32_t *last)
	throw(error) = delete;

template<>
inline void frame::write (uint32_t *first, uint32_t *last)
	throw(error) = delete;

template<>
inline void frame::write (int64_t *first, int64_t *last)
	throw(error) = delete;

template<>
inline void frame::write (uint64_t *first, uint64_t *last)
	throw(error) = delete;

template<>
inline void frame::write (float *first, float *last)
	throw(error) {
	if ((last - first) != channel_count) {
		error::raise(error::FormatBadChannelCountValue);
	}
	::std::copy(first, last, first_);
}

template<>
inline void frame::read<::std::vector<uint8_t>::iterator> (
		::std::vector<uint8_t>::iterator output)
	= delete;

template<>
inline void frame::read<::std::vector<uint16_t>::iterator> (
		::std::vector<uint16_t>::iterator output)
	= delete;

template<>
inline void frame::read<::std::vector<int32_t>::iterator> (
		::std::vector<int32_t>::iterator output)
	= delete;

template<>
inline void frame::read<::std::vector<uint32_t>::iterator> (
		::std::vector<uint32_t>::iterator output) 
	= delete;

template<>
inline void frame::read<::std::vector<int64_t>::iterator> (
		::std::vector<int64_t>::iterator output)
	= delete;

template<>
inline void frame::read<::std::vector<uint64_t>::iterator> (
		::std::vector<uint64_t>::iterator output) 
	= delete;

template<>
inline void frame::read<::std::vector<float>::iterator> (::std::vector<float>::iterator output) {
	::std::copy(first_, last_, output);
}

template<>
inline void frame::read<uint8_t> (uint8_t *output)
	= delete;

template<>
inline void frame::read<uint16_t> (uint16_t *output)
	= delete;

template<>
inline void frame::read<int32_t> (int32_t *output)
	= delete;

template<>
inline void frame::read<uint32_t> (uint32_t *output)
	= delete;

template<>
inline void frame::read<int64_t> (int64_t *output)
	= delete;

template<>
inline void frame::read<uint64_t> (uint64_t *output)
	= delete;

template<>
inline void frame::read<float *> (float *output) {
	::std::copy(first_, last_, output);
}

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */