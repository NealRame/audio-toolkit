#include "audio_format.h"

using namespace com::nealrame::audio;

format::format (unsigned int channel_count, unsigned int frame_rate)
	throw(error) {
	if (channel_count < 1) {
		error::raise(error::FormatUnhandledChannelCountValueError);
	}
	channel_count_ = channel_count;
	set_frame_rate(frame_rate);
}

format & format::set_frame_rate (unsigned int rate) throw(error) {
	switch (rate) {
	case  8000:
	case 16000:
	case 22050:
	case 44100:
	case 48000:
	case 96000:
		frame_rate_ = rate;
		break;

	default:
		error::raise(error::FormatUnhandledSampleRateValueError);
	}
	return *this;
}

double format::duration (size_type frame_count) const noexcept {
	return static_cast<double>(
		frame_count/static_cast<double>(frame_rate_)
	);
}

format::size_type format::frame_count (double duration) const noexcept {
	return static_cast<double>(frame_rate_)*duration;
}

bool format::operator== (const format &other) const noexcept {
	return channel_count_ == other.channel_count_ 
		&& frame_rate_ == other.frame_rate_;
}