/// utils_buffer.cc
///
///

#include "utils_buffer.h"

#include <cstdlib>

using namespace com::nealrame::utils;

buffer::buffer () :
	capacity_(0),
	size_(0),
	data_(nullptr) { 
}

buffer::buffer (size_t size) :
	capacity_(size),
	size_(size) {
	data_ = static_cast<uint8_t *>(malloc(capacity_));
}

buffer::buffer (size_t size, size_t capacity) :
	capacity_(std::max(size, capacity)),
	size_(size) {
	data_ = static_cast<uint8_t *>(malloc(capacity_));
}

buffer::buffer (const void *data, size_t size) :
	buffer(size) {
	memcpy(data_, data, size);
}

buffer::buffer (const void *data, size_t size, size_t capacity) :
	buffer(size, capacity) {
	memcpy(data_, data, size);
}

buffer::buffer (const buffer &other) :
	buffer() {
	(*this) = other;
}

buffer::buffer (buffer &&other) :
	buffer() {
	(*this) = std::move(other); 
}

buffer::~buffer () {
	if (data_ != nullptr) {
		free(data_);
	}
}

buffer & buffer::operator= (const buffer &other) {
	reserve(other.size_);
	memcpy(data_, other.data_, size_);
	return *this;
}

buffer & buffer::operator= (buffer &&other) {
	this->~buffer();
	data_ = other.data_;
	size_ = other.size_;
	capacity_ = other.capacity_;
	other.data_ = nullptr;
	return *this;
}

void buffer::reserve (size_t new_cap) {
	if (new_cap > capacity_) {
		capacity_ = new_cap;
		data_ = static_cast<uint8_t *>(realloc(data_, capacity_));
	}
}
/// Reduces the [capacity] of this `buffer` to [size].
/// All iterators, including the past the end iterator, are
/// potentially invalidated.
void buffer::shrink_to_fit () {
	if (data_ != nullptr) {
		capacity_ = size_;
		data_ = static_cast<uint8_t *>(realloc(data_, capacity_));
	}
}

void buffer::append (const void *data, size_t size) {
	size_t new_size = size_ + size;
	if (new_size > capacity_) {
		reserve(new_size);
	}
	memcpy(static_cast<uint8_t *>(data_) + size_, data, size);
	size_ = new_size;
}

void buffer::copy (const void *data, size_t size, size_t offset) {
	size_t new_size = offset + size;
	if (new_size > capacity_) {
		reserve(new_size);
	}
	memcpy(static_cast<uint8_t *>(data_) + offset, data, size);
	size_ = std::max(size_, new_size);
}

void buffer::fill (uint8_t value, size_t count, size_t offset) {
	size_t new_size = offset + count;
	if (new_size > capacity_) {
		reserve(new_size);
	}
	memset(static_cast<uint8_t *>(data_) + offset,
			static_cast<int>(value), count);
	size_ = std::max(size_, new_size);
}

void buffer::resize (size_t size) {
	reserve(size);
	size_ = size;
}

void buffer::resize (size_t size, uint8_t value) {
	if (size > capacity_) {
		reserve(size);
	}
	if (size > size_) {
		memset(static_cast<uint8_t *>(data_) + size,
				value, size - size_);
	}
	size_ = size;
}
