/*!
 * \file iscontainer.h
 *
 * \author L3nn0x
 * \date april 2016
 *
 * This file contains a type trait to properly estimate if the templated type is iterable (is a container)
 * Use like this: is_container<T>::value. Contains template type if need be: is_container<T>::type
 */
#ifndef _ISCONTAINER_H_
#define _ISCONTAINER_H_

namespace RoseCommon {

template<typename T>
struct has_const_iterator
{
	private:
		typedef char                      yes;
		typedef struct { char array[2]; } no;

		template<typename C> static yes test(typename C::const_iterator*);
		template<typename C> static no  test(...);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(yes);
		typedef T type;
};

template <typename T>
struct has_begin_end
{
	template<typename C> static char (&f(typename std::enable_if<
		std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
			typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

	template<typename C> static char (&f(...))[2];

	template<typename C> static char (&g(typename std::enable_if<
		std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
	typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

	template<typename C> static char (&g(...))[2];

	static bool const beg_value = sizeof(f<T>(0)) == 1;
	static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T>
  struct is_container : std::integral_constant<bool,
	has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value> {};

}

#endif /* !_ISCONTAINER_H_ */
