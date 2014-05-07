/// audio_debug.cc
///
/// Created on: April 20, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)

#include "audio_debug.h"

using namespace com::nealrame::audio;

std::ostream & com::nealrame::audio::operator<< (std::ostream &stream, format fmt) {
	stream << "channel count: " << fmt.channel_count();
	stream << "   frame rate: " << fmt.sample_rate();
	return stream;
}

std::ostream & com::nealrame::audio::operator<< (std::ostream &stream, sequence::const_frame frame) {
	std::ostream output(stream.rdbuf());
	output.setf(
		std::ios_base::dec
			| std::ios_base::fixed
			| std::ios_base::showpoint
			| std::ios_base::showpos
			| std::ios_base::right
	);
	output.precision(8);
#if defined(DEBUG) // of course it's defined !!!
	stream << "first_ = " << &(*frame.first_) << ", last_ = " << &(*frame.last_);
#endif
	for (unsigned int i = 0, count = frame.channel_count(); i < count; ++i) {
		stream << "; [" << i << "]=";
		output << frame[i];
		stream << ", ";
	}
	return stream;
}
