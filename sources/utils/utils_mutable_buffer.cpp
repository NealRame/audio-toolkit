#include <algorithm>
#include <cassert>
#include <cstring>

#include "utils_mutable_buffer.h"

using namespace com::nealrame::utils;

mutable_buffer::mutable_buffer (size_t length, size_t capacity) {
	length_ = length;
	capacity_ = std::max(length_, capacity);
	data_ = capacity_ > 0 ? malloc(capacity_) : nullptr;
}

mutable_buffer::mutable_buffer (size_t length) :
	mutable_buffer(length, length) {
}

mutable_buffer::mutable_buffer () :
	mutable_buffer(static_cast<size_t>(0), static_cast<size_t>(0)) {
}

mutable_buffer::mutable_buffer (const void *data, size_t length, size_t capacity) :
	mutable_buffer(length, capacity) {
	memcpy(data_, data, length_);
}

mutable_buffer::mutable_buffer (const void *data, size_t length) :
	mutable_buffer(data, length, length) {
}

mutable_buffer::mutable_buffer (const abstract_buffer &other) :
	mutable_buffer(other.data(), other.length()) {
}

mutable_buffer::mutable_buffer (const mutable_buffer &other) :
	mutable_buffer(other.data(), other.length(), other.capacity()) {
}

mutable_buffer::mutable_buffer (mutable_buffer &&buffer) {
	length_ = buffer.length_;
	capacity_ = buffer.capacity_;
	data_ = buffer.data_;
	buffer.length_ = buffer.capacity_ = 0;
	buffer.data_ = nullptr;
}

mutable_buffer::~mutable_buffer () {
	if (data_) {
		free(data_);
	}
}

mutable_buffer & mutable_buffer::operator=(const abstract_buffer &other) {
	length_ = other.length();
	reserve(length_);
	if (data_ != nullptr) {
		memcpy(data_, other.data(), length_);
	}
	return *this;
}

mutable_buffer & mutable_buffer::operator=(const mutable_buffer &other) {
	length_ = other.length_;
	reserve(other.capacity_);
	if (data_ != nullptr) {
		memcpy(data_, other.data_, length_);
	}
	return *this;
}

void mutable_buffer::copy (const void *data, size_t length, size_t offset) {
	if ((offset + length) > capacity_) {
		enlarge((offset + length) - capacity_);
	}
	memcpy(static_cast<uint8_t *>(data_) + offset, data, length);
	length_ = std::max(length_, offset + length);
}

void mutable_buffer::fill (uint8_t value, size_t count, size_t offset) {
	if ((offset + count) > capacity_) {
		enlarge((offset + count) - capacity_);
	}
	memset(static_cast<uint8_t *>(data_) + offset, value, count);
	length_ = std::max(length_, offset + count);
}

void mutable_buffer::append (const void *data, size_t length) {
	copy(data, length, length_);
}

void mutable_buffer::append (const abstract_buffer &buf) {
	copy(buf.data(), buf.length(), length_);
}

void mutable_buffer::append (const std::string &s) {
	copy(s.c_str(), s.length() + 1, length_);
}

void mutable_buffer::assign (void *data, size_t length, size_t capacity) {
	if (data_) {
		free(data_);
	}
	data_ = data;
	capacity_ = capacity;
	length_ = length;
}

void mutable_buffer::assign (void *data, size_t length) {
	assign(data, length, length);
}

void mutable_buffer::reserve (size_t c) {
	capacity_ = c;
	length_ = std::min(length_, capacity_);
	data_ = realloc(data_, capacity_);
	if (capacity_ == 0) {
		data_ = nullptr;
	}
}

void mutable_buffer::enlarge (size_t c) {
	reserve(capacity_ + c);
}

void mutable_buffer::shrink (size_t c) {
	reserve(capacity_ < c ? 0 : capacity_ - c);
}

void mutable_buffer::swap (mutable_buffer &buffer) {
	std::swap(capacity_, buffer.capacity_);
	std::swap(data_, buffer.data_);
	std::swap(length_, buffer.length_);
}

buffer mutable_buffer::slice(size_t begin) {
	return buffer(*this).slice(begin);
}

buffer mutable_buffer::slice(size_t begin, size_t end) {
	return buffer(*this).slice(begin, end);
}