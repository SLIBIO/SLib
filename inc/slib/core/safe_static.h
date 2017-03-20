/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_SAFE_STATIC
#define CHECKHEADER_SLIB_CORE_SAFE_STATIC

#include "definition.h"

#include "spin_lock.h"

#include <new>

/****************************************************************
 C++ 11 introduces thread-safe static local variable initialization.
 But we need customized implementation to check the storage-status on
 exit especially for Win32 applications, and also to avoid the option
 "-fno-threadsafe-statics" on GCC compilers.
****************************************************************/

#if defined(SLIB_PLATFORM_IS_WIN32)
#	define SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME) static slib::_SafeStaticDestructor<TYPE> _static_destructor_instance_##NAME(&NAME);
#	define SLIB_SAFE_STATIC_CHECK_FREED(NAME) _static_destructor_instance_##NAME.flagFreed
#else
#	define SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME)
#	define SLIB_SAFE_STATIC_CHECK_FREED(NAME) 0
#endif

#define SLIB_SAFE_STATIC(TYPE, NAME, ...) \
	SLIB_ALIGN(8) static sl_uint8 _static_safemem_##NAME[sizeof(TYPE)]; \
	static sl_int32 _static_safeflag_##NAME = 0; \
	TYPE& NAME = *(reinterpret_cast<TYPE*>(_static_safemem_##NAME)); \
	SLIB_STATIC_SPINLOCK(_static_safelock_##NAME); \
	if (_static_safeflag_##NAME == 0) { \
		_static_safelock_##NAME.lock(); \
		if (_static_safeflag_##NAME == 0) { \
			new (&NAME) TYPE(__VA_ARGS__); \
			_static_safeflag_##NAME = 1; \
		} \
		_static_safelock_##NAME.unlock(); \
	} \
	SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME)

#define SLIB_SAFE_STATIC_GETTER(TYPE, FUNC, ...) \
	static TYPE* FUNC() { \
		SLIB_SAFE_STATIC(TYPE, ret, ##__VA_ARGS__) \
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) { \
			return 0; \
		} \
		return &ret; \
	}

#define SLIB_STATIC_ZERO_INITIALIZED(TYPE, NAME) \
	SLIB_ALIGN(8) static char _static_safemem_##NAME[sizeof(TYPE)] = {0}; \
	TYPE& NAME = *(reinterpret_cast<TYPE*>(_static_safemem_##NAME)); \
	SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME)


#include "detail/safe_static.h"

#endif

