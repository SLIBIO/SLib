#ifndef CHECKHEADER_SLIB_CORE_BASE
#define CHECKHEADER_SLIB_CORE_BASE

#include "definition.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Base
{
public:
	// Memory Allocation Functions
	static void* createMemory(sl_size size);
	
	static void* reallocMemory(void* ptr, sl_size sizeNew);
	
	static void freeMemory(void* ptr);

	
	// Memory Utilities
	static void copyMemory(void* dst, const void* src, sl_size count);
	
	static void zeroMemory(void* dst, sl_size count);
	
	
	static void resetMemory(void* dst, sl_uint8 value, sl_size count);
	
	static void resetMemory2(void* dst, sl_uint16 value, sl_size count);
	
	static void resetMemory4(void* dst, sl_uint32 value, sl_size count);
	
	static void resetMemory8(void* dst, sl_uint64 value, sl_size count);
	
	
	static sl_int32 compareZero(const void* mem, sl_size count);
	
	static sl_int32 compareMemory(const void* mem1, const void* mem2, sl_size count);
	
	static sl_int32 compareMemory2(const void* mem1, const void* mem2, sl_size count);
	
	static sl_int32 compareMemory4(const void* mem1, const void* mem2, sl_size count);
	
	static sl_int32 compareMemory8(const void* mem1, const void* mem2, sl_size count);
	
	
	static const void* findMemory(const void* mem, sl_uint8 pattern, sl_size count);
	
	static const void* findMemoryUntilZero(const void* mem, sl_uint8 pattern, sl_size count = SLIB_SIZE_MAX);
	
	static const void* findMemory2(const void* mem, sl_uint16 pattern, sl_size count);
	
	static const void* findMemory4(const void* mem, sl_uint32 pattern, sl_size count);
	
	static const void* findMemory8(const void* mem, sl_uint64 pattern, sl_size count);
	
	
	static const void* findMemoryReverse(const void* mem, sl_uint8 pattern, sl_size count);
	
	static const void* findMemoryReverse2(const void* mem, sl_uint16 pattern, sl_size count);
	
	static const void* findMemoryReverse4(const void* mem, sl_uint32 pattern, sl_size count);
	
	static const void* findMemoryReverse8(const void* mem, sl_uint64 pattern, sl_size count);
	
	
	static sl_int32 compareString(const void* s1, const void* s2, sl_size limitCount = SLIB_SIZE_MAX);
	
	static sl_int32 compareString2(const void* s1, const void* s2, sl_size limitCount = SLIB_SIZE_MAX);
	
	static sl_int32 compareString4(const void* s1, const void* s2, sl_size limitCount = SLIB_SIZE_MAX);
	
	
	static sl_size copyString(void* dst, const void* src, sl_size limitCount = SLIB_SIZE_MAX);
	
	static sl_size copyString2(void* dst, const void* src, sl_size limitCount = SLIB_SIZE_MAX);
	
	static sl_size copyString4(void* dst, const void* src, sl_size limitCount = SLIB_SIZE_MAX);
	
	
	static sl_size getStringLength(const void* src, sl_size limitCount = SLIB_SIZE_MAX);
	
	static sl_size getStringLength2(const void* src, sl_size limitCount = SLIB_SIZE_MAX);
	
	static sl_size getStringLength4(const void* src, sl_size limitCount = SLIB_SIZE_MAX);
	

	// Interlocked Functions (Synchronization of Atom Operations)
	static sl_int32 interlockedIncrement32(sl_int32* pValue);
	
	static sl_int32 interlockedDecrement32(sl_int32* pValue);
	
	static sl_int32 interlockedAdd32(sl_int32* pDst, sl_int32 value);
	
	static sl_bool interlockedCompareExchange32(sl_int32* pDst, sl_int32 value, sl_int32 comperand);
	
#ifdef SLIB_ARCH_IS_64BIT
	
	static sl_int64 interlockedIncrement64(sl_int64* pValue);
	
	static sl_int64 interlockedDecrement64(sl_int64* pValue);
	
	static sl_int64 interlockedAdd64(sl_int64* pDst, sl_int64 value);
	
	static sl_bool interlockedCompareExchange64(sl_int64* pDst, sl_int64 value, sl_int64 comperand);

	SLIB_INLINE static sl_reg interlockedIncrement(sl_reg* pValue)
	{
		return interlockedIncrement64(pValue);
	}
	
	SLIB_INLINE static sl_reg interlockedDecrement(sl_reg* pValue)
	{
		return interlockedDecrement64(pValue);
	}
	
	SLIB_INLINE static sl_reg interlockedAdd(sl_reg* pDst, sl_reg value)
	{
		return interlockedAdd64(pDst, value);
	}
	
	SLIB_INLINE static sl_bool interlockedCompareExchange(sl_reg* pDst, sl_reg value, sl_reg comperand)
	{
		return interlockedCompareExchange64(pDst, value, comperand);
	}
	
	SLIB_INLINE static void* interlockedAddPtr(void** pDst, sl_reg value)
	{
		return (void*)interlockedAdd64((sl_int64*)(pDst), value);
	}
	
	SLIB_INLINE static sl_bool interlockedCompareExchangePtr(void** pDst, const void* value, const void* comperand)
	{
		return interlockedCompareExchange64((sl_int64*)(pDst), (sl_int64)value, (sl_int64)comperand);
	}
	
#else
	
	SLIB_INLINE static sl_reg interlockedIncrement(sl_reg* pValue)
	{
		return interlockedIncrement32(pValue);
	}
	
	SLIB_INLINE static sl_reg interlockedDecrement(sl_reg* pValue)
	{
		return interlockedDecrement32(pValue);
	}
	
	SLIB_INLINE static sl_reg interlockedAdd(sl_reg* pDst, sl_reg value)
	{
		return interlockedAdd32(pDst, value);
	}
	
	SLIB_INLINE static sl_bool interlockedCompareExchange(sl_reg* pDst, sl_reg value, sl_reg comperand)
	{
		return interlockedCompareExchange32(pDst, value, comperand);
	}
	
	SLIB_INLINE static void* interlockedAddPtr(void** pDst, sl_reg value)
	{
		return (void*)interlockedAdd32((sl_int32*)pDst, value);
	}
	
	SLIB_INLINE static sl_bool interlockedCompareExchangePtr(void** pDst, const void* value, const void* comperand)
	{
		return interlockedCompareExchange32((sl_int32*)pDst, (sl_int32)value, (sl_int32)comperand);
	}
	
#endif

	static void sleep(sl_uint32 millis);
	
	static void yield();
	
	static void yield(sl_uint32 ellapsed);
};

SLIB_NAMESPACE_END

#if !defined(_LIBCPP_NEW) && !defined(_NEW_) && !defined(_NEW)

#	ifndef __PLACEMENT_NEW_INLINE
#		define __PLACEMENT_NEW_INLINE
SLIB_INLINE void* (operator new)(sl_size_t in_size, void* in_pWhere)
{
	return in_pWhere;
}

SLIB_INLINE void(operator delete)(void* in_pVoid, void* in_pWhere)
{
}
#	endif

#	ifndef __PLACEMENT_VEC_NEW_INLINE
#		define __PLACEMENT_VEC_NEW_INLINE
SLIB_INLINE void* (operator new[])(sl_size_t in_size, void* in_pWhere)
{
    return in_pWhere;
}

SLIB_INLINE void(operator delete[])(void* in_pVoid, void* in_pWhere)
{
}
#	endif

#endif


#endif //CHECKHEADER_SLIB_CORE_BASE

