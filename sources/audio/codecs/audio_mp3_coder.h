/*
 * audio_mp3_coder.h
 *
 * Created on: March 09, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_MP3_CODER_H_
#define AUDIO_MP3_CODER_H_

#include "audio_coder.h"

namespace com {
namespace nealrame {
namespace audio {
namespace codec {
class MP3_coder : public coder {
public:
	using coder::encode;
	virtual void encode(std::ofstream &, const audio::sequence &) const throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_MP3_CODEC_H_ */