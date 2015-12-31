#include "../../../inc/slib/network/ip_address.h"
#include "../../../inc/slib/network/os.h"
#include "../../../inc/slib/core/math.h"

SLIB_NETWORK_NAMESPACE_BEGIN

const sl_uint8 IPv4Address::_any[4] = {0, 0, 0, 0};
const sl_uint8 IPv4Address::_broadcast[4] = { 255, 255, 255, 255 };
const sl_uint8 IPv4Address::_loopback[4] = { 127, 0, 0, 1 };
const sl_uint8 IPv4Address::_multicast_begin[4] = { 224, 0, 0, 0 };
const sl_uint8 IPv4Address::_multicast_end[4] = { 239, 255, 255, 255 };

String IPv4Address::toString() const
{
	String ret = String::fromUint32(a()) + "." + String::fromUint32(b()) + "." + String::fromUint32(c()) + "." + String::fromUint32(d());
	return ret;
}

template <class CT>
SLIB_INLINE sl_int32 _IPv4Address_parse(IPv4Address* obj, const CT* sz, sl_uint32 i, sl_uint32 n)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	int v[4];
	for (int k = 0; k < 4; k++) {
		int t = 0;
		int s = 0;
		for (; i < n; i++) {
			int h = sz[i];
			if (h >= '0' && h <= '9') {
				s = s * 10 + (h - '0');
				if (s > 255) {
					return SLIB_PARSE_ERROR;
				}
				t++;
			} else {
				break;
			}
		}
		if (t == 0) {
			return SLIB_PARSE_ERROR;
		}
		if (k < 3) {
			if (i >= n || sz[i] != '.') {
				return SLIB_PARSE_ERROR;
			}
			i++;
		}
		v[k] = s;
	}
	if (obj) {
		obj->m[0] = (sl_uint8)(v[0]);
		obj->m[1] = (sl_uint8)(v[1]);
		obj->m[2] = (sl_uint8)(v[2]);
		obj->m[3] = (sl_uint8)(v[3]);
	}
	return i;
}

sl_int32 IPv4Address::parse(IPv4Address* out, const char* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _IPv4Address_parse(out, sz, posBegin, n);
}

sl_int32 IPv4Address::parse(IPv4Address* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _IPv4Address_parse(out, sz, posBegin, n);
}

sl_bool IPv4Address::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv4Address_parse(this, s.getBuf(), 0, n) == n;
}

void IPv4Address::makeNetworkMask(sl_uint32 networkPrefixLength)
{
	int p = networkPrefixLength;
	if (p >= 32) {
		m[0] = 255;
		m[1] = 255;
		m[2] = 255;
		m[3] = 255;
	} else if (p == 31) {
		m[0] = 255;
		m[1] = 255;
		m[2] = 255;
		m[3] = 254;
	} else if (p == 0) {
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
	} else {
		int t = ((1 << (p + 1)) - 1) << (32 - p);
		m[0] = (t >> 24) & 255;
		m[1] = (t >> 16) & 255;
		m[2] = (t >> 8) & 255;
		m[3] = t & 255;
	}
}

sl_uint32 IPv4Address::getNetworkPrefixLengthFromMask() const
{
	sl_uint32 t = toInt();
	if (t == 0) {
		return 0;
	}
	return 32 - Math::getLeastSignificantBits(t);
}

sl_bool IPv4Address::setHostName(const String& hostName)
{
	*this = Network::getIPv4AddressFromHostName(hostName);
	return isNotAny();
}

const sl_uint8 IPv6Address::_any[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const sl_uint8 IPv6Address::_loopback[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

String IPv6Address::toString() const
{
	String ret;
	for (int i = 0; i < 8; i++) {
		if (i > 0) {
			ret += ":";
		}
		ret += String::fromUint32(get(i), 16, 4);
	}
	return ret;
}

template <class CT>
SLIB_INLINE sl_int32 _IPv6Address_parse(IPv6Address* obj, const CT* sz, sl_uint32 i, sl_uint32 n)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	int k = 0;
	int v[8];
	int skip_START = -1;
	for (k = 0; k < 8;) {
		int t = 0;
		int s = 0;
		for (; i < n; i++) {
			int h = sz[i];
			int x = 0;
			if (h >= '0' && h <= '9') {
				x = h - '0';
			} else if (h >= 'A' && h <= 'F') {
				x = h - ('A' - 10);
			} else if (h >= 'a' && h <= 'f') {
				x = h - ('a' - 10);
			} else {
				break;
			}
			s = (s << 4) | x;
			if (s > 0x10000) {
				return SLIB_PARSE_ERROR;
			}
			t++;
		}
		if (i >= n || sz[i] != ':') {
			if (t == 0) {
				if (skip_START != k) {
					return SLIB_PARSE_ERROR;
				}
			} else {
				v[k] = s;
				k++;
			}
			break;
		}
		if (t == 0) {
			if (k == 0) {
				if (i < n - 1 && sz[i + 1] == ':') {
					skip_START = 0;
					i += 2;
				} else {
					return SLIB_PARSE_ERROR;
				}
			} else {
				if (skip_START >= 0) {
					return SLIB_PARSE_ERROR;
				}
				skip_START = k;
				i++;
			}
		} else {
			v[k] = s;
			k++;
			i++;
		}
	}
	if (k == 8) {
		if (skip_START >= 0) {
			return SLIB_PARSE_ERROR;
		} else {
			if (obj) {
				for (int q = 0; q < 8; q++) {
					obj->set(q, v[q]);
				}
			}
		}
	} else {
		if (skip_START < 0) {
			return SLIB_PARSE_ERROR;
		} else {
			if (obj) {
				int q;
				for (q = 0; q < skip_START; q++) {
					obj->set(q, v[q]);
				}
				int x = skip_START + 8 - k;
				for (; q < x; q++) {
					obj->set(q, 0);
				}
				for (; q < 8; q++) {
					obj->set(q, v[q - 8 + k]);
				}
			}
		}
	}
	return i;
}

sl_int32 IPv6Address::parse(IPv6Address* out, const char* sz, sl_uint32 posStart, sl_uint32 n)
{
	return _IPv6Address_parse(out, sz, posStart, n);
}

sl_int32 IPv6Address::parse(IPv6Address* out, const sl_char16* sz, sl_uint32 posStart, sl_uint32 n)
{
	return _IPv6Address_parse(out, sz, posStart, n);
}

sl_bool IPv6Address::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv6Address_parse(this, s.getBuf(), 0, n) == n;
}

sl_bool IPv6Address::setHostName(const String& hostName)
{
	*this = Network::getIPv6AddressFromHostName(hostName);
	return isNotAny();
}

const IPAddress::_IPAddress IPAddress::_none = { IPAddress::typeNone, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

int IPAddress::compare(const IPAddress& other) const
{
	if (m_type < other.m_type) {
		return -1;
	}
	if (m_type > other.m_type) {
		return 1;
	}
	switch (m_type) {
	case typeNone:
		return 0;
	case typeIPv4:
		return ((IPv4Address*)(void*)(m_data))->compare(*((IPv4Address*)(void*)(other.m_data)));
	case typeIPv6:
		return ((IPv6Address*)(void*)(m_data))->compare(*((IPv6Address*)(void*)(other.m_data)));
	}
	return 0;
}

sl_uint32 IPAddress::hashCode() const
{
	switch (m_type) {
	case typeNone:
		return 0;
	case typeIPv4:
		return ((IPv4Address*)(void*)(m_data))->hashCode();
	case typeIPv6:
		return ((IPv6Address*)(void*)(m_data))->hashCode();
	}
	return 0;
}

String IPAddress::toString() const
{
	switch (m_type)
	{
	case typeIPv4:
		return (*((IPv4Address*)(void*)(m_data))).toString();
	case typeIPv6:
		return (*((IPv6Address*)(void*)(m_data))).toString();
	default:
		return String::null();
	}
}

template <class CT>
SLIB_INLINE sl_int32 _IPAddress_parse(IPAddress* obj, const CT* sz, sl_uint32 posStart, sl_uint32 len)
{
	if (posStart >= len) {
		return SLIB_PARSE_ERROR;
	}
	sl_int32 index;
	IPv4Address a4;
	index = IPv4Address::parse(&a4, sz, posStart, len);
	if (index != SLIB_PARSE_ERROR) {
		if (obj) {
			*obj = a4;
		}
		return index;
	}
	IPv6Address a6;
	index = IPv6Address::parse(&a6, sz, posStart, len);
	if (index != SLIB_PARSE_ERROR) {
		if (obj) {
			*obj = a6;
		}
		return index;
	}
	return SLIB_PARSE_ERROR;
}

sl_int32 IPAddress::parse(IPAddress* out, const char* sz, sl_uint32 posStart, sl_uint32 len)
{
	return _IPAddress_parse(out, sz, posStart, len);
}

sl_int32 IPAddress::parse(IPAddress* out, const sl_char16* sz, sl_uint32 posStart, sl_uint32 len)
{
	return _IPAddress_parse(out, sz, posStart, len);
}

sl_bool IPAddress::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPAddress_parse(this, s.getBuf(), 0, n) == n;
}

sl_bool IPAddress::setHostName(const String& hostName)
{
	*this = Network::getIPAddressFromHostName(hostName);
	return isNotNone();
}

SLIB_NETWORK_NAMESPACE_END
