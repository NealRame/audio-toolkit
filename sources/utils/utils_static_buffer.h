/*
 * audio_static_buffer.h
 *
 * Created on: March 08, 2014
 *     Author: [NealRame](mailto:contact@nealrame.com)
 */
#ifndef UTILS_STATIC_BUFFER_H_
#define UTILS_STATIC_BUFFER_H_

#include <cstdint>
#include <exception> 
#include <iterator>
#include <string>

#include <utils/base_buffer>

namespace com {
namespace nealrame {
namespace utils {
/// com::nealrame::static_buffer
/// =====================
///
/// The `static_buffer` class provides an implementation of the buffer 
/// interface.
///
/// The content of the static_buffer can be altered but there is _no way to 
/// alter its underlying storage area_ (neither its adress nor its length).
/// 
/// It does not own the underlying data, and so is cheap to copy or assign.
class static_buffer : public buffer {
public:
	/// Create a static_buffer given a raw memory location and its length.
	static_buffer (void *data, size_t length);
	/// Create an empty static_buffer.
	static_buffer () : static_buffer(nullptr, 0) {
	}	
	/// Create a static_buffer given an other `static_buffer`.
	static_buffer (static_buffer &&o) : 
		static_buffer(o.data(), o.length()) {
	}
	/// Create a static_buffer given an `buffer`.
	static_buffer (buffer &o) : 
		static_buffer(o.data(), o.length()) {
	}
public:
	/// Copy `length` bytes from a given raw data pointer within this
	/// `static_buffer` starting at offset `offset`.
	///
	/// Return the number of bytes being copied.
	virtual size_t copy (const void *, size_t length, size_t offset);
	/// Fill `length` bytes within this `static_buffer` with a specified
	/// value `v` starting at offset `offset`.
	///
	/// Return the number of bytes being overwritten.
	virtual size_t fill (uint8_t v, size_t length, size_t offset);
	/// Replace the underlying data of this `static_buffer` with the
	/// underlying data from the given `buffer`.
	void set (buffer &);
	/// Replace the underlying data of this `static_buffer` with the
	/// specified ones.
	void set (void *data, size_t length);
	/// Return a slice begining at position `begin` up to the end of this
	/// of this `static_buffer`.
	///
	/// **The underlying data of the return `static_buffer` is shared with 
	/// this `static_buffer`, modify the data of one may also modify the 
	/// data of the other.**
	static_buffer slice(size_t pos);
	/// Return a slice begining at position `begin` up to the position
	/// `end` of this `static_buffer`.
	///
	/// **The underlying data of the return `static_buffer` is shared with 
	/// this `static_buffer`, modify the data of one may also modify the 
	/// data of the other.**
	static_buffer slice(size_t begin, size_t end);
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_STATIC_BUFFER_H_ */