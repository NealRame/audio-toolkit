/// audio_error.h
/// 
/// Created on: April 14, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_CODEC_H_
#define AUDIO_CODEC_H_

#include <audio/codecs/coder>
#include <audio/codecs/decoder>

#include <string>
#include <memory>

namespace com {
namespace nealrame {
namespace audio {

std::shared_ptr<codec::coder> get_coder(const std::string &ext);
std::shared_ptr<codec::decoder> get_decoder(const std::string &ext);

sequence load_buffer(const std::string &filename);
void store_buffer(const std::string &filename, const sequence &);

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_CODEC_H_ */