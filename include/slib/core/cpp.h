/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_CPP
#define CHECKHEADER_SLIB_CORE_CPP

#include "definition.h"

namespace slib
{
	
	template <class T> struct RemoveReference { typedef T Type; };
	template <class T> struct RemoveReference<T&> { typedef T Type; };
	template <class T> struct RemoveReference<T&&> { typedef T Type; };

	template <class T> struct RemoveConst { typedef T Type; };
	template <class T> struct RemoveConst<const T> { typedef T Type; };

	template <class T> struct RemoveConstReference { typedef T Type; };
	template <class T> struct RemoveConstReference<T const&> { typedef T Type; };
	template <class T> struct RemoveConstReference<T&> { typedef T Type; };
	template <class T> struct RemoveConstReference<T&&> { typedef T Type; };

	template <class T, T v> struct ConstValue { constexpr static T value = v; };

	template <class T> struct IsLValueHelper : ConstValue<bool, false> {};
	template <class T> struct IsLValueHelper<T&> : ConstValue<bool, true> {};
	template <class T> constexpr bool IsLValue() { return IsLValueHelper<T>::value; }

	template <class T1, class T2> struct IsSameTypeHelper : ConstValue<bool, false> {};
	template <class T> struct IsSameTypeHelper<T, T> : ConstValue<bool, true> {};
	template <class T1, class T2> constexpr bool IsSameType() { return IsSameTypeHelper<T1, T2>::value; }

	template <class T>
	constexpr typename RemoveReference<T>::Type&& Move(T&& v)
	{
		return static_cast<typename RemoveReference<T>::Type&&>(v);
	}

	template <class T>
	constexpr T&& Forward(typename RemoveReference<T>::Type& v)
	{
		return static_cast<T&&>(v);
	}

	template <class T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& v)
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

	template<class T, sl_size_t N>
	constexpr sl_size_t CountOfArray(const T (&)[N])
	{
		return N;
	}

	template <class T> struct PropertyTypeHelper { typedef T const& ArgType; typedef T const& RetType; };
	
	template <class T> T&& DeclaredValue() noexcept;
	
	template <class FROM, class TO>
	class IsConvertibleHelper
	{
	private:
		template<class T> static void _test_implicit(T);

		template<class _FROM, class _TO, class _T = decltype(_test_implicit<_TO>(DeclaredValue<_FROM>()))>
		static ConstValue<bool, true> _test(int);

		template <class _FROM, class _TO>
		static ConstValue<bool, false> _test(...);

	public:
		typedef decltype(_test<FROM, TO>(0)) type;

	};
	
	template <typename FROM, typename TO>
	struct IsConvertible : public IsConvertibleHelper<FROM, TO>::type {};

}

#endif
