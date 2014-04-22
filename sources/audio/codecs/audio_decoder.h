/// audio_decoder.h
/// 
/// Created on: April 19, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_DECODER_H_
#define AUDIO_DECODER_H_

#include <fstream>
#include <memory>
#include <string>

#include <audio/error>

namespace com {
namespace nealrame {
namespace audio {
class buffer;
namespace codec {
class decoder {
public:
	virtual ~decoder () {}
public:
	virtual buffer decode (const std::string &) const throw(error);
	virtual buffer decode (std::ifstream &) const throw(error) = 0;
};
} /* namespace codec */
} /* namespace audio */
} /* namespace nealrame */
} /* namespace com */
#endif /* AUDIO_DECODER_H_ */