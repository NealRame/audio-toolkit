#ifndef AUDIO_SEQUENCE_H_
#define AUDIO_SEQUENCE_H_

#include <functional>
#include <iterator>
#include <vector>

#include <utils/abstract_buffer>
#include <utils/buffer>
#include <audio/format>
#include <audio/frame>
#include <audio/frame_container>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::sequence
/// 
/// The `sequence` class provides representation of an audio buffer. It is
/// nothing more than a frame container with an assigned format descriptor.
class sequence : public frame_container {
	class format format_;
	com::nealrame::utils::buffer frames_;
public:
	sequence () = delete;
	sequence (const class format format, com::nealrame::utils::abstract_buffer &);
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