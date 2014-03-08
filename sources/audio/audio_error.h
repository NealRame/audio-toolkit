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
		FormatBadValue,
		FormatBadChannelCountValue,
		FormatBadSampleRateValue,
		FormatBadBitDepthValue,
		FormatUndefined,
		NotImplemented,
	};
public:
	static void raise (status s) throw(error) {
		throw error{s};
	}
public:
	const status status;
};

#if defined(TEST)
	void PrintTo (const enum error::status s, ::std::ostream *os);
#endif

} // namespace audio
} // namespace nealrame
} // namespace com

# endif /* AUDIO_ERROR_H_ */ 