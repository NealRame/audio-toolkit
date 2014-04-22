/*
 * audio_dynamic_buffer.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef UTILS_DYNAMIC_BUFFER_H_
#define UTILS_DYNAMIC_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include <utils/base_buffer>
#include <utils/static_buffer>

namespace com {
namespace nealrame {
namespace utils {
/// com::nealrame::dynamic_buffer
/// =============================
///
/// The `dynamic_buffer` class provides a safe representation of a buffer.
/// The content and the storage area of such buffers can be altered.
/// 
/// As the underlying datas are owned of the buffer, assign a buffer to
/// another or create a buffer may cost some memory allocations and copies.
class dynamic_buffer : public buffer {
public:
	/// Create a buffer with a given length and a given capacity.
	dynamic_buffer (size_t length, size_t capacity);
	/// Create a buffer with a given length and a capacity equals to the
	/// length.
	explicit dynamic_buffer (size_t length);
	/// Create an empty buffer with no capacity.
	dynamic_buffer ();
	/// Create a buffer with a given size and a given capacity, and copy
	/// `length` bytes from the given data to this buffer.
	dynamic_buffer (const void *, size_t length, size_t capacity);
	/// Create a buffer with a given length and a capacity equals to the
	/// length and copy `length` bytes from the given data to this buffer.
	dynamic_buffer (const void *, size_t length);
	/// Create a buffer copying the data of the given buffer. The capacity
	/// is adjusted to the length of the data.
	dynamic_buffer (const buffer &);
	/// Create a buffer copying the data of the given buffer. The capacity
	/// is adjusted to the capacity of the given buffer.
	dynamic_buffer (const dynamic_buffer &);
	/// Move constructor.
	dynamic_buffer (dynamic_buffer &&);
	virtual ~dynamic_buffer ();
public:
	/// Copy data from a given `buffer` to this buffer. The
	/// capacity of this buffer will be adjusted to the size of the
	/// passed buffer.
	dynamic_buffer & operator=(const buffer &);
	/// Copy data from a given `dynamic_buffer` to this buffer. The
	/// capacity of this buffer will be adjusted to the capacity of
	/// the passed buffer.
	dynamic_buffer & operator=(const dynamic_buffer &);
public:
	/// Append the given data to this buffer, increasing its capacity if
	/// needed.
	void append (const void *data, size_t len);
	/// Append the given `string` with the null terminated character to
	/// this buffer, increasing its capacity if needed.
	void append (const std::string &);
	/// Append the given `buffer` to this buffer, increasing its
	/// capacity if needed.
	void append (const buffer &);
	/// Append an unitialized chunk of data at the end of this 
	/// `dynamic_buffer`. Increase the size by `length` bytes. Increase the
	/// capacity if needed. 
	void append (size_t length);
	/// Assign the given data. This buffer became responsible of the 
	/// passed data and will release memory location when destructed.
	void assign (void *data, size_t length, size_t capacity);
	/// Assign the given data. This buffer became responsible of the 
	/// passed data and will release memory location when destructed.
	void assign (void *data, size_t length);
	/// Copy `length` bytes from a given raw data pointer within this
	/// `dynamic_buffer` starting at offset `offset`. If needed, the
	/// capacity may increase.
	///
	/// Return the number of bytes being copied.
	virtual size_t copy (const void *, size_t length, size_t offset);
	/// Fill `length` bytes within this `dynamic_buffer` with a specified
	/// value `v` starting at offset `offset`. If needed, the capacity may 
	/// increase.
	///
	/// Return the number of bytes being copied.
	virtual size_t fill (uint8_t v, size_t length, size_t offset);
	/// Try to store in this `dynamic_buffer` starting at the given `offset` 
	/// at most `len` bytes read from the given input stream `in`.
	///
	/// Set this buffer length with the effective count of bytes read.
	/// Return the number of bytes read.
	virtual size_t fill (std::istream &in, size_t len, size_t offset = 0);
	/// Try to fill this `dynamic_buffer` starting at the given `offset` 
	/// with bytes read from the given input stream `in`.
	///
	/// Set this buffer length with the effective count of bytes read.
	/// Return the number of bytes read.
	virtual size_t fill (std::istream &in, size_t offset = 0);
	/// Set this `dynamic_buffer` length
	void set_length (size_t length) { length_ = length; }
	// Clear this buffer,  leaving its capacity as is.
	void clear () { length_ = 0; }
	/// Push back the given element at the end of this buffer, increasing
	/// its capacity if needed.
	template <typename T>
	void push_back(const T &value) {
		if (! (length_ < capacity_)) {
			enlarge(sizeof(T));
		}
		*(reinterpret_cast<T *>((uint8_t *)data_ + length_)) = value;
		length_ += sizeof(T);
	}
public:
	/// Get the current capacity of this buffer.
	size_t capacity () const { return capacity_; }
	/// Set this buffer capacity to the given value.
	/// If `c` is smaller than this buffer current length then elements
	/// beyond `c` will be lost.
	void reserve (size_t c);
	/// Increase this buffer capacity by the specified value.
	/// `b.extend(c)` is equivalent to calling `b.reserve(b.capacity()+c)`
	void enlarge (size_t c);
	/// Decrease current capacity by specified value.
	/// `b.shrink(c)` is equivalent to calling `b.reserve(b.capacity()-c)`
	void shrink (size_t c);
public:
	/// Return a slice begining at position `begin` up to the end of this
	/// of this `static_buffer`.
	///
	/// **The underlying data of the return `static_buffer` is shared with 
	/// this `static_buffer`, modify the data of one may also modify the 
	/// data of the other.**
	static_buffer slice(size_t begin);
	/// Return a slice begining at position `begin` up to the position
	/// `end` of this `static_buffer`.
	///
	/// **The underlying data of the return `static_buffer` is shared with 
	/// this `static_buffer`, modify the data of one may also modify the 
	/// data of the other.**
	static_buffer slice(size_t begin, size_t end);
public:
	void swap (dynamic_buffer &);
private:
	size_t capacity_;
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_DYNAMIC_BUFFER_H_ */