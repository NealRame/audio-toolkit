#include "audio_buffer.h"
#include "audio_error.h"
#include "audio_frame.h"

using namespace com::nealrame::audio;

buffer::buffer (class format fmt) :
	format_(fmt) {
}

buffer::buffer (class format format, unsigned int frame_count) :
	format_(format),
	frames_(static_cast<size_t>(0), format_.size(frame_count)) {

}

buffer::buffer (class format format, double duration) :
	format_(format),
	frames_(static_cast<size_t>(0), format_.size(duration)) {
}

buffer::buffer (class format format, com::nealrame::utils::abstract_buffer frames) :
	format_(format),
	frames_(frames.length()) {
}

format buffer::format () const {
	return format_;
}

double buffer::duration () const {
	return format_.duration(frame_count());
}

unsigned int buffer::frame_count () const {
	return format_.frame_count(frames_.length());
}

frame buffer::frame_at(unsigned int frame_index) {
	return *(begin() + frame_index);
}

buffer::iterator buffer::begin () {
	return iterator();
	// return iterator(format_, frames_.begin<float>());
}

buffer::iterator buffer::end () {
	return iterator();
	// return begin() + frame_count();
}

buffer::const_iterator buffer::begin () const {
	return const_iterator();
}

buffer::const_iterator buffer::end () const {
	return const_iterator();
}