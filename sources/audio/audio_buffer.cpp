#include "audio_buffer.h"
#include "audio_error.h"
#include "audio_frame.h"

using namespace com::nealrame;

struct audio::buffer::impl {
	impl (audio::format fmt) :
		sequence(fmt, raw_buffer) {
	}
	impl (audio::format fmt, utils::buffer &raw) :
		raw_buffer(raw),
		sequence(fmt, raw_buffer) {
	}
	com::nealrame::utils::dynamic_buffer raw_buffer;
	class sequence sequence;
};

audio::buffer::buffer (class format fmt) :
	pimpl_(new impl(fmt)) {
}

audio::buffer::buffer (class format fmt, unsigned int frame_count) :
	pimpl_(new impl(fmt)) {
	reserve(frame_count);
}

audio::buffer::buffer (class format fmt, double duration) :
	pimpl_(new impl(fmt)) {
	reserve(duration);
}

audio::buffer::buffer (class format fmt, utils::buffer &raw) :
	pimpl_(new impl(fmt, raw)) {
}

audio::buffer::~buffer () {
}

audio::format audio::buffer::format () const {
	return pimpl_->sequence.format();
}

double audio::buffer::duration () const {
	return pimpl_->sequence.duration();
}

unsigned int audio::buffer::frame_count () const {
	return pimpl_->sequence.frame_count();
}

void audio::buffer::append (const_frame &f) throw(error) {
	if (f.channel_count != format().channel_count()) {
		error::raise(error::FormatDoesNotMatch);
	}
	pimpl_->raw_buffer.append(f.size());
	*(pimpl_->sequence.end() - 1) = f;
}

void audio::buffer::append (const class sequence &seq) throw(error) {
	if (seq.format() != format()) {
		error::raise(error::FormatDoesNotMatch);
	}
	pimpl_->raw_buffer.append(seq.raw_buffer());
}

size_t audio::buffer::capacity () const { 
	return pimpl_->raw_buffer.capacity(); 
}

void audio::buffer::reserve (double d) {
	pimpl_->raw_buffer.reserve(format().size(d));
}

void audio::buffer::reserve (unsigned int c) {
	pimpl_->raw_buffer.reserve(format().size(c));
}

void audio::buffer::enlarge (double d) {
	pimpl_->raw_buffer.enlarge(format().size(d));
}

void audio::buffer::enlarge (unsigned int c) {
	pimpl_->raw_buffer.enlarge(format().size(c));
}

void audio::buffer::shrink (double d) {
	pimpl_->raw_buffer.shrink(format().size(d));
}

void audio::buffer::shrink (unsigned int c) {
	pimpl_->raw_buffer.shrink(format().size(c));
}

com::nealrame::utils::buffer & audio::buffer::raw_buffer () {
	return pimpl_->raw_buffer;
}

const com::nealrame::utils::buffer & audio::buffer::raw_buffer () const {
	return const_cast<buffer *>(this)->raw_buffer();
}

class audio::sequence & audio::buffer::sequence() {
	return pimpl_->sequence;
}

const class audio::sequence & audio::buffer::sequence() const {
	return const_cast<audio::buffer *>(this)->sequence();
}
