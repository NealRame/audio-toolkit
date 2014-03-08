#ifndef UTILS_MUTABLE_BUFFER_H_
#define UTILS_MUTABLE_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include "utils_abstract_buffer.h"
#include "utils_buffer.h"

namespace com {
namespace nealrame {
namespace utils {
/// com::nealrame::mutable_buffer
/// =============================
///
/// The `mutable_buffer` class provides a safe representation of a buffer.
/// The content and the storage area of such buffers can be altered.
/// 
/// As the underlying datas are owned of the buffer, assign a buffer to
/// another or create a buffer may cost some memory allocations and copies.
class mutable_buffer : public abstract_buffer {
public:
	/// Create a buffer with a given length and a given capacity.
	mutable_buffer (size_t length, size_t capacity);
	/// Create a buffer with a given length and a capacity equals to the
	/// length.
	explicit mutable_buffer (size_t length);
	/// Create an empty buffer with no capacity.
	mutable_buffer ();
	/// Create a buffer with a given size and a given capacity, and copy
	/// `length` bytes from the given data to this buffer.
	mutable_buffer (const void *, size_t length, size_t capacity);
	/// Create a buffer with a given length and a capacity equals to the
	/// length and copy `length` bytes from the given data to this buffer.
	mutable_buffer (const void *, size_t length);
	/// Create a buffer copying the data of the given buffer. The capacity
	/// is adjusted to the length of the data.
	mutable_buffer (const abstract_buffer &);
	/// Create a buffer copying the data of the given buffer. The capacity
	/// is adjusted to the capacity of the given buffer.
	mutable_buffer (const mutable_buffer &);
	/// Move constructor.
	mutable_buffer (mutable_buffer &&);
	virtual ~mutable_buffer ();
public:
	/// Copy data from a given `abstract_buffer` to this buffer. The
	/// capacity of this buffer will be adjusted to the size of the
	/// passed buffer.
	mutable_buffer & operator=(const abstract_buffer &);
	/// Copy data from a given `mutable_buffer` to this buffer. The
	/// capacity of this buffer will be adjusted to the capacity of
	/// the passed buffer.
	mutable_buffer & operator=(const mutable_buffer &);
public:
	/// Get the current capacity of this buffer.
	size_t capacity () const { return capacity_; }
	/// Append the given data to this buffer, increasing its capacity if
	/// needed.
	void append (const void *data, size_t len);
	/// Append the given `string` with the null terminated character to
	/// this buffer, increasing its capacity if needed.
	void append (const std::string &);
	/// Append the given `abstract_buffer` to this buffer, increasing its
	/// capacity if needed.
	void append (const abstract_buffer &);
	/// Assign the given data. This buffer became responsible of the 
	/// passed data and will release memory location when destructed.
	void assign (void *data, size_t length, size_t capacity);
	/// Assign the given data. This buffer became responsible of the 
	/// passed data and will release memory location when destructed.
	void assign (void *data, size_t length);
	/// Copy len bytes from data to this buffer starting at a given 
	/// offset, increasing its capacity if needed.
	virtual void copy (const void *data, size_t len, size_t offset);
	/// Fill a given count of bytes to this buffer with a given byte
	/// value, starting at a given offset and increasing its capacity if
	/// needed.
	virtual void fill (uint8_t value, size_t count, size_t offset);
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
	/// Return a slice of this buffer starting at the given position up
	/// to the end.
	///
	/// **The underlying data of the return buffer is shared with this
	/// buffer, modify the data of one buffer may also modify the data
	/// of the other.**
	buffer slice(size_t begin);
	/// Return a slice of this buffer starting at the given position up
	/// to the given end position.
	///
	/// **The underlying data of the return buffer is shared with this
	/// buffer, modify the data of one buffer may also modify the data
	/// of the other.**
	buffer slice(size_t begin, size_t end);
public:
	void swap (mutable_buffer &);
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_MUTABLE_BUFFER_H_ */