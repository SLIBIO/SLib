#ifndef CHECKHEADER_SLIB_CORE_BASE
#define CHECKHEADER_SLIB_CORE_BASE

#include "definition.h"

#include "cpp.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Base
{
public:
	// Memory Allocation Functions
	static void* createMemory(sl_size size);
	
	static void* reallocMemory(void* ptr, sl_size sizeNew);
	
	static void freeMemory(void* ptr);

	static void* createZeroMemory(sl_size size);
	
	// Memory Utilities
	static void copyMemory(void* dst, const void* src, sl_size count);
	
	static void zeroMemory(void* dst, sl_size size);
	
	
	static void resetMemory(void* dst, sl_uint8 value, sl_size count);
	
	static void resetMemory2(sl_uint16* dst, sl_uint16 value, sl_size count);
	
	static void resetMemory2(sl_int16* dst, sl_int16 value, sl_size count);
	
	static void resetMemory4(sl_uint32* dst, sl_uint32 value, sl_size count);
	
	static void resetMemory4(sl_int32* dst, sl_int32 value, sl_size count);
	
	static void resetMemory8(sl_uint64* dst, sl_uint64 value, sl_size count);
	
	static void resetMemory8(sl_int64* dst, sl_int64 value, sl_size count);
	
	
	
	static sl_bool equalsMemory(const void* mem1, const void* mem2, sl_size count);
	
	static sl_bool equalsMemory2(const sl_uint16* mem1, const sl_uint16* mem2, sl_size count);
	
	static sl_bool equalsMemory2(const sl_int16* mem1, const sl_int16* mem2, sl_size count);
	
	static sl_bool equalsMemory4(const sl_uint32* mem1, const sl_uint32* sl_uint32, sl_size count);
	
	static sl_bool equalsMemory4(const sl_int32* mem1, const sl_int32* sl_int32, sl_size count);
	
	static sl_bool equalsMemory8(const sl_uint64* mem1, const sl_uint64* mem2, sl_size count);
	
	static sl_bool equalsMemory8(const sl_int64* mem1, const sl_int64* mem2, sl_size count);
	
	
	static sl_int32 compareMemory(const sl_uint8* mem1, const sl_uint8* mem2, sl_size count);
	
	static sl_int32 compareMemory(const sl_int8* mem1, const sl_int8* mem2, sl_size count);
	
	static sl_int32 compareMemory2(const sl_uint16* mem1, const sl_uint16* mem2, sl_size count);
	
	static sl_int32 compareMemory2(const sl_int16* mem1, const sl_int16* mem2, sl_size count);
	
	static sl_int32 compareMemory4(const sl_uint32* mem1, const sl_uint32* sl_uint32, sl_size count);
	
	static sl_int32 compareMemory4(const sl_int32* mem1, const sl_int32* sl_int32, sl_size count);
	
	static sl_int32 compareMemory8(const sl_uint64* mem1, const sl_uint64* mem2, sl_size count);
	
	static sl_int32 compareMemory8(const sl_int64* mem1, const sl_int64* mem2, sl_size count);
	
	
	static sl_bool equalsMemoryZero(const void* mem, sl_size count);
	
	static sl_bool equalsMemoryZero2(const sl_uint16* mem, sl_size count);
	
	static sl_bool equalsMemoryZero2(const sl_int16* mem, sl_size count);
	
	static sl_bool equalsMemoryZero4(const sl_uint32* mem, sl_size count);
	
	static sl_bool equalsMemoryZero4(const sl_int32* mem, sl_size count);
	
	static sl_bool equalsMemoryZero8(const sl_uint64* mem, sl_size count);
	
	static sl_bool equalsMemoryZero8(const sl_int64* mem, sl_size count);

	
	static sl_int32 compareMemoryZero(const sl_uint8* mem, sl_size count);
	
	static sl_int32 compareMemoryZero(const sl_int8* mem, sl_size count);
	
	static sl_int32 compareMemoryZero2(const sl_uint16* mem, sl_size count);
	
	static sl_int32 compareMemoryZero2(const sl_int16* mem, sl_size count);
	
	static sl_int32 compareMemoryZero4(const sl_uint32* mem, sl_size count);
	
	static sl_int32 compareMemoryZero4(const sl_int32* mem, sl_size count);
	
	static sl_int32 compareMemoryZero8(const sl_uint64* mem, sl_size count);
	
	static sl_int32 compareMemoryZero8(const sl_int64* mem, sl_size count);
	
	
	static const sl_uint8* findMemory(const void* mem, sl_uint8 pattern, sl_size count);

	static const sl_int8* findMemory(const sl_int8* mem, sl_int8 pattern, sl_size count);
	
	static const sl_uint16* findMemory2(const sl_uint16* mem, sl_uint16 pattern, sl_size count);
	
	static const sl_int16* findMemory2(const sl_int16* mem, sl_int16 pattern, sl_size count);
	
	static const sl_uint32* findMemory4(const sl_uint32* mem, sl_uint32 pattern, sl_size count);
	
	static const sl_int32* findMemory4(const sl_int32* mem, sl_int32 pattern, sl_size count);
	
	static const sl_uint64* findMemory8(const sl_uint64* mem, sl_uint64 pattern, sl_size count);
	
	static const sl_int64* findMemory8(const sl_int64* mem, sl_int64 pattern, sl_size count);
	
	
	static const sl_uint8* findMemoryReverse(const void* mem, sl_uint8 pattern, sl_size count);
	
	static const sl_uint16* findMemoryReverse2(const sl_uint16* mem, sl_uint16 pattern, sl_size count);
	
	static const sl_int16* findMemoryReverse2(const sl_int16* mem, sl_int16 pattern, sl_size count);
	
	static const sl_uint32* findMemoryReverse4(const sl_uint32* mem, sl_uint32 pattern, sl_size count);
	
	static const sl_int32* findMemoryReverse4(const sl_int32* mem, sl_int32 pattern, sl_size count);
	
	static const sl_uint64* findMemoryReverse8(const sl_uint64* mem, sl_uint64 pattern, sl_size count);
	
	static const sl_int64* findMemoryReverse8(const sl_int64* mem, sl_int64 pattern, sl_size count);

	
	static const sl_uint8* findMemoryUntilZero(const void* mem, sl_uint8 pattern, sl_size count);
	
	
	static sl_bool equalsString(const sl_char8* s1, const sl_char8* s2, sl_reg count = -1);
	
	static sl_bool equalsString2(const sl_char16* s1, const sl_char16* s2, sl_reg count = -1);
	
	static sl_bool equalsString4(const sl_char32* s1, const sl_char32* s2, sl_reg count = -1);

	
	static sl_int32 compareString(const sl_char8* s1, const sl_char8* s2, sl_reg count = -1);
	
	static sl_int32 compareString2(const sl_char16* s1, const sl_char16* s2, sl_reg count = -1);
	
	static sl_int32 compareString4(const sl_char32* s1, const sl_char32* s2, sl_reg count = -1);
	
	
	static sl_size copyString(sl_char8* dst, const sl_char8* src, sl_reg count = -1);
	
	static sl_size copyString2(sl_char16* dst, const sl_char16* src, sl_reg count = -1);
	
	static sl_size copyString4(sl_char32* dst, const sl_char32* src, sl_reg count = -1);
	
	
	static sl_size getStringLength(const sl_char8* src, sl_reg count = -1);
	
	static sl_size getStringLength2(const sl_char16* src, sl_reg count = -1);
	
	static sl_size getStringLength4(const sl_char32* src, sl_reg count = -1);
	

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
#endif

	static sl_reg interlockedIncrement(sl_reg* pValue);
	
	static sl_reg interlockedDecrement(sl_reg* pValue);
	
	static sl_reg interlockedAdd(sl_reg* pDst, sl_reg value);
	
	static sl_bool interlockedCompareExchange(sl_reg* pDst, sl_reg value, sl_reg comperand);
	
	static void* interlockedAddPtr(void** pDst, sl_reg value);
	
	static sl_bool interlockedCompareExchangePtr(void** pDst, const void* value, const void* comperand);
	
	
	static void sleep(sl_uint32 millis);
	
	static void yield();
	
	static void yield(sl_uint32 ellapsed);
	
};

SLIB_NAMESPACE_END

#endif

