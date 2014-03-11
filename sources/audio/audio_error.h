/*
 * audio_error.h
 * 
 * Created on: Dec 28, 2013
 *     Author: Neal.Rame. <contact@nealrame.com>
 */
#ifndef AUDIO_ERROR_H_
#define AUDIO_ERROR_H_

#if defined(TEST)
#	include <ostream>
#endif

namespace com {
namespace nealrame {
namespace audio {
/// com::nealrame::audio::format
/// ============================
///
/// 
class error {
public:
	enum status {
		// format errors
		FormatDoesNotMatch,
		FormatBadValue,
		FormatBadChannelCountValue,
		FormatBadSampleRateValue,
		FormatBadBitDepthValue,
		FormatUndefined,
		// codec errors
		CodecNoSuitableCoderFound,
		CodecNoSuitableDecoderFound,
		CodecFormatError,
		CodecIOError,
		CodecUnexpectedError,
		// misc errors
		NotImplemented,
	};
public:
	static void raise (status s) throw(error) {
		throw error{s, nullptr};
	}
	static void raise (status s, const char *msg) throw(error) {
		throw error{s, msg};
	}
public:
	const status status;
	const char *message;
};

#if defined(TEST)
	void PrintTo (const enum error::status s, ::std::ostream *os);
#endif

} // namespace audio
} // namespace nealrame
} // namespace com

# endif /* AUDIO_ERROR_H_ */ 