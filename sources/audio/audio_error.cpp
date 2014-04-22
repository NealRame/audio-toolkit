#include "audio_error.h"

#include <sstream>

using namespace com::nealrame::audio;

error::error (enum status s) :
	status_(s) {
}

error::error (enum status s, const char *msg) :
	status_(s),
	message_(msg) {
}

error::error (enum status s, const std::string &msg) :
	status_(s),
	message_(msg) {
}

error & error::operator= (const error &other) noexcept {
	status_ = other.status_;
	message_ = other.what();
	return *this;
}

error & error::operator= (const std::exception &other) noexcept {
	status_ = status::UnexpectedError;
	message_ = other.what();
	return *this;
}

const char * error::status_string (enum status status) {
	switch (status) {
		case FormatDoesNotMatch:
			return "FormatDoesNotMatch";
		case FormatBadValue:
			return "FormatBadValue";
		case FormatBadChannelCountValue:
			return "FormatBadChannelCountValue";
		case FormatBadSampleRateValue:
			return "FormatBadSampleRateValue";
		case FormatBadBitDepthValue:
			return "FormatBadBitDepthValue";
		case FormatUndefined:
			return "FormatUndefined";
		case CodecNoSuitableCoderFound:
			return "CodecNoSuitableCoderFound";
		case CodecNoSuitableDecoderFound:
			return "CodecNoSuitableDecoderFound";
		case CodecFormatError:
			return "CodecFormatError";
		case CodecIOError:
			return "CodecIOError";
		case CodecUnexpectedError:
			return "CodecUnexpectedError";
		case NotImplemented:
			return "NotImplemented";
		case UnexpectedError:
			return "UnexpectedError";
	}
	return "UnknownError";
}

std::string error::to_string () const noexcept {
	std::ostringstream oss;
	oss << status_string(status_) << ": " << message_;
	return oss.str();
}

std::ostream & error::operator<< (std::ostream &stream) const {
	return (stream << to_string());
}

#if defined(TEST)
void PrintTo (const enum error::status s, ::std::ostream *os) {
	*os << status_string(s);
}
#endif