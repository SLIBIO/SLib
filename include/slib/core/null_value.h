/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_NULL_VALUE
#define CHECKHEADER_SLIB_CORE_NULL_VALUE

namespace slib
{
	
	template <class T>
	struct NullValue
	{
		SLIB_INLINE static T get()
		{
			return T();
		}
	};
	
#define SLIB_DEFINE_NULL_VALUE(T, VALUE) \
	template <> \
	struct NullValue<T> \
	{ \
		SLIB_INLINE static T get() \
		{ \
			return VALUE; \
		} \
	};
	
	
	SLIB_DEFINE_NULL_VALUE(char, 0)
	SLIB_DEFINE_NULL_VALUE(signed char, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned char, 0)
	SLIB_DEFINE_NULL_VALUE(char16_t, 0)
	SLIB_DEFINE_NULL_VALUE(char32_t, 0)
	SLIB_DEFINE_NULL_VALUE(short, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned short, 0)
	SLIB_DEFINE_NULL_VALUE(int, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned int, 0)
	SLIB_DEFINE_NULL_VALUE(long, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned long, 0)
	SLIB_DEFINE_NULL_VALUE(sl_int64, 0)
	SLIB_DEFINE_NULL_VALUE(sl_uint64, 0)
	SLIB_DEFINE_NULL_VALUE(bool, false)
	SLIB_DEFINE_NULL_VALUE(float, 0)
	SLIB_DEFINE_NULL_VALUE(double, 0)
	
	template <class T>
	struct NullValue<T*>
	{
		SLIB_INLINE static T* get()
		{
			return sl_null;
		}
	};
	
}

#endif
