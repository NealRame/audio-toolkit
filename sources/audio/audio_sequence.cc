/// audio_buffer.h
/// 
/// Created on: April 17, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)s

#include "audio_sequence.h"

using namespace com::nealrame::audio;

sequence::sequence (const class format &format) noexcept :
	format_(format) {
}

sequence::sequence (const class format &format, format::size_type frame_count) :
	format_(format),
	frames_(2*frame_count) {
}

sequence::sequence (const class format &format, double duration) :
	sequence(format, format.frame_count(duration)) {
}

sequence::sequence (const sequence &rhs) :
	format_(rhs.format()),
	frames_(rhs.frames_) {
}

sequence::sequence (sequence &&rhs) noexcept :
	format_(rhs.format()),
	frames_(std::move(rhs.frames_)) {
}

sequence & sequence::operator= (const sequence &rhs) {
	format_ = rhs.format();
	frames_ = rhs.frames_;
	return *this;
}

sequence & sequence::operator= (sequence &&rhs) noexcept {
	format_ = rhs.format();
	frames_ = std::move(rhs.frames_);
	return *this;
}

void sequence::swap (sequence &rhs) noexcept {
	std::swap(format_, rhs.format_);
	std::swap(frames_, rhs.frames_);
}

double sequence::duration () const noexcept {
	return format_.duration(frame_count());
}

format::size_type sequence::frame_count () const noexcept {
	return frames_.size()/format_.channel_count();
}

format::size_type sequence::capacity () const noexcept {
	return frames_.capacity()/format_.channel_count();
}

sequence::frame sequence::at (format::size_type idx) {
	format::size_type channel_count = format_.channel_count();
	auto it = frames_.begin() + channel_count*idx;
	return frame(it, it + channel_count);
}

float * sequence::data (format::size_type index) noexcept {
	return frames_.data() + index*format_.channel_count();
}

const float * sequence::data (format::size_type index) const noexcept {
	return const_cast<sequence *>(this)->data(index);
}

format::size_type sequence::copy (float *pcm, format::size_type count, format::size_type offset) const {
	auto channel_count = format_.channel_count();

	offset = std::max(offset*channel_count, frames_.size());
	count = std::min(count*channel_count, frames_.size() - offset);

	auto it = frames_.begin() + offset, end = it + count;
	while (it != end) {
		*pcm++ = *it++;
	}

	return count;
}

format::size_type sequence::copy (float **pcm, format::size_type count, format::size_type offset) const {
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

void sequence::append (const float *pcm, format::size_type frame_count) {
	for (int i = 0, count = format_.channel_count()*frame_count; i < count; ++i) {
		frames_.push_back(pcm[i]);
	}
}

void sequence::append (const float * const *pcm, format::size_type frame_count) {
	for (format::size_type j = 0; j < frame_count; ++j) {
		for (format::size_type i = 0, count = format_.channel_count(); i < count; ++i) {
			frames_.push_back(pcm[i][j]);
		}
	}
}

void sequence::append (const_frame frame) {
	for (auto sample: frame) {
		frames_.push_back(sample);
	}
}

void sequence::append (const sequence &rhs) throw(error) {
	for (auto sample: rhs.frames_) {
		frames_.push_back(sample);
	}
}

void sequence::reserve (double duration) {
	reserve(format_.frame_count(duration));
}

void sequence::reserve (format::size_type frame_count) {
	frames_.reserve(frame_count*format_.channel_count());
}

sequence::frame_iterator sequence::set_duration (double duration) {
	return set_frame_count(format_.frame_count(duration));
}

sequence::frame_iterator sequence::set_frame_count (format::size_type frame_count) {
	reserve(frame_count);
	auto it = end();
	frames_.resize(frame_count*format_.channel_count());
	return it;
}
