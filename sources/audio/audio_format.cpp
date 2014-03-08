/**
 * audio_format.cpp
 *
 * Created on: Dec 28, 2013
 *     Author: Neal.Rame. <contact@nealrame.com>
 */
#include "audio_format.h"

using namespace com::nealrame::audio;

format::format (
	unsigned int channel_count, 
	unsigned int sample_rate) throw(audio::error) {
	set_channel_count(channel_count);
	set_sample_rate(sample_rate);
}

bool format::operator==(const format &other) const {
	return channel_count_ == other.channel_count_
			&& sample_rate_ == other.sample_rate_;
}

format & format::set_channel_count (unsigned int count) throw(audio::error) {
	if (count < 1) {
		error::raise(error::FormatBadChannelCountValue);
	}
	channel_count_ = count;
	return *this;
}

format & format::set_sample_rate (unsigned int rate) throw(audio::error) {
	switch (rate) {
		case  8000:
		case 16000:
		case 22050:
		case 44100:
		case 48000:
		case 96000:
			sample_rate_ = rate;
			break;
		default:
			error::raise(error::FormatBadSampleRateValue);
	}
	return *this;
}

double format::duration (unsigned int frame_count) const {
	return static_cast<double>(frame_count/static_cast<double>(sample_rate_));
}

double format::duration (size_t size) const {
	return duration(frame_count(size));
}

unsigned int format::frame_count (size_t size) const {
	return size/(channel_count_*sizeof(float));
}

unsigned int format::frame_count (double duration) const {
	return static_cast<double>(sample_rate_)*duration;
}

size_t format::size (unsigned int frame_count) const {
	return static_cast<size_t>(frame_count*channel_count_*sizeof(float));
}

size_t format::size (double duration) const {
	return size(frame_count(duration));
}

#if defined(TEST)
void com::nealrame::audio::PrintTo (const format &fmt, ::std::ostream *os) {
	*os 	<< "{ "	<< "channels:"
			<< fmt.channel_count() << ", "
			<< "sample_rate: " << fmt.sample_rate()
		<< " }";
}
#endif