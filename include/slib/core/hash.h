/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_HASH
#define CHECKHEADER_SLIB_CORE_HASH

#include "definition.h"

namespace slib
{

	template <class T>
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
	
	SLIB_INLINE constexpr sl_size Rehash64ToSize(sl_uint64 x) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return Rehash64(x);
#else
		return Rehash32((sl_uint32)(x >> 32) ^ (sl_uint32)x);
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
		constexpr sl_size operator()(sl_int64 v) const noexcept { return Rehash64(v); }
	};

	template <>
	class Hash<sl_uint64>
	{
	public:
		constexpr sl_size operator()(sl_uint64 v) const noexcept { return Rehash64(v); }
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
		SLIB_INLINE sl_size operator()(double v) const noexcept { return Rehash64(*(reinterpret_cast<sl_uint64*>(&v))); }
	};

	template <class T>
	class Hash<T const*>
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
	class Hash<T*>
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

}

#endif
