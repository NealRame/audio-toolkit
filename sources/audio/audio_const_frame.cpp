#include <algorithm>
#include <limits>

#include "audio_error.h"
#include "audio_frame.h"
#include "audio_const_frame.h"

using namespace com::nealrame::audio;

const_frame::const_frame (const frame &frame) :
	channel_count(frame.channel_count),
	first_(frame.begin()),
	last_(frame.end()) {
}

const_frame::const_frame (unsigned int channel_count, iterator first) :
	channel_count(channel_count),
	first_(first),
	last_(first_ + channel_count) {
}
