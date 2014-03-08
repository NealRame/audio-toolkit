#ifndef AUDIO_BUFFER_H_
#define AUDIO_BUFFER_H_

#include <functional>
#include <iterator>
#include <vector>

#include <utils/abstract_buffer>
#include <utils/mutable_buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/frame_container>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::buffer
/// 
/// The `buffer` class provides representation of an audio buffer. It is
/// nothing more than a sample container with an assign format descriptor.
class buffer : frame_container {
	class format format_;
	com::nealrame::utils::mutable_buffer frames_;
public:
	explicit buffer (class format);
	buffer (class format, unsigned int frame_count);
	buffer (class format, double duration);
	buffer (class format, com::nealrame::utils::abstract_buffer);
	virtual class format format () const;
	virtual double duration () const;
	virtual unsigned int frame_count () const;
	virtual frame frame_at (unsigned int frame_index);
	virtual iterator begin ();
	virtual iterator end ();
	virtual const_iterator begin () const;
	virtual const_iterator end () const;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_BUFFER_H_ */