#ifndef UTILS_BUFFER_H_
#define UTILS_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include "utils_abstract_buffer.h"

namespace com {
namespace nealrame {
namespace utils {
/// com::nealrame::buffer
/// =====================
///
/// The `buffer` class provides a sage representation of a fixed size buffer. 
/// The content of the buffer can be altered but there is _no way to alter its
/// underlying storage area_ (address and/or length).
/// 
/// It does not own the underlying data, and so is cheap to copy or assign.
class buffer : public abstract_buffer {
public:
	// Create a buffer given an `abstract_buffer`.
	buffer (abstract_buffer &o) : buffer(o.data(), o.length()) { }
	// Create a buffer given an other `buffer`.
	buffer (buffer &&o) : buffer(static_cast<abstract_buffer &>(o)) { }
	// Create an empty buffer.
	buffer () : buffer(nullptr, 0) { }
	// Create a buffer given a raw memory location and its length.
	buffer (void *data, size_t length);
public:
	// Copy len bytes from data to this buffer starting at a given offset.
	virtual void copy (const void *data, size_t len, size_t offset);
	// Fill a given count of bytes to this buffer with a given byte value,
	// starting at a given offset.
	virtual void fill (uint8_t value, size_t count, size_t offset);
	// Replace the underlying data of this buffer with the ones from the
	// given `abstract_buffer`.
	void set (abstract_buffer &);
	// Replace the underlying data of this buffer with the given ones. 
	void set (void *data, size_t length);
	// Return a slice of this buffer starting at the given position up to
	// the end.
	//
	// **The underlying data of the return buffer is shared with this
	// buffer, modify the data of one buffer may also modify the data
	// of the other.**
	buffer slice(size_t begin);
	// Return a slice of this buffer starting at the given position up to
	// the given end position.
	//
	// **The underlying data of the return buffer is shared with this
	// buffer, modify the data of one buffer may also modify the data
	// of the other.**
	buffer slice(size_t begin, size_t end);
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILSBUFFER_H_ */