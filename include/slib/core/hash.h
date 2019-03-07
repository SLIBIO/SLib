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

#ifndef CHECKHEADER_SLIB_CORE_HASH
#define CHECKHEADER_SLIB_CORE_HASH

#include "definition.h"

namespace slib
{

	template <class T, sl_bool isEnum=__is_enum(T)>
	class Hash;
	
	SLIB_INLINE constexpr sl_uint8 Rehash8(sl_uint8 x) noexcept
	{
		return x ^ (x >> 4) ^ (x >> 7);
	}
	
	SLIB_INLINE constexpr sl_uint16 Rehash16(sl_uint16 x) noexcept
	{
		return x ^ (x >> 4) ^ (x >> 7) ^ (x >> 12);
	}

	SLIB_INLINE constexpr sl_uint32 Rehash32(sl_uint32 x) noexcept
	{
		return x ^ (x >> 4) ^ (x >> 7) ^ (x >> 12) ^ (x >> 16) ^ (x >> 19) ^ (x >> 20) ^ (x >> 24) ^ (x >> 27);
	}
	
	SLIB_INLINE constexpr sl_uint64 Rehash64(sl_uint64 x) noexcept
	{
		return x ^ (x >> 4) ^ (x >> 7) ^ (x >> 12) ^ (x >> 16) ^ (x >> 19) ^ (x >> 20) ^ (x >> 24) ^ (x >> 27) ^ (x >> 31) ^ (x >> 36) ^ (x >> 40) ^ (x >> 45) ^ (x >> 49) ^ (x >> 52) ^ (x >> 57) ^ (x >> 60);
	}
	
	SLIB_INLINE constexpr sl_size Rehash(sl_size x) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return Rehash64(x);
#else
		return Rehash32(x);
#endif
	}
	
	SLIB_INLINE constexpr sl_uint32 Rehash64To32(sl_uint64 x) noexcept
	{
		return Rehash32((sl_uint32)(x >> 32) ^ (sl_uint32)x);
	}
	
	SLIB_INLINE constexpr sl_size Rehash64ToSize(sl_uint64 x) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return Rehash64(x);
#else
		return Rehash64To32(x);
#endif
	}
	
	sl_uint32 HashBytes32(const void* buf, sl_size n) noexcept;
	
	sl_uint64 HashBytes64(const void* buf, sl_size n) noexcept;
	
	sl_size HashBytes(const void* buf, sl_size n) noexcept;

	template <>
	class Hash<char>
	{
	public:
		constexpr sl_size operator()(char v) const noexcept { return Rehash8(v); }
	};

	template <>
	class Hash<signed char>
	{
	public:
		constexpr sl_size operator()(signed char v) const noexcept { return Rehash8(v); }
	};

	template <>
	class Hash<unsigned char>
	{
	public:
		constexpr sl_size operator()(unsigned char v) const noexcept { return Rehash8(v); }
	};

	template <>
	class Hash<wchar_t>
	{
	public:
		constexpr sl_size operator()(wchar_t v) const noexcept { return Rehash32(v); }
	};
	
	template <>
	class Hash<char16_t>
	{
	public:
		constexpr sl_size operator()(char16_t v) const noexcept { return Rehash16(v); }
	};
	
	template <>
	class Hash<char32_t>
	{
	public:
		constexpr sl_size operator()(char32_t v) const noexcept { return Rehash32(v); }
	};

	template <>
	class Hash<short>
	{
	public:
		constexpr sl_size operator()(short v) const noexcept { return Rehash16(v); }
	};

	template <>
	class Hash<unsigned short>
	{
	public:
		constexpr sl_size operator()(unsigned short v) const noexcept { return Rehash16(v); }
	};

	template <>
	class Hash<int>
	{
	public:
		constexpr sl_size operator()(int v) const noexcept { return Rehash32(v); }
	};

	template <>
	class Hash<unsigned int>
	{
	public:
		constexpr sl_size operator()(unsigned int v) const noexcept { return Rehash32(v); }
	};

	template <>
	class Hash<long>
	{
	public:
		constexpr sl_size operator()(long v) const noexcept { return Rehash32((sl_uint32)v); }
	};

	template <>
	class Hash<unsigned long>
	{
	public:
		constexpr sl_size operator()(unsigned long v) const noexcept { return Rehash32((sl_uint32)v); }
	};

	template <>
	class Hash<sl_int64>
	{
	public:
		constexpr sl_size operator()(sl_int64 v) const noexcept { return Rehash64ToSize(v); }
	};

	template <>
	class Hash<sl_uint64>
	{
	public:
		constexpr sl_size operator()(sl_uint64 v) const noexcept { return Rehash64ToSize(v); }
	};

	template <>
	class Hash<float>
	{
	public:
		SLIB_INLINE sl_size operator()(float v) const noexcept { return Rehash32(*(reinterpret_cast<sl_uint32*>(&v))); }
	};

	template <>
	class Hash<double>
	{
	public:
		SLIB_INLINE sl_size operator()(double v) const noexcept { return Rehash64ToSize(*(reinterpret_cast<sl_uint64*>(&v))); }
	};

	template <class T>
	class Hash<T const*, sl_false>
	{
	public:
		SLIB_INLINE sl_size operator()(T const* v) const noexcept
		{
#ifdef SLIB_ARCH_IS_64BIT
			return Rehash64((sl_uint64)((const void*)v));
#else
			return Rehash32((sl_uint32)((const void*)v));
#endif
		}
	};

	template <class T>
	class Hash<T*, sl_false>
	{
	public:
		SLIB_INLINE sl_size operator()(T* v) const noexcept
		{
#ifdef SLIB_ARCH_IS_64BIT
			return Rehash64((sl_uint64)((const void*)v));
#else
			return Rehash32((sl_uint32)((const void*)v));
#endif
		}
	};

	template <class T>
	class Hash<T, sl_true>
	{
	public:
		SLIB_INLINE sl_size operator()(T v) const noexcept
		{
#ifdef SLIB_ARCH_IS_64BIT
			return Rehash64((sl_uint64)((sl_int64)v));
#else
			return Rehash32((sl_uint32)((sl_int32)v));
#endif
		}
		
	};
	
}

#endif
