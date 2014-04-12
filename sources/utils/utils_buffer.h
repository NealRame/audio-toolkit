#ifndef BUFFER_H_
#define BUFFER_H_

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace com {
namespace nealrame {
namespace utils {
class buffer {
public:
	template <typename T> using iterator = T *;
	template <typename T>
		using const_iterator
			= typename std::remove_const<T>::type *;
public:
	template <typename Iterator>
	struct base_slice {
		Iterator first;
		Iterator last;
		size_t count () const {
			return last - first;
		}
		Iterator begin () {
			return first;
		}
		Iterator end () {
			return last;
		}
	};
	template <typename T> using slice = base_slice<iterator<T>>;
	template <typename T> using const_slice
		= base_slice<const_iterator<T>>;
public:
	buffer ();
	explicit buffer (size_t size);
	buffer (size_t size, size_t capacity);
	buffer (const void *data, size_t size);
	buffer (const void *data, size_t size, size_t capacity);
	buffer (const buffer &);
	buffer (buffer &&);
	virtual ~buffer ();
public:
	/// Copy assignment operator. 
	/// Replaces the contents with a copy of the contents of other.
	buffer & operator= (const buffer &);
	/// Move assignment operator. 
	/// Replaces the contents with those of `other` using move semantics
	/// (i.e. the data in other is moved from `other` into this `buffer`).
	/// `other` is in a valid but unspecified state afterwards.
	buffer & operator= (buffer &&);
public:
	/// Returns the current capacity of this `buffer`.
	size_t capacity () const noexcept {
		return capacity_;
	}
	/// Returns the number of elements of thie `buffer`.
	template <typename T>
	size_t count () const noexcept {
		return size_/sizeof(T);
	}
	void reserve (size_t new_cap);
	/// Reduces the [capacity] of this `buffer` to [size].
	void shrink_to_fit ();
	/// Returns the size of this `buffer`.
	size_t size () const noexcept {
		return size_;
	}
public:
	/// Appends the given raw data at the end of this `buffer`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void append (const void *data, size_t size);
	/// Appends the data of the given `buffer` at the end of this `buffer`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void append (const buffer &other);
	/// Removes all elements from this `buffer`.
	void clear () noexcept;
	/// Copies the given raw data into this `buffer` starting at the
	/// specified `offset`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void copy (const void *data, size_t size, size_t offset);
	/// Copies the given `buffer` into this `buffer` starting at the
	/// specified `offset`.
	/// If necessary, this `buffer` [`capacity`] can be increased in order
	/// to receive all the data.
	void copy (const buffer &other, size_t offset);
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
	template <typename T> 
	T * data () noexcept {
		return static_cast<T *>(data_);
	}
	/// Returns a pointer to const to the begining of the `buffer`.
	template <typename T>
	const T * data () const noexcept {
		return const_cast<buffer *>(this)->data<T>();
	}
public:
	/// Returns a `slice` of this `buffer` begining at given position
	/// `first` up to the given position `last`.
	template <typename T>
	slice<T> get_slice (size_t first, size_t last) {
		size_t n = count<T>();
		auto it = begin<T>();
		return slice<T> {
			it + std::min(first, n),
			it + std::min(last, n)
		};
	}
	/// Returns a `slice` of this `buffer` begining at given position
	/// `pos` up to last element.
	template <typename T>
	slice<T> get_slice (size_t pos) {
		size_t n = count<T>();
		auto it = begin<T>();
		return {
			it + std::min(pos, n),
			it + n
		};
	}
	/// Returns a `const_slice` of this `buffer` begining at given
	/// position `first` up to the given position `last`.
	template <typename T>
	const_slice<T> get_slice (size_t first, size_t last) const {
		return const_cast<buffer *>(this)->get_slice<T>(first, last);
	}
	/// Returns a `const_slice` of this `buffer` begining at given
	/// position `pos` up to last element.
	template <typename T>
	const_slice<T> get_slice (size_t pos) const {
		return const_cast<buffer *>(this)->get_slice<T>(pos);
	}

public:
	/// Returns an `iterator` to the begining of this `buffer`.
	template <typename T>
	iterator<T> begin () {
		return data<T>();
	}
	/// Returns a `const_iterator` to the begining of this `buffer`.
	template <typename T>
	const_iterator<T> begin () const {
		const_cast<buffer *>(this)->begin<T>();
	}
	/// Returns a `const_iterator` to the begining of this `buffer`.
	template <typename T>
	const_iterator<T> cbegin () const {
		const_cast<buffer *>(this)->begin<T>();
	}
	/// Returns an `iterator` to the element following the last element of
	/// this `buffer`.
	template <typename T>
	iterator<T> end () {
		return data<T>() + count<T>();
	}
	/// Returns an `iterator` to the element following the last element of
	/// this `buffer`.
	template <typename T>
	const_iterator<T> end () const {
		return const_cast<buffer *>(this)->end<T>();
	}
	/// Returns an `iterator` to the element following the last element of
	/// this `buffer`.
	template <typename T>
	const_iterator<T> cend () const {
		return const_cast<buffer *>(this)->end<T>();
	}
	/// Returns a reversed `iterator` to the last element of this
	/// `buffer`.
	template <typename T>
	std::reverse_iterator<iterator<T>> rbegin () {
		return std::reverse_iterator<iterator<T>>(end<T>());
	}
	/// Returns a reversed `const_iterator` to the last element of this
	/// `buffer`.
	template <typename T>
	std::reverse_iterator<const_iterator<T>> rbegin () const {
		return const_cast<buffer *>(this)->rbegin<T>();
	}
	/// Returns a reversed `const_iterator` to the last element of this
	/// `buffer`.
	template <typename T>
	std::reverse_iterator<const_iterator<T>> rcbegin () const {
		return const_cast<buffer *>(this)->rbegin<T>();
	}
	/// Returns a reversed `iterator` to the element preceding the first
	/// element of this `buffer`.
	template <typename T>
	std::reverse_iterator<iterator<T>> rend () {
		return std::reverse_iterator<iterator<T>>(begin<T>());
	}
	/// Returns a reversed `iterator` to the element preceding the first
	/// element of this `buffer`.
	template <typename T>
	std::reverse_iterator<const_iterator<T>> rend () const {
		return const_cast<buffer *>(this)->rend<T>();
	}
	/// Returns a reversed `const_iterator` to the element preceding the
	/// first element of this `buffer`.
	template <typename T>
	std::reverse_iterator<const_iterator<T>> rcend () const {
		return const_cast<buffer *>(this)->rend<T>();
	}
private:
	size_t capacity_;
	size_t size_;
	void *data_;
};
} /* namespace utils */
} /* namespace nealrame */
} /* namespace com */

#endif /* BUFFER_H_ */