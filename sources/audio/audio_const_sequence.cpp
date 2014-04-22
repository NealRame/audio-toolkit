#include "../utils/utils_buffer.h"
#include "audio_const_sequence.h"


using namespace com::nealrame;
using namespace com::nealrame::audio;

struct const_sequence::impl {
	impl (class format fmt, const utils::buffer &buffer) :
		format(fmt),
		raw_buffer(buffer) {
	}
	class format format;
	const utils::buffer &raw_buffer;
};

const_sequence::const_sequence (const class format &fmt, const utils::buffer &buffer) :
	pimpl_(std::unique_ptr<impl>(new impl(fmt, buffer))) {
}

const_sequence::const_sequence (const sequence &other) :
	pimpl_(std::unique_ptr<impl>(new impl(other.format(), other.data())) {
}

const_sequence::const_sequence (const_sequence &&other) :
	pimpl_(std::unique_ptr<impl>(
		new impl(other.pimpl_->format, other.pimpl_->raw_buffer))) {
}

const_sequence::~const_sequence () {
}

class format const_sequence::format () const {
	return pimpl_->format;
}

double const_sequence::duration () const {
	return pimpl_->format.duration(pimpl_->raw_buffer.length());
}

unsigned int const_sequence::frame_count () const {
	return pimpl_->format.frame_count(pimpl_->raw_buffer.length());
}

size_t const_sequence::size () const {
	return pimpl_->raw_buffer.length();
}

const float * const_sequence::frames_buffer () {
	return static_cast<const float *>(pimpl_->raw_buffer.data());
}

const com::nealrame::utils::buffer & const_sequence::data () {
	return pimpl_->raw_buffer;
}

const_frame const_sequence::frame_at (size_type frame_index) {
	return *(begin() + frame_index);
}

const_sequence const_sequence::subsequence (size_type frame_index, size_type count) {
	size_t begin = pimpl_->format.size(static_cast<unsigned int>(frame_index));
	size_t end = begin + pimpl_->format.size(static_cast<unsigned int>(count));
	utils::static_buffer slice = pimpl_->raw_buffer.slice(begin, end);
	return const_sequence(pimpl_->format, slice);
}

const_sequence::iterator const_sequence::begin () {
	return iterator(
		pimpl_->format.channel_count(),
		pimpl_->raw_buffer.begin<float>()
	);
}

const_sequence::iterator const_sequence::end () {
	return begin() + frame_count();
}
