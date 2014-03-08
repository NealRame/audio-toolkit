#ifndef UTILSINTERLEAVEDITERATOR_H_
#define UTILSINTERLEAVEDITERATOR_H_

#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <iostream>
#include <iomanip>

#include "utils_abstract_buffer.h"

namespace com {
namespace nealrame {
namespace utils {

template<typename T, size_t N>
class base_interleaved_iterator : 
	std::iterator<std::random_access_iterator_tag,
			std::array<std::reference_wrapper<T>, N>> {
protected:
	typedef abstract_buffer::iterator<T> iterator;
public:
	base_interleaved_iterator (iterator it) : it_(it) { }
	base_interleaved_iterator () : base_interleaved_iterator(iterator()) { }
	base_interleaved_iterator(const base_interleaved_iterator &it) : 
		base_interleaved_iterator(it.it_) {
	}
	virtual base_interleaved_iterator & operator= (const base_interleaved_iterator& it) {
		it_ = it.it_;
		return *this;
	}
	virtual bool operator<  (const base_interleaved_iterator &it) const {
		return it_ < it.it_;
	}
	virtual bool operator>  (const base_interleaved_iterator &it) const {
		return it_ > it.it_;
	}
	virtual bool operator<= (const base_interleaved_iterator &it) const {
		return ! (*this > it);
	}
	virtual bool operator>= (const base_interleaved_iterator &it) const {
		return ! (*this < it);
	}
	virtual bool operator== (const base_interleaved_iterator &it) const {
		return it_ == it.it_; 
	}
	virtual bool operator!= (const base_interleaved_iterator &it) const {
		return it_ != it.it_;
	}
	virtual base_interleaved_iterator & operator+= (int8_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (int16_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (int32_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (int64_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint8_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint16_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint32_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator+= (uint64_t i) {
		it_ += N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (int8_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (int16_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (int32_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint8_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint16_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint32_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator-= (uint64_t i) {
		it_ -= N*i;
		return *this;
	}
	virtual base_interleaved_iterator & operator++ () {
		return *this += 1;
	}
	virtual base_interleaved_iterator & operator-- () {
		return *this -= 1;
	}
protected:
	iterator it_;
};

template<typename T, size_t N>
class interleaved_iterator : public base_interleaved_iterator<T, N> {
};

template<typename T>
class interleaved_iterator<T, 1> : public base_interleaved_iterator<T, 1> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>();
	}
public:
	using base_interleaved_iterator<T, 1>::operator++;
	using base_interleaved_iterator<T, 1>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 1> operator* () {
		return std::array<std::reference_wrapper<T>, 1>{{
			*(this->it_)
		}};
	}
	std::array<std::reference_wrapper<T>, 1> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 1>{{
			*(this->it_ + i)
		}};
	}
	std::array<T, 1> operator[] (unsigned int i) const {
		return std::array<T, 1>{
			*(this->it_ + i)
		};
	}
	std::array<T, 1> operator* () const {
		return std::array<T, 1>{
			*(this->it_)
		};
	}
};

template<typename T>
class interleaved_iterator<T, 2> : public base_interleaved_iterator<T, 2> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/2;
	}
public:
	using base_interleaved_iterator<T, 2>::operator++;
	using base_interleaved_iterator<T, 2>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) const {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) const { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 2> operator* () {
		return std::array<std::reference_wrapper<T>, 2>{{
			*(this->it_),
			*(this->it_ + 1)
		}};
	}
	std::array<std::reference_wrapper<T>, 2> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 2>{{
			*(this->it_ + 2*i),
			*(this->it_ + 2*i + 1)
		}};
	}
	std::array<T, 2> operator* () const {
		return std::array<T, 2>{{
			*(this->it_),
			*(this->it_ + 1) 
		}};
	}
	std::array<T, 2> operator[] (unsigned int i) const {
		return std::array<T, 2>{{
			*(this->it_ + 2*i),
			*(this->it_ + 2*i + 1)
		}};
	}
};

template<typename T>
class interleaved_iterator<T, 3> : public base_interleaved_iterator<T, 3> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/3;
	}
	static interleaved_iterator begin (const abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (const abstract_buffer &buf) {
		return begin(buf) + buf.count<T>();
	}
public:
	using base_interleaved_iterator<T, 3>::operator++;
	using base_interleaved_iterator<T, 3>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 3> operator* () {
		return std::array<std::reference_wrapper<T>, 3>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2)
		}};
	}
	std::array<std::reference_wrapper<T>, 3> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 3>{{
			*(this->it_ + 3*i),
			*(this->it_ + 3*i + 1),
			*(this->it_ + 3*i + 2)
		}};
	}
	std::array<T, 3> operator* () const {
		return std::array<T, 3>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2)
		}};
	}
	std::array<T, 3> operator[] (unsigned int i) const {
		return std::array<T, 3>{{
			*(this->it_ + 3*i),
			*(this->it_ + 3*i + 1),
			*(this->it_ + 3*i + 2)
		}};
	}
};

template<typename T>
class interleaved_iterator<T, 4> : public base_interleaved_iterator<T, 4> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/4;
	}
public:
	using base_interleaved_iterator<T, 4>::operator++;
	using base_interleaved_iterator<T, 4>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 4> operator* () {
		return std::array<std::reference_wrapper<T>, 4>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3)
		}};
	}
	std::array<std::reference_wrapper<T>, 4> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 4>{{
			*(this->it_ + 4*i),
			*(this->it_ + 4*i + 1),
			*(this->it_ + 4*i + 2),
			*(this->it_ + 4*i + 3)
		}};
	}
	std::array<T, 4> operator* () const {
		return std::array<T, 4>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3)
		}};
	}
	std::array<T, 4> operator[] (unsigned int i) const {
		return std::array<T, 4>{{
			*(this->it_ + 4*i),
			*(this->it_ + 4*i + 1),
			*(this->it_ + 4*i + 2),
			*(this->it_ + 4*i + 3)
		}};
	}
};

template<typename T>
class interleaved_iterator<T, 5> : public base_interleaved_iterator<T, 5> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/5;
	}
public:
	using base_interleaved_iterator<T, 5>::operator++;
	using base_interleaved_iterator<T, 5>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 5> operator* () {
		return std::array<std::reference_wrapper<T>, 5>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4)
		}};
	}
	std::array<std::reference_wrapper<T>, 5> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 5>{{
			*(this->it_ + 5*i),
			*(this->it_ + 5*i + 1),
			*(this->it_ + 5*i + 2),
			*(this->it_ + 5*i + 3),
			*(this->it_ + 5*i + 4)
		}};
	}
	std::array<T, 5> operator* () const {
		return std::array<T, 5>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4)
		}};
	}
	std::array<T, 5> operator[] (unsigned int i) const {
		return std::array<T, 5>{{
			*(this->it_ + 5*i),
			*(this->it_ + 5*i + 1),
			*(this->it_ + 5*i + 2),
			*(this->it_ + 5*i + 3),
			*(this->it_ + 5*i + 4)
		}};
	}
};

template<typename T>
class interleaved_iterator<T, 6> : public base_interleaved_iterator<T, 6> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/6;
	}
public:
	using base_interleaved_iterator<T, 6>::operator++;
	using base_interleaved_iterator<T, 6>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 6> operator* () {
		return std::array<std::reference_wrapper<T>, 6>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5)
		}};
	}
	std::array<std::reference_wrapper<T>, 6> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 6>{{
			*(this->it_ + 6*i),
			*(this->it_ + 6*i + 1),
			*(this->it_ + 6*i + 2),
			*(this->it_ + 6*i + 3),
			*(this->it_ + 6*i + 4),
			*(this->it_ + 6*i + 5)
		}};
	}
	std::array<T, 6> operator* () const {
		return std::array<T, 6>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5)
		}};
	}
	std::array<T, 6> operator[] (unsigned int i) const {
		return std::array<T, 6>{{
			*(this->it_ + 6*i),
			*(this->it_ + 6*i + 1),
			*(this->it_ + 6*i + 2),
			*(this->it_ + 6*i + 3),
			*(this->it_ + 6*i + 4),
			*(this->it_ + 6*i + 5)
		}};
	}
};

template<typename T>
class interleaved_iterator<T, 7> : public base_interleaved_iterator<T, 7> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/7;
	}
public:
	using base_interleaved_iterator<T, 7>::operator++;
	using base_interleaved_iterator<T, 7>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 7> operator* () {
		return std::array<std::reference_wrapper<T>, 7>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6)
		}};
	}
	std::array<std::reference_wrapper<T>, 7> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 7>{{
			*(this->it_ + 7*i),
			*(this->it_ + 7*i + 1),
			*(this->it_ + 7*i + 2),
			*(this->it_ + 7*i + 3),
			*(this->it_ + 7*i + 4),
			*(this->it_ + 7*i + 5),
			*(this->it_ + 7*i + 6)
		}};
	}
	std::array<T, 7> operator* () const {
		return std::array<T, 7>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6)
		}};
	}
	std::array<T, 7> operator[] (unsigned int i) const {
		return std::array<T, 7>{{
			*(this->it_ + 7*i),
			*(this->it_ + 7*i + 1),
			*(this->it_ + 7*i + 2),
			*(this->it_ + 7*i + 3),
			*(this->it_ + 7*i + 4),
			*(this->it_ + 7*i + 5),
			*(this->it_ + 7*i + 6)
		}};
	}
};

template<typename T>
class interleaved_iterator<T, 8> : public base_interleaved_iterator<T, 8> {
private:
	typedef abstract_buffer::iterator<T> iterator;
	interleaved_iterator (iterator it) { this->it_ = it; }
public:
	static interleaved_iterator begin (abstract_buffer &buf) {
		return interleaved_iterator(buf.begin<T>());
	}
	static interleaved_iterator end (abstract_buffer &buf) {
		return begin(buf) + buf.count<T>()/8;
	}
public:
	using base_interleaved_iterator<T, 8>::operator++;
	using base_interleaved_iterator<T, 8>::operator--;
	template<typename IntegralType>
	interleaved_iterator operator+ (IntegralType i) {
		return (interleaved_iterator(*this) += i);
	}
	template<typename IntegralType>
	interleaved_iterator operator- (IntegralType i) { 
		return (interleaved_iterator(*this) -= i);
	}
	interleaved_iterator operator++ (int) {
		interleaved_iterator it(*this);
		++(*this);
		return it;
	}
	interleaved_iterator operator-- (int) {
		interleaved_iterator it(*this);
		--(*this);
		return it;
	}
	std::array<std::reference_wrapper<T>, 8> operator* () {
		return std::array<std::reference_wrapper<T>, 8>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6),
			*(this->it_ + 7)
		}};
	}
	std::array<std::reference_wrapper<T>, 8> operator[] (unsigned int i) {
		return std::array<std::reference_wrapper<T>, 8>{{
			*(this->it_ + 8*i),
			*(this->it_ + 8*i + 1),
			*(this->it_ + 8*i + 2),
			*(this->it_ + 8*i + 3),
			*(this->it_ + 8*i + 4),
			*(this->it_ + 8*i + 5),
			*(this->it_ + 8*i + 6),
			*(this->it_ + 8*i + 7)
		}};
	}
	std::array<T, 8> operator* () const {
		return std::array<T, 8>{{
			*(this->it_),
			*(this->it_ + 1),
			*(this->it_ + 2),
			*(this->it_ + 3),
			*(this->it_ + 4),
			*(this->it_ + 5),
			*(this->it_ + 6),
			*(this->it_ + 7)
		}};
	}
	std::array<T, 8> operator[] (unsigned int i) const {
		return std::array<T, 8>{{
			*(this->it_ + 8*i),
			*(this->it_ + 8*i + 1),
			*(this->it_ + 8*i + 2),
			*(this->it_ + 8*i + 3),
			*(this->it_ + 8*i + 4),
			*(this->it_ + 8*i + 5),
			*(this->it_ + 8*i + 6),
			*(this->it_ + 8*i + 7) 
		}};
	}
};

template<typename T, size_t N>
void interlace (std::array<std::reference_wrapper<const abstract_buffer>, N> bufs, abstract_buffer &buf) {
	size_t min_count = std::numeric_limits<unsigned int>::max();
	size_t size = buf.length();

	std::for_each(bufs.begin(), bufs.end(),
		[&min_count](const abstract_buffer &buf) {
			min_count = std::min(buf.count<T>(), min_count);
		});

	while (min_count*N*sizeof(T) > size) {
		min_count--;
	}

	auto it = interleaved_iterator<T, N>::begin(buf);

	for (unsigned int i = 0; i < min_count; ++i) {
		for (unsigned int j = 0; j < N; ++j) {
			const abstract_buffer &channel = bufs[j];
			it[i][j].get() = *(channel.begin<T>() + i);
		}
	}
}

template<typename T, size_t N>
void deinterlace(const abstract_buffer &buf, std::array<std::reference_wrapper<abstract_buffer>, N> bufs) {
	size_t min_count = std::numeric_limits<size_t>::max();

	std::for_each(bufs.begin(), bufs.end(), [&min_count](abstract_buffer &buf) {
		min_count = std::min(min_count, buf.count<T>());
	});

	auto it = interleaved_iterator<const T, N>::begin(buf);
	unsigned int count = std::min(min_count, buf.count<T>()/N);
	
	for (unsigned int i = 0; i < count; ++i) {
		for (unsigned int j = 0; j < N; ++j) {
			abstract_buffer & dest = bufs[j].get();
			dest.begin<T>()[i] = it[i][j].get();
		}
	}
}

} // namespace utils
} // namespace nealrame
} // namespace com

#endif /* UTILSINTERLEAVEDITERATOR_H_ */