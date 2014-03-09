#ifndef UTILS_ABSTRACTBUFFER_H_
#define UTILS_ABSTRACTBUFFER_H_

#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>
#include <type_traits>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/reverse_iterator.hpp>

namespace com {
namespace nealrame {
namespace utils {
/// com::nealrame::buffer
/// ==============================
///
/// The `buffer` class is the base class for `buffer` and 
/// `mutable_buffer`.
///
/// It provides implementation for common routines like accessing the
/// underlying data or get its length or the count of a particular kind of 
/// elements this buffer can holds.
///
/// It also provides a generic templated random access iterator. For exemple,
/// it is possible to iterate over double or unsigned int element using thoses
/// iterators.
class buffer {
public:
	virtual ~buffer () {}
public:
	/// Copy `length` bytes from a given `buffer` within this
	/// `static_buffer` starting at offset `offset`.
	///
	/// Return the number of bytes being copied.
	virtual size_t copy (const buffer &source,
				size_t source_offset, size_t length,
				size_t destination_offset) {
		return copy(static_cast<const uint8_t *>(source.data()) + source_offset,
				length, destination_offset);
	}
	/// Copy `length` bytes from a given raw data pointer within this
	/// `static_buffer` starting at offset `offset`.
	///
	/// Return the number of bytes being copied.
	virtual size_t copy (const void *, size_t len, size_t off) = 0;
	/// Fill `length` bytes within this `static_buffer` with a specified
	/// value `v` starting at offset `offset`.
	///
	/// Return the number of bytes being overwritten.
	virtual size_t fill (uint8_t v, size_t len, size_t off) = 0;
	/// Get the underlying data of this `buffer`.
	virtual void * data () final { return length_ > 0 ? data_:nullptr; }
	/// Get the underlying data of this `buffer`.
	virtual const void * data () const final
	{ return const_cast<buffer *>(this)->data(); }
	/// Get the length of this `buffer`.
	virtual size_t length () const final { return length_; }
	/// Get the count of T element that this `buffer` holds.
	template <typename T>
	size_t count () const {
		return length_/sizeof(T);
	}
public: // Iterators /////////////////////////////////////////////////////////
	template <typename T>
	class base_iterator :
		public boost::iterator_facade<base_iterator<T>, T, boost::random_access_traversal_tag> {
		friend class boost::iterator_core_access;
		template <class> friend class base_iterator;
		struct enabler { };
		T & dereference () const {
			return *ptr_;
		}
		template <class OtherValue>
		bool equal (base_iterator<OtherValue> const &other) const {
			return ptr_ == other.ptr_;
		}
		ptrdiff_t distance_to (base_iterator const &other) const {
			return ptr_ - other.ptr_;
		}
		void advance (ptrdiff_t n) {
			ptr_ += n;
		}
		void increment () { advance( 1); }
		void decrement () { advance(-1); }
		T *ptr_;
	public:
		base_iterator () : ptr_(nullptr)   { }
		base_iterator (T *ptr) : ptr_(ptr) { }
		template <typename U>
		base_iterator (base_iterator<U> const &other,
				typename std::enable_if<std::is_convertible<U *, T *>::value, enabler>::type = enabler()) :
			ptr_(other.ptr_) {
		}
	};

	template <typename T>
	using iterator = base_iterator<T>;

	template <typename T>
	using const_iterator = base_iterator<T const>;

	template <typename T>
	class reverse_iterator : public boost::reverse_iterator<iterator<T>> { };

	template <typename T>
	class const_reverse_iterator : public boost::reverse_iterator<const_iterator<T>> { };

	/// Return an _iterator_ pointing to the first `T` element of this
	/// `buffer`.
	template <typename T> iterator<T> begin () {
		return iterator<T>(static_cast<T *>(data_));
	}
	/// Return an _iterator_ pointing to the _past-the-end_ `T` element of
	/// this `buffer`.
	template <typename T> iterator<T> end () {
		return begin<T>() + count<T>();
	}
	/// Return a _constant iterator_ pointing to the first `T` element of
	/// this `buffer`.
	template <typename T> const_iterator<T> begin () const {
		return const_iterator<T>(static_cast<const T *>(data_));
	}
	/// Return a _constant iterator_ pointing to the _past-the-end_ `T`
	/// element of this `buffer`.
	template <typename T> const_iterator<T> end () const {
		return begin<T>() + count<T>();
	}
	/// Return a _constant iterator_ pointing to the first `T` element of
	/// this `buffer`.
	template <typename T> const_iterator<T> cbegin () const {
		return begin<T>();
	}
	/// Return a _constant iterator_ pointing to the _past-the-end_ `T`
	/// element of this `buffer`.
	template <typename T> const_iterator<T> cend () const {
		return end<T>();
	}
	/// Return a _reverse iterator_ pointing to the last `T` element of
	/// this `buffer`.
	template <typename T> reverse_iterator<T> rbegin () {
		return reverse_iterator<T>(end<T>());
	}
	/// Return a _reverse iterator_ pointing to the theorical `T` element
	// preceding the first element of this `buffer`.
	template <typename T> reverse_iterator<T> rend () {
		return reverse_iterator<T>(begin<T>());
	}
	/// Return a _constant reverse iterator_ pointing to the last `T`
	/// element of this `buffer`.
	template <typename T> const_reverse_iterator<T> rbegin () const {
		return const_reverse_iterator<T>(end<T>());
	}
	/// Return a _constant reverse iterator_ pointing to the theorical `T`
	/// element preceding the first element of this `buffer`.
	template <typename T> const_reverse_iterator<T> rend () const {
		return const_reverse_iterator<T>(begin<T>());
	}
	/// Return a _constant reverse iterator_ pointing to the last `T`
	/// element of this `buffer`.
	template <typename T> const_reverse_iterator<T> crbegin () const {
		return rbegin<T>();
	}
	/// Return a _constant reverse iterator_ pointing to the theorical `T`
	/// element preceding the first element of this `buffer`.
	template <typename T> const_reverse_iterator<T> crend () const {
		return rend<T>();
	}
protected:
	size_t capacity_;
	size_t length_;
	void *data_;
};
} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILS_BASE_BUFFER_H_ */