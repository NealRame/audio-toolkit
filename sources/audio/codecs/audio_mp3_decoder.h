/*
 * audio_mp3_decoder.h
 *
 * Created on: March 18, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_MP3_DECODER_H_
#define AUDIO_MP3_DECODER_H_

#include "audio_decoder.h"

namespace com {
namespace nealrame {
namespace audio {
namespace codec {
class MP3_decoder: public decoder {
public:
	using decoder::decode;
	virtual std::shared_ptr<buffer> decode(std::ifstream &) const throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_MP3_DECODER_H_ */