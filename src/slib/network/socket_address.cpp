#include "../../../inc/slib/network/socket_address.h"

#include "../../../inc/slib/core/setting.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	pragma comment(lib, "ws2_32.lib")
#else
#	include <unistd.h>
#	include <sys/socket.h>
#	if defined(SLIB_PLATFORM_IS_LINUX)
#		include <linux/tcp.h>
#	else
#		include <netinet/tcp.h>
#	endif
#	include <netinet/in.h>
#endif

SLIB_NETWORK_NAMESPACE_BEGIN

const SocketAddress::_SocketAddress SocketAddress::_none = { { IPAddressType::None, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, 0 };

SocketAddress::SocketAddress(const String& str)
{
	parse(str);
}

void SocketAddress::setNone()
{
	ip.setNone();
	port = 0;
}

int SocketAddress::compare(const SocketAddress& other) const
{
	int c = ip.compare(other.ip);
	if (c == 0) {
		return Compare<sl_int32>::compare(port, other.port);
	}
	return c;
}

sl_uint32 SocketAddress::hashCode() const
{
	sl_uint64 t = ip.hashCode();
	t = t * 31 + port;
	return sl_hash(t);
}

String SocketAddress::toString() const
{
	if (ip.isIPv4()) {
		return ip.toString() + ":" + String::fromInt32(port);
	} else if (ip.isIPv6()) {
		return "[" + ip.toString() + "]:" + String::fromInt32(port);
	} else {
		return ":" + String::fromInt32(port);
	}
}

template <class CT>
SLIB_INLINE sl_reg _SocketAddress_parse(SocketAddress* obj, const CT* sz, sl_size pos, sl_size len)
{
	if (pos >= len) {
		return SLIB_PARSE_ERROR;
	}
	IPAddress ip;
	if (sz[0] == '[') {
		IPv6Address addr;
		pos++;
		pos = IPv6Address::parse(&addr, sz, pos, len);
		if (pos == SLIB_PARSE_ERROR || pos >= len) {
			return SLIB_PARSE_ERROR;
		}
		if (sz[pos] != ']') {
			return SLIB_PARSE_ERROR;
		}
		pos++;
		ip = addr;
	} else {
		IPv4Address addr;
		pos = IPv4Address::parse(&addr, sz, pos, len);
		if (pos == SLIB_PARSE_ERROR) {
			return SLIB_PARSE_ERROR;
		}
		ip = addr;
	}
	if (pos >= len) {
		return SLIB_PARSE_ERROR;
	}
	if (sz[pos] != ':') {
		return SLIB_PARSE_ERROR;
	}
	pos++;
	sl_uint32 port;
	pos = String::parseUint32(10, &port, sz, pos, len);
	if (pos == SLIB_PARSE_ERROR) {
		return SLIB_PARSE_ERROR;
	}
	if (obj) {
		obj->ip = ip;
		obj->port = port;
	}
	return pos;
}

sl_reg SocketAddress::parse(SocketAddress* out, const char* sz, sl_size posBegin, sl_size len)
{
	return _SocketAddress_parse(out, sz, posBegin, len);
}

sl_reg SocketAddress::parse(SocketAddress* out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _SocketAddress_parse(out, sz, posBegin, len);
}

sl_bool SocketAddress::parse(const String& s, SocketAddress* out)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _SocketAddress_parse(out, s.getData(), 0, n) == n;
}

sl_bool SocketAddress::parse(const String& s)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _SocketAddress_parse(this, s.getData(), 0, n) == n;
}

sl_uint32 SocketAddress::getSystemSocketAddress(void* addr)
{
	if (ip.isIPv4()) {
		sockaddr_in& out = *((sockaddr_in*)addr);
		Base::resetMemory(&out, 0, sizeof(sockaddr_in));
		out.sin_family = AF_INET;
		out.sin_addr.s_addr = htonl(ip.getIPv4().getInt());
		out.sin_port = htons((sl_uint16)(port));
		return sizeof(sockaddr_in);
	} else if (ip.isIPv6()) {
		IPv6Address ipv6 = ip.getIPv6();
		sockaddr_in6& out = *((sockaddr_in6*)addr);
		Base::resetMemory(&out, 0, sizeof(sockaddr_in6));
		out.sin6_family = AF_INET6;
		for (int i = 0; i < 8; i++) {
			sl_uint16* w = (sl_uint16*)(&(out.sin6_addr));
			w[i] = htons(ipv6.getElement(i));
		}
		out.sin6_port = htons((sl_uint16)(port));
		return sizeof(sockaddr_in6);
	}
	return 0;
}

sl_bool SocketAddress::setSystemSocketAddress(const void* _in, sl_uint32 size)
{
	sockaddr_storage& in = *((sockaddr_storage*)_in);
	if (in.ss_family == AF_INET) {
		if (size == 0 || size == sizeof(sockaddr_in)) {
			sockaddr_in& addr = *((sockaddr_in*)&in);
			ip = IPv4Address(ntohl(addr.sin_addr.s_addr));
			port = ntohs(addr.sin_port);
			return sl_true;
		}
	} else if (in.ss_family == AF_INET6) {
		if (size == 0 || size == sizeof(sockaddr_in6)) {
			sockaddr_in6& addr = *((sockaddr_in6*)&in);
			IPv6Address ipv6;
			for (int i = 0; i < 8; i++) {
				sl_uint16* w = (sl_uint16*)(&(addr.sin6_addr));
				ipv6.setElement(i, ntohs(w[i]));
			}
			ip = ipv6;
			port = ntohs(addr.sin6_port);
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool SocketAddress::setHostAddress(const String& address)
{
	sl_reg index = address.lastIndexOf(':');
	if (index < 0) {
		port = 0;
		return ip.setHostName(address);
	} else {
		port = address.substring(index + 1).parseInt32();
		return ip.setHostName(address.substring(0, index));
	}
}

sl_bool SocketAddress::parseIPv4Range(const String& str, IPv4Address* _from, IPv4Address* _to)
{
	IPv4Address from;
	IPv4Address to;
	sl_reg index = str.indexOf('-');
	if (index > 0) {
		if (from.parse(str.substring(0, index))) {
			if (to.parse(str.substring(index + 1))) {
				if (to >= from) {
					if (_from) {
						*_from = from;
					}
					if (_to) {
						*_to = to;
					}
					return sl_true;
				}
			}
		}
	} else {
		if (from.parse(str)) {
			to = from;
			if (_from) {
				*_from = from;
			}
			if (_to) {
				*_to = to;
			}
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool SocketAddress::parsePortRange(const String& str, sl_uint32* from, sl_uint32* to)
{
	return SettingUtil::parseUint32Range(str, from, to);
}

sl_bool SocketAddress::operator==(const SocketAddress& other) const
{
	return port == other.port && ip == other.ip;
}

sl_bool SocketAddress::operator!=(const SocketAddress& other) const
{
	return ! (*this == other);
}

template <>
int Compare<SocketAddress>::compare(const SocketAddress& a, const SocketAddress& b)
{
	return a.compare(b);
}

template <>
sl_bool Compare<SocketAddress>::equals(const SocketAddress& a, const SocketAddress& b)
{
	return a == b;
}

template <>
sl_uint32 Hash<SocketAddress>::hash(const SocketAddress& a)
{
	return a.hashCode();
}

SLIB_NETWORK_NAMESPACE_END
