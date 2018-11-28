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
#	define SLIB_SAFE_STATIC_DESTRUCTOR(TYPE, NAME) static slib:: _priv_SafeStaticDestructor<TYPE> _static_destructor_instance_##NAME(&NAME);
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


#include "detail/safe_static.inc"

#endif

