#ifndef CHECKHEADER_SLIB_CORE_CPP
#define CHECKHEADER_SLIB_CORE_CPP

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template <class T> struct RemoveReference { typedef T Type; };
template <class T> struct RemoveReference<T&> { typedef T Type; };
template <class T> struct RemoveReference<T&&> { typedef T Type; };

template <class T> struct RemoveConst { typedef T Type; };
template <class T> struct RemoveConst<const T> { typedef T Type; };

template <class T> struct RemoveConstReference { typedef T Type; };
template <class T> struct RemoveConstReference<T const&> { typedef T Type; };

template <class T, T v> struct ConstValue { SLIB_CONSTEXPR static T value = v; };

template <class T> struct IsLValueT : ConstValue<bool, false> {};
template <class T> struct IsLValueT<T&> : ConstValue<bool, true> {};
template <class T> constexpr bool IsLValue() { return IsLValueT<T>::value; }

template <class T1, class T2> struct IsSameTypeT : ConstValue<bool, false> {};
template <class T> struct IsSameTypeT<T, T> : ConstValue<bool, true> {};
template <class T1, class T2> constexpr bool IsSameType() { return IsSameTypeT<T1, T2>::value; }

template <class T>
SLIB_CONSTEXPR typename RemoveReference<T>::Type&& Move(T&& v)
{
	return static_cast<typename RemoveReference<T>::Type&&>(v);
}

template <class T>
SLIB_CONSTEXPR T&& Forward(typename RemoveReference<T>::Type& v)
{
	return static_cast<T&&>(v);
}

template <class T>
SLIB_CONSTEXPR T&& Forward(typename RemoveReference<T>::Type&& v)
{
	static_assert(!(IsLValue<T>()), "Can't forward an rvalue as an lvalue.");
	return static_cast<T&&>(v);
}

template <class T>
SLIB_INLINE void Swap(T& a, T& b)
{
	T t(Move(a));
	a = Move(b);
	b = Move(t);
}

template<typename T, sl_size_t N>
SLIB_CONSTEXPR sl_size_t CountOfArray(const T (&)[N])
{
	return N;
}

template <class T> struct PropertyTypeHelper { typedef T const& ArgType; typedef T const& RetType; };

SLIB_NAMESPACE_END

#endif
