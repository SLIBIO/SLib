#ifndef CHECKHEADER_SLIB_NETWORK_IP_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_IP_ADDRESS

#include "definition.h"

#include "../core/mio.h"
#include "../core/string.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT IPv4Address
{
public:
	sl_uint8 m[4];
	
public:
	SLIB_INLINE IPv4Address()
	{
	}
	
	SLIB_INLINE IPv4Address(const sl_uint8* addr)
	{
		set(addr);
	}
	
	SLIB_INLINE IPv4Address(sl_uint8 a, sl_uint8 b, sl_uint8 c, sl_uint8 d)
	{
		set(a, b, c, d);
	}
	
	SLIB_INLINE IPv4Address(sl_uint32 addr)
	{
		set(addr);
	}
	
	SLIB_INLINE IPv4Address(const IPv4Address& other)
	{
		set(other.m);
	}
	
	SLIB_INLINE IPv4Address(const String& address)
	{
		if (! parse(address)) {
			set(0, 0, 0, 0);
		}
	}

public:
	SLIB_INLINE sl_uint32 a() const
	{
		return m[0];
	}
	
	SLIB_INLINE sl_uint32 b() const
	{
		return m[1];
	}
	
	SLIB_INLINE sl_uint32 c() const
	{
		return m[2];
	}
	
	SLIB_INLINE sl_uint32 d() const
	{
		return m[3];
	}
	
	SLIB_INLINE sl_uint32 toInt() const
	{
		return MIO::readUint32BE(m);
	}
	
	SLIB_INLINE sl_uint8* bytes()
	{
		return m;
	}
	
	SLIB_INLINE const sl_uint8* bytes() const
	{
		return m;
	}
	
	SLIB_INLINE void get(sl_uint8* b) const
	{
		b[0] = m[0];
		b[1] = m[1];
		b[2] = m[2];
		b[3] = m[3];
	}
	
	SLIB_INLINE void set(const IPv4Address& addr)
	{
		set(addr.m);
	}
	
	SLIB_INLINE void set(sl_uint32 addr)
	{
		MIO::writeUint32BE(m, addr);
	}
	
	SLIB_INLINE void set(sl_uint8 a, sl_uint8 b, sl_uint8 c, sl_uint8 d)
	{
		m[0] = a;
		m[1] = b;
		m[2] = c;
		m[3] = d;
	}
	
	SLIB_INLINE void set(const sl_uint8* b)
	{
		m[0] = b[0];
		m[1] = b[1];
		m[2] = b[2];
		m[3] = b[3];
	}
	
	SLIB_INLINE void setA(sl_uint8 a)
	{
		m[0] = a;
	}
	
	SLIB_INLINE void setB(sl_uint8 b)
	{
		m[1] = b;
	}
	
	SLIB_INLINE void setC(sl_uint8 c)
	{
		m[2] = c;
	}
	
	SLIB_INLINE void setD(sl_uint8 d)
	{
		m[3] = d;
	}
	
	SLIB_INLINE sl_uint8 get(int index) const
	{
		if (index < 0 || index >= 4) {
			return 0;
		}
		return m[index];
	}

	SLIB_INLINE void set(int index, sl_uint8 v)
	{
		if (index < 0 || index >= 4) {
			return;
		}
		m[index] = v;
	}
	
public:
	// [a, b, c, d]
	SLIB_INLINE sl_uint8 operator[](int index) const
	{
		return get(index);
	}
	
	SLIB_INLINE IPv4Address& operator=(const IPv4Address& other)
	{
		set(other.m);
		return *this;
	}

	SLIB_INLINE IPv4Address& operator=(const String& address)
	{
		if (! parse(address)) {
			set(0, 0, 0, 0);
		}
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const IPv4Address& other) const
	{
		return toInt() == other.toInt();
	}
	
	SLIB_INLINE sl_bool operator!=(const IPv4Address& other) const
	{
		return !(*this == other);
	}
	
	SLIB_INLINE sl_bool operator>=(const IPv4Address& other) const
	{
		return toInt() >= other.toInt();
	}
	
	SLIB_INLINE sl_bool operator>(const IPv4Address& other) const
	{
		return toInt() > other.toInt();
	}

	SLIB_INLINE sl_bool operator<=(const IPv4Address& other) const
	{
		return toInt() <= other.toInt();
	}
	
	SLIB_INLINE sl_bool operator<(const IPv4Address& other) const
	{
		return toInt() < other.toInt();
	}
	
public:
	SLIB_INLINE int compare(const IPv4Address& other) const
	{
		return Compare<sl_uint32>::compare(toInt(), other.toInt());
	}

	SLIB_INLINE sl_uint32 hashCode() const
	{
		return Hash<sl_uint32>::hash(toInt());
	}
	
	// "a.b.c.d"
	String toString() const;
	
	static sl_int32 parse(IPv4Address* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parse(IPv4Address* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_bool parse(const String& str, IPv4Address* _out);

	sl_bool parse(const String& str);

	void makeNetworkMask(sl_uint32 networkPrefixLength);
	
	sl_uint32 getNetworkPrefixLengthFromMask() const;
	
	sl_bool setHostName(const String& hostName);
	
public:
	SLIB_INLINE sl_bool isAny() const
	{
		return toInt() == 0;
	}

	SLIB_INLINE sl_bool isNotAny() const
	{
		return toInt() != 0;
	}

	SLIB_INLINE void setAny()
	{
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
	}

	SLIB_INLINE sl_bool isZero() const
	{
		return toInt() == 0;
	}

	SLIB_INLINE sl_bool isNotZero() const
	{
		return toInt() != 0;
	}

	SLIB_INLINE void setZero()
	{
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
	}

	SLIB_INLINE void setMaximum()
	{
		m[0] = 255;
		m[1] = 255;
		m[2] = 255;
		m[3] = 255;
	}

	SLIB_INLINE sl_bool isLoopback() const
	{
		return m[0] == 127;
	}
	
	SLIB_INLINE sl_bool isMulticast() const
	{
		return m[0] >= 224 && m[0] <= 239;
	}

	SLIB_INLINE sl_bool isBroadcast() const
	{
		return toInt() == 0xFFFFFFFF;
	}

	SLIB_INLINE sl_bool isHost() const
	{
		sl_uint32 n = toInt();
		return n != 0 && m[0] < 224 && m[0] != 127;
	}

	SLIB_INLINE sl_bool isPrivate() const
	{
		sl_uint32 n = toInt();
		// 10.0.0.0 - 10.255.255.255
		if (n >= 0x0A000000 && n <= 0x0AFFFFFF) {
			return sl_true;
		}
		// 172.16.0.0 - 172.31.255.255
		if (n >= 0xAC100000 && n <= 0xAC1FFFFF) {
			return sl_true;
		}
		// 192.168.0.0 - 192.168.255.255
		if (n >= 0xC0A80000 && n <= 0xC0A8FFFF) {
			return sl_true;
		}
		return sl_false;
	}

private:
	static const sl_uint8 _any[4];
	static const sl_uint8 _broadcast[4];
	static const sl_uint8 _loopback[4];
	static const sl_uint8 _multicast_begin[4];
	static const sl_uint8 _multicast_end[4];
	
public:
	SLIB_INLINE static const IPv4Address& zero()
	{
		return *((IPv4Address*)((void*)(&_any)));
	}

	SLIB_INLINE static const IPv4Address& getZero()
	{
		return *((IPv4Address*)((void*)(&_any)));
	}


	SLIB_INLINE static const IPv4Address& maximum()
	{
		return *((IPv4Address*)((void*)(&_broadcast)));
	}

	SLIB_INLINE static const IPv4Address& getMaximum()
	{
		return *((IPv4Address*)((void*)(&_broadcast)));
	}


	SLIB_INLINE static const IPv4Address& any()
	{
		return *((IPv4Address*)((void*)(&_any)));
	}
	
	SLIB_INLINE static const IPv4Address& getAny()
	{
		return *((IPv4Address*)((void*)(&_any)));
	}

	
	SLIB_INLINE static const IPv4Address& getBroadcast()
	{
		return *((IPv4Address*)((void*)(&_broadcast)));
	}
	
	SLIB_INLINE static const IPv4Address& getLoopback()
	{
		return *((IPv4Address*)((void*)(&_loopback)));
	}
	
	
	SLIB_INLINE static const IPv4Address& getMulticastBegin()
	{
		return *((IPv4Address*)((void*)(&_multicast_begin)));
	}
	
	SLIB_INLINE static const IPv4Address& getMulticastEnd()
	{
		return *((IPv4Address*)((void*)(&_multicast_end)));
	}

};

template <>
SLIB_INLINE int Compare<IPv4Address>::compare(const IPv4Address& a, const IPv4Address& b)
{
	return a.compare(b);
}

template <>
SLIB_INLINE sl_bool Compare<IPv4Address>::equals(const IPv4Address& a, const IPv4Address& b)
{
	return a == b;
}

template <>
SLIB_INLINE sl_uint32 Hash<IPv4Address>::hash(const IPv4Address& a)
{
	return a.hashCode();
}


class SLIB_EXPORT IPv4AddressInfo
{
public:
	IPv4Address address;
	sl_uint32 networkPrefixLength;
	
public:
	SLIB_INLINE const IPv4Address& getAddress() const
	{
		return address;
	}
	
	SLIB_INLINE void setAddress(const IPv4Address& addr)
	{
		this->address = addr;
	}
	
	
	SLIB_INLINE sl_uint32 getNetworkPrefixLength() const
	{
		return networkPrefixLength;
	}
	
	SLIB_INLINE void setNetworkPrefixLength(sl_uint32 len)
	{
		networkPrefixLength = len;
	}
	
	
	SLIB_INLINE IPv4Address getNetworkMask() const
	{
		IPv4Address ret;
		ret.makeNetworkMask(networkPrefixLength);
		return ret;
	}
	
	SLIB_INLINE void setNetworkMask(const IPv4Address& mask)
	{
		networkPrefixLength = mask.getNetworkPrefixLengthFromMask();
	}
    
};


class SLIB_EXPORT IPv6Address
{
public:
	sl_uint8 m[16];

public:
	SLIB_INLINE IPv6Address()
	{
	}

	// 8 elements
	SLIB_INLINE IPv6Address(const sl_uint16* s)
	{
		for (int i = 0; i < 8; i++) {
			set(i, s[i]);
		}
	}
	
	// 16 elements
	SLIB_INLINE IPv6Address(const sl_uint8* b)
	{
		Base::copyMemory(m, b, 16);
	}
	
	SLIB_INLINE IPv6Address(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7)
	{
		set(s0, s1, s2, s3, s4, s5, s6, s7);
	}
	
	SLIB_INLINE IPv6Address(const IPv6Address& other)
	{
		Base::copyMemory(m, other.m, 16);
	}
	
	SLIB_INLINE IPv6Address(const String& address)
	{
		if (! parse(address)) {
			Base::resetMemory(m, 0, 16);
		}
	}

	SLIB_INLINE IPv6Address(const IPv4Address& ip)
	{
		set(0, 0, 0, 0, 0, 0xFFFF, (sl_uint16)((ip.a() << 8) | ip.b()), (sl_uint16)((ip.c() << 8) | ip.d()));
	}

public:
	SLIB_INLINE sl_uint8* bytes()
	{
		return m;
	}
	
	SLIB_INLINE const sl_uint8* bytes() const
	{
		return m;
	}
	
	SLIB_INLINE void get(sl_uint8* b) const
	{
		Base::copyMemory(b, m, 16);
	}

	// 8 elements
	SLIB_INLINE void set(const sl_uint16* s)
	{
		for (int i = 0; i < 8; i++) {
			set(i, s[i]);
		}
	}
	
	// 16 elements
	SLIB_INLINE void set(const sl_uint8* b)
	{
		Base::copyMemory(m, b, 16);
	}
	
	SLIB_INLINE void set(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7)
	{
		set(0, s0);
		set(1, s1);
		set(2, s2);
		set(3, s3);
		set(4, s4);
		set(5, s5);
		set(6, s6);
		set(7, s7);
	}
	
	SLIB_INLINE sl_uint16 get(int index) const
	{
		if (index < 0 || index >= 16) {
			return 0;
		}
		sl_uint16 ret = (sl_uint16)((((sl_uint32)m[index * 2]) << 8) | ((sl_uint32)m[index * 2 + 1]));
		return ret;
	}
	
	SLIB_INLINE void set(int index, sl_uint16 s)
	{
		m[index * 2] = (sl_uint8)(s >> 8);
		m[index * 2 + 1] = (sl_uint8)(s);
	}
	
public:
	// [s0, s1, s2, s3, s4, s5, s6, s7, s8]
	SLIB_INLINE sl_uint16 operator[](int index) const
	{
		return get(index);
	}
	
	SLIB_INLINE IPv6Address& operator=(const IPv6Address& other)
	{
		Base::copyMemory(m, other.m, 16);
		return *this;
	}
	
	SLIB_INLINE IPv6Address& operator=(const String& address)
	{
		if (! parse(address)) {
			Base::resetMemory(m, 0, 16);
		}
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const IPv6Address& other) const
	{
		return Base::compareMemory(m, other.m, 16) == 0;
	}
	
	SLIB_INLINE sl_bool operator!=(const IPv6Address& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE sl_bool operator<=(const IPv6Address& other) const
	{
		return Base::compareMemory(m, other.m, 16) <= 0;
	}

	SLIB_INLINE sl_bool operator<(const IPv6Address& other) const
	{
		return Base::compareMemory(m, other.m, 16) < 0;
	}

	SLIB_INLINE sl_bool operator>=(const IPv6Address& other) const
	{
		return Base::compareMemory(m, other.m, 16) >= 0;
	}

	SLIB_INLINE sl_bool operator>(const IPv6Address& other) const
	{
		return Base::compareMemory(m, other.m, 16) > 0;
	}
	
public:
	SLIB_INLINE int compare(const IPv6Address& other) const
	{
		return Base::compareMemory(m, other.m, 16);
	}

	SLIB_INLINE sl_uint32 hashCode() const
	{
		return sl_hash(m, 16);
	}

	// "s0:s1:s2:s3:s4:s5:s6:s7"
	String toString() const;
	
	static sl_int32 parse(IPv6Address* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parse(IPv6Address* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_bool parse(const String& str, IPv6Address* _out);

	sl_bool parse(const String& str);
	
	sl_bool setHostName(const String& hostName);
	
public:
	SLIB_INLINE sl_bool isAny() const
	{
		return Base::compareZero(m, 16) == 0;
	}

	SLIB_INLINE sl_bool isNotAny() const
	{
		return Base::compareZero(m, 16) != 0;
	}

	SLIB_INLINE void setAny()
	{
		Base::zeroMemory(m, 16);
	}
	
	SLIB_INLINE sl_bool isZero() const
	{
		return Base::compareZero(m, 16) == 0;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return Base::compareZero(m, 16) != 0;
	}
	
	SLIB_INLINE void setZero()
	{
		Base::zeroMemory(m, 16);
	}

	SLIB_INLINE sl_bool isLoopback() const
	{
		return Base::compareMemory(_loopback, m, 16) == 0;
	}
	
	SLIB_INLINE sl_bool isIPv4Transition() const
	{
		return m[0] == 0 && m[1] == 0 && m[2] == 0 && m[3] == 0 && m[4] == 0 && m[5] == 0 && m[6] == 0 && m[7] == 0 && m[8] == 0 && m[9] == 0 && m[10] == 255 && m[11] == 255;
	}
	
	SLIB_INLINE IPv4Address getIPv4Transition() const
	{
		if (isIPv4Transition()) {
			return IPv4Address(m[12], m[13], m[14], m[15]);
		} else {
			return IPv4Address::any();
		}
	}
	
private:
	static const sl_uint8 _any[16];
	static const sl_uint8 _loopback[16];
	
public:
	SLIB_INLINE static const IPv6Address& zero()
	{
		return *((IPv6Address*)((void*)(&_any)));
	}
	
	SLIB_INLINE static const IPv6Address& getZero()
	{
		return *((IPv6Address*)((void*)(&_any)));
	}
	
	SLIB_INLINE static const IPv6Address& any()
	{
		return *((IPv6Address*)((void*)(&_any)));
	}
	
	SLIB_INLINE static const IPv6Address& getAny()
	{
		return *((IPv6Address*)((void*)(&_any)));
	}

	
	SLIB_INLINE static const IPv6Address& getLoopback()
	{
		return *((IPv6Address*)((void*)(&_loopback)));
	}

};

template <>
SLIB_INLINE int Compare<IPv6Address>::compare(const IPv6Address& a, const IPv6Address& b)
{
	return a.compare(b);
}

template <>
SLIB_INLINE sl_bool Compare<IPv6Address>::equals(const IPv6Address& a, const IPv6Address& b)
{
	return a == b;
}

template <>
SLIB_INLINE sl_uint32 Hash<IPv6Address>::hash(const IPv6Address& a)
{
	return a.hashCode();
}

class SLIB_EXPORT IPv6AddressInfo
{
public:
	IPv6Address address;
	
public:
	SLIB_INLINE const IPv6Address& getAddress() const
	{
		return address;
	}
	SLIB_INLINE void setAddress(const IPv6Address& addr)
	{
		this->address = addr;
	}
};



#define _SLIB_NET_IPADDRESS_SIZE 16

enum IPAddressType {
    ipAddressType_None = 0,
    ipAddressType_IPv4 = 1,
    ipAddressType_IPv6 = 2
};

class SLIB_EXPORT IPAddress
{
private:
	IPAddressType m_type;
	sl_uint8 m_data[_SLIB_NET_IPADDRESS_SIZE];
	
	struct _IPAddress
	{
		IPAddressType type;
		sl_uint8 data[_SLIB_NET_IPADDRESS_SIZE];
	};
	static const _IPAddress _none;

public:
	SLIB_INLINE IPAddress()
	{
		m_type = ipAddressType_None;
	}
	
	SLIB_INLINE IPAddress(const IPAddress& other)
	{
		*this = other;
	}
	
	SLIB_INLINE IPAddress(const IPv4Address& other)
	{
		*this = other;
	}
	
	SLIB_INLINE IPAddress(const IPv6Address& other)
	{
		*this = other;
	}
	

public:
	static SLIB_INLINE const IPAddress& none()
	{
		return *((IPAddress*)((void*)(&_none)));
	}
	
	SLIB_INLINE sl_bool isNone() const
	{
		return m_type == ipAddressType_None;
	}
	
	SLIB_INLINE sl_bool isNotNone() const
	{
		return m_type != ipAddressType_None;
	}

	SLIB_INLINE void setNone()
	{
		m_type = ipAddressType_None;
	}
	
	SLIB_INLINE IPAddressType getType() const
	{
		return m_type;
	}

	SLIB_INLINE sl_bool isIPv4() const
	{
		return m_type == ipAddressType_IPv4;
	}
	
	SLIB_INLINE const IPv4Address& getIPv4() const
	{
		return *((IPv4Address*)(void*)(m_data));
	}

	SLIB_INLINE sl_bool isIPv6() const
	{
		return m_type == ipAddressType_IPv6;
	}
	
	SLIB_INLINE const IPv6Address& getIPv6() const
	{
		return *((IPv6Address*)(void*)(m_data));
	}

public:
	SLIB_INLINE IPAddress& operator=(const IPAddress& other)
	{
		m_type = other.m_type;
		Base::copyMemory(m_data, other.m_data, _SLIB_NET_IPADDRESS_SIZE);
		return *this;
	}
	
	SLIB_INLINE IPAddress& operator=(const IPv4Address& other)
	{
		m_type = ipAddressType_IPv4;
		*((IPv4Address*)(void*)(m_data)) = other;
		return *this;
	}

	SLIB_INLINE IPAddress& operator=(const IPv6Address& other)
	{
		m_type = ipAddressType_IPv6;
		*((IPv6Address*)(void*)(m_data)) = other;
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const IPAddress& other) const
	{
		if (m_type != other.m_type) {
			return sl_false;
		}
		switch (m_type) {
		case ipAddressType_None:
			return sl_true;
		case ipAddressType_IPv4:
			return (*((IPv4Address*)(void*)(m_data)) == *((IPv4Address*)(void*)(other.m_data)));
		case ipAddressType_IPv6:
			return (*((IPv6Address*)(void*)(m_data)) == *((IPv6Address*)(void*)(other.m_data)));
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool operator!=(const IPAddress& other) const
	{
		return !(*this == other);
	}

public:
	int compare(const IPAddress& other) const;
	
	sl_uint32 hashCode() const;
	
	
	String toString() const;
	
	static sl_int32 parse(IPAddress* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parse(IPAddress* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);

	static sl_bool parse(const String& str, IPAddress* _out);
	
	sl_bool parse(const String& str);

	
	sl_bool setHostName(const String& hostName);
	
};

template <>
SLIB_INLINE int Compare<IPAddress>::compare(const IPAddress& a, const IPAddress& b)
{
	return a.compare(b);
}

template <>
SLIB_INLINE sl_bool Compare<IPAddress>::equals(const IPAddress& a, const IPAddress& b)
{
	return a == b;
}

template <>
SLIB_INLINE sl_uint32 Hash<IPAddress>::hash(const IPAddress& a)
{
	return a.hashCode();
}

SLIB_NETWORK_NAMESPACE_END

#endif
