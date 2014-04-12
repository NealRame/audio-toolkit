#ifndef BUFFER_H_
#define BUFFER_H_

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <type_traits>
#include <iostream>

namespace com {
namespace nealrame {
namespace utils {
template <typename T>
class buffer {
public:
	using iterator = T *;
	using const_iterator = typename std::remove_const<T>::type *;
public:
	/// Creates a `buffer` given a raw data area of `count` `T` elements.
	buffer (T *data, size_t count) :
		data_(data),
		count_(count) {
	}
	/// Creates a `buffer` given a raw data area with a given size of
	/// `len` bytes.
	buffer (void *data, size_t len) :
		buffer(static_cast<T *>(data), len/sizeof(T)) {
	}
	/// Creates a `buffer` based on an other one.
	buffer (buffer<T> &&other) :
		buffer(other.data_, other.count_) {
	}
	/// Creates an empty `buffer`.
	buffer () :
		buffer(static_cast<void *>(nullptr), 0) {
	}
public:
	/// Returns the size of this `buffer`.
	size_t size () const {
		return sizeof(T)*count_;
	}
	/// Returns the number of element of thie `buffer`.
	size_t count () const {
		return count_;
	}
public:
	/// Returns a pointer to the begining of the `buffer`.
	T * data () {
		return data_;
	}
	/// Returns a pointer to const to the begining of the `buffer`.
	const T * data () const {
		return const_cast<buffer<T> *>(this)->data();
	}
	/// Returns a pointer to the begining of the `buffer`.
	void * raw_data () {
		return data();
	}
	/// Returns a pointer to const to the begining of the `buffer`.
	const void * raw_data () const {
		return const_cast<buffer<T> *>(this)->data();	
	}
public:
	/// Copy the given `count` element from the specified `buffer` into
	/// this `buffer` starting at the given `offset`.
	/// Returns the number of elements actually copied.
	template<typename U>
	size_t copy (const buffer<U> &other, size_t offset) {
		if (offset < count_) {
			// size_t len = std::min(
			// 		size() - offset*sizeof(T),
			// 		other.size());

			// memcpy(, other.data_, count*sizeof(T));
			return count;
		}
		return 0;
	}
	/// Fill the given `count` of element of this `buffer` starting at the
	/// given `offset` with the specified `value`.
	/// Returns the number of elements actually written.
	size_t fill (T value, size_t count, size_t offset) {
		if (offset < count_) {
			count = std::min(count_ - offset, count);
			std::fill_n(begin() + offset, count, value);
			return count;
		}
		return 0;
	}
public:
	/// Return a slice begining at position `begin` up to the position
	/// `end` of this `static_buffer`.
	///
	/// _The underlying data of the return `static_buffer` is shared with
	/// this `static_buffer`, modify the data of one may also modify the
	/// data of the other._
	buffer<T> slice(size_t pos, size_t end) {
		end = std::min(count_, end);
		if (pos < end) {
			return buffer(data_ + pos, end - pos);
		}
		return buffer();
	}
	/// Return a slice begining at position `begin` up to the end of this
	/// of this `static_buffer`.
	///
	/// _The underlying data of the return `static_buffer` is shared with
	/// this `static_buffer`, modify the data of one may also modify the
	/// data of the other._
	buffer<T> slice(size_t pos) {
		return slice(pos, count_);
	}
public:
	/// Returns an `iterator` to the begining of this `buffer`.
	iterator begin () {
		return data();
	}
	/// Returns a `const_iterator` to the begining of this `buffer`.
	const_iterator begin () const {
		const_cast<buffer<T> *>(this)->begin();
	}
	/// Returns a `const_iterator` to the begining of this `buffer`.
	const_iterator cbegin () const {
		const_cast<buffer<T> *>(this)->begin();
	}
	/// Returns an `iterator` to the element following the last element of
	/// this `buffer`.
	iterator end () {
		return data() + count_;
	}
	/// Returns an `iterator` to the element following the last element of
	/// this `buffer`.
	const_iterator end () const {
		return const_cast<buffer<T> *>(this)->end();
	}
	/// Returns an `iterator` to the element following the last element of
	/// this `buffer`.
	const_iterator cend () const {
		return const_cast<buffer<T> *>(this)->end();
	}
	/// Returns a reversed `iterator` to the last element of this
	/// `buffer`.
	std::reverse_iterator<iterator> rbegin () {
		return std::reverse_iterator<iterator>(end() - 1);
	}
	/// Returns a reversed `iterator` to the last element of this
	/// `buffer`.
	std::reverse_iterator<const_iterator> rbegin () const {
		return const_cast<buffer<T> *>(this)->rbegin();
	}
	/// Returns a reversed `iterator` to the last element of this
	/// `buffer`.
	std::reverse_iterator<const_iterator> rcbegin () const {
		return const_cast<buffer<T> *>(this)->rbegin();
	}
	/// Returns a reversed `iterator` to the element preceding the first
	/// element of this `buffer`.
	std::reverse_iterator<iterator> rend () {
		return std::reverse_iterator<iterator>(begin() - 1);
	}
	/// Returns a reversed `iterator` to the element preceding the first
	/// element of this `buffer`.
	std::reverse_iterator<const_iterator> rend () const {
		return const_cast<buffer<T> *>(this)->rend();
	}
	/// Returns a reversed `iterator` to the element preceding the first
	/// element of this `buffer`.
	std::reverse_iterator<const_iterator> rcend () const {
		return const_cast<buffer<T> *>(this)->rend();
	}
private:
	T * data_;
	size_t count_;
};
} /* namespace utils */
} /* namespace nealrame */
} /* namespace com */

#endif /* BUFFER_H_ */