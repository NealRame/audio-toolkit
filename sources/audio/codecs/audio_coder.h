/// audio_coder.h
/// 
/// Created on: April 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_CODER_H_
#define AUDIO_CODER_H_

#include <ostream>
#include <string>

#include <audio/error>

namespace com {
namespace nealrame {
namespace audio {
class sequence;
namespace codec {
class coder {
public:
	/// Encode the given sequence to the given filename.
	/// See `sequence` documentation for more details about `sequence`.
	virtual void encode (const std::string &, const sequence &) const
		throw(error) final;
	
	/// Encode the given sequence to the given output stream.
	/// See `sequence` documentation for more details about `sequence`.
	virtual void encode (std::ostream &, const sequence &) const
		throw(error) final;

protected:
	virtual void encode_ (std::ostream &, const sequence &) const 
		throw(error) = 0;
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_CODER_H_ */