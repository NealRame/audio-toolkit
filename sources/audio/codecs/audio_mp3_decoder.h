/// audio_mp3_decoder.h
/// Created on: April 26, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_MP3_DECODER_H_
#define AUDIO_MP3_DECODER_H_

#include "audio_decoder.h"

namespace com {
namespace nealrame {
namespace audio {
class buffer;
namespace codec {
class MP3_decoder: public decoder {
protected:
	virtual buffer decode_ (std::istream &) const throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_MP3_DECODER_H_ */