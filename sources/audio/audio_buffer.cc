/// audio_buffer.h
/// 
/// Created on: April 17, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)s

#include "audio_buffer.h"

using namespace com::nealrame::audio;

buffer::buffer (const class format &format) noexcept :
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

buffer::buffer (buffer &&rhs) noexcept :
	format_(rhs.format()),
	frames_(std::move(rhs.frames_)) {
}

buffer & buffer::operator= (const buffer &rhs) {
	format_ = rhs.format();
	frames_ = rhs.frames_;
	return *this;
}

buffer & buffer::operator= (buffer &&rhs) noexcept {
	format_ = rhs.format();
	frames_ = std::move(rhs.frames_);
	return *this;
}

void buffer::swap (buffer &rhs) noexcept {
	std::swap(format_, rhs.format_);
	std::swap(frames_, rhs.frames_);
}

double buffer::duration () const noexcept {
	return format_.duration(frame_count());
}

format::size_type buffer::frame_count () const noexcept {
	return frames_.size()/format_.channel_count();
}

format::size_type buffer::capacity () const noexcept {
	return frames_.capacity()/format_.channel_count();
}

buffer::frame buffer::at (format::size_type idx) {
	format::size_type channel_count = format_.channel_count();
	auto it = frames_.begin() + channel_count*idx;
	return frame(it, it + channel_count);
}

float * buffer::data (format::size_type index) noexcept {
	return frames_.data() + index*format_.channel_count();
}

const float * buffer::data (format::size_type index) const noexcept {
	return const_cast<buffer *>(this)->data(index);
}

format::size_type buffer::copy (float *pcm, format::size_type count, format::size_type offset) const {
	auto channel_count = format_.channel_count();

	offset = std::max(offset*channel_count, frames_.size());
	count = std::min(count*channel_count, frames_.size() - offset);

	auto it = frames_.begin() + offset, end = it + count;
	while (it != end) {
		*pcm++ = *it++;
	}

	return count;
}

format::size_type buffer::copy (float **pcm, format::size_type count, format::size_type offset) const {
	auto channel_count = format_.channel_count();

	offset = std::min(offset*channel_count, frames_.size());
	count = std::min(count*channel_count, frames_.size() - offset);

	auto it = frames_.begin() + offset, end = it + count;
	while (it != end) {
		for (auto i = 0; i < channel_count; ++i) {
			*pcm[i]++ = *it++;
		}
	}
	return count;
}

void buffer::append (const float *pcm, format::size_type frame_count) {
	for (int i = 0, count = format_.channel_count()*frame_count; i < count; ++i) {
		frames_.push_back(pcm[i]);
	}
}

void buffer::append (const float * const *pcm, format::size_type frame_count) {
	for (format::size_type j = 0; j < frame_count; ++j) {
		for (format::size_type i = 0, count = format_.channel_count(); i < count; ++i) {
			frames_.push_back(pcm[i][j]);
		}
	}
}

void buffer::append (const_frame frame) {
	for (auto sample: frame) {
		frames_.push_back(sample);
	}
}

void buffer::append (const buffer &rhs) throw(error) {
	for (auto sample: rhs.frames_) {
		frames_.push_back(sample);
	}
}

void buffer::reserve (double duration) {
	reserve(format_.frame_count(duration));
}

void buffer::reserve (format::size_type frame_count) {
	frames_.reserve(frame_count*format_.channel_count());
}

buffer::frame_iterator buffer::set_duration (double duration) {
	return set_frame_count(format_.frame_count(duration));
}

buffer::frame_iterator buffer::set_frame_count (format::size_type frame_count) {
	reserve(frame_count);
	auto it = end();
	frames_.resize(frame_count*format_.channel_count());
	return it;
}
