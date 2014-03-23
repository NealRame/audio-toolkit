/*
 * audio_sample.h
 *
 * Created on: March 11, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef AUDIO_SAMPLE_H_
#define AUDIO_SAMPLE_H_

#include <cstdint>
#include <cmath>
#include <limits>

namespace com {
namespace nealrame {
namespace audio {

template<typename T>
inline float value_to_sample (T v) {
	float max =
		std::fmax(
			std::fabs(std::numeric_limits<T>::min()),
			std::numeric_limits<T>::max());

	float f = static_cast<float>(v)/max;

	if (f > 1) {
		return 1.f;
	}

	if (f < -1) {
		return -1.f;
	}

	return f;
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
	float max =
		std::fmax(
			std::fabs(std::numeric_limits<T>::min()),
			std::numeric_limits<T>::max());

	sample = sample*max;

	if (sample > std::numeric_limits<T>::max()) {
		return std::numeric_limits<T>::max();
	}

	if (sample < std::numeric_limits<T>::min()) {
		return std::numeric_limits<T>::min();
	}

	return sample;
}

template<>
inline uint8_t sample_to_value (float sample) {
	return static_cast<uint8_t>(sample_to_value<int8_t>(sample));
}

template<>
inline uint16_t sample_to_value (float sample) {
	return static_cast<int16_t>(sample_to_value<int16_t>(sample));
}

template<>
inline uint32_t sample_to_value (float sample) {
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