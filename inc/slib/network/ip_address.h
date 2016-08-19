#ifndef CHECKHEADER_SLIB_NETWORK_IP_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_IP_ADDRESS

#include "definition.h"

#include "../core/list.h"
#include "../core/string.h"
#include "../core/parse.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT IPv4Address
{
public:
	sl_uint8 a;
	sl_uint8 b;
	sl_uint8 c;
	sl_uint8 d;
	
public:
	enum
	{
		Any = 0, // 0.0.0.0
		Maximum = 0xFFFFFFFF, // 255.255.255.255
		Broadcast = 0xFFFFFFFF, // 255.255.255.255
		Loopback = 0x7F000001, // 127.0.0.1
		MulticastBegin = 0xE0000000, // 224.0.0.0
		MulticastEnd = 0xEFFFFFFF, // 239.255.255.255
	};
	
public:
	SLIB_INLINE IPv4Address() = default;
	
	SLIB_INLINE IPv4Address(const IPv4Address& other) = default;

	IPv4Address(const sl_uint8* addr);
	
	IPv4Address(sl_uint8 a, sl_uint8 b, sl_uint8 c, sl_uint8 d);
	
	IPv4Address(sl_uint32 addr);
	
	IPv4Address(const String& address);

public:
	void setElements(sl_uint8 a, sl_uint8 b, sl_uint8 c, sl_uint8 d);
	
	sl_uint32 getInt() const;
	
	void setInt(sl_uint32 addr);
	
	void getBytes(sl_uint8* bytes) const;

	void setBytes(const sl_uint8* bytes);
	
	static const IPv4Address& zero();
	
	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	void setZero();
	
	sl_bool isLoopback() const;
	
	sl_bool isMulticast() const;
	
	sl_bool isBroadcast() const;
	
	sl_bool isHost() const;
	
	sl_bool isPrivate() const;
	
	int compare(const IPv4Address& other) const;
	
	sl_uint32 hashCode() const;
	
	// "a.b.c.d"
	String toString() const;
	
	sl_bool setString(const String& str);

	void makeNetworkMask(sl_uint32 networkPrefixLength);
	
	sl_uint32 getNetworkPrefixLengthFromMask() const;
	
	sl_bool setHostName(const String& hostName);
	
	
	SLIB_DECLARE_PARSE_FUNCTIONS(IPv4Address)
	

public:
	SLIB_INLINE IPv4Address& operator=(const IPv4Address& other) = default;
	
	IPv4Address& operator=(sl_uint32 addr);

	IPv4Address& operator=(const String& address);
	
	sl_bool operator==(const IPv4Address& other) const;
	
	sl_bool operator==(sl_uint32 addr) const;
	
	sl_bool operator!=(const IPv4Address& other) const;

	sl_bool operator!=(sl_uint32 addr) const;
	
	sl_bool operator>=(const IPv4Address& other) const;
	
	sl_bool operator>(const IPv4Address& other) const;

	sl_bool operator<=(const IPv4Address& other) const;
	
	sl_bool operator<(const IPv4Address& other) const;

private:
	static const sl_uint8 _zero[4];
	
};

template <>
int Compare<IPv4Address>::compare(const IPv4Address& a, const IPv4Address& b);

template <>
sl_bool Compare<IPv4Address>::equals(const IPv4Address& a, const IPv4Address& b);

template <>
sl_uint32 Hash<IPv4Address>::hash(const IPv4Address& a);

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPv4Address)


class SLIB_EXPORT IPv4AddressInfo
{
public:
	IPv4Address address;
	sl_uint32 networkPrefixLength;
	
public:
	IPv4Address getNetworkMask() const;
	
	void setNetworkMask(const IPv4Address& mask);
    
};

template <>
int Compare<IPv4AddressInfo>::compare(const IPv4AddressInfo& a, const IPv4AddressInfo& b);

template <>
sl_bool Compare<IPv4AddressInfo>::equals(const IPv4AddressInfo& a, const IPv4AddressInfo& b);

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPv4AddressInfo)


class SLIB_EXPORT IPv6Address
{
public:
	sl_uint8 m[16];

public:
	IPv6Address();
	
	IPv6Address(const IPv6Address& other);

	// 8 elements
	IPv6Address(const sl_uint16* s);
	
	IPv6Address(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7);
	
	// 16 elements
	IPv6Address(const sl_uint8* b);
	
	IPv6Address(const IPv4Address& ipv4);
	
	IPv6Address(const String& address);

public:
	// not checking index bound (0~7)
	sl_uint16 getElement(int index) const;
	
	// not checking index bound (0~7)
	void setElement(int index, sl_uint16 s);
	
	// 8 elements
	void getElements(sl_uint16* s) const;
	
	// 8 elements
	void setElements(const sl_uint16* s);
	
	void setElements(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7);

	// 16 elements
	void getBytes(sl_uint8* bytes) const;
	
	// 16 elements
	void setBytes(const sl_uint8* bytes);
	
	static const IPv6Address& zero();
	
	void setZero();

	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	static const IPv6Address& getLoopback();
	
	sl_bool isLoopback() const;

	IPv4Address getIPv4Transition() const;

	void setIPv4Transition(const IPv4Address& ipv4);
	
	sl_bool isIPv4Transition() const;
	
	int compare(const IPv6Address& other) const;
	
	sl_uint32 hashCode() const;
	
	// "s0:s1:s2:s3:s4:s5:s6:s7"
	String toString() const;
	
	sl_bool setString(const String& str);
	
	sl_bool setHostName(const String& hostName);
	
	
	SLIB_DECLARE_PARSE_FUNCTIONS(IPv6Address)
	

public:
	IPv6Address& operator=(const IPv6Address& other);
	
	IPv6Address& operator=(const String& address);

	sl_bool operator==(const IPv6Address& other) const;
	
	sl_bool operator!=(const IPv6Address& other) const;

	sl_bool operator<=(const IPv6Address& other) const;

	sl_bool operator<(const IPv6Address& other) const;

	sl_bool operator>=(const IPv6Address& other) const;

	sl_bool operator>(const IPv6Address& other) const;
	
private:
	static const sl_uint8 _zero[16];
	static const sl_uint8 _loopback[16];

};

template <>
int Compare<IPv6Address>::compare(const IPv6Address& a, const IPv6Address& b);

template <>
sl_bool Compare<IPv6Address>::equals(const IPv6Address& a, const IPv6Address& b);

template <>
sl_uint32 Hash<IPv6Address>::hash(const IPv6Address& a);

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPv6Address)



#define _SLIB_NET_IPADDRESS_SIZE 16

enum class IPAddressType
{
    None = 0,
    IPv4 = 1,
    IPv6 = 2
};

class SLIB_EXPORT IPAddress
{
public:
	IPAddressType type;
	sl_uint8 m[_SLIB_NET_IPADDRESS_SIZE];
	
public:
	IPAddress();
	
	IPAddress(const IPAddress& other);
	
	IPAddress(const IPv4Address& other);
	
	IPAddress(const IPv6Address& other);
	
	IPAddress(const String& address);
	
public:
	static const IPAddress& none();
	
	void setNone();
	
	sl_bool isNone() const;
	
	sl_bool isNotNone() const;
	
	sl_bool isIPv4() const;
	
	const IPv4Address& getIPv4() const;
	
	void setIPv4(const IPv4Address& addr);

	sl_bool isIPv6() const;
	
	const IPv6Address& getIPv6() const;
	
	void setIPv6(const IPv6Address& addr);
	
	int compare(const IPAddress& other) const;
	
	sl_uint32 hashCode() const;
	
	String toString() const;
	
	sl_bool setString(const String& str);
	
	sl_bool setHostName(const String& hostName);
	
	
	SLIB_DECLARE_PARSE_FUNCTIONS(IPAddress)
	
	
public:
	IPAddress& operator=(const IPAddress& other);
	
	IPAddress& operator=(const IPv4Address& other);

	IPAddress& operator=(const IPv6Address& other);
	
	IPAddress& operator=(const String& address);
	
	sl_bool operator==(const IPAddress& other) const;
	
	sl_bool operator!=(const IPAddress& other) const;

private:
	struct _IPAddress
	{
		IPAddressType type;
		sl_uint8 m[_SLIB_NET_IPADDRESS_SIZE];
	};
	static const _IPAddress _none;
	
};

template <>
int Compare<IPAddress>::compare(const IPAddress& a, const IPAddress& b);

template <>
sl_bool Compare<IPAddress>::equals(const IPAddress& a, const IPAddress& b);

template <>
sl_uint32 Hash<IPAddress>::hash(const IPAddress& a);

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPAddress)

SLIB_NETWORK_NAMESPACE_END


SLIB_NETWORK_NAMESPACE_BEGIN

SLIB_INLINE IPv4Address::IPv4Address(const sl_uint8* addr)
: a(addr[0]), b(addr[1]), c(addr[2]), d(addr[3])
{
}

SLIB_INLINE IPv4Address::IPv4Address(sl_uint8 _a, sl_uint8 _b, sl_uint8 _c, sl_uint8 _d)
: a(_a), b(_b), c(_c), d(_d)
{
}

SLIB_INLINE IPv4Address::IPv4Address(sl_uint32 addr)
: a((sl_uint8)(addr >> 24)), b((sl_uint8)(addr >> 16)), c((sl_uint8)(addr >> 8)), d((sl_uint8)(addr))
{
}

SLIB_INLINE sl_uint32 IPv4Address::getInt() const
{
	return ((sl_uint32)(a) << 24) | ((sl_uint32)(b) << 16) | ((sl_uint32)(c) << 8) | ((sl_uint32)(d));
}

SLIB_INLINE void IPv4Address::setInt(sl_uint32 addr)
{
	a = (sl_uint8)(addr >> 24);
	b = (sl_uint8)(addr >> 16);
	c = (sl_uint8)(addr >> 8);
	d = (sl_uint8)(addr);
}

SLIB_INLINE const IPv4Address& IPv4Address::zero()
{
	return *((IPv4Address*)((void*)(&_zero)));
}

SLIB_INLINE sl_bool IPv4Address::isZero() const
{
	return getInt() == 0;
}

SLIB_INLINE sl_bool IPv4Address::isNotZero() const
{
	return getInt() != 0;
}

SLIB_INLINE IPv4Address& IPv4Address::operator=(sl_uint32 addr)
{
	a = (sl_uint8)(addr >> 24);
	b = (sl_uint8)(addr >> 16);
	c = (sl_uint8)(addr >> 8);
	d = (sl_uint8)(addr);
	return *this;
}

SLIB_INLINE sl_bool IPv4Address::operator==(const IPv4Address& other) const
{
	return getInt() == other.getInt();
}

SLIB_INLINE sl_bool IPv4Address::operator==(sl_uint32 addr) const
{
	return getInt() == addr;
}

SLIB_INLINE sl_bool IPv4Address::operator!=(const IPv4Address& other) const
{
	return getInt() != other.getInt();
}

SLIB_INLINE sl_bool IPv4Address::operator!=(sl_uint32 addr) const
{
	return getInt() != addr;
}


SLIB_INLINE const IPv6Address& IPv6Address::zero()
{
	return *((IPv6Address*)((void*)(&_zero)));
}

SLIB_INLINE const IPv6Address& IPv6Address::getLoopback()
{
	return *((IPv6Address*)((void*)(&_loopback)));
}


SLIB_INLINE IPAddress::IPAddress() : type(IPAddressType::None)
{
}

SLIB_INLINE const IPAddress& IPAddress::none()
{
	return *((IPAddress*)((void*)(&_none)));
}

SLIB_INLINE void IPAddress::setNone()
{
	type = IPAddressType::None;
}

SLIB_INLINE sl_bool IPAddress::isNone() const
{
	return type == IPAddressType::None;
}

SLIB_INLINE sl_bool IPAddress::isNotNone() const
{
	return type != IPAddressType::None;
}


SLIB_NETWORK_NAMESPACE_END

#endif
