#include <algorithm>
#include <limits>

#include "audio_error.h"
#include "audio_frame.h"

using namespace com::nealrame::audio;

frame::frame (unsigned int channel_count, iterator first) :
	channel_count(channel_count),
	first_(first),
	last_(first_ + channel_count) {
}
