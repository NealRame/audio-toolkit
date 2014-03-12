#include <audio/sequence>

using namespace com::nealrame::audio;

sequence::sequence (const class format &fmt, com::nealrame::utils::buffer &buffer) :
	format_(fmt),
	raw_buffer_(buffer) {
}

sequence::sequence (sequence &&other) :
	format_(other.format_),
	raw_buffer_(other.raw_buffer_.data(), other.size()) {
}

sequence::~sequence () {
}

class format sequence::format () const {
	return format_;
}

double sequence::duration () const {
	return format_.duration(raw_buffer_.length());
}

unsigned int sequence::frame_count () const {
	return format_.frame_count(raw_buffer_.length());
}

size_t sequence::size () const {
	return raw_buffer_.length();
}

frame sequence::frame_at (size_type frame_index) {
	return *(begin() + frame_index);
}

const_frame sequence::frame_at (size_type frame_index) const {
	return *(begin() + frame_index);
}

sequence::iterator sequence::begin () {
	return iterator(format_.channel_count(), raw_buffer_.begin<float>());
}

sequence::iterator sequence::end () {
	return begin() + frame_count();
}

sequence::const_iterator sequence::begin () const {
	return const_cast<sequence *>(this)->begin();
}

sequence::const_iterator sequence::end () const {
	return begin() + frame_count();
}
