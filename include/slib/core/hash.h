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

	constexpr sl_uint32 Rehash(sl_uint32 x) noexcept
	{
		return x ^ (x >> 4) ^ (x >> 7) ^ (x >> 12) ^ (x >> 16) ^ (x >> 19) ^ (x >> 20) ^ (x >> 24) ^ (x >> 27);
	}

	constexpr sl_uint32 Hash64(sl_uint64 x) noexcept
	{
		return Rehash((sl_uint32)(x ^ (x >> 32)));
	}

	sl_uint32 HashBytes(const void* buf, sl_size n) noexcept;

	template <>
	class Hash<char>
	{
	public:
		constexpr sl_uint32 operator()(char v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<signed char>
	{
	public:
		constexpr sl_uint32 operator()(signed char v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<unsigned char>
	{
	public:
		constexpr sl_uint32 operator()(unsigned char v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<wchar_t>
	{
	public:
		constexpr sl_uint32 operator()(wchar_t v) const noexcept { return Rehash(v);}
	};
	
	template <>
	class Hash<char16_t>
	{
	public:
		constexpr sl_uint32 operator()(char16_t v) const noexcept { return Rehash(v);}
	};
	
	template <>
	class Hash<char32_t>
	{
	public:
		constexpr sl_uint32 operator()(char32_t v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<short>
	{
	public:
		constexpr sl_uint32 operator()(short v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<unsigned short>
	{
	public:
		constexpr sl_uint32 operator()(unsigned short v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<int>
	{
	public:
		constexpr sl_uint32 operator()(int v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<unsigned int>
	{
	public:
		constexpr sl_uint32 operator()(unsigned int v) const noexcept { return Rehash(v);}
	};

	template <>
	class Hash<long>
	{
	public:
		constexpr sl_uint32 operator()(long v) const noexcept { return Rehash((sl_uint32)v);}
	};

	template <>
	class Hash<unsigned long>
	{
	public:
		constexpr sl_uint32 operator()(unsigned long v) const noexcept { return Rehash((sl_uint32)v);}
	};

	template <>
	class Hash<sl_int64>
	{
	public:
		constexpr sl_uint32 operator()(sl_int64 v) const noexcept { return Hash64(v); }
	};

	template <>
	class Hash<sl_uint64>
	{
	public:
		constexpr sl_uint32 operator()(sl_uint64 v) const noexcept { return Hash64(v); }
	};

	template <>
	class Hash<float>
	{
	public:
		SLIB_INLINE sl_uint32 operator()(float v) const noexcept { return Rehash(*(reinterpret_cast<sl_uint32*>(&v))); }
	};

	template <>
	class Hash<double>
	{
	public:
		SLIB_INLINE sl_uint32 operator()(double v) const noexcept { return Hash64(*(reinterpret_cast<sl_uint64*>(&v))); }
	};

	template <class T>
	class Hash<T const*>
	{
	public:
		SLIB_INLINE sl_uint32 operator()(T const* v) const noexcept
		{
#ifdef SLIB_ARCH_IS_64BIT
			return Hash64((sl_uint64)((const void*)v));
#else
			return Rehash((sl_uint32)((const void*)v));
#endif
		}
	};

	template <class T>
	class Hash<T*>
	{
	public:
		SLIB_INLINE sl_uint32 operator()(T* v) const noexcept
		{
#ifdef SLIB_ARCH_IS_64BIT
			return Hash64((sl_uint64)((const void*)v));
#else
			return Rehash((sl_uint32)((const void*)v));
#endif
		}
	};

}

#endif
