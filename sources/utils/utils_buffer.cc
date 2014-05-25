/// utils_buffer.cc
///
///

#include "utils_buffer.h"
#include <cstdlib>

using namespace com::nealrame::utils;

buffer::buffer () :
	size_(0),
	data_(nullptr) {
}

buffer::buffer (size_type size) :
	buffer() {
	resize(size);
}

buffer::buffer (const void *data, size_type size) :
	buffer(size) {
	memcpy(this->data<void>(), data, size);
}

buffer::buffer (const buffer &rhs) {
	*this = rhs;
}

buffer::buffer (buffer &&rhs) {
	*this = std::move(rhs);
}

buffer::~buffer () {
	if (data_ != nullptr) {
		free(data_);
	}
}

buffer & buffer::operator= (const buffer &rhs) {
	resize(rhs.size());
	memcpy(data<void>(), rhs.data<void>(), size_);
	return *this;
}

buffer & buffer::operator= (buffer &&rhs) {
	size_ = rhs.size();
	std::swap(data_, rhs.data_);
	return *this;
}

void buffer::resize (size_type size) {
	size_ = size;
	data_ = realloc(data_, size);
}

void buffer::append (const void *data, size_t size) {
	copy(data, size, size_);
}

void buffer::copy (const void *data, size_type size, size_type offset) {
	size_t new_size = offset + size;
	if (new_size > size_) {
		resize(new_size);
	}
	memcpy(this->data<int8_t>() + offset, data, size);
}
