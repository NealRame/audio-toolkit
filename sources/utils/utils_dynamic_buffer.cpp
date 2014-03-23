#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>

#include "utils_dynamic_buffer.h"

using namespace com::nealrame::utils;

dynamic_buffer::dynamic_buffer (size_t length, size_t capacity) {
	length_ = length;
	capacity_ = std::max(length_, capacity);
	data_ = capacity_ > 0 ? malloc(capacity_) : nullptr;
}

dynamic_buffer::dynamic_buffer (size_t length) :
	dynamic_buffer(length, length) {
}

dynamic_buffer::dynamic_buffer () :
	dynamic_buffer(static_cast<size_t>(0), static_cast<size_t>(0)) {
}

dynamic_buffer::dynamic_buffer (const void *data, size_t length, size_t capacity) :
	dynamic_buffer(length, capacity) {
	memcpy(data_, data, length_);
}

dynamic_buffer::dynamic_buffer (const void *data, size_t length) :
	dynamic_buffer(data, length, length) {
}

dynamic_buffer::dynamic_buffer (const buffer &other) :
	dynamic_buffer(other.data(), other.length()) {
}

dynamic_buffer::dynamic_buffer (const dynamic_buffer &other) :
	dynamic_buffer(other.data(), other.length(), other.capacity()) {
}

dynamic_buffer::dynamic_buffer (dynamic_buffer &&buffer) {
	length_ = buffer.length_;
	capacity_ = buffer.capacity_;
	data_ = buffer.data_;
	buffer.length_ = buffer.capacity_ = 0;
	buffer.data_ = nullptr;
}

dynamic_buffer::~dynamic_buffer () {
	if (data_) {
		free(data_);
	}
}

dynamic_buffer & dynamic_buffer::operator=(const buffer &other) {
	length_ = other.length();
	reserve(length_);
	if (data_ != nullptr) {
		memcpy(data_, other.data(), length_);
	}
	return *this;
}

dynamic_buffer & dynamic_buffer::operator=(const dynamic_buffer &other) {
	length_ = other.length_;
	reserve(other.capacity_);
	if (data_ != nullptr) {
		memcpy(data_, other.data_, length_);
	}
	return *this;
}

size_t dynamic_buffer::copy (const void *data, size_t length, size_t offset) {
	if ((offset + length) > capacity_) {
		enlarge((offset + length) - capacity_);
	}
	memcpy(static_cast<uint8_t *>(data_) + offset, data, length);
	length_ = std::max(length_, offset + length);
	return length;
}

size_t dynamic_buffer::fill (uint8_t value, size_t count, size_t offset) {
	if ((offset + count) > capacity_) {
		enlarge((offset + count) - capacity_);
	}
	memset(static_cast<uint8_t *>(data_) + offset, value, count);
	length_ = std::max(length_, offset + count);
	return count;
}

size_t dynamic_buffer::fill (std::istream &in, size_t count, size_t offset) {
	if ((offset + count) > capacity_) {
		reserve(offset + count);
	}
	in.read(static_cast<char *>(data_) + offset, count);
	size_t n = in.gcount();
	length_ = std::max(length_, offset + n);
	return n;
}

size_t dynamic_buffer::fill (std::istream &in, size_t offset) {
	size_t n = 0;
	if (offset < capacity_) {
		in.read(static_cast<char *>(data_), capacity_ - offset);
		n = in.gcount();
		length_ = std::max(length_, offset + n);
	}
	return n;
}

void dynamic_buffer::append (const void *data, size_t length) {
	copy(data, length, length_);
}

void dynamic_buffer::append (const buffer &buf) {
	copy(buf.data(), buf.length(), length_);
}

void dynamic_buffer::append (const std::string &s) {
	copy(s.c_str(), s.length() + 1, length_);
}

void dynamic_buffer::append (size_t length) {
	if ((length_ + length) > capacity_) {
		enlarge(length);
	}
	length_ += length;
}

void dynamic_buffer::assign (void *data, size_t length, size_t capacity) {
	if (data_) {
		free(data_);
	}
	data_ = data;
	capacity_ = capacity;
	length_ = length;
}

void dynamic_buffer::assign (void *data, size_t length) {
	assign(data, length, length);
}

void dynamic_buffer::reserve (size_t c) {
	capacity_ = c;
	length_ = std::min(length_, capacity_);
	data_ = realloc(data_, capacity_);
	if (capacity_ == 0) {
		data_ = nullptr;
	}
}

void dynamic_buffer::enlarge (size_t c) {
	reserve(capacity_ + c);
}

void dynamic_buffer::shrink (size_t c) {
	reserve(capacity_ < c ? 0 : capacity_ - c);
}

void dynamic_buffer::swap (dynamic_buffer &buffer) {
	std::swap(capacity_, buffer.capacity_);
	std::swap(data_, buffer.data_);
	std::swap(length_, buffer.length_);
}

static_buffer dynamic_buffer::slice(size_t begin) {
	return static_buffer(*this).slice(begin);
}

static_buffer dynamic_buffer::slice(size_t begin, size_t end) {
	return static_buffer(*this).slice(begin, end);
}
