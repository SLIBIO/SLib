#ifndef CHECKHEADER_SLIB_CORE_CPP
#define CHECKHEADER_SLIB_CORE_CPP

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template <class T> struct RemoveReference { typedef T Type; };
template <class T> struct RemoveReference<T&> { typedef T Type; };
template <class T> struct RemoveReference<T&&> { typedef T Type; };

template <class T, T v> struct ConstValue { SLIB_CONSTEXPR static T value = v; };

template <class T> struct IsLValue : ConstValue<bool, false> {};
template <class T> struct IsLValue<T&> : ConstValue<bool, true> {};

template <class T>
SLIB_INLINE typename RemoveReference<T>::Type&& Move(T&& v)
{
	return static_cast<typename RemoveReference<T>::Type&&>(v);
}

template <class T>
SLIB_INLINE T&& Forward(typename RemoveReference<T>::Type& v)
{
	return static_cast<T&&>(v);
}

template <class T>
SLIB_INLINE T&& Forward(typename RemoveReference<T>::Type&& v)
{
	static_assert(!(IsLValue<T>::value), "Can not forward an rvalue as an lvalue.");
	return static_cast<T&&>(v);
}

template <class T>
SLIB_INLINE void Swap(T& a, T& b)
{
	T t(Move(a));
	a = Move(b);
	b = Move(t);
}

SLIB_NAMESPACE_END

#endif
