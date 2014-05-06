/// audio_wave_decoder.h
/// Created on: March 09, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_WAVE_DECODER_H_
#define AUDIO_WAVE_DECODER_H_

#include "audio_decoder.h"

namespace com {
namespace nealrame {
namespace audio {
class sequence;
namespace codec {
class WAVE_decoder: public decoder {
protected:
	virtual sequence decode_ (std::istream &) const throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_WAVE_DECODER_H_ */