#include "utils_buffer.h"

#include <cstdlib>

#include <iostream>

using namespace com::nealrame::utils;

buffer::buffer () :
	capacity_(0),
	size_(0),
	data_(nullptr) {
}

buffer::buffer (size_t size) :
	capacity_(size),
	size_(size) {
	data_ = malloc(capacity_);
}

buffer::buffer (size_t size, size_t capacity) :
	capacity_(std::max(size, capacity)),
	size_(size) {
	data_ = malloc(capacity_);
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
	std::cerr << "copy assignment!" << std::endl;
	capacity_ = size_ = other.size_;
	reserve(capacity_);
	memcpy(data_, other.data_, size_);
	return *this;
}

buffer & buffer::operator= (buffer &&other) {
	std::cerr << "move assignment!" << std::endl;
	this->~buffer();
	data_ = other.data_;
	size_ = other.size_;
	capacity_ = other.capacity_;
	other.data_ = nullptr;
	return *this;
}

void buffer::reserve (size_t new_cap) {
	capacity_ = new_cap;
	size_ = std::min(size_, capacity_);
	if (capacity_ > 0) {
		data_ = realloc(data_, capacity_);
	} else if (data_ != nullptr) {
		free(data_);
		data_ = nullptr;
	}
}

void buffer::shrink_to_fit () {
	reserve(size_);
}

void buffer::resize (size_t size) {
	if (size > capacity_) {
		reserve(size);
	}
	size_ = size;
}

void buffer::resize (size_t size, uint8_t value) {
	if (size > capacity_) {
		reserve(size);
	}
	if (size > size_) {
		memset(data<uint8_t>() + size, value, size - size_);
	}
	size_ = size;
}

void buffer::append (const void *data, size_t size) {
	size_t new_size = size_ + size;
	if (new_size > capacity_) {
		reserve(new_size);
	}
	memcpy(this->data<uint8_t>() + size_, data, size);
	size_ = new_size;
}

void buffer::append (const buffer &other) {
	append(other.data_, other.size_);
}

void buffer::clear () noexcept {
	size_ = 0;
}

void buffer::copy (const void *data, size_t size, size_t offset) {
	size_t new_size = offset + size;
	if (new_size > capacity_) {
		reserve(new_size);
	}
	memcpy(this->data<uint8_t>() + offset, data, size);
	size_ = std::max(size_, new_size);
}

void buffer::copy (const buffer &other, size_t offset) {
	copy(other.data_, other.size_, offset);
}

void buffer::fill (uint8_t value, size_t count, size_t offset) {
	size_t new_size = offset + count;
	if (new_size > capacity_) {
		reserve(new_size);
	}
	memset(data<uint8_t>() + offset, static_cast<int>(value), count);
	size_ = std::max(size_, new_size);
}
