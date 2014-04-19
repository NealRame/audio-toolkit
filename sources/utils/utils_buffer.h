/// utils_buffer.h
///
/// Created on: April 07, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef UTILS_BUFFER_H_
#define UTILS_BUFFER_H_

#include <algorithm>
#include <iterator>

namespace com {
namespace nealrame {
namespace utils {

class buffer {
public:
	using size_type = size_t;
	using iterator = uint8_t *;
	using const_iterator = const uint8_t *;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using reverse_const_iterator = std::reverse_iterator<const_iterator>;
public:
	/// Constructs an empty `buffer` with no capacity.
	buffer ();
	/// Constructs a `buffer` with a given size.
	/// The capacity of the constructed `buffer` is equal to its size.
	/// The `buffer` data are left un-initialized.
	/// *parameters*:
	/// - `size`
	buffer (size_t size);
	/// Constructs a `buffer` with a given size and capacity.
	/// The capacity will be equal to `max(size, capacity)`.
	/// The `buffer` data are are left un-initialized.
	/// *parameters*:
	/// - `size`
	/// - `capacity`
	buffer (size_t size, size_t capacity);
	/// Constructs a `buffer` with a given size and data.
	/// The capacity of the constructed `buffer` is equal to its size.
	/// The `buffer` data are initialized by copying the given data.
	/// *parameters*:
	/// - `data`
	/// - `size`
	buffer (const void *data, size_t size);
	/// Constructs a `buffer` with a given size capacity and data.
	/// The capacity will be equal to `max(size, capacity)`.
	/// The `buffer` data are initiliazed by copying the given data.
	/// *parameters*:
	/// - `data`
	/// - `size`
	buffer (const void *data, size_t size, size_t capacity);
	/// Copy constructor
	buffer (const buffer &other);
	/// Move constructor
	buffer (buffer &&other);
	/// Destructor
	~buffer ();
public:
	/// Copy assignment operator. 
	/// Replaces the contents with a copy of the contents of other.
	/// *parameters:*
	/// - `other`
	buffer & operator= (const buffer &other);
	/// Move assignment operator. 
	/// Replaces the contents with those of `other` using move semantics
	/// (i.e. the data in other is moved from `other` into this `buffer`).
	/// `other` is in a valid but unspecified state afterwards.
	/// *parameters:*
	/// - `other`
	buffer & operator= (buffer &&other);
public:
	/// Returns the current capacity of this `buffer`.
	size_t capacity () const noexcept 
	{ return capacity_; }
	/// Sets the capacity of this `buffer` to at least `new_cap`.
	/// New storage is allocated if necessary.
	/// If new_cap is greater than capacity(), all iterators and 
	/// references, including the past-the-end iterator, are invalidated.
	/// Otherwise, no iterators or references are invalidated.
	void reserve (size_t new_cap);
	/// Reduces the [capacity] of this `buffer` to [size].
	/// All iterators, including the past the end iterator, are
	/// potentially invalidated.
	void shrink_to_fit ();
	/// Returns the size of this `buffer`.
	size_t size () const noexcept
	{ return size_; }
public:
	/// Appends the given raw data at the end of this `buffer`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void append (const void *data, size_t size);
	/// Appends the data of the given `buffer` at the end of this `buffer`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void append (const buffer &other)
	{ append(other.data_, other.size_); }
	/// Removes all elements from this `buffer`.
	void clear () noexcept
	{ size_ = 0; }
	/// Copies the given raw data into this `buffer` starting at the
	/// specified `offset`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void copy (const void *data, size_t size, size_t offset);
	/// Copies the given `buffer` into this `buffer` starting at the
	/// specified `offset`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void copy (const buffer &other, size_t offset) {
		copy(other.data_, other.size_, offset);
	}
	/// Sets the given `count` of bytes starting at the specified `offset`
	/// in this buffer with the given value.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void fill (uint8_t value, size_t count, size_t offset);
	/// Resizes this `buffer` to the given `size`.
	/// If the current size is greater than the requested one, the
	/// `buffer` is reduced to its first count elements.
	void resize (size_t size);
	/// Resizes the container to contain count elements.
	/// If the current size is less than the requested one, additional
	/// elements are appended and initialized with the given `value`.
	/// If the current size is greater than the requested one, the
	/// `buffer` is reduced to its first count elements.
	void resize (size_t size, uint8_t value);
public:
	/// Returns a pointer to the begining of the `buffer`.
	void * data () noexcept 
	{ return data_; }
	/// Returns a pointer to const to the begining of the `buffer`.
	const void * data () const noexcept
	{ return const_cast<buffer *>(this)->data(); }
public:
	/// Returns an `iterator` to the begining of this
	/// `buffer`.
	iterator begin ()
	{ return static_cast<uint8_t *>(data()); }
	/// Returns a `const_iterator` to the begining of this
	/// `buffer`.
	const_iterator begin () const
	{ return const_cast<buffer *>(this)->begin(); }
	/// Returns a `const_iterator` to the begining of this
	/// `buffer`.
	const_iterator cbegin () const
	{ return const_cast<buffer *>(this)->begin(); }
	/// Returns an `iterator` to the element following the last
	/// element of this `buffer`.
	iterator end ()
	{ return static_cast<uint8_t *>(data_) + size_; }
	/// Returns a `const_iterator` to the element following the
	/// last element of this `buffer`.
	const_iterator end () const
	{ return const_cast<buffer *>(this)->end(); }
	/// Returns a `const_iterator` to the element following the
	/// last element of this `buffer`.
	const_iterator cend () const
	{ return const_cast<buffer *>(this)->end(); }
	/// Returns a reversed `iterator` to the last element of this
	/// `buffer`.
	reverse_iterator rbegin ()
	{ return reverse_iterator(end()); }
	/// Returns a reversed `const_iterator` to the last element of
	/// this `buffer`.
	reverse_const_iterator rbegin () const
	{ return const_cast<buffer *>(this)->rbegin(); }
	/// Returns a reversed `const_iterator` to the last element of
	/// this `buffer`.
	reverse_const_iterator rcbegin () const
	{ return const_cast<buffer *>(this)->rbegin(); }
	/// Returns a reversed `iterator` to the element preceding the
	/// first element of this `buffer`.
	reverse_iterator rend ()
	{ return reverse_iterator(begin()); }
	/// Returns a reversed `const_iterator` to the element
	/// preceding the first element of this `buffer`.
	reverse_const_iterator rend () const
	{ return  const_cast<buffer *>(this)->rend(); }
	/// Returns a reversed `const_iterator` to the element
	/// preceding the first element of this `buffer`.
	reverse_const_iterator rcend () const
	{ return  const_cast<buffer *>(this)->rend(); }
private:
	size_t capacity_;
	size_t size_;
	uint8_t *data_;
};
} /* namespace utils */
} /* namespace nealrame */
} /* namespace com */

#endif /* UTILS_BUFFER_H_ */