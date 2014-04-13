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
	/// Sets the capacity of this `buffer` to at least `new_cap`.
	/// New storage is allocated if necessary.
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
	template <typename T>
	struct slice {
		using iterator = T *;
		using const_iterator = typename std::remove_const<T>::type *;

		/// Returns the number of elements of this `slice`.
		size_t count () const 
		{ return last - first + 1; }

		/// Returns an `iterator` to the begining of this `slice`.
		iterator begin ()
		{ return first; }
		/// Returns a `const_iterator` to the begining of this 
		/// `slice`.
		const_iterator begin () const
		{ return const_cast<slice *>(this)->begin(); }
		/// Returns a `const_iterator` to the begining of this 
		/// `slice`.
		const_iterator cbegin () const
		{ return const_cast<slice *>(this)->begin(); }

		/// Returns an `iterator` to the element following the last
		/// element of this `slice`.
		iterator end ()
		{ return last + 1; }
		/// Returns a `const_iterator` to the element following the
		/// last element of this `slice`.
		const_iterator end () const
		{ return const_cast<slice *>(this)->end(); }
		/// Returns a `const_iterator` to the element following the
		/// last element of this `slice`.
		const_iterator cend () const
		{ return const_cast<slice *>(this)->end(); }

		/// Returns a reversed `iterator` to the last element of this
		/// `slice`.
		std::reverse_iterator<const_iterator> rbegin ()
		{ return std::reverse_iterator<iterator>(end()); }
		/// Returns a reversed `const_iterator` to the last element of
		/// this `slice`.
		std::reverse_iterator<const_iterator> rbegin () const
		{ return const_cast<slice *>(this)->rbegin(); }
		/// Returns a reversed `const_iterator` to the last element of
		/// this `slice`.
		std::reverse_iterator<const_iterator> rcbegin () const
		{ return const_cast<slice *>(this)->rbegin(); }

		/// Returns a reversed `iterator` to the element preceding the
		/// first element of this `slice`.
		std::reverse_iterator<iterator> rend ()
		{ return std::reverse_iterator<iterator>(begin()); }
		/// Returns a reversed `const_iterator` to the element
		/// preceding the first element of this `slice`.
		std::reverse_iterator<const_iterator> rend () const
		{ return  const_cast<slice *>(this)->rend(); }
		/// Returns a reversed `const_iterator` to the element
		/// preceding the first element of this `slice`.
		std::reverse_iterator<const_iterator> rcend () const
		{ return  const_cast<slice *>(this)->rend(); }

		iterator first;
		iterator last;
	};
	template <typename T>
	struct const_slice {
		using iterator = typename std::remove_const<T>::type *;
		using const_iterator = iterator;

		/// Returns the number of elements of this `const_slice`.
		size_t count () const 
		{ return last - first; }

		/// Returns an `iterator` to constant to the begining of this
		/// `const_slice`.
		iterator begin () const
		{ return first; }
		/// Returns an `iterator` to constant to the begining of this
		/// `const_slice`.
		iterator cbegin () const
		{ return const_cast<const_slice *>(this)->begin(); }

		/// Returns an `iterator` to constant to the element following
		/// the last element of this `const_slice`.
		const_iterator end () const
		{ return last; }
		/// Returns an `iterator` to constant to the element following
		/// the last element of this `const_slice`.
		const_iterator cend () const
		{ return const_cast<const_slice *>(this)->end(); }

		/// Returns a reversed `iterator` to constant to the last
		/// element of this `const_slice`.
		std::reverse_iterator<iterator> rbegin () const
		{ return std::reverse_iterator<iterator>(end()); }
		/// Returns a reversed `iterator` to constant to the last
		/// element of this `const_slice`.
		std::reverse_iterator<iterator> rcbegin () const
		{ return const_cast<const_slice *>(this)->rbegin(); }

		/// Returns a reversed `iterator` to constant to the element
		/// preceding the first element of this `const_slice`.
		std::reverse_iterator<iterator> rend () const
		{ return std::reverse_iterator<iterator>(begin()); }
		/// Returns a reversed `iterator` to constant to the element
		/// preceding the first element of this `const_slice`.
		std::reverse_iterator<iterator> rcend () const
		{ return  const_cast<const_slice *>(this)->rend(); }

		iterator first;
		iterator last;
	};
public:
	/// Returns a `slice` of this `buffer` begining at given position
	/// `first` up to the given position `last`.
	template <typename T>
	slice<T> get_slice (size_t first, size_t last) {
		T * d = data<T>();
		size_t n = size_/sizeof(T);
		last = std::min(last, n - 1);
		if (first < n) {
			return { d + first, d + last };
		}
		return { d + n, d + n };
	}
	/// Returns a `slice` of this `buffer` begining at given position
	/// `pos` up to last element.
	template <typename T>
	slice<T> get_slice (size_t pos) {
		T * d = data<T>();
		size_t n = size_/sizeof(T);
		if (pos < n) {
			return { d + pos, d + n - 1 };
		}
		return { d + n, d + n };
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
private:
	size_t capacity_;
	size_t size_;
	void *data_;
};
} /* namespace utils */
} /* namespace nealrame */
} /* namespace com */

#endif /* BUFFER_H_ */