#include "utils_static_buffer.h"

using namespace com::nealrame::utils;

static_buffer::static_buffer (void *source, size_t length) {
	set(source, length);
}

size_t static_buffer::copy (const void *source, 
				size_t length, size_t offset) {
	if (offset < length_) {
		length = std::min(length, length_ - offset);
		memcpy(static_cast<uint8_t *>(data_) + offset, source, length);
		return length;
	}
	return 0;
}

size_t static_buffer::fill (uint8_t value, size_t length, size_t offset) {
	if (offset < length_) {
		length = std::min(length, length_ - offset);
		memset(static_cast<uint8_t *>(data_) + offset, value, length);
		return length;
	}
	return 0;
}

void static_buffer::set (buffer &other) {
	set(other.data(), other.length());
}

void static_buffer::set(void *source, size_t length) {
	length_ = length;
	data_ = length_ ? source : nullptr;
}

static_buffer static_buffer::slice(size_t begin) {
	return slice(begin, length_ - 1);
}

static_buffer static_buffer::slice(size_t begin, size_t end) {
	end = std::min(end, length_ - 1);
	return static_buffer(static_cast<uint8_t *>(data_) + begin,
			begin > end ? end - begin : 0);
}
