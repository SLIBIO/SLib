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

SLIB_NAMESPACE_BEGIN

void* Base::createMemory(sl_size size)
{
#ifndef FORCE_MEM_ALIGNED
	void* ptr = malloc(size);
	if (ptr == sl_null) {
		return sl_null;
	}
	return ptr;
#else
	sl_reg ptr = (sl_reg)(malloc(size + 16));
	if (ptr == sl_null) {
		return sl_null;
	}
	sl_reg ptr_aligned = (ptr | 7) + 9;
	*(char*)(ptr_aligned - 1) = (char)(ptr_aligned - ptr);
	*(sl_size*)(ptr) = size;
	return (void*)(ptr_aligned);
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
	void* ptrNew = realloc(ptr, sizeNew);
	if (ptrNew == sl_null) {
		return sl_null;
	}
	return ptrNew;
#else
	void* ptr_new = createMemory(sizeNew);
	sl_size size_old = *(sl_size*)((sl_reg)ptr - *((unsigned char*)ptr - 1));
	copyMemory(ptr_new, ptr, size_old);
	freeMemory(ptr);
	return ptr_new;
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
SLIB_NAMESPACE_END

// Interlocked Functions (Synchronization of Atom Operations)
#ifdef SLIB_PLATFORM_IS_WINDOWS

#include <windows.h>

SLIB_NAMESPACE_BEGIN
sl_int32 Base::interlockedIncrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
	return (sl_int32)InterlockedIncrement((LONG*)pValue);
}

sl_int32 Base::interlockedDecrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
	return (sl_int32)InterlockedDecrement((LONG*)pValue);
}

sl_int32 Base::interlockedAdd32(sl_int32* pDst, sl_int32 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
	return ((sl_int32)InterlockedExchangeAdd((LONG*)pDst, (LONG)value)) + value;
}

sl_bool Base::interlockedCompareExchange32(sl_int32* pDst, sl_int32 value, sl_int32 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
	sl_int32 old;
#if (SLIB_COMPILER >= SLIB_COMPILER_VISUALSTUDIO_7)
	old = ((sl_int32)InterlockedCompareExchange((LONG*)pDst, (LONG)value, (LONG)comperand));
#else
	old = ((sl_int32)InterlockedCompareExchange((void**)pDst, (void*)value, (void*)comperand));
#endif
	return old == comperand;
}

#ifdef SLIB_ARCH_IS_64BIT
sl_int64 Base::interlockedIncrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
	return (sl_int64)InterlockedIncrement64((LONGLONG*)pValue);
}

sl_int64 Base::interlockedDecrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
	return (sl_int64)InterlockedDecrement64((LONGLONG*)pValue);
}

sl_int64 Base::interlockedAdd64(sl_int64* pDst, sl_int64 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
	return ((sl_int64)InterlockedExchangeAdd64((LONGLONG*)pDst, (LONGLONG)value)) + value;
}

sl_bool Base::interlockedCompareExchange64(sl_int64* pDst, sl_int64 value, sl_int64 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
	sl_int64 old = ((sl_int64)InterlockedCompareExchange64((LONGLONG*)pDst, (LONGLONG)value, (LONGLONG)comperand));
	return old == comperand;
}
#endif

SLIB_NAMESPACE_END

#elif defined(SLIB_PLATFORM_IS_APPLE)

#include <libkern/OSAtomic.h>

SLIB_NAMESPACE_BEGIN
sl_int32 Base::interlockedIncrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
	return OSAtomicIncrement32Barrier((int32_t*)pValue);
}

sl_int32 Base::interlockedDecrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
	return OSAtomicDecrement32Barrier((int32_t*)pValue);
}

sl_int32 Base::interlockedAdd32(sl_int32* pDst, sl_int32 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
	return OSAtomicAdd32Barrier((int32_t)value, (int32_t*)pDst);
}

sl_bool Base::interlockedCompareExchange32(sl_int32* pDst, sl_int32 value, sl_int32 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
	return OSAtomicCompareAndSwap32Barrier((int32_t)comperand, (int32_t)value, (int32_t*)pDst) != false;
}

#ifdef SLIB_ARCH_IS_64BIT
sl_int64 Base::interlockedIncrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
	return OSAtomicIncrement64Barrier(pValue);
}

sl_int64 Base::interlockedDecrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
	return OSAtomicDecrement64Barrier(pValue);
}

sl_int64 Base::interlockedAdd64(sl_int64* pDst, sl_int64 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
	return OSAtomicAdd64Barrier(value, pDst);
}

sl_bool Base::interlockedCompareExchange64(sl_int64* pDst, sl_int64 value, sl_int64 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
	return OSAtomicCompareAndSwap64Barrier(comperand, value, pDst) != false;
}
#endif
SLIB_NAMESPACE_END

#elif defined(SLIB_PLATFORM_IS_UNIX)

SLIB_NAMESPACE_BEGIN
sl_int32 Base::interlockedIncrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
	return __sync_fetch_and_add(pValue, 1) + 1;
}

sl_int32 Base::interlockedDecrement32(sl_int32* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pValue));
	return __sync_fetch_and_add(pValue, -1) - 1;
}

sl_int32 Base::interlockedAdd32(sl_int32* pDst, sl_int32 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
	return __sync_fetch_and_add(pDst, value) + value;
}

sl_bool Base::interlockedCompareExchange32(sl_int32* pDst, sl_int32 value, sl_int32 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_4(pDst));
	return __sync_bool_compare_and_swap(pDst, comperand, value) != 0;
}

#ifdef SLIB_ARCH_IS_64BIT
sl_int64 Base::interlockedIncrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
	return __sync_fetch_and_add(pValue, 1) + 1;
}

sl_int64 Base::interlockedDecrement64(sl_int64* pValue)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pValue));
	return __sync_fetch_and_add(pValue, -1) - 1;
}

sl_int64 Base::interlockedAdd64(sl_int64* pDst, sl_int64 value)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
	return __sync_fetch_and_add(pDst, value) + value;
}

sl_bool Base::interlockedCompareExchange64(sl_int64* pDst, sl_int64 value, sl_int64 comperand)
{
	SLIB_ASSERT(SLIB_IS_ALIGNED_8(pDst));
	return __sync_bool_compare_and_swap(pDst, comperand, value) != 0;
}
#endif

SLIB_NAMESPACE_END
#endif


SLIB_NAMESPACE_BEGIN
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
