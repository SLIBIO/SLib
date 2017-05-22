/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_CAST
#define CHECKHEADER_SLIB_CORE_CAST

#include "definition.h"

#include "cpp.h"
#include "string.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <string>
#endif

namespace slib
{
	
	template <class T1, class T2>
	class Cast
	{
	public:
		template <class T>
		SLIB_INLINE T2 operator()(T&& v) const noexcept
		{
			return T2(Forward<T>(v));
		}
	};
	
	template <class T>
	class Cast<T, T>
	{
	public:
		SLIB_INLINE T const& operator()(const T& v) const noexcept
		{
			return v;
		}
	};

	
	class String;
	class String16;
	
	template <>
	class Cast<String, signed char>
	{
	public:
		signed char operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, unsigned char>
	{
	public:
		unsigned char operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, short>
	{
	public:
		short operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, unsigned short>
	{
	public:
		unsigned short operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, int>
	{
	public:
		int operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, unsigned int>
	{
	public:
		unsigned int operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, long>
	{
	public:
		long operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, unsigned long>
	{
	public:
		unsigned long operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, sl_int64>
	{
	public:
		sl_int64 operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, sl_uint64>
	{
	public:
		sl_uint64 operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, float>
	{
	public:
		float operator()(const String& v) const noexcept;
	};
	
	template <>
	class Cast<String, double>
	{
	public:
		double operator()(const String& v) const noexcept;
	};
	
#ifdef SLIB_SUPPORT_STD_TYPES
	template <>
	class Cast<String, std::string>
	{
	public:
		std::string operator()(const String& v) const noexcept;
	};
#endif

	
	template <>
	class Cast<String16, signed char>
	{
	public:
		signed char operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, unsigned char>
	{
	public:
		unsigned char operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, short>
	{
	public:
		short operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, unsigned short>
	{
	public:
		unsigned short operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, int>
	{
	public:
		int operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, unsigned int>
	{
	public:
		unsigned int operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, long>
	{
	public:
		long operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, unsigned long>
	{
	public:
		unsigned long operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, sl_int64>
	{
	public:
		sl_int64 operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, sl_uint64>
	{
	public:
		sl_uint64 operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, float>
	{
	public:
		float operator()(const String16& v) const noexcept;
	};
	
	template <>
	class Cast<String16, double>
	{
	public:
		double operator()(const String16& v) const noexcept;
	};
	
#ifdef SLIB_SUPPORT_STD_TYPES
	template <>
	class Cast<String16, std::u16string>
	{
	public:
		std::u16string operator()(const String16& v) const noexcept;
	};
#endif
	
	
	template <>
	class Cast<signed char, String>
	{
	public:
		String operator()(signed char v) const noexcept;
	};
	
	template <>
	class Cast<unsigned char, String>
	{
	public:
		String operator()(unsigned char v) const noexcept;
	};

	template <>
	class Cast<short, String>
	{
	public:
		String operator()(short v) const noexcept;
	};
	
	template <>
	class Cast<unsigned short, String>
	{
	public:
		String operator()(unsigned short v) const noexcept;
	};
	
	template <>
	class Cast<int, String>
	{
	public:
		String operator()(int v) const noexcept;
	};
	
	template <>
	class Cast<unsigned int, String>
	{
	public:
		String operator()(unsigned int v) const noexcept;
	};
	
	template <>
	class Cast<long, String>
	{
	public:
		String operator()(long v) const noexcept;
	};
	
	template <>
	class Cast<unsigned long, String>
	{
	public:
		String operator()(unsigned long v) const noexcept;
	};
	
	template <>
	class Cast<sl_int64, String>
	{
	public:
		String operator()(sl_int64 v) const noexcept;
	};
	
	template <>
	class Cast<sl_uint64, String>
	{
	public:
		String operator()(sl_uint64 v) const noexcept;
	};
	
	template <>
	class Cast<float, String>
	{
	public:
		String operator()(float v) const noexcept;
	};
	
	template <>
	class Cast<double, String>
	{
	public:
		String operator()(double v) const noexcept;
	};
	
	
	template <>
	class Cast<signed char, String16>
	{
	public:
		String16 operator()(signed char v) const noexcept;
	};
	
	template <>
	class Cast<unsigned char, String16>
	{
	public:
		String16 operator()(unsigned char v) const noexcept;
	};
	
	template <>
	class Cast<short, String16>
	{
	public:
		String16 operator()(short v) const noexcept;
	};
	
	template <>
	class Cast<unsigned short, String16>
	{
	public:
		String16 operator()(unsigned short v) const noexcept;
	};
	
	template <>
	class Cast<int, String16>
	{
	public:
		String16 operator()(int v) const noexcept;
	};
	
	template <>
	class Cast<unsigned int, String16>
	{
	public:
		String16 operator()(unsigned int v) const noexcept;
	};
	
	template <>
	class Cast<long, String16>
	{
	public:
		String16 operator()(long v) const noexcept;
	};
	
	template <>
	class Cast<unsigned long, String16>
	{
	public:
		String16 operator()(unsigned long v) const noexcept;
	};
	
	template <>
	class Cast<sl_int64, String16>
	{
	public:
		String16 operator()(sl_int64 v) const noexcept;
	};
	
	template <>
	class Cast<sl_uint64, String16>
	{
	public:
		String16 operator()(sl_uint64 v) const noexcept;
	};
	
	template <>
	class Cast<float, String16>
	{
	public:
		String16 operator()(float v) const noexcept;
	};
	
	template <>
	class Cast<double, String16>
	{
	public:
		String16 operator()(double v) const noexcept;
	};
	
}

#endif
