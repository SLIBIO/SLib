#ifndef CHECKHEADER_SLIB_CORE_HASH
#define CHECKHEADER_SLIB_CORE_HASH

#include "definition.h"

SLIB_NAMESPACE_BEGIN

sl_uint32 sl_rehash(sl_uint32 code);
sl_uint32 sl_hash(char n);
sl_uint32 sl_hash(unsigned char n);
sl_uint32 sl_hash(short n);
sl_uint32 sl_hash(unsigned short n);
sl_uint32 sl_hash(int n);
sl_uint32 sl_hash(unsigned int n);
sl_uint32 sl_hash(long n);
sl_uint32 sl_hash(unsigned long n);
sl_uint32 sl_hash(long long n);
sl_uint32 sl_hash(unsigned long long n);
sl_uint32 sl_hash(const void* ptr);
sl_uint32 sl_hash(float f);
sl_uint32 sl_hash(double f);
sl_uint32 sl_hash(const void* data, sl_size size);

template <class TYPE>
class SLIB_EXPORT Hash
{
public:
	static sl_uint32 hash(const TYPE& v);
	
};


SLIB_INLINE sl_uint32 sl_rehash(sl_uint32 code)
{
	code ^= (code >> 20) ^ (code >> 12);
	code ^= (code >> 7) ^ (code >> 4);
	return code;
}

SLIB_INLINE sl_uint32 sl_hash(char n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned char n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(short n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned short n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(int n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned int n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(long n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned long n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(long long n)
{
	return sl_rehash((sl_uint32)(n ^ (n >> 32)));
}

SLIB_INLINE sl_uint32 sl_hash(unsigned long long n)
{
	return sl_rehash((sl_uint32)(n ^ (n >> 32)));
}

SLIB_INLINE sl_uint32 sl_hash(const void* ptr)
{
#ifdef SLIB_ARCH_IS_64BIT
	return sl_hash((sl_uint64)ptr);
#else
	return sl_hash((sl_uint32)ptr);
#endif
}

SLIB_INLINE sl_uint32 sl_hash(float f)
{
	return sl_rehash(*(sl_uint32*)(void*)(&f));
}

SLIB_INLINE sl_uint32 sl_hash(double f)
{
	return sl_hash(*(sl_uint64*)(void*)(&f));
}

template <class TYPE>
SLIB_INLINE sl_uint32 Hash<TYPE>::hash(const TYPE& v)
{
	return sl_hash(v);
}

SLIB_NAMESPACE_END

#endif
