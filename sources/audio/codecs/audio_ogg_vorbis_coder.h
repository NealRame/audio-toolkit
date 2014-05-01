/// audio_ogg_vorbis_coder.h
/// Created on: May 1, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_OGG_VORBIS_CODER_H_
#define AUDIO_OGG_VORBIS_CODER_H_

#include <audio/codecs/coder>

namespace com {
namespace nealrame {
namespace audio {
class buffer;
namespace codec {
class OGGVorbis_coder : public coder {
protected:
	virtual void encode_ (std::ostream &, const buffer &) const
		throw(error);
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_OGG_VORBIS_CODEC_H_ */