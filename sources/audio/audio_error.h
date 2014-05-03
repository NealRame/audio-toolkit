/// audio_error.h
/// 
/// Created on: April 14, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_ERROR_H_
#define AUDIO_ERROR_H_

#include <exception>
#include <string>

namespace com {
namespace nealrame {
namespace audio {
/// class com::nealrame::audio::error
/// =================================
class error : public std::exception {
public:
	enum status {
		CodecFormatError,
		CoderNotFound,
		CodecUnexpectedError,
		DecoderNotFound,
		FormatUnhandledChannelCountValueError,
		FormatUnhandledSampleRateValueError,
		FormatUnhandledSampleQuantificationValueError,
		FormatMismatchedError,
		IOError,
	};
public:
	static void raise (enum status s)
		throw(error) {
		throw error(s, std::string());
	}
	static void raise (enum status s, const char *msg)
		throw(error) {
		throw error(s, msg);
	}
	static void raise (enum status s, const std::string &msg)
		throw(error) {
		throw error(s, msg);
	}
	static void raise (enum status s, std::string &&msg)
		throw(error) {
		throw error(s, std::move(msg));
	}
public:
	error (status);
	error (status, const std::string &);
	error (status, std::string &&);
	error (const error &);
	error (error &&);
public:
	error & operator= (const error &other);
	error & operator= (error &&other);
public:
	virtual const char * what () const noexcept;
	enum status status () const noexcept;
private:
	enum status status_;
	std::string message_;
};
} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_ERROR_H_ */