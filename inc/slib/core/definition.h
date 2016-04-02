#ifndef CHECKHEADER_SLIB_CORE_DEFINITION
#define CHECKHEADER_SLIB_CORE_DEFINITION

#include "option.h"

#define SLIB_NAMESPACE_BEGIN namespace slib {
#define SLIB_NAMESPACE_END }

/************************************************************************/
/* SLIB Native Type Declarations                                        */
/************************************************************************/

#if defined(SLIB_COMPILER_IS_VISUALSTUDIO)

#if defined (_DEBUG)
#define SLIB_DEBUG
#endif

#	define SLIB_STDCALL		__stdcall
#	define SLIB_INLINE		__forceinline
#	define SLIB_THREAD		__declspec(thread)
#	define SLIB_INT64(v)	(v##i64)
#	define SLIB_UINT64(v)	(v##ui64)
typedef __int64				sl_int64;
typedef unsigned __int64	sl_uint64;
typedef size_t sl_size_t;

#define SLIB_ALIGN(var, n) __declspec(align(n)) var
#define SLIB_EXPORT
#define SLIB_VISIBLE_LOCAL

#elif defined(SLIB_COMPILER_IS_GCC)

#if ! defined (__OPTIMIZE__)
#define SLIB_DEBUG
#endif

#	define SLIB_STDCALL		__attribute__((stdcall))
#	define SLIB_INLINE		inline __attribute__((always_inline))
#	define SLIB_INT64(v)	(v##LL)
#	define SLIB_UINT64(v)	(v##ULL)
#	define SLIB_THREAD		__thread
typedef long long			sl_int64;
typedef unsigned long long	sl_uint64;
typedef __SIZE_TYPE__ sl_size_t;

#define SLIB_ALIGN(var, n) var __attribute__((aligned(n)))
#if __GNUC__ >= 4
#define SLIB_EXPORT __attribute__((visibility("default")))
#define SLIB_VISIBLE_LOCAL __attribute__((visibility("hidden")))
#else
#define SLIB_EXPORT
#define SLIB_VISIBLE_LOCAL
#endif

#endif

// Basic Type Definition
typedef int					sl_int;
typedef unsigned int		sl_uint;
typedef signed char			sl_int8;
typedef unsigned char		sl_uint8;
typedef short				sl_int16;
typedef unsigned short		sl_uint16;
#if defined(__SIZEOF_INT__)
#	if __SIZEOF_INT__ == 4
typedef int					sl_int32;
typedef unsigned int		sl_uint32;
#	else
typedef long				sl_int32;
typedef unsigned long		sl_uint32;
#	endif
#else
typedef int					sl_int32;
typedef unsigned int		sl_uint32;
#endif
typedef float				sl_real;

typedef void*				sl_ptr;
typedef const void*			sl_cptr;

#define sl_null				nullptr

typedef bool sl_bool;
#define sl_true true
#define sl_false false

#define SLIB_UINT8_MAX		(sl_uint8)(0xFF)
#define SLIB_INT8_MAX		(sl_int8)(0x7F)
#define SLIB_INT8_MIN		(sl_int8)(-0x80)
#define SLIB_UINT16_MAX		(sl_uint16)(0xFFFF)
#define SLIB_INT16_MAX		(sl_int16)(0x7FFF)
#define SLIB_INT16_MIN		(sl_int16)(-0x8000)
#define SLIB_UINT32_MAX		0xFFFFFFFF
#define SLIB_INT32_MAX		0x7FFFFFFF
#define SLIB_INT32_MIN		-0x80000000
#define SLIB_UINT64_MAX		SLIB_UINT64(0xFFFFFFFFFFFFFFFF)
#define SLIB_INT64_MAX		SLIB_INT64(0x7FFFFFFFFFFFFFFF)
#define SLIB_INT64_MIN		SLIB_INT64(-0x8000000000000000)

#ifdef SLIB_ARCH_IS_64BIT
typedef sl_uint64			sl_size;
typedef sl_int64			sl_reg;
#define SLIB_SIZE_MAX		SLIB_UINT64_MAX
#define SLIB_REG_MAX		SLIB_INT64_MAX
#define SLIB_REG_MIN		SLIB_INT64_MIN
#else
typedef sl_uint32			sl_size;
typedef sl_int32			sl_reg;
#define SLIB_SIZE_MAX		SLIB_UINT32_MAX
#define SLIB_REG_MAX		SLIB_INT32_MAX
#define SLIB_REG_MIN		SLIB_INT32_MIN
#endif

typedef char				sl_char8;
#if defined(SLIB_COMPILER_IS_VISUALSTUDIO)
typedef wchar_t				sl_char16;
#else
typedef sl_uint16			sl_char16;
#endif
typedef sl_uint32			sl_char32;

// Unicode String Constant Definition
#if defined(SLIB_COMPILER_IS_VISUALSTUDIO)

#	define SLIB_UNICODE(quote)		(const sl_char16*)(L##quote)
#	define SLIB_USE_UNICODE16

#define SLIB_STRINGIFY(...) (#__VA_ARGS__)
#define SLIB_STRINGIFY_UNICODE(...) SLIB_UNICODE(#__VA_ARGS__)

#elif defined(SLIB_COMPILER_IS_GCC)

#	if defined(__SIZEOF_WCHAR_T__) && (__SIZEOF_WCHAR_T__ == 2)
#		define SLIB_UNICODE(quote)		(const sl_char16*)(L##quote)
#		define SLIB_USE_UNICODE16
#	elif defined(__SIZEOF_WCHAR_T__) && (__SIZEOF_WCHAR_T__ == 4)
#		define SLIB_UNICODE(quote)		(const sl_char32*)(L##quote)
#		define SLIB_USE_UNICODE32
#	endif

#define SLIB_STRINGIFY(x...) (#x)
#define SLIB_STRINGIFY_UNICODE(x...) SLIB_UNICODE(#x)

#endif

SLIB_INLINE void sl_blank_proc(const void*) {}
#define SLIB_UNUSED(x) sl_blank_proc(&x)

#define SLIB_PARSE_ERROR (sl_reg)(-1)

#define SLIB_IS_ALIGNED(p, a) (!((unsigned long)(p) & ((a) - 1)))
#define SLIB_IS_ALIGNED_4(x) (!(((sl_reg)((void*)(x))) & 3))
#define SLIB_IS_ALIGNED_8(x) (!(((sl_reg)((void*)(x))) & 7))

#define SLIB_MAX(a, b) ((a)>(b)?(a):(b))
#define SLIB_MIN(a, b) ((a)<(b)?(a):(b))

template <class T>
SLIB_INLINE void sl_swap(T& a, T& b)
{
	T o = b;
	b = a;
	a = o;
}
#define SLIB_SWAP(a, b) sl_swap(a, b);

#define SLIB_CHECK_FLAG(v, flag) (((v) & (flag)) != 0)
#define SLIB_SET_FLAG(v, flag) v |= (flag);
#define SLIB_RESET_FLAG(v, flag) v &= (~(flag));

#define SLIB_GET_BYTE(A,n) ((sl_uint8)((A) >> (n << 3)))
#define SLIB_GET_BYTE0(A) ((sl_uint8)(A))
#define SLIB_GET_BYTE1(A) ((sl_uint8)((A) >> 8))
#define SLIB_GET_BYTE2(A) ((sl_uint8)((A) >> 16))
#define SLIB_GET_BYTE3(A) ((sl_uint8)((A) >> 24))
#define SLIB_GET_BYTE4(A) ((sl_uint8)((A) >> 32))
#define SLIB_GET_BYTE5(A) ((sl_uint8)((A) >> 40))
#define SLIB_GET_BYTE6(A) ((sl_uint8)((A) >> 48))
#define SLIB_GET_BYTE7(A) ((sl_uint8)((A) >> 56))
#define SLIB_GET_WORD(A,n) ((sl_uint16)((A) >> (n << 4)))
#define SLIB_GET_WORD0(A) ((sl_uint16)(A))
#define SLIB_GET_WORD1(A) ((sl_uint16)((A) >> 16))
#define SLIB_GET_WORD2(A) ((sl_uint16)((A) >> 32))
#define SLIB_GET_WORD3(A) ((sl_uint16)((A) >> 48))
#define SLIB_GET_DWORD(A,n) ((sl_uint32)((A) >> (n << 5)))
#define SLIB_GET_DWORD0(A) ((sl_uint32)(A))
#define SLIB_GET_DWORD1(A) ((sl_uint32)((A) >> 32))

#define SLIB_MAKE_WORD(A,B) ((((sl_uint32)(A))<<8) | ((sl_uint32)(B)))
#define SLIB_MAKE_DWORD(A,B,C,D) ((((sl_uint32)(A))<<24) | (((sl_uint32)(B))<<16) | (((sl_uint32)(C))<<8) | ((sl_uint32)(D)))
#define SLIB_MAKE_DWORD2(A,B) ((((sl_uint32)(A))<<16) | ((sl_uint32)(B)))
#define SLIB_MAKE_QWORD(A,B,C,D,E,F,G,H) (((sl_uint64)(SLIB_MAKE_DWORD(A,B,C,D)) << 32) | SLIB_MAKE_DWORD(E,F,G,H))
#define SLIB_MAKE_QWORD2(A,B,C,D) (((sl_uint64)(SLIB_MAKE_DWORD2(A,B)) << 32) | SLIB_MAKE_DWORD2(C,D))
#define SLIB_MAKE_QWORD4(A, B) ((((sl_uint64)(A)) << 32) | (B))

/************************************************************************/
/* Operating System Related Difinitions                                 */
/************************************************************************/
#if (SLIB_PLATFORM==SLIB_PLATFORM_WIN32)
#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT 0x0501
#	endif
#endif

void sl_abort(const char* msg, const char* file, sl_uint32 line);
void sl_log(const char* tag, const char* msg);
void sl_log_error(const char* tag, const char* msg);

#if defined(SLIB_DEBUG)
#define SLIB_ASSERT(_Expression) ( (!!(_Expression)) || (sl_abort(#_Expression, __FILE__, __LINE__), 0) )
#define SLIB_ABORT(MESSAGE) (sl_abort(MESSAGE, __FILE__, __LINE__))
#else
#define SLIB_ASSERT(_Expression)
#define SLIB_ABORT(MESSAGE)
#endif

#endif
