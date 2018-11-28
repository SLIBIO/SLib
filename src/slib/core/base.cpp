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

#include "slib/core/base.h"

#include "slib/core/system.h"
#include "slib/core/math.h"

#if !defined(SLIB_PLATFORM_IS_APPLE)
#include <malloc.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <wchar.h>

#include <string>

#ifdef SLIB_PLATFORM_IS_WINDOWS
#	include "slib/core/platform_windows.h"
#endif

#if defined(SLIB_ARCH_IS_32BIT)
#	define NOT_SUPPORT_ATOMIC_64BIT
#endif

namespace slib
{
	
#if SLIB_WCHAR_SIZE == 2
	typedef wchar_t _base_char16;
#else
	typedef char16_t _base_char16;
#endif
#if SLIB_WCHAR_SIZE == 4
	typedef wchar_t _base_char32;
#else
	typedef char32_t _base_char32;
#endif

	void* Base::createMemory(sl_size size) noexcept
	{
		return ::malloc(size);
	}

	void Base::freeMemory(void* ptr) noexcept
	{
		::free(ptr);
	}

	void* Base::reallocMemory(void* ptr, sl_size sizeNew) noexcept
	{
		if (sizeNew == 0) {
			::free(ptr);
			return ::malloc(1);
		} else {
			return ::realloc(ptr, sizeNew);
		}
	}

	void* Base::createZeroMemory(sl_size size) noexcept
	{
		void* ptr = ::malloc(size);
		if (ptr) {
			::memset(ptr, 0, size);
		}
		return ptr;
	}

	void Base::copyMemory(void* dst, const void* src, sl_size count) noexcept
	{
		::memcpy(dst, src, count);
	}
	
	void Base::moveMemory(void* dst, const void* src, sl_size count) noexcept
	{
		::memmove(dst, src, count);
	}

	void Base::zeroMemory(void* dst, sl_size size) noexcept
	{
		::memset(dst, 0, size);
	}

	void Base::resetMemory(void* dst, sl_uint8 value, sl_size count) noexcept
	{
		::memset(dst, value, count);
	}

	void Base::resetMemory2(sl_uint16* dst, sl_uint16 value, sl_size count) noexcept
	{
		std::char_traits<_base_char16>::assign((_base_char16*)dst, count, (_base_char16)value);
	}

	void Base::resetMemory2(sl_int16* dst, sl_int16 value, sl_size count) noexcept
	{
		std::char_traits<_base_char16>::assign((_base_char16*)dst, count, (_base_char16)value);
	}

	void Base::resetMemory4(sl_uint32* dst, sl_uint32 value, sl_size count) noexcept
	{
		std::char_traits<_base_char32>::assign((_base_char32*)dst, count, (_base_char32)value);
	}

	void Base::resetMemory4(sl_int32* dst, sl_int32 value, sl_size count) noexcept
	{
		std::char_traits<_base_char32>::assign((_base_char32*)dst, count, (_base_char32)value);
	}

	void Base::resetMemory8(sl_uint64* dst, sl_uint64 value, sl_size count) noexcept
	{
		std::char_traits<sl_uint64>::assign(dst, count, value);
	}

	void Base::resetMemory8(sl_int64* dst, sl_int64 value, sl_size count) noexcept
	{
		std::char_traits<sl_int64>::assign(dst, count, value);
	}

	sl_bool Base::equalsMemory(const void* m1, const void* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count) == 0;
	}

	sl_bool Base::equalsMemory2(const sl_uint16* m1, const sl_uint16* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count << 1) == 0;
	}

	sl_bool Base::equalsMemory2(const sl_int16* m1, const sl_int16* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count << 1) == 0;
	}

	sl_bool Base::equalsMemory4(const sl_uint32* m1, const sl_uint32* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count << 2) == 0;
	}

	sl_bool Base::equalsMemory4(const sl_int32* m1, const sl_int32* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count << 2) == 0;
	}

	sl_bool Base::equalsMemory8(const sl_uint64* m1, const sl_uint64* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count << 3) == 0;
	}

	sl_bool Base::equalsMemory8(const sl_int64* m1, const sl_int64* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count << 3) == 0;
	}

	sl_int32 Base::compareMemory(const sl_uint8* m1, const sl_uint8* m2, sl_size count) noexcept
	{
		return ::memcmp(m1, m2, count);
	}

	sl_int32 Base::compareMemory(const sl_int8* m1, const sl_int8* m2, sl_size count) noexcept
	{
		return std::char_traits<sl_int8>::compare(m1, m2, count);
	}

	sl_int32 Base::compareMemory2(const sl_uint16* m1, const sl_uint16* m2, sl_size count) noexcept
	{
		return std::char_traits<_base_char16>::compare((_base_char16*)m1, (_base_char16*)m2, count);
	}

	sl_int32 Base::compareMemory2(const sl_int16* m1, const sl_int16* m2, sl_size count) noexcept
	{
		return std::char_traits<sl_int16>::compare(m1, m2, count);
	}

	sl_int32 Base::compareMemory4(const sl_uint32* m1, const sl_uint32* m2, sl_size count) noexcept
	{
		return std::char_traits<_base_char32>::compare((_base_char32*)m1, (_base_char32*)m2, count);
	}

	sl_int32 Base::compareMemory4(const sl_int32* m1, const sl_int32* m2, sl_size count) noexcept
	{
		return std::char_traits<sl_int32>::compare(m1, m2, count);
	}

	sl_int32 Base::compareMemory8(const sl_uint64* m1, const sl_uint64* m2, sl_size count) noexcept
	{
		return std::char_traits<sl_uint64>::compare(m1, m2, count);
	}

	sl_int32 Base::compareMemory8(const sl_int64* m1, const sl_int64* m2, sl_size count) noexcept
	{
		return std::char_traits<sl_int64>::compare(m1, m2, count);
	}

	sl_bool Base::equalsMemoryZero(const void* _m, sl_size count) noexcept
	{
		sl_uint8* m = (sl_uint8*)_m;
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Base::equalsMemoryZero2(const sl_uint16* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Base::equalsMemoryZero2(const sl_int16* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Base::equalsMemoryZero4(const sl_uint32* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Base::equalsMemoryZero4(const sl_int32* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Base::equalsMemoryZero8(const sl_uint64* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool Base::equalsMemoryZero8(const sl_int64* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_int32 Base::compareMemoryZero(const sl_uint8* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return 1;
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero(const sl_int8* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			sl_int8 v = m[i];
			if (v) {
				if (v < 0) {
					return -1;
				} else {
					return 1;
				}
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero2(const sl_uint16* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return 1;
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero2(const sl_int16* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			sl_int16 v = m[i];
			if (v) {
				if (v < 0) {
					return -1;
				} else {
					return 1;
				}
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero4(const sl_uint32* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return 1;
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero4(const sl_int32* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			sl_int32 v = m[i];
			if (v) {
				if (v < 0) {
					return -1;
				} else {
					return 1;
				}
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero8(const sl_uint64* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			if (m[i]) {
				return 1;
			}
		}
		return 0;
	}

	sl_int32 Base::compareMemoryZero8(const sl_int64* m, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			sl_int64 v = m[i];
			if (v) {
				if (v < 0) {
					return -1;
				} else {
					return 1;
				}
			}
		}
		return 0;
	}

	const sl_uint8* Base::findMemory(const void* mem, sl_uint8 pattern, sl_size count) noexcept
	{		
		return (const sl_uint8*)(::memchr(mem, pattern, count));
	}

	const sl_int8* Base::findMemory(const sl_int8* mem, sl_int8 pattern, sl_size count) noexcept
	{
		return (const sl_int8*)(::memchr(mem, pattern, count));
	}

	const sl_uint16* Base::findMemory2(const sl_uint16* m, sl_uint16 pattern, sl_size count) noexcept
	{
		return (const sl_uint16*)(std::char_traits<_base_char16>::find((_base_char16*)m, count, (_base_char16)pattern));
	}

	const sl_int16* Base::findMemory2(const sl_int16* m, sl_int16 pattern, sl_size count) noexcept
	{
		return (const sl_int16*)(std::char_traits<_base_char16>::find((_base_char16*)m, count, (_base_char16)pattern));
	}

	const sl_uint32* Base::findMemory4(const sl_uint32* m, sl_uint32 pattern, sl_size count) noexcept
	{
		return (const sl_uint32*)(std::char_traits<_base_char32>::find((_base_char32*)m, count, (_base_char32)pattern));
	}

	const sl_int32* Base::findMemory4(const sl_int32* m, sl_int32 pattern, sl_size count) noexcept
	{
		return (const sl_int32*)(std::char_traits<_base_char32>::find((_base_char32*)m, count, (_base_char32)pattern));
	}

	const sl_uint64* Base::findMemory8(const sl_uint64* m, sl_uint64 pattern, sl_size count) noexcept
	{
		return std::char_traits<sl_uint64>::find(m, count, pattern);
	}

	const sl_int64* Base::findMemory8(const sl_int64* m, sl_int64 pattern, sl_size count) noexcept
	{
		return std::char_traits<sl_int64>::find(m, count, pattern);
	}

	const sl_uint8* Base::findMemoryReverse(const void* mem, sl_uint8 pattern, sl_size count) noexcept
	{
		sl_uint8* m = (sl_uint8*)mem;
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_uint16* Base::findMemoryReverse2(const sl_uint16* m, sl_uint16 pattern, sl_size count) noexcept
	{
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_int16* Base::findMemoryReverse2(const sl_int16* m, sl_int16 pattern, sl_size count) noexcept
	{
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_uint32* Base::findMemoryReverse4(const sl_uint32* m, sl_uint32 pattern, sl_size count) noexcept
	{
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_int32* Base::findMemoryReverse4(const sl_int32* m, sl_int32 pattern, sl_size count) noexcept
	{
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_uint64* Base::findMemoryReverse8(const sl_uint64* mem, sl_uint64 pattern, sl_size count) noexcept
	{
		sl_uint64* m = (sl_uint64*)mem;
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_int64* Base::findMemoryReverse8(const sl_int64* mem, sl_int64 pattern, sl_size count) noexcept
	{
		sl_int64* m = (sl_int64*)mem;
		for (sl_reg i = count - 1; i >= 0; i--) {
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

	const sl_uint8* Base::findMemoryUntilZero(const void* mem, sl_uint8 pattern, sl_size count) noexcept
	{
		sl_uint8* m = (sl_uint8*)mem;
		for (sl_size i = 0; i < count; i++) {
			if (m[i] == 0) {
				break;
			}
			if (m[i] == pattern) {
				return m + i;
			}
		}
		return sl_null;
	}

#define STRING_LENGTH_LIMIT 0x1000000

	sl_bool Base::equalsString(const sl_char8 *s1, const sl_char8 *s2, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		return ::strncmp(s1, s2, count) == 0;
	}

	sl_bool Base::equalsString2(const sl_char16 *s1, const sl_char16 *s2, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
#if SLIB_WCHAR_SIZE == 2
		return ::wcsncmp((wchar_t*)s1, (wchar_t*)s2, count) == 0;
#else
		for (sl_reg i = 0; i < count; i++) {
			if (s1[i] != s2[i]) {
				return sl_false;
			}
			if (s1[i] == 0) {
				break;
			}
		}
		return sl_true;
#endif
	}

	sl_bool Base::equalsString4(const sl_char32 *s1, const sl_char32 *s2, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
#if SLIB_WCHAR_SIZE == 4
		return ::wcsncmp((wchar_t*)s1, (wchar_t*)s2, count) == 0;
#else
		for (sl_reg i = 0; i < count; i++) {
			if (s1[i] != s2[i]) {
				return sl_false;
			}
			if (s1[i] == 0) {
				break;
			}
		}
		return sl_true;
#endif
	}

	sl_int32 Base::compareString(const sl_char8 *s1, const sl_char8 *s2, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		return ::strncmp(s1, s2, count);
	}

	sl_int32 Base::compareString2(const sl_char16 *s1, const sl_char16 *s2, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
#if SLIB_WCHAR_SIZE == 2
		return ::wcsncmp((wchar_t*)s1, (wchar_t*)s2, count);
#else
		for (sl_reg i = 0; i < count; i++) {
			if (s1[i] < s2[i]) {
				return -1;
			} else if (s1[i] > s2[i]) {
				return 1;
			}
			if (s1[i] == 0) {
				break;
			}
		}
		return 0;
#endif
	}

	sl_int32 Base::compareString4(const sl_char32 *s1, const sl_char32 *s2, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
#if SLIB_WCHAR_SIZE == 4
		return ::wcsncmp((wchar_t*)s1, (wchar_t*)s2, count);
#else
		for (sl_reg i = 0; i < count; i++) {
			if (s1[i] < s2[i]) {
				return -1;
			} else if (s1[i] > s2[i]) {
				return 1;
			}
			if (s1[i] == 0) {
				break;
			}
		}
		return 0;
#endif
	}

	sl_size Base::copyString(sl_char8* dst, const sl_char8* src, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		for (sl_reg i = 0; i < count; i++) {
			dst[i] = src[i];
			if (dst[i] == 0) {
				return i + 1;
			}
		}
		return count;
	}

	sl_size Base::copyString2(sl_char16* dst, const sl_char16* src, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		for (sl_reg i = 0; i < count; i++) {
			dst[i] = src[i];
			if (dst[i] == 0) {
				return i + 1;
			}
		}
		return count;
	}

	sl_size Base::copyString4(sl_char32* dst, const sl_char32* src, sl_reg count) noexcept
	{
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		for (sl_reg i = 0; i < count; i++) {
			dst[i] = src[i];
			if (dst[i] == 0) {
				return i + 1;
			}
		}
		return count;
	}

	sl_size Base::getStringLength(const sl_char8* sz, sl_reg count) noexcept
	{
		if (count == 0) {
			return 0;
		}
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		return ::strnlen(sz, count);
	}

	sl_size Base::getStringLength2(const sl_char16* sz, sl_reg count) noexcept
	{
		if (count == 0) {
			return 0;
		}
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
#if SLIB_WCHAR_SIZE == 2
		return ::wcsnlen((wchar_t*)sz, count);
#else
		for (sl_reg i = 0; i < count; i++) {
			if (sz[i] == 0) {
				return i;
			}
		}
		return count;
#endif
	}

	sl_size Base::getStringLength4(const sl_char32* sz, sl_reg count) noexcept
	{
		if (count == 0) {
			return 0;
		}
		if (count < 0) {
			count = STRING_LENGTH_LIMIT;
		}
		for (sl_reg i = 0; i < count; i++) {
			if (sz[i] == 0) {
				return i;
			}
		}
		return count;
	}

	sl_int32 Base::interlockedIncrement32(sl_int32* pValue) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
#ifdef SLIB_PLATFORM_IS_WINDOWS
		return (sl_int32)InterlockedIncrement((LONG*)pValue);
#else
		return __atomic_add_fetch(pValue, 1, __ATOMIC_RELAXED);
#endif
	}

	sl_int32 Base::interlockedDecrement32(sl_int32* pValue) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
#ifdef SLIB_PLATFORM_IS_WINDOWS
		return (sl_int32)InterlockedDecrement((LONG*)pValue);
#else
		return __atomic_add_fetch(pValue, -1, __ATOMIC_RELAXED);
#endif
	}

	sl_int32 Base::interlockedAdd32(sl_int32* pDst, sl_int32 value) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
#ifdef SLIB_PLATFORM_IS_WINDOWS
		return ((sl_int32)InterlockedExchangeAdd((LONG*)pDst, (LONG)value)) + value;
#else
		return __atomic_add_fetch(pDst, value, __ATOMIC_RELAXED);
#endif
	}

	sl_bool Base::interlockedCompareExchange32(sl_int32* pDst, sl_int32 value, sl_int32 comparand) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
#ifdef SLIB_PLATFORM_IS_WINDOWS
		sl_int32 old;
#	if (SLIB_COMPILER >= SLIB_COMPILER_VC7)
		old = ((sl_int32)InterlockedCompareExchange((LONG*)pDst, (LONG)value, (LONG)comparand));
#	else
		old = ((sl_int32)InterlockedCompareExchange((void**)pDst, (void*)value, (void*)comparand));
#	endif
		return old == comparand;
#else
		return __sync_bool_compare_and_swap(pDst, comparand, value) != 0;
#endif
	}

	sl_int64 Base::interlockedIncrement64(sl_int64* pValue) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
#ifdef NOT_SUPPORT_ATOMIC_64BIT
		SpinLocker lock(SpinLockPoolForBase::get(pValue));
		sl_int64 r = *pValue = *pValue + 1;
		return r;
#else
#	ifdef SLIB_PLATFORM_IS_WINDOWS
		return (sl_int64)InterlockedIncrement64((LONGLONG*)pValue);
#	else
		return __atomic_add_fetch(pValue, 1, __ATOMIC_RELAXED);
#	endif
#endif
	}

	sl_int64 Base::interlockedDecrement64(sl_int64* pValue) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
#ifdef NOT_SUPPORT_ATOMIC_64BIT
		SpinLocker lock(SpinLockPoolForBase::get(pValue));
		sl_int64 r = *pValue = *pValue - 1;
		return r;
#else
#	ifdef SLIB_PLATFORM_IS_WINDOWS
		return (sl_int64)InterlockedDecrement64((LONGLONG*)pValue);
#	else
		return __atomic_add_fetch(pValue, -1, __ATOMIC_RELAXED);
#	endif
#endif
	}

	sl_int64 Base::interlockedAdd64(sl_int64* pDst, sl_int64 value) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
#ifdef NOT_SUPPORT_ATOMIC_64BIT
		SpinLocker lock(SpinLockPoolForBase::get(pDst));
		sl_int64 r = *pDst = *pDst + value;
		return r;
#else
#	ifdef SLIB_PLATFORM_IS_WINDOWS
		return ((sl_int64)InterlockedExchangeAdd64((LONGLONG*)pDst, (LONGLONG)value)) + value;
#	else
		return __atomic_add_fetch(pDst, value, __ATOMIC_RELAXED);
#	endif
#endif
	}

	sl_bool Base::interlockedCompareExchange64(sl_int64* pDst, sl_int64 value, sl_int64 comparand) noexcept
	{
		SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
#ifdef NOT_SUPPORT_ATOMIC_64BIT
		SpinLocker lock(SpinLockPoolForBase::get(pDst));
		sl_int64 o = *pDst;
		if (o == comparand) {
			o = value;
			return sl_true;
		}
		return sl_false;
#else
#	ifdef SLIB_PLATFORM_IS_WINDOWS
		sl_int64 old = ((sl_int64)InterlockedCompareExchange64((LONGLONG*)pDst, (LONGLONG)value, (LONGLONG)comparand));
		return old == comparand;
#	else
		return __sync_bool_compare_and_swap(pDst, comparand, value) != 0;
#	endif
#endif
	}

	sl_reg Base::interlockedIncrement(sl_reg* pValue) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return interlockedIncrement64(pValue);
#else
		return interlockedIncrement32(pValue);
#endif
	}

	sl_reg Base::interlockedDecrement(sl_reg* pValue) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return interlockedDecrement64(pValue);
#else
		return interlockedDecrement32(pValue);
#endif
	}

	sl_reg Base::interlockedAdd(sl_reg* pDst, sl_reg value) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return interlockedAdd64(pDst, value);
#else
		return interlockedAdd32(pDst, value);
#endif
	}

	sl_bool Base::interlockedCompareExchange(sl_reg* pDst, sl_reg value, sl_reg comparand) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return interlockedCompareExchange64(pDst, value, comparand);
#else
		return interlockedCompareExchange32(pDst, value, comparand);
#endif
	}

	void* Base::interlockedAddPtr(void** pDst, sl_reg value) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return (void*)interlockedAdd64((sl_int64*)(pDst), value);
#else
		return (void*)interlockedAdd32((sl_int32*)pDst, value);
#endif
	}

	sl_bool Base::interlockedCompareExchangePtr(void** pDst, const void* value, const void* comparand) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return interlockedCompareExchange64((sl_int64*)(pDst), (sl_int64)value, (sl_int64)comparand);
#else
		return interlockedCompareExchange32((sl_int32*)pDst, (sl_int32)value, (sl_int32)comparand);
#endif
	}

	void Base::sleep(sl_uint32 millis) noexcept
	{
		System::sleep(millis);
	}

	void Base::yield() noexcept
	{
		System::yield();
	}

	void Base::yield(sl_uint32 elapsed) noexcept
	{
		System::yield(elapsed);
	}

}
