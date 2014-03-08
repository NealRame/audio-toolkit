#include "utils_buffer.h"

using namespace com::nealrame::utils;

buffer::buffer (void *data, size_t length) {
	set(data, length);
}

void buffer::copy (const void *data, size_t len, size_t offset) {
	if (offset < length_) {
		memcpy(static_cast<uint8_t *>(data_) + offset,
			data,
			std::min(len, length_ - offset));
	}
}

void buffer::fill (uint8_t value, size_t count, size_t offset) { 
	if (offset < length_) {
		memset(static_cast<uint8_t *>(data_) + offset,
			value,
			std::min(count, length_ - offset));
	}
}

void buffer::set (abstract_buffer &other) {
	set(other.data(), other.length());
}

void buffer::set(void *data, size_t length) {
	length_ = length;
	data_ = length_ ? data : nullptr;
}

buffer buffer::slice(size_t begin) {
	return slice(begin, length_ - 1);
}

buffer buffer::slice(size_t begin, size_t end) {
	end = std::min(end, length_ - 1);
	return buffer(static_cast<uint8_t *>(data_) + begin,
			begin > end ? end - begin : 0);
}
