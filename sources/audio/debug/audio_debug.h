/// audio_debug.h
///
/// Created on: April 20, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef AUDIO_DEBUG_H_
#define AUDIO_DEBUG_H_

#include <ostream>
#include <audio/buffer>
#include <audio/format>

namespace com {
namespace nealrame {
namespace audio {

std::ostream & operator<< (std::ostream &stream, format fmt);
std::ostream & operator<< (std::ostream &stream, buffer::const_frame frame);

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_DEBUG_H_ */