#ifndef CHECKHEADER_SLIB_NETWORK_SOCKET_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_SOCKET_ADDRESS

#include "definition.h"

#include "ip_address.h"

#include "../core/string.h"
#include "../core/parse.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT SocketAddress
{
public:
	IPAddress ip;
	sl_uint32 port;

public:
	SocketAddress();
	
	SocketAddress(const SocketAddress& other);

	SocketAddress(sl_int32 port);

	SocketAddress(const IPAddress& ip, sl_int32 port);
	
	SocketAddress(const String& str);

public:
	static const SocketAddress& none();

	void setNone();

	sl_bool isValid() const;
	
	sl_bool isInvalid() const;
	
	int compare(const SocketAddress& other) const;
	
	sl_uint32 hashCode() const;

	/*
		Address Format
			IPv4 - a.b.c.d:port
			Ipv6 - [s0:s1:s2:s3:s4:s5:s6:s7]:port
	*/
	String toString() const;
	
	sl_bool setString(const String& str);
	
	sl_uint32 getSystemSocketAddress(void* addr);
	
	sl_bool setSystemSocketAddress(const void* addr, sl_uint32 size = 0);

	// HostName:port
	sl_bool setHostAddress(const String& address);
	
	
	SLIB_DECLARE_PARSE_FUNCTIONS(SocketAddress)

	static sl_bool parseIPv4Range(const String& str, IPv4Address* from = sl_null, IPv4Address* to = sl_null);

	static sl_bool parsePortRange(const String& str, sl_uint32* from = sl_null, sl_uint32* to = sl_null);
	
public:
	SocketAddress& operator=(const SocketAddress& other) = default;
	
	SocketAddress& operator=(const String& str);
	
	sl_bool operator==(const SocketAddress& other) const;
	
	sl_bool operator!=(const SocketAddress& other) const;
	
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
	
};

template <>
int Compare<SocketAddress>::compare(const SocketAddress& a, const SocketAddress& b);

template <>
sl_bool Compare<SocketAddress>::equals(const SocketAddress& a, const SocketAddress& b);

template <>
sl_uint32 Hash<SocketAddress>::hash(const SocketAddress& a);

SLIB_NETWORK_NAMESPACE_END

#endif
