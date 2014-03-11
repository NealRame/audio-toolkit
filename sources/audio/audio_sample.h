#ifndef AUDIO_SAMPLE_H_
#define AUDIO_SAMPLE_H_

#include <cstdint>
#include <limits>

namespace com {
namespace nealrame {
namespace audio {

template<typename T>
inline float value_to_sample (T v) {
	return static_cast<float>(v)/::std::numeric_limits<T>::max();
}

template<>
inline float value_to_sample<uint8_t> (uint8_t v) {
	return value_to_sample<int8_t>(static_cast<int8_t>(v));
}

template<>
inline float value_to_sample<uint16_t> (uint16_t v) {
	return value_to_sample<int16_t>(static_cast<int16_t>(v));
}

template<>
inline float value_to_sample<uint32_t> (uint32_t v) {
	return value_to_sample<int32_t>(static_cast<int32_t>(v));
}

template<>
inline float value_to_sample<float> (float v) {
	return v;
}

template<typename T>
inline T sample_to_value (float sample) {
	return static_cast<T>(
		sample*::std::numeric_limits<T>::max());
}

template<>
inline uint8_t sample_to_value (float sample) {
	return static_cast<uint8_t>(sample_to_value<int8_t>(sample));
}

template<>
inline int16_t sample_to_value (float sample) {
	return static_cast<int16_t>(sample_to_value<int16_t>(sample));
}

template<>
inline int32_t sample_to_value (float sample) {
	return static_cast<int32_t>(sample_to_value<int32_t>(sample));
}

template<>
inline float sample_to_value<float> (float sample) {
	return sample;
}

} // namespace audio
} // namespace nealrame
} // namespace com

#endif /* AUDIO_SAMPLE_H_ */