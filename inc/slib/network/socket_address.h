#ifndef CHECKHEADER_SLIB_NETWORK_SOCKET_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_SOCKET_ADDRESS

#include "definition.h"

#include "ip_address.h"

#include "../core/mio.h"
#include "../core/string.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT SocketAddress
{
public:
	IPAddress ip;
	sl_uint32 port;

private:
	struct _SocketAddress
	{
		struct {
			IPAddressType type;
			sl_uint8 data[_SLIB_NET_IPADDRESS_SIZE];
		} ip;
		sl_uint32 port;
	};
	static const _SocketAddress _none;

public:
	SLIB_INLINE SocketAddress()
	{
		port = 0;
	}

	SLIB_INLINE SocketAddress(sl_int32 _port)
	{
		port = _port;
	}

	SLIB_INLINE SocketAddress(const IPAddress& _ip, sl_int32 _port)
	{
		this->ip = _ip;
		this->port = _port;
	}
	
	SLIB_INLINE SocketAddress(const String& str)
	{
		parse(str);
	}

	SLIB_INLINE SocketAddress(const SocketAddress& other)
	{
		this->ip = other.ip;
		this->port = other.port;
	}

public:
	SLIB_INLINE SocketAddress& operator=(const SocketAddress& other)
	{
		this->ip = other.ip;
		this->port = other.port;
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const SocketAddress& other) const
	{
		return port == other.port && ip == other.ip;
	}
	
	SLIB_INLINE sl_bool operator!=(const SocketAddress& other) const
	{
		return ! (*this == other);
	}

public:
	static SLIB_INLINE const SocketAddress& none()
	{
		return *((SocketAddress*)((void*)(&_none)));
	}

	SLIB_INLINE sl_bool isValid() const
	{
		return ip.isNotNone() && port != 0;
	}
	
	SLIB_INLINE sl_bool isInvalid() const
	{
		return ip.isNone() || port == 0;
	}

	SLIB_INLINE void setNone()
	{
		ip.setNone();
		port = 0;
	}

public:
	int compare(const SocketAddress& other) const;
	
	sl_uint32 hashCode() const;

	/*
		Address Format
			IPv4 - a.b.c.d:port
			Ipv6 - [s0:s1:s2:s3:s4:s5:s6:s7]:port
	*/
	String toString() const;
	
	static sl_int32 parse(SocketAddress* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_int32 parse(SocketAddress* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX);
	
	static sl_bool parse(const String& str, SocketAddress* out);

	sl_bool parse(const String& str);
	
	sl_uint32 getSystemSocketAddress(void* addr);
	
	sl_bool setSystemSocketAddress(const void* addr, sl_uint32 size = 0);

	// HostName:port
	sl_bool setHostAddress(const String& address);

public:
	static sl_bool parseIPv4Range(const String& str, IPv4Address* from = sl_null, IPv4Address* to = sl_null);

	static sl_bool parsePortRange(const String& str, sl_uint32* from = sl_null, sl_uint32* to = sl_null);

};

template <>
SLIB_INLINE int Compare<SocketAddress>::compare(const SocketAddress& a, const SocketAddress& b)
{
	return a.compare(b);
}

template <>
SLIB_INLINE sl_bool Compare<SocketAddress>::equals(const SocketAddress& a, const SocketAddress& b)
{
	return a == b;
}

template <>
SLIB_INLINE sl_uint32 Hash<SocketAddress>::hash(const SocketAddress& a)
{
	return a.hashCode();
}

SLIB_NETWORK_NAMESPACE_END

#endif
