/// utils_buffer.h
///
/// Created on: April 07, 2014
///     Author: [NealRame](mailto:contact@nealrame.com)
#ifndef UTILS_BUFFER_H_
#define UTILS_BUFFER_H_

#include <algorithm>
#include <functional>
#include <iterator>

namespace com {
namespace nealrame {
namespace utils {

class buffer {
public:
	using size_type = size_t;

	template <typename T> using pointer = T *;
	template <typename T> using pointer_to_const = const typename std::remove_const<T>::type *;

	template <typename T> using reference = T &;
	template <typename T> using const_reference = const typename std::remove_const<T>::type &;

	template <typename T> using iterator = pointer<T>;
	template <typename T> using const_iterator = pointer_to_const<T>;
	template <typename T> using reverse_iterator = std::reverse_iterator<iterator<T>>;
	template <typename T> using reverse_const_iterator = std::reverse_iterator<const_iterator<T>>;

public:
	/// Constructs an empty `buffer`.
	buffer ();

	/// Constructs a `buffer` with a given size.
	/// The capacity of the constructed `buffer` is equal to its size.
	/// The `buffer` data are left un-initialized.
	/// *parameters*:
	/// - `size`
	buffer (size_t size);

	/// Constructs a `buffer` with a given size and data.
	/// The capacity of the constructed `buffer` is equal to its size.
	/// The `buffer` data are initialized by copying the given data.
	/// *parameters*:
	/// - `data`
	/// - `size`
	buffer (const void *data, size_t size);

	/// Copy constructor
	buffer (const buffer &rhs);

	/// Move constructor
	buffer (buffer &&rhs);

	/// Destructor
	~buffer ();

public:
	/// Copy assignment operator. 
	/// Replaces the contents with a copy of the contents of other.
	/// *parameters:*
	/// - `other`
	buffer & operator= (const buffer &rhs);

	/// Move assignment operator. 
	/// Replaces the contents with those of `other` using move semantics
	/// (i.e. the data in other is moved from `other` into this `buffer`).
	/// `other` is in a valid but unspecified state afterwards.
	/// *parameters:*
	/// - `other`
	buffer & operator= (buffer &&rhs);

	/// Returns the count of element of type T this `buffer` can contains.
	template <typename T>
	size_type count () const noexcept
	{ return size_ / sizeof(T); }

	/// Returns the size of this `buffer`.
	size_type size () const noexcept
	{ return size_; }

	/// Set this `buffer` size to the given value.
	void resize (size_type size);
	
public:
	/// Appends the given data to this `buffer`.
	/// The size of this `buffer` is increased by the size of the given
	/// data.
	/// *Parameters:*
	/// - `data`
	/// - `size`
	void append (const void *data, size_t size);

	/// Appends the given 'buffer' to this `buffer`.
	/// The size of this `buffer` is increased by the size of the given
	/// `buffer`.
	/// *Parameters:*
	/// - `rhs`
	void append (const buffer &rhs)
	{ append (rhs.data<void>(), rhs.size()); }

	/// Copies the given raw data into this `buffer` starting at the
	/// specified `offset`.
	/// If necessary, the size of this `buffer` can be increased in order
	/// to receive all the data.
	/// *Parameters:*
	/// - `data`
	/// - `size`
	/// - `offset`
	void copy (const void *data, size_t size, size_t offset);

	/// Copies the given `buffer` into this `buffer` starting at the
	/// specified `offset`.
	/// If necessary, the size of this `buffer` can be increased in order
	/// to receive all the data.
	/// *Parameters:*
	/// - `rhs`
	/// - `offset`
	void copy (const buffer &rhs, size_t offset)
	{ return copy(rhs.data<void>(), rhs.size_, offset); }

public:
	/// Returns a pointer to the begining of the `buffer`.
	template <typename T>
	pointer<T> data () noexcept
	{ return reinterpret_cast<T *>(data_); }

	/// Returns a pointer to const to the begining of the `buffer`.
	template <typename T>
	pointer_to_const<T> data () const noexcept
	{ return const_cast<buffer *>(this)->data<T>(); }

public:
	/// Returns an `iterator` to the begining of this
	/// `buffer`.
	template <typename T>
	iterator<T> begin ()
	{ return data<T>(); }

	/// Returns a `const_iterator` to the begining of this
	/// `buffer`.
	template <typename T>
	const_iterator<T> begin () const
	{ return const_cast<buffer *>(this)->begin<T>(); }

	/// Returns a `const_iterator` to the begining of this
	/// `buffer`.
	template <typename T>
	const_iterator<T> cbegin () const
	{ return const_cast<buffer *>(this)->begin<T>(); }

	/// Returns an `iterator` to the element following the last
	/// element of this `buffer`.
	template <typename T>
	iterator<T> end ()
	{ return begin<T>() + count<T>(); }

	/// Returns a `const_iterator` to the element following the
	/// last element of this `buffer`.
	template <typename T>
	const_iterator<T> end () const
	{ return const_cast<buffer *>(this)->end<T>(); }

	/// Returns a `const_iterator` to the element following the
	/// last element of this `buffer`.
	template <typename T>
	const_iterator<T> cend () const
	{ return const_cast<buffer *>(this)->end<T>(); }

	/// Returns a reversed `iterator` to the last element of this
	/// `buffer`.
	template <typename T>
	reverse_iterator<T> rbegin ()
	{ return reverse_iterator<T>(end<T>()); }

	/// Returns a reversed `const_iterator` to the last element of
	/// this `buffer`.
	template <typename T>
	reverse_const_iterator<T> rbegin () const
	{ return const_cast<buffer *>(this)->rbegin<T>(); }

	/// Returns a reversed `const_iterator` to the last element of
	/// this `buffer`.
	template <typename T>
	reverse_const_iterator<T> rcbegin () const
	{ return const_cast<buffer *>(this)->rbegin<T>(); }

	/// Returns a reversed `iterator` to the element preceding the
	/// first element of this `buffer`.
	template <typename T>
	reverse_iterator<T> rend ()
	{ return reverse_iterator<T>(begin<T>()); }

	/// Returns a reversed `const_iterator` to the element
	/// preceding the first element of this `buffer`.
	template <typename T>
	reverse_const_iterator<T> rend () const
	{ return  const_cast<buffer *>(this)->rend<T>(); }

	/// Returns a reversed `const_iterator` to the element
	/// preceding the first element of this `buffer`.
	template <typename T>
	reverse_const_iterator<T> rcend () const
	{ return  const_cast<buffer *>(this)->rend<T>(); }

public:
	template <typename T>
	reference<T> at (size_type index)
	{ return *(begin<T>() + index); }

	template <typename T>
	const_reference<T> at (size_type index) const
	{ return const_cast<buffer *>(this)->at<T>(index); }

private:
	size_t size_;
	void * data_;
};
} /* namespace utils */
} /* namespace nealrame */
} /* namespace com */

#endif /* UTILS_BUFFER_H_ */