/// audio_buffer.h
/// 
/// Created on: April 17, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)s

#include "audio_sequence.h"

using namespace com::nealrame::audio;

sequence::frame::frame(const frame &rhs)
{ *this = rhs; }

sequence::frame::frame(iterator first, iterator last)
{
	first_ = first; last_ = last;
}

sequence::frame & sequence::frame::operator=(const frame &rhs)
{
    std::copy(rhs.first_, rhs.last_, first_);
	return *this; 
}

format::size_type sequence::frame::channel_count() const
{ return last_ - first_; }

sequence::frame::reference sequence::frame::at(format::size_type channel)
{ return *(first_ + channel); }

sequence::frame::iterator sequence::frame::begin()
{ return first_; }

sequence::frame::iterator sequence::frame::end()
{ return last_; }

struct sequence::impl {
	class format format;
	std::vector<float> frames;
};

sequence::sequence(const class format &format) noexcept :
	d_(new impl{format})
{ }

sequence::sequence(const class format &format, format::size_type frame_count) :
	d_(new impl({format, std::vector<float>(2*frame_count)}))
{ }

sequence::sequence(const class format &format, double duration) :
	sequence(format, format.frame_count(duration))
{ }

sequence::sequence(const sequence &rhs)
{ *this = rhs; }

sequence::sequence(sequence &&rhs) noexcept :
	d_(std::move(rhs.d_))
{ }

sequence::~sequence()
{ }

sequence & sequence::operator=(const sequence &rhs)
{
	d_.reset(new impl{rhs.d_->format, rhs.d_->frames});
	return *this;
}

sequence & sequence::operator= (sequence &&rhs) noexcept
{
	d_ = std::move(rhs.d_);
	return *this;
}

void sequence::swap(sequence &rhs) noexcept
{ d_.swap(rhs.d_); }


class format & sequence::format() noexcept
{ return d_->format; }

double sequence::duration() const noexcept
{ return d_->format.duration(frame_count()); }

format::size_type sequence::frame_count() const noexcept
{ return d_->frames.size()/d_->format.channel_count(); }

format::size_type sequence::capacity () const noexcept
{ return d_->frames.capacity()/d_->format.channel_count(); }

sequence::frame sequence::at(format::size_type idx)
{
	format::size_type channel_count = d_->format.channel_count();
	auto it = d_->frames.data() + channel_count*idx;
	return frame(it, it + channel_count);
}

float * sequence::data(format::size_type index) noexcept
{ return d_->frames.data() + index*d_->format.channel_count(); }

const float * sequence::data(format::size_type index) const noexcept
{ return const_cast<sequence *>(this)->data(index); }

format::size_type sequence::copy(float *pcm, format::size_type count, format::size_type offset) const
{
	auto channel_count = d_->format.channel_count();

	offset = std::max(offset*channel_count, d_->frames.size());
	count = std::min(count*channel_count, d_->frames.size() - offset);

	auto it = d_->frames.begin() + offset, end = it + count;
	while (it != end) {
		*pcm++ = *it++;
	}

	return count;
}

format::size_type sequence::copy(float **pcm, format::size_type count, format::size_type offset) const
{
	auto channel_count = d_->format.channel_count();

	offset = std::min(offset*channel_count, d_->frames.size());
	count = std::min(count*channel_count, d_->frames.size() - offset);

	auto it = d_->frames.begin() + offset, end = it + count;
	while (it != end) {
		for (auto i = 0; i < channel_count; ++i) {
			*pcm[i]++ = *it++;
		}
	}

	return count;
}

void sequence::append(const float *pcm, format::size_type frame_count)
{
	for (int i = 0, count = d_->format.channel_count()*frame_count; i < count; ++i) {
		d_->frames.push_back(pcm[i]);
	}
}

void sequence::append(const float * const *pcm, format::size_type frame_count) {
	for (format::size_type j = 0; j < frame_count; ++j) {
		for (format::size_type i = 0, count = d_->format.channel_count(); i < count; ++i) {
			d_->frames.push_back(pcm[i][j]);
		}
	}
}

void sequence::append(const frame &frame) {
	for (auto sample: frame) {
		d_->frames.push_back(sample);
	}
}

void sequence::append(const sequence &rhs) throw(error) {
	for (auto sample: rhs.d_->frames) {
		d_->frames.push_back(sample);
	}
}

void sequence::reserve(double duration)
{ reserve(d_->format.frame_count(duration)); }

void sequence::reserve(format::size_type frame_count)
{ d_->frames.reserve(frame_count*d_->format.channel_count()); }

sequence::frame_iterator sequence::set_duration (double duration)
{ return set_frame_count(d_->format.frame_count(duration)); }

sequence::frame_iterator sequence::set_frame_count (format::size_type frame_count)
{
	reserve(frame_count);
	auto it = end();
	d_->frames.resize(frame_count*d_->format.channel_count());
	return it;
}

/// Returns a `frame_iterator` on the first frame of this `sequence`.
sequence::frame_iterator sequence::begin()
{ return frame_iterator(at(0)); }

/// Returns a `const_frame_iterator` on the first audio frame of this
/// `sequence`.
sequence::const_frame_iterator sequence::begin () const
{ return const_cast<sequence *>(this)->begin(); }

/// Returns a `const_frame_iterator` on the first audio frame of this
/// `sequence`.
sequence::const_frame_iterator sequence::cbegin () const
{ return const_cast<sequence *>(this)->begin(); }

/// Returns a `frame_iterator` on the frame following the last audio 
/// frame of this `sequence`.
sequence::frame_iterator sequence::end ()
{ return begin() + frame_count(); }

/// Returns a `const_frame_iterator` on the frame following the last
/// audio frame of this `sequence`.
sequence::const_frame_iterator sequence::end () const
{ return const_cast<sequence *>(this)->end(); }

/// Returns a `const_frame_iterator` on the frame following the last
/// audio frame of this `sequence`.
sequence::const_frame_iterator sequence::cend () const
{ return const_cast<sequence *>(this)->end(); }
