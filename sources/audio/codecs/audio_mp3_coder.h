/// audio_mp3_coder.h
/// Created on: April 26, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_MP3_CODER_H_
#define AUDIO_MP3_CODER_H_

#include <audio/codecs/coder>

namespace com {
namespace nealrame {
namespace audio {
class sequence;
namespace codec {
class MP3_coder : public coder {
protected:
	virtual void encode_ (std::ostream &, const sequence &) const
		throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_MP3_CODEC_H_ */