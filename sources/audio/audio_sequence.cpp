#include "audio_sequence.h"
#include "../utils/utils_buffer.h"

using namespace com::nealrame;
using namespace com::nealrame::audio;

struct sequence::impl {
	impl (class format format, utils::buffer &buffer) :
		format(format),
		raw_buffer(buffer) {
	}
	class format format;
	utils::buffer &raw_buffer;
};

sequence::sequence (const class format &fmt, utils::buffer &buffer) :
	pimpl_(std::unique_ptr<impl>(new impl(fmt, buffer))) {
}

sequence::sequence (sequence &&other) :
	pimpl_(std::unique_ptr<impl>(new impl(other.format(), other.data())) {
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

sequence sequence::subsequence (size_type frame_index, size_type count) {
	size_t begin = format_.size(static_cast<unsigned int>(frame_index));
	size_t end = begin + format_.size(static_cast<unsigned int>(count));
	utils::static_buffer slice = raw_buffer_.slice(begin, end);
	return sequence(format_, slice);
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
