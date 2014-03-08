#include <algorithm>
#include <limits>

#include "audio_error.h"
#include "audio_frame.h"

using namespace com::nealrame::audio;

frame::frame(const frame &other) :
	channel_count(other.channel_count),
	first_(other.first_),
	last_(other.last_) {
}

frame::frame (unsigned int channel_count, samples &samples) :
	channel_count(channel_count),
	first_(samples.begin<float>()),
	last_(first_ + channel_count) {
}

frame::frame (unsigned int channel_count, samples::iterator<float> first) :
	channel_count(channel_count),
	first_(first),
	last_(first_ + channel_count) {
}
