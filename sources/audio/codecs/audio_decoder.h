/// audio_decoder.h
/// 
/// Created on: April 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_DECODER_H_
#define AUDIO_DECODER_H_

#include <istream>
#include <string>

#include <audio/error>

namespace com {
namespace nealrame {
namespace audio {
class sequence;
namespace codec {
class decoder {
public:
	/// Decodes the given file.
	/// 
	/// *Parameters:*
	/// - `filepath`
	///   Path of the file to be decoded.
	///
	/// *Exceptions:*
	/// - `com::nealrame::audio::error`
	virtual sequence decode (const std::string &filepath) const throw(error) final;

	/// Decodes the given stream.
	///
	/// *Parameters:*
	/// - `stream`
	///   The stream to be decoded.
	///
	/// *Exceptions:*
	/// - `com::nealrame::audio::error`
	virtual sequence decode (std::istream &stream) const throw(error) final;

protected:
	virtual sequence decode_ (std::istream &) const throw(error) = 0;
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_DECODER_H_ */