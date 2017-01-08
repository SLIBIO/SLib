#ifndef CHECKHEADER_SLIB_CORE_HASH
#define CHECKHEADER_SLIB_CORE_HASH

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class Hash;

SLIB_CONSTEXPR sl_uint32 Rehash(sl_uint32 x)
{
	return x ^ (x >> 4) ^ (x >> 7) ^ (x >> 12) ^ (x >> 16) ^ (x >> 19) ^ (x >> 20) ^ (x >> 24) ^ (x >> 27);
}

SLIB_CONSTEXPR sl_uint32 Hash64(sl_uint64 x)
{
	return Rehash((sl_uint32)(x ^ (x >> 32)));
}

sl_uint32 HashBytes(const void* buf, sl_size n);

template <>
class Hash<char>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(char v) const { return Rehash(v);}
};

template <>
class Hash<unsigned char>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(unsigned char v) const { return Rehash(v);}
};

template <>
class Hash<wchar_t>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(wchar_t v) const { return Rehash(v);}
};

template <>
class Hash<short>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(short v) const { return Rehash(v);}
};

template <>
class Hash<unsigned short>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(unsigned short v) const { return Rehash(v);}
};

template <>
class Hash<int>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(int v) const { return Rehash(v);}
};

template <>
class Hash<unsigned int>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(unsigned int v) const { return Rehash(v);}
};

template <>
class Hash<long>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(long v) const { return Rehash((sl_uint32)v);}
};

template <>
class Hash<unsigned long>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(unsigned long v) const { return Rehash((sl_uint32)v);}
};

template <>
class Hash<sl_int64>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(sl_int64 v) const { return Hash64(v); }
};

template <>
class Hash<sl_uint64>
{
public:
	SLIB_CONSTEXPR sl_uint32 operator()(sl_uint64 v) const { return Hash64(v); }
};

template <>
class Hash<float>
{
public:
	SLIB_INLINE sl_uint32 operator()(float v) const { return Rehash(*(reinterpret_cast<sl_uint32*>(&v))); }
};

template <>
class Hash<double>
{
public:
	SLIB_INLINE sl_uint32 operator()(double v) const { return Hash64(*(reinterpret_cast<sl_uint64*>(&v))); }
};

template <class T>
class Hash<T const*>
{
public:
	SLIB_INLINE sl_uint32 operator()(T const* v) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return Hash64((sl_uint64)((const void*)v));
#else
		return Rehash((sl_uint32)((const void*)v));
#endif
	}
};

template <class T>
class Hash<T*>
{
public:
	SLIB_INLINE sl_uint32 operator()(T* v) const
	{
#ifdef SLIB_ARCH_IS_64BIT
		return Hash64((sl_uint64)((const void*)v));
#else
		return Rehash((sl_uint32)((const void*)v));
#endif
	}
};

SLIB_NAMESPACE_END

#endif
