#ifndef AUDIO_CONST_FRAME_H_
#define AUDIO_CONST_FRAME_H_

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include <utils/buffer>
#include <audio/error>
#include <audio/format>

namespace com {
namespace nealrame {
namespace audio {
class frame;
/// com::nealrame::audio::buffer
/// 
/// The `buffer` class provides representation of an audio buffer. It is 
/// nothing more than a sample container with an assign format descriptor.
class const_frame {
public:
	typedef com::nealrame::utils::buffer::const_iterator<float> iterator;
	typedef iterator const_iterator;
public:
	const_frame (const frame &);
	const_frame (const_frame &&);
	const_frame (unsigned int channel_count, iterator first);
	const_frame () = delete;
public:
	size_t size () const { return channel_count*sizeof(float); }
public:
	iterator begin () { return first_; }
	iterator end () { return last_; }
	iterator begin () const { return first_; }
	iterator end () const { return last_; }
	iterator cbegin () const { return begin(); }
	iterator cend () const { return end(); }
public:
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
public:
	const unsigned int channel_count;
private:
	iterator first_;
	iterator last_;
};

template<>
inline void const_frame::read<::std::vector<float>::iterator> (::std::vector<float>::iterator output) {
	::std::copy(first_, last_, output);
}

template<>
inline void const_frame::read<float *> (float *output) {
	::std::copy(first_, last_, output);
}

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_CONST_FRAME_H_ */