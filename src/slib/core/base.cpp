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

void Base::copyMemory(void* dst, const void* src, sl_size count)
{
	sl_uint8* d = (sl_uint8*)dst;
	sl_uint8* s = (sl_uint8*)src;
	for (sl_size i = 0; i < count; i++) {
		d[i] = s[i];
	}
}

void Base::zeroMemory(void* dst, sl_size count)
{
	sl_uint8* d = (sl_uint8*)dst;
	for (sl_size i = 0; i < count; i++) {
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

void Base::resetMemory2(void* dst, sl_uint16 value, sl_size count)
{
	sl_uint16* d = (sl_uint16*)dst;
	for (sl_size i = 0; i < count; i++) {
		d[i] = value;
	}
}

void Base::resetMemory4(void* dst, sl_uint32 value, sl_size count)
{
	sl_uint32* d = (sl_uint32*)dst;
	for (sl_size i = 0; i < count; i++) {
		d[i] = value;
	}
}

void Base::resetMemory8(void* dst, sl_uint64 value, sl_size count)
{
	sl_uint64* d = (sl_uint64*)dst;
	for (sl_size i = 0; i < count; i++) {
		d[i] = value;
	}
}

sl_int32 Base::compareZero(const void* mem, sl_size count)
{
	sl_uint8* m = (sl_uint8*)mem;
	for (sl_size i = 0; i < count; i++) {
		if (m[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory(const void* mem1, const void* mem2, sl_size count)
{
	sl_uint8* m1 = (sl_uint8*)mem1;
	sl_uint8* m2 = (sl_uint8*)mem2;
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory2(const void* mem1, const void* mem2, sl_size count)
{
	sl_uint16* m1 = (sl_uint16*)mem1;
	sl_uint16* m2 = (sl_uint16*)mem2;
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory4(const void* mem1, const void* mem2, sl_size count)
{
	sl_uint32* m1 = (sl_uint32*)mem1;
	sl_uint32* m2 = (sl_uint32*)mem2;
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

sl_int32 Base::compareMemory8(const void* mem1, const void* mem2, sl_size count)
{
	sl_uint64* m1 = (sl_uint64*)mem1;
	sl_uint64* m2 = (sl_uint64*)mem2;
	for (sl_size i = 0; i < count; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
	}
	return 0;
}

const void* Base::findMemory(const void* mem, sl_uint8 pattern, sl_size count)
{
	sl_uint8* m = (sl_uint8*)mem;
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemoryUntilZero(const void* mem, sl_uint8 pattern, sl_size count)
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

const void* Base::findMemory2(const void* mem, sl_uint16 pattern, sl_size count)
{
	sl_uint16* m = (sl_uint16*)mem;
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemory4(const void* mem, sl_uint32 pattern, sl_size count)
{
	sl_uint32* m = (sl_uint32*)mem;
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemory8(const void* mem, sl_uint64 pattern, sl_size count)
{
	sl_uint64* m = (sl_uint64*)mem;
	for (sl_size i = 0; i < count; i++) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemoryReverse(const void* mem, sl_uint8 pattern, sl_size count)
{
	sl_uint8* m = (sl_uint8*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemoryReverse2(const void* mem, sl_uint16 pattern, sl_size count)
{
	sl_uint16* m = (sl_uint16*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemoryReverse4(const void* mem, sl_uint32 pattern, sl_size count)
{
	sl_uint32* m = (sl_uint32*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

const void* Base::findMemoryReverse8(const void* mem, sl_uint64 pattern, sl_size count)
{
	sl_uint64* m = (sl_uint64*)mem;
	for (sl_reg i = count - 1; i >= 0; i--) {
		if (m[i] == pattern) {
			return m + i;
		}
	}
	return sl_null;
}

sl_int32 Base::compareString(const void *s1, const void *s2, sl_size limitCount)
{
	const sl_uint8* m1 = (const sl_uint8*)s1;
	const sl_uint8* m2 = (const sl_uint8*)s2;
	for (sl_size i = 0; i < limitCount; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
		if (m1[i] == 0) {
			return 0;
		}
	}
	return 0;
}

sl_int32 Base::compareString2(const void *s1, const void *s2, sl_size limitCount)
{
	const sl_uint16* m1 = (const sl_uint16*)s1;
	const sl_uint16* m2 = (const sl_uint16*)s2;
	for (sl_size i = 0; i < limitCount; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
		if (m1[i] == 0) {
			return 0;
		}
	}
	return 0;
}

sl_int32 Base::compareString4(const void *s1, const void *s2, sl_size limitCount)
{
	const sl_uint32* m1 = (const sl_uint32*)s1;
	const sl_uint32* m2 = (const sl_uint32*)s2;
	for (sl_size i = 0; i < limitCount; i++) {
		if (m1[i] < m2[i]) {
			return -1;
		} else if (m1[i] > m2[i]) {
			return 1;
		}
		if (m1[i] == 0) {
			return 0;
		}
	}
	return 0;
}

sl_size Base::copyString(void* dst, const void* src, sl_size limitCount)
{
	sl_uint8* d = (sl_uint8*)dst;
	const sl_uint8* s = (const sl_uint8*)src;
	for (sl_size i = 0; i < limitCount; i++) {
		d[i] = s[i];
		if (d[i] == 0) {
			return i + 1;
		}
	}
	return limitCount;
}

sl_size Base::copyString2(void* dst, const void* src, sl_size limitCount)
{
	sl_uint16* d = (sl_uint16*)dst;
	const sl_uint16* s = (const sl_uint16*)src;
	for (sl_size i = 0; i < limitCount; i++) {
		d[i] = s[i];
		if (d[i] == 0) {
			return i + 1;
		}
	}
	return limitCount;
}

sl_size Base::copyString4(void* dst, const void* src, sl_size limitCount)
{
	sl_uint32* d = (sl_uint32*)dst;
	const sl_uint32* s = (const sl_uint32*)src;
	for (sl_size i = 0; i < limitCount; i++) {
		d[i] = s[i];
		if (d[i] == 0) {
			return i + 1;
		}
	}
	return limitCount;
}

sl_size Base::getStringLength(const void* sz, sl_size limitCount)
{
	const sl_uint8* s = (const sl_uint8*)sz;
	for (sl_size i = 0; i < limitCount; i++) {
		if (s[i] == 0) {
			return i;
		}
	}
	return 0;
}

sl_size Base::getStringLength2(const void* sz, sl_size limitCount)
{
	const sl_uint16* s = (const sl_uint16*)sz;
	for (sl_size i = 0; i < limitCount; i++) {
		if (s[i] == 0) {
			return i;
		}
	}
	return 0;
}

sl_size Base::getStringLength4(const void* sz, sl_size limitCount)
{
	const sl_uint32* s = (const sl_uint32*)sz;
	for (sl_size i = 0; i < limitCount; i++) {
		if (s[i] == 0) {
			return i;
		}
	}
	return 0;
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
#	if (SLIB_COMPILER >= SLIB_COMPILER_VISUALSTUDIO_7)
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
