#include <audio/sequence>

using namespace com::nealrame::audio;

sequence::sequence (class format format, com::nealrame::utils::abstract_buffer &frames) :
	format_(format),
	frames_(frames) {
}

class format sequence::format () const {
	return format_;
}

double sequence::duration () const {
	return format_.duration(frame_count());
}

unsigned int sequence::frame_count () const {
	return frames_.count<float>()/format_.channel_count(); 
}

frame sequence::frame_at (unsigned int frame_index) {
	return *(begin() + frame_index);
}

sequence::iterator sequence::begin () {
	return iterator(format_.channel_count(), frames_.begin<float>());
}

sequence::iterator sequence::end () {
	return begin() + frame_count();
}

sequence::const_iterator sequence::begin () const {
	return const_iterator();
}

sequence::const_iterator sequence::end () const {
	return const_iterator();
}
