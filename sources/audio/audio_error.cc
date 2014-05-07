#include "audio_error.h"

using namespace com::nealrame::audio;

error::error (enum status s) :
	error(s, std::string()) {
}

error::error (enum status status, const std::string &msg) :
	status_(status),
	message_(msg) {
}

error::error (enum status status, std::string &&msg) :
	status_(status),
	message_(std::move(msg)) {
}

error::error (const error &other) {
	(*this) = other;
}

error::error (error &&other) {
	(*this) = other;
}

enum error::status error::status () const noexcept {
	return status_;
}

const char * error::what () const noexcept {
	return message_.data();
}

error & error::operator= (const error &other) {
	status_ = other.status_;
	message_ = other.message_;
	return *this;
}

error & error::operator= (error &&other) {
	status_ = other.status_;
	message_ = std::move(other.message_);
	return *this;
}
