/**
 * audio_pcm_decoder.h
 *
 * Created on: March 09, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_PCM_DECODER_H_
#define AUDIO_PCM_DECODER_H_

#include "audio_decoder.h"

namespace com {
namespace nealrame {
namespace audio {
namespace codec {
class PCM_decoder: public decoder {
public:
	using decoder::decode;
	virtual std::shared_ptr<buffer> decode(std::ifstream &) const throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_PCM_DECODER_H_ */