#include "audio_error.h"

#if defined(TEST)
void com::nealrame::audio::PrintTo (const enum error::status s, ::std::ostream *os) {
	switch (s) {
		case error::FormatBadValue:
			*os << "error::FormatBadValue"; 
			break;
		case error::FormatBadChannelCountValue:
			*os << "error::FormatBadChannelCountValue";
			break;
		case error::FormatBadSampleRateValue:
			*os << "error::FormatBadSampleRateValue";
			break;
		case error::FormatBadBitDepthValue:
			*os << "error::FormatBadBitDepthValue";
			break;
		case error::FormatUndefined:
			*os << "error::FormatUndefined";
			break;
		case error::NotImplemented:
			*os << "error::NotImplemented";
			break;
		default:
			*os << (unsigned int)s;
			break;
	}

	if (message) {
		*os << endl << message;
	}
}
#endif