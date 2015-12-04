#ifndef CHECKHEADER_SLIB_NETWORK_MAC_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_MAC_ADDRESS

#include "definition.h"

#include "ip_address.h"

#include "../core/mio.h"
#include "../core/string.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT MacAddress
{
public:
	sl_uint8 m[6];
private:
	static const sl_uint8 _zero[6];
	static const sl_uint8 _broadcast[6];

public:
	SLIB_INLINE MacAddress()
	{
	}

	SLIB_INLINE MacAddress(const sl_uint8* _m)
	{
		set(_m);
	}
	
	SLIB_INLINE MacAddress(sl_uint8 m0, sl_uint8 m1, sl_uint8 m2, sl_uint8 m3, sl_uint8 m4, sl_uint8 m5)
	{
		m[0] = m0;
		m[1] = m1;
		m[2] = m2;
		m[3] = m3;
		m[4] = m4;
		m[5] = m5;
	}
	
	SLIB_INLINE MacAddress(const MacAddress& other)
	{
		m[0] = other.m[0];
		m[1] = other.m[1];
		m[2] = other.m[2];
		m[3] = other.m[3];
		m[4] = other.m[4];
		m[5] = other.m[5];
	}
	
	SLIB_INLINE MacAddress(const String& address)
	{
		if (!parse(address)) {
			setZero();
		}
	}

	SLIB_INLINE static const MacAddress& zero()
	{
		return *((MacAddress*)((void*)(&_zero)));
	}

	SLIB_INLINE static const MacAddress& getZero()
	{
		return *((MacAddress*)((void*)(&_zero)));
	}

	SLIB_INLINE sl_bool isZero() const
	{
		return m[0] == 0 && m[1] == 0 && m[2] == 0 && m[3] == 0 && m[4] == 0 && m[5] == 0;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return !isZero();
	}

	SLIB_INLINE void setZero()
	{
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
		m[4] = 0;
		m[5] = 0;
	}

	SLIB_INLINE static const MacAddress& broadcast()
	{
		return *((MacAddress*)((void*)(&_broadcast)));
	}

	SLIB_INLINE static const MacAddress& getBroadcast()
	{
		return *((MacAddress*)((void*)(&_broadcast)));
	}

	SLIB_INLINE sl_bool isBroadcast() const
	{
		return m[0] == 255 && m[1] == 255 && m[2] == 255 && m[3] == 255 && m[4] == 255 && m[5] == 255;
	}

	SLIB_INLINE sl_bool isNotBroadcast() const
	{
		return !isBroadcast();
	}

	SLIB_INLINE void setBroadcast()
	{
		m[0] = 255;
		m[1] = 255;
		m[2] = 255;
		m[3] = 255;
		m[4] = 255;
		m[5] = 255;
	}
	
	SLIB_INLINE sl_bool isMulticast() const
	{
		return (m[0] & 1);
	}

	SLIB_INLINE sl_bool isNotMulticast() const
	{
		return (m[0] & 1) == 0;
	}

	SLIB_INLINE void makeMulticast(const IPv4Address& addrMulticast)
	{
		m[0] = 1;
		m[1] = 0;
		m[2] = 0x5e;
		m[3] = addrMulticast.m[1] & 0x7F;
		m[4] = addrMulticast.m[2];
		m[5] = addrMulticast.m[3];
	}
	
	SLIB_INLINE void makeMulticast(const IPv6Address& addrMulticast)
	{
		m[0] = 0x33;
		m[1] = 0x33;
		m[2] = addrMulticast.m[12];
		m[3] = addrMulticast.m[13];
		m[4] = addrMulticast.m[14];
		m[5] = addrMulticast.m[15];
	}
	
	SLIB_INLINE void set(const sl_uint8* _m)
	{
		m[0] = _m[0];
		m[1] = _m[1];
		m[2] = _m[2];
		m[3] = _m[3];
		m[4] = _m[4];
		m[5] = _m[5];
	}

	SLIB_INLINE sl_uint8 get(int index) const
	{
		if (index < 0 || index >= 6) {
			return 0;
		}
		return m[index];
	}
	
	SLIB_INLINE void set(int index, sl_uint8 v)
	{
		m[index] = v;
	}

	SLIB_INLINE sl_uint8* bytes()
	{
		return m;
	}

	SLIB_INLINE sl_uint8& operator[](int index)
	{
		return m[index];
	}
	
	SLIB_INLINE MacAddress& operator=(const MacAddress& other)
	{
		m[0] = other.m[0];
		m[1] = other.m[1];
		m[2] = other.m[2];
		m[3] = other.m[3];
		m[4] = other.m[4];
		m[5] = other.m[5];
		return *this;
	}
	
	SLIB_INLINE MacAddress& operator=(const String& address)
	{
		if (!parse(address)) {
			setZero();
		}
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const MacAddress& other) const
	{
		return m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2] && m[3] == other.m[3] && m[4] == other.m[4] && m[5] == other.m[5];
	}
	
	SLIB_INLINE sl_bool operator!=(const MacAddress& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE int compare(const MacAddress& other) const
	{
		return Base::compareMemory(m, other.m, 6);
	}

	SLIB_INLINE sl_uint32 hashCode() const
	{
		return sl_hash(m, 6);
	}

	// m0-m1-m2-m3-m4-m5, m0:m1:m2:m3:m4:m5
	String toString(sl_char8 sep = '-') const;
	
	static sl_int32 parse(MacAddress* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_char8 sep = 0);
	static sl_int32 parse(MacAddress* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_char8 sep = 0);
	sl_bool parse(const String& str, sl_char8 sep = 0);

};

template <>
SLIB_INLINE int Compare<MacAddress>::compare(const MacAddress& a, const MacAddress& b)
{
	return a.compare(b);
}

template <>
SLIB_INLINE sl_bool Compare<MacAddress>::equals(const MacAddress& a, const MacAddress& b)
{
	return a == b;
}

template <>
SLIB_INLINE sl_uint32 Hash<MacAddress>::hash(const MacAddress& a)
{
	return a.hashCode();
}
SLIB_NETWORK_NAMESPACE_END

#endif
