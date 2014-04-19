/// audio_buffer.h
/// 
/// Created on: April 17, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)s

#include "audio_buffer.h"

using namespace com::nealrame::audio;

buffer::buffer (const class format &format) :
	format_(format) {
}

buffer::buffer (const class format &format, format::size_type frame_count) :
	format_(format),
	frames_(frame_count) {
}

buffer::buffer (const class format &format, double duration) :
	buffer(format, format.frame_count(duration)) {
}

buffer::buffer (const buffer &rhs) :
	format_(rhs.format()),
	frames_(rhs.frames_) {
}

buffer::buffer (buffer &&rhs) :
	format_(rhs.format()),
	frames_(std::move(rhs.frames_)) {
}

buffer & buffer::operator= (const buffer &rhs) {
	format_ = rhs.format();
	frames_ = rhs.frames_;
	return *this;
}

buffer & buffer::operator= (buffer &&rhs) {
	format_ = rhs.format();
	frames_ = std::move(rhs.frames_);
	return *this;
}

double buffer::duration () const noexcept {
	return format_.duration(frame_count());
}

format::size_type buffer::frame_count () const noexcept {
	return frames_.size()/format_.channel_count();
}

void buffer::append (const buffer &rhs) throw(error) {
	if (format_ != rhs.format()) {
		error::raise(error::FormatMismatched);
	}
	auto it = set_frame_count(frame_count() + rhs.frame_count());
	std::copy(rhs.begin(), rhs.end(), it);
}

void buffer::reserve (double duration) {
	reserve(format_.frame_count(duration));
}

void buffer::reserve (format::size_type frame_count) {
	frames_.reserve(frame_count*format_.channel_count());
}

buffer::frame_iterator buffer::set_duration (double duration) {
	return set_frame_count(format_.duration(duration));
}

buffer::frame_iterator buffer::set_frame_count (format::size_type frame_count) {
	reserve(frame_count);
	auto it = end();
	frames_.resize(frame_count*format_.channel_count());
	return it;
}
