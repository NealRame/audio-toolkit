// audio_wave_coder.h
// Created on: March 19, 2014
//     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_WAVE_CODER_H_
#define AUDIO_WAVE_CODER_H_

#include <audio/codecs/coder>

namespace com {
namespace nealrame {
namespace audio {
class buffer;
namespace codec {
class WAVE_coder : public coder {
public:
	using coder::encode;
	virtual void encode(std::ofstream &, const buffer &) const throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_WAVE_CODEC_H_ */