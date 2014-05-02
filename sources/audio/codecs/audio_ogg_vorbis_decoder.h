/// audio_ogg_vorbis_decoder.h
/// Created on: May 1, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_OGG_VORBIS_DECODER_H_
#define AUDIO_OGG_VORBIS_DECODER_H_

#include <audio/codecs/decoder>

namespace com {
namespace nealrame {
namespace audio {
class buffer;
namespace codec {
class OGGVorbis_decoder : public decoder {
protected:
	virtual buffer decode_ (std::istream &) const
		throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_OGG_VORBIS_DECODER_H_ */