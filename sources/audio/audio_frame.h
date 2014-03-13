/*
 * audio_frame.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
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

/// com::nealrame::audio::frame
/// 
/// The `frame` class provides representation of an audio buffer frame. 
/// An audio buffer frame contain a fixed number of audio samples. This number
/// is determined by the number of channel of the `sequence` containing the
/// audio frame.
class frame {
public:
	typedef com::nealrame::utils::buffer::iterator<float> iterator;
	typedef com::nealrame::utils::buffer::const_iterator<float> const_iterator;
public:
	frame (unsigned int channel_count, iterator first);
	frame () = delete;
public:
	/// Return the memory footprint of this `frame`.
	size_t size () const { return channel_count*sizeof(float); }
public:
	/// Return an iterator on the begining of this `frame`.
	iterator begin () { return first_; }
	/// Return an iterator on the end of this `frame`.
	iterator end () { return last_; }
	/// Return a constant iterator on the begining of this `frame`.	
	const_iterator begin () const { return first_; }
	/// Return a constant iterator on the end of this `frame`.
	const_iterator end () const { return last_; }
	/// Return a constant iterator on the begining of this `frame`.	
	const_iterator cbegin () const { return begin(); }
	/// Return a constant iterator on the end of this `frame`.
	const_iterator cend () const { return end(); }
public:
	/// Update this `frame` samples with value picked from the given
	/// container.
	/// The given container must contain at least `channel_count` values 
	/// convertible to float.
	template<typename Container>
	inline void write (Container c)
		throw(error) {
		write(c.begin(), c.end());
	}
	/// Update this `frame` samples with value picked from the given 
	/// iterators range.
	/// The given range must contain at least `channel_count` values
	/// convertible to float otherwise a `error::FormatBadChannelCountValue`
	/// is raised.
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
	/// Update this `frame` samples with value picked from the given 
	/// iterators range.
	/// The given range must contain at least `channel_count` values
	/// convertible to float otherwise a `error::FormatBadChannelCountValue`
	/// is raised.
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
	/// Output this this `frame` samples to the given `OutputIterator`.
	template<typename OutputIterator>
	inline void read (OutputIterator output) {
		typedef typename OutputIterator::value_type value_type;
		auto it = output;
		::std::for_each(first_, last_, [&it](const float &v) {
			*it++ = sample_to_value<value_type>(v);
		});
	}
	/// Output this `frame` samples to the given `Pointer` sequence.
	template<typename Type>
	inline void read (Type *a) {
		auto it = a;
		::std::for_each(first_, last_, [&it](const float &v) {
			*it++ = sample_to_value<Type>(v);
		});
	}
	/// Affect values picked from the given `std::vector` to this `frame`
	/// samples.
	template<typename T> frame & operator= (const std::vector<T> &v) {
		write<std::vector<T>>(v);
		return *this;
	}
	/// Affect values picked from the given `std::initializer_list` to this
	/// `frame` samples.
	template<typename T> frame & operator= (const std::initializer_list<T> &v) {
		write<std::initializer_list<T>>(v);
		return *this;
	}
	/// Affect sample picked from the given `frame` to this `frame`.
	/// Raise an `error::FormatBadChannelCountValue` if this `frame` and 
	/// the given one don't have the same number of samples.
	frame & operator= (const frame &other) {
		write(other.begin(), other.end());
		return *this;
	}
	/// Affect sample picked from the given `const_rame` to this `frame`.
	/// Raise an `error::FormatBadChannelCountValue` if this `frame` and 
	/// the given `const_frame` don't have the same number of samples.
	frame & operator= (const_frame &other) {
		write(other.begin(), other.end());
		return *this;
	}
public:
	/// The number of channel this `const_frame` contains.
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