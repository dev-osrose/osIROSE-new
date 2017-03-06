// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file riterator.h
 *
 * \author L3nn0x
 * \date march 2016
 *
 * Contains a custom random iterator implementation
 */

#ifndef __CUSTOM_ITERATOR_H__
#define __CUSTOM_ITERATOR_H__

#include <iterator>

namespace Core {
/*!
 * \class RIterator
 *
 * \brief A custom random access iterator implementation
 *
 * This class can be used as is to implement a custom random access iterator.
 *
 * \author L3nn0x
 * \date march 2016
 */

template<typename T>
class RIterator : public std::iterator<std::random_access_iterator_tag, T> {
	public:
		RIterator(T* ptr = nullptr) : ptr_(ptr) {}
		RIterator(const RIterator<T>& it) = default;
		~RIterator() {}

		RIterator<T>&	operator=(const RIterator<T>& it) = default;
		RIterator<T>&	operator=(T* ptr) {ptr_ = ptr; return *this;}

		operator	bool() const {return ptr_ != nullptr;}

		bool			operator==(const RIterator<T>& it) const {return ptr_ == it.getConstPtr();}
		bool			operator!=(const RIterator<T>& it) const {return ptr_ != it.getConstPtr();}
		RIterator<T>&	operator+=(const std::ptrdiff_t& movement) {ptr_ += movement; return *this;}
		RIterator<T>&	operator-=(const std::ptrdiff_t& movement) {ptr_ -= movement; return *this;}
		RIterator<T>&	operator++() {++ptr_; return *this;}
		RIterator<T>&	operator--() {++ptr_; return *this;}
		RIterator<T>	operator++(int/*std::ptrdiff_t*/) {auto tmp(*this); ++ptr_; return tmp;}
		RIterator<T>	operator--(int/*std::ptrdiff_t*/) {auto tmp(*this); ++ptr_; return tmp;}
		RIterator<T>	operator+(const std::ptrdiff_t& movement) {auto old = ptr_; ptr_ += movement; auto tmp(*this); ptr_ = old; return tmp;}
		RIterator<T>	operator-(const std::ptrdiff_t& movement) {auto old = ptr_; ptr_ -= movement; auto tmp(*this); ptr_ = old; return tmp;}

		std::ptrdiff_t	operator-(const RIterator<T>& it) {return std::distance(it.getPtr(), this->getPtr());}

		T&			operator*() {return *ptr_;}
		const T&	operator*() const {return *ptr_;}
		T*			operator->() {return ptr_;}

		T*			getPtr() const {return ptr_;}
		const T*	getConstPtr() const {return ptr_;}

	protected:
		T*	ptr_;
};
}

#endif
