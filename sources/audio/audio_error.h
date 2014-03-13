/*
 * audio_error.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_ERROR_H_
#define AUDIO_ERROR_H_

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::error
/// ============================
///
class error {
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
	};
public:
	/// Raise an error with a specified `error::status`.
	static void raise (status s) throw(error) {
		throw error{s, nullptr};
	}
	/// Raise an error with the specified `error::status` a given error
	/// message.
	static void raise (status s, const char *msg) throw(error) {
		throw error{s, msg};
	}
public:
	/// this error status.
	const status status;
	/// this error message.
	const char *message;
};
#if defined(TEST)
#include <ostream>
void PrintTo (const enum error::status s, ::std::ostream *os);
#endif
} // namespace audio
} // namespace nealrame
} // namespace com

# endif /* AUDIO_ERROR_H_ */ 