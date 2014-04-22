#include "audio_buffer.h"
#include "audio_error.h"
#include "audio_frame.h"

using namespace com::nealrame;

struct audio::buffer::impl {
	class format format;
	com::nealrame::utils::dynamic_buffer raw_buffer;
};

audio::buffer::buffer (class format fmt) :
	pimpl_(new impl({fmt})) {
}

audio::buffer::buffer (class format fmt, unsigned int frame_count) :
	pimpl_(new impl({fmt})) {
	set_frame_count(frame_count);
}

audio::buffer::buffer (class format fmt, double duration) :
	pimpl_(new impl{fmt}) {
	set_duration(duration);
}

audio::buffer::~buffer () {
}

audio::format audio::buffer::format () const {
	return pimpl_->format;
}

double audio::buffer::duration () const {
	return pimpl_->format.duration(pimpl_->raw_buffer.length());
}

unsigned int audio::buffer::frame_count () const {
	return pimpl_->format.frame_count(pimpl_->raw_buffer.length());
}

void audio::buffer::append (double duration, bool clear) {
	utils::dynamic_buffer &buffer = pimpl_->raw_buffer;

	size_t size = format().size(duration);

	size_t old_size = buffer.length();
	size_t new_size = old_size + size;

	buffer.enlarge(size);
	buffer.set_length(new_size);

	if (clear) {
		buffer.fill(0, size, old_size);
	}
}

void audio::buffer::append (unsigned int frame_count, bool clear) {
	utils::dynamic_buffer &buffer = pimpl_->raw_buffer;

	size_t size = format().size(frame_count);
	size_t old_size = buffer.length();
	size_t new_size = old_size + size;

	buffer.enlarge(size);
	buffer.set_length(new_size);

	if (clear) {
		buffer.fill(0, size, old_size);
	}
}

void audio::buffer::append (const class sequence &seq) throw(error) {
	if (seq.format() != format()) {
		error::raise(error::FormatDoesNotMatch);
	}
	pimpl_->raw_buffer.append(seq.data());
}

void audio::buffer::cutdown (double duration) {
	pimpl_->raw_buffer.shrink(format().size(duration));
}

void audio::buffer::cutdown (unsigned int frame_count) {
	pimpl_->raw_buffer.shrink(format().size(frame_count));
}

void audio::buffer::set_duration (double time, bool clear) {
	double current = duration();
	if (time > current) {
		append(time - current, clear);
	} else {
		cutdown(current - time);
	}
}

void audio::buffer::set_frame_count (unsigned int count, bool clear) {
	unsigned int current = frame_count();
	if (count > current) {
		append(count - current, clear);
	} else {
		cutdown(current - count);
	}
}

com::nealrame::utils::buffer & audio::buffer::raw_buffer () {
	return pimpl_->raw_buffer;
}

const com::nealrame::utils::buffer & audio::buffer::raw_buffer () const {
	return const_cast<buffer *>(this)->raw_buffer();
}

class audio::sequence audio::buffer::sequence() {
	return audio::sequence(format(), raw_buffer());
}

const class audio::sequence audio::buffer::sequence() const {
	return const_cast<audio::buffer *>(this)->sequence();
}
