/*
 * audio_error.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_ERROR_H_
#define AUDIO_ERROR_H_

#include <exception>
#include <ostream>
#include <string>

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::error
/// ============================
///
class error : public std::exception {
public:
	/// error status enumeration
	enum status {
		/// format errors
		FormatDoesNotMatch,
		FormatBadValue,
		FormatBadChannelCountValue,
		FormatBadSampleRateValue,
		FormatBadBitDepthValue,
		FormatUndefined,
		/// codec errors
		CodecNoSuitableCoderFound,
		CodecNoSuitableDecoderFound,
		CodecFormatError,
		CodecIOError,
		CodecUnexpectedError,
		/// misc errors
		NotImplemented,
		UnexpectedError,
	};
public:
	error (status);
	error (status, const char *);
	error (status, const std::string &);
public:
	/// Raise an error with a specified `error::status`.
	static void raise (status s) throw(error) {
		throw error(s);
	}
	/// Raise an error with the specified `error::status` a given error
	/// message.
	static void raise (status s, const char *msg) throw(error) {
		throw error(s, msg);
	}
	/// Raise an error with the specified `status` code and a given error
	/// message.
	static void raise (status s, const std::string &msg) throw(error) {
		throw error(s, msg);
	}
	static const char * status_string (enum status);
public:
	error & operator= (const error &other) noexcept;
	error & operator= (const std::exception &other) noexcept;
	std::string to_string () const noexcept;
	enum status status () const noexcept {
		return status_;
	}
	virtual const char * what () const noexcept {
		return message_.c_str();
	}
	std::ostream & operator<< (std::ostream &) const;
private:
	/// this error status.
	enum status status_;
	/// this error message.
	std::string message_;
};
#if defined(TEST)
void PrintTo (const enum error::status s, ::std::ostream *os);
#endif
} // namespace audio
} // namespace nealrame
} // namespace com

# endif /* AUDIO_ERROR_H_ */ 