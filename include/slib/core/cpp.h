/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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

	template <class T>
	constexpr T* RemoveConstPointerVariable(const T* t)
	{
		return (T*)t;
	}

	template <class T, T v> struct ConstValue { constexpr static T value = v; };

	template <class T> struct IsLValueHelper : ConstValue<bool, false> {};
	template <class T> struct IsLValueHelper<T&> : ConstValue<bool, true> {};
	template <class T> constexpr bool IsLValue() noexcept { return IsLValueHelper<T>::value; }

	template <class T1, class T2> struct IsSameTypeHelper : ConstValue<bool, false> {};
	template <class T> struct IsSameTypeHelper<T, T> : ConstValue<bool, true> {};
	template <class T1, class T2> constexpr bool IsSameType() noexcept { return IsSameTypeHelper<T1, T2>::value; }

	template <class T>
	constexpr typename RemoveReference<T>::Type&& Move(T&& v) noexcept
	{
		return static_cast<typename RemoveReference<T>::Type&&>(v);
	}

	template <class T>
	constexpr T&& Forward(typename RemoveReference<T>::Type& v) noexcept
	{
		return static_cast<T&&>(v);
	}

	template <class T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& v) noexcept
	{
		static_assert(!(IsLValue<T>()), "Can't forward an rvalue as an lvalue.");
		return static_cast<T&&>(v);
	}

	template <class T>
	SLIB_INLINE void Swap(T& a, T& b) noexcept
	{
		T t(Move(a));
		a = Move(b);
		b = Move(t);
	}

	template<class T, sl_size_t N>
	constexpr sl_size_t CountOfArray(const T (&)[N]) noexcept
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

		template<class OTHER_FROM, class OTHER_TO, class OTHER_T = decltype(_test_implicit<OTHER_TO>(DeclaredValue<OTHER_FROM>()))>
		static ConstValue<bool, true> _test(int);

		template <class OTHER_FROM, class OTHER_TO>
		static ConstValue<bool, false> _test(...);

	public:
		typedef decltype(_test<FROM, TO>(0)) type;

	};
	
	template <typename FROM, typename TO>
	struct IsConvertible : public IsConvertibleHelper<FROM, TO>::type {};

}

#endif
