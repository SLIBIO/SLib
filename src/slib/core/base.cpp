#include "../../../inc/slib/core/base.h"

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/math.h"

//#define FORCE_MEM_ALIGNED

#if defined(SLIB_PLATFORM_IS_APPLE)
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include <stdio.h>

#ifdef SLIB_PLATFORM_IS_WINDOWS
#include <windows.h>
#endif

#if defined(SLIB_PLATFORM_IS_APPLE)
#include <libkern/OSAtomic.h>
#endif

SLIB_NAMESPACE_BEGIN

void* Base::createMemory(sl_size size)
{
#ifndef FORCE_MEM_ALIGNED
	return malloc(size);
#else
	sl_reg ptr = (sl_reg)(malloc(size + 16));
	if (ptr) {
		sl_reg ptr_aligned = (ptr | 7) + 9;
		*(char*)(ptr_aligned - 1) = (char)(ptr_aligned - ptr);
		*(sl_size*)(ptr) = size;
		return (void*)(ptr_aligned);
	}
	return sl_null;
#endif
}

void Base::freeMemory(void* ptr)
{
#ifndef FORCE_MEM_ALIGNED
	free(ptr);
#else
	sl_reg offset = *((unsigned char*)ptr - 1);
	free((void*)((sl_reg)ptr - offset));
#endif
}

void* Base::reallocMemory(void* ptr, sl_size sizeNew)
{
	if (sizeNew == 0) {
		freeMemory(ptr);
		return createMemory(0);
	}
#ifndef FORCE_MEM_ALIGNED
	return realloc(ptr, sizeNew);
#else
	sl_size sizeOld = *(sl_size*)((sl_reg)ptr - *((unsigned char*)ptr - 1));
	if (sizeOld == sizeNew) {
		return ptr;
	}
	void* ptrNew = createMemory(sizeNew);
	if (ptrNew) {
		copyMemory(ptrNew, ptr, sizeOld);
		freeMemory(ptr);
	}
	return ptrNew;
#endif
}

void* Base::createZeroMemory(sl_size size)
{
	void* ret = createMemory(size);
	if (ret) {
		zeroMemory(ret, size);
	}
	return ret;
}

void Base::copyMemory(void* dst, const void* src, sl_size count)
{
	sl_uint8* d = (sl_uint8*)dst;
	sl_uint8* s = (sl_uint8*)src;
	for (sl_size i = 0; i < count; i++) {
		d[i] = s[i];
	}
}

void Base::zeroMemory(void* dst, sl_size size)
{
	sl_uint8* d = (sl_uint8*)dst;
	for (sl_size i = 0; i < size; i++) {
		d[i] = 0;
	}
}

void Base::resetMemory(void* dst, sl_uint8 value, sl_size count)
{
	sl_uint8* d = (sl_uint8*)dst;
	for (sl_size i = 0; i < count; i++) {
		d[i] = value;
	}
}

void Base::resetMemory2(sl_uint16* dst, sl_uint16 value, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		dst[i] = value;
	}
}

void Base::resetMemory2(sl_int16* dst, sl_int16 value, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		dst[i] = value;
	}
}

void Base::resetMemory4(sl_uint32* dst, sl_uint32 value, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		dst[i] = value;
	}
}

void Base::resetMemory4(sl_int32* dst, sl_int32 value, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		dst[i] = value;
	}
}

void Base::resetMemory8(sl_uint64* dst, sl_uint64 value, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		dst[i] = value;
	}
}

void Base::resetMemory8(sl_int64* dst, sl_int64 value, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		dst[i] = value;
	}
}

sl_bool Base::equalsMemory(const void* _m1, const void* _m2, sl_size count)
{
	sl_uint8* m1 = (sl_uint8*)_m1;
	sl_uint8* m2 = (sl_uint8*)_m2;
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemory2(const sl_uint16* m1, const sl_uint16* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemory2(const sl_int16* m1, const sl_int16* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemory4(const sl_uint32* m1, const sl_uint32* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemory4(const sl_int32* m1, const sl_int32* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemory8(const sl_uint64* m1, const sl_uint64* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemory8(const sl_int64* m1, const sl_int64* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] != m2[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_int32 Base::compareMemory(const sl_uint8* m1, const sl_uint8* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory(const sl_int8* m1, const sl_int8* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory2(const sl_uint16* m1, const sl_uint16* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory2(const sl_int16* m1, const sl_int16* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory4(const sl_uint32* m1, const sl_uint32* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory4(const sl_int32* m1, const sl_int32* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory8(const sl_uint64* m1, const sl_uint64* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory8(const sl_int64* m1, const sl_int64* m2, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_bool Base::equalsMemoryZero(const void* _m, sl_size count)
{
	sl_uint8* m = (sl_uint8*)_m;
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemoryZero2(const sl_uint16* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemoryZero2(const sl_int16* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemoryZero4(const sl_uint32* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemoryZero4(const sl_int32* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemoryZero8(const sl_uint64* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Base::equalsMemoryZero8(const sl_int64* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_int32 Base::compareMemoryZero(const sl_uint8* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemoryZero(const sl_int8* m, sl_size count)
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

sl_int32 Base::compareMemoryZero2(const sl_uint16* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemoryZero2(const sl_int16* m, sl_size count)
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

sl_int32 Base::compareMemoryZero4(const sl_uint32* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemoryZero4(const sl_int32* m, sl_size count)
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

sl_int32 Base::compareMemoryZero8(const sl_uint64* m, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemoryZero8(const sl_int64* m, sl_size count)
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

const sl_uint8* Base::findMemory(const void* mem, sl_uint8 pattern, sl_size count)
{
	sl_uint8* m = (sl_uint8*)mem;
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int8* Base::findMemory(const sl_int8* m, sl_int8 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint16* Base::findMemory2(const sl_uint16* m, sl_uint16 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int16* Base::findMemory2(const sl_int16* m, sl_int16 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint32* Base::findMemory4(const sl_uint32* m, sl_uint32 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int32* Base::findMemory4(const sl_int32* m, sl_int32 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint64* Base::findMemory8(const sl_uint64* m, sl_uint64 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int64* Base::findMemory8(const sl_int64* m, sl_int64 pattern, sl_size count)
{
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint8* Base::findMemoryReverse(const void* mem, sl_uint8 pattern, sl_size count)
{
	sl_uint8* m = (sl_uint8*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint16* Base::findMemoryReverse2(const sl_uint16* m, sl_uint16 pattern, sl_size count)
{
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int16* Base::findMemoryReverse2(const sl_int16* m, sl_int16 pattern, sl_size count)
{
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint32* Base::findMemoryReverse4(const sl_uint32* m, sl_uint32 pattern, sl_size count)
{
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int32* Base::findMemoryReverse4(const sl_int32* m, sl_int32 pattern, sl_size count)
{
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint64* Base::findMemoryReverse8(const sl_uint64* mem, sl_uint64 pattern, sl_size count)
{
	sl_uint64* m = (sl_uint64*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_int64* Base::findMemoryReverse8(const sl_int64* mem, sl_int64 pattern, sl_size count)
{
	sl_int64* m = (sl_int64*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const sl_uint8* Base::findMemoryUntilZero(const void* mem, sl_uint8 pattern, sl_size count)
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

sl_bool Base::equalsString(const sl_char8 *s1, const sl_char8 *s2, sl_reg count)
{
	if (count < 0) {
		count = STRING_LENGTH_LIMIT;
	}
	for (sl_reg i = 0; i < count; i++) {
		if (s1[i] != s2[i]) {
			return sl_false;
		}
		if (s1[i] == 0) {
			break;
		}
	}
	return sl_true;
}

sl_bool Base::equalsString2(const sl_char16 *s1, const sl_char16 *s2, sl_reg count)
{
	if (count < 0) {
		count = STRING_LENGTH_LIMIT;
	}
	for (sl_reg i = 0; i < count; i++) {
		if (s1[i] != s2[i]) {
			return sl_false;
		}
		if (s1[i] == 0) {
			break;
		}
	}
	return sl_true;
}

sl_bool Base::equalsString4(const sl_char32 *s1, const sl_char32 *s2, sl_reg count)
{
	if (count < 0) {
		count = STRING_LENGTH_LIMIT;
	}
	for (sl_reg i = 0; i < count; i++) {
		if (s1[i] != s2[i]) {
			return sl_false;
		}
		if (s1[i] == 0) {
			break;
		}
	}
	return sl_true;
}

sl_int32 Base::compareString(const sl_char8 *s1, const sl_char8 *s2, sl_reg count)
{
	const sl_uint8* m1 = (const sl_uint8*)s1;
	const sl_uint8* m2 = (const sl_uint8*)s2;
	if (count < 0) {
		count = STRING_LENGTH_LIMIT;
	}
	for (sl_reg i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
		if (m1[i] == 0) {
			break;
		}
	}
	return 0;
}

sl_int32 Base::compareString2(const sl_char16 *s1, const sl_char16 *s2, sl_reg count)
{
	if (count < 0) {
		count = STRING_LENGTH_LIMIT;
	}
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
}

sl_int32 Base::compareString4(const sl_char32 *s1, const sl_char32 *s2, sl_reg count)
{
	if (count < 0) {
		count = STRING_LENGTH_LIMIT;
	}
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
}

sl_size Base::copyString(sl_char8* dst, const sl_char8* src, sl_reg count)
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

sl_size Base::copyString2(sl_char16* dst, const sl_char16* src, sl_reg count)
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

sl_size Base::copyString4(sl_char32* dst, const sl_char32* src, sl_reg count)
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

sl_size Base::getStringLength(const sl_char8* sz, sl_reg count)
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

sl_size Base::getStringLength2(const sl_char16* sz, sl_reg count)
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

sl_size Base::getStringLength4(const sl_char32* sz, sl_reg count)
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

sl_int32 Base::interlockedIncrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	return (sl_int32)InterlockedIncrement((LONG*)pValue);
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicIncrement32Barrier((int32_t*)pValue);
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_fetch_and_add(pValue, 1) + 1;
#endif
}

sl_int32 Base::interlockedDecrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	return (sl_int32)InterlockedDecrement((LONG*)pValue);
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicDecrement32Barrier((int32_t*)pValue);
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_fetch_and_add(pValue, -1) - 1;
#endif
}

sl_int32 Base::interlockedAdd32(sl_int32* pDst, sl_int32 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	return ((sl_int32)InterlockedExchangeAdd((LONG*)pDst, (LONG)value)) + value;
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicAdd32Barrier((int32_t)value, (int32_t*)pDst);
#elif defined(SLIB_PLATFORM_IS_UNIX)
	__sync_fetch_and_add(pDst, value) + value;
#endif
}

sl_bool Base::interlockedCompareExchange32(sl_int32* pDst, sl_int32 value, sl_int32 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	sl_int32 old;
#	if (SLIB_COMPILER >= SLIB_COMPILER_VC7)
	old = ((sl_int32)InterlockedCompareExchange((LONG*)pDst, (LONG)value, (LONG)comperand));
#	else
	old = ((sl_int32)InterlockedCompareExchange((void**)pDst, (void*)value, (void*)comperand));
#	endif
	return old == comperand;
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicCompareAndSwap32Barrier((int32_t)comperand, (int32_t)value, (int32_t*)pDst) != false;
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_bool_compare_and_swap(pDst, comperand, value) != 0;
#endif
}

#ifdef SLIB_ARCH_IS_64BIT
sl_int64 Base::interlockedIncrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	return (sl_int64)InterlockedIncrement64((LONGLONG*)pValue);
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicIncrement64Barrier(pValue);
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_fetch_and_add(pValue, 1) + 1;
#endif
}

sl_int64 Base::interlockedDecrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	return (sl_int64)InterlockedDecrement64((LONGLONG*)pValue);
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicDecrement64Barrier(pValue);
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_fetch_and_add(pValue, -1) - 1;
#endif
}

sl_int64 Base::interlockedAdd64(sl_int64* pDst, sl_int64 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	return ((sl_int64)InterlockedExchangeAdd64((LONGLONG*)pDst, (LONGLONG)value)) + value;
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicAdd64Barrier(value, pDst);
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_fetch_and_add(pDst, value) + value;
#endif
}

sl_bool Base::interlockedCompareExchange64(sl_int64* pDst, sl_int64 value, sl_int64 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
#ifdef SLIB_PLATFORM_IS_WINDOWS
	sl_int64 old = ((sl_int64)InterlockedCompareExchange64((LONGLONG*)pDst, (LONGLONG)value, (LONGLONG)comperand));
	return old == comperand;
#elif defined(SLIB_PLATFORM_IS_APPLE)
	return OSAtomicCompareAndSwap64Barrier(comperand, value, pDst) != false;
#elif defined(SLIB_PLATFORM_IS_UNIX)
	return __sync_bool_compare_and_swap(pDst, comperand, value) != 0;
#endif
}

#endif

sl_reg Base::interlockedIncrement(sl_reg* pValue)
{
#ifdef SLIB_ARCH_IS_64BIT
	return interlockedIncrement64(pValue);
#else
	return interlockedIncrement32(pValue);
#endif
}

sl_reg Base::interlockedDecrement(sl_reg* pValue)
{
#ifdef SLIB_ARCH_IS_64BIT
	return interlockedDecrement64(pValue);
#else
	return interlockedDecrement32(pValue);
#endif
}

sl_reg Base::interlockedAdd(sl_reg* pDst, sl_reg value)
{
#ifdef SLIB_ARCH_IS_64BIT
	return interlockedAdd64(pDst, value);
#else
	return interlockedAdd32(pDst, value);
#endif
}

sl_bool Base::interlockedCompareExchange(sl_reg* pDst, sl_reg value, sl_reg comperand)
{
#ifdef SLIB_ARCH_IS_64BIT
	return interlockedCompareExchange64(pDst, value, comperand);
#else
	return interlockedCompareExchange32(pDst, value, comperand);
#endif
}

void* Base::interlockedAddPtr(void** pDst, sl_reg value)
{
#ifdef SLIB_ARCH_IS_64BIT
	return (void*)interlockedAdd64((sl_int64*)(pDst), value);
#else
	return (void*)interlockedAdd32((sl_int32*)pDst, value);
#endif
}

sl_bool Base::interlockedCompareExchangePtr(void** pDst, const void* value, const void* comperand)
{
#ifdef SLIB_ARCH_IS_64BIT
	return interlockedCompareExchange64((sl_int64*)(pDst), (sl_int64)value, (sl_int64)comperand);
#else
	return interlockedCompareExchange32((sl_int32*)pDst, (sl_int32)value, (sl_int32)comperand);
#endif
}

void Base::sleep(sl_uint32 millis)
{
	System::sleep(millis);
}

void Base::yield()
{
	System::yield();
}

void Base::yield(sl_uint32 ellapsed)
{
	System::yield(ellapsed);
}

SLIB_NAMESPACE_END
