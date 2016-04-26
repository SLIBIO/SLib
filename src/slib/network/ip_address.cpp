#include "../../../inc/slib/network/ip_address.h"

#include "../../../inc/slib/network/os.h"
#include "../../../inc/slib/core/math.h"

SLIB_NETWORK_NAMESPACE_BEGIN

SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPv4Address)

const sl_uint8 IPv4Address::_zero[4] = {0, 0, 0, 0};

IPv4Address::IPv4Address(const String& address)
{
	if (! parse(address)) {
		set(0, 0, 0, 0);
	}
}

void IPv4Address::set(sl_uint8 _a, sl_uint8 _b, sl_uint8 _c, sl_uint8 _d)
{
	a = _a;
	b = _b;
	c = _c;
	d = _d;
}

void IPv4Address::getBytes(sl_uint8* bytes) const
{
	bytes[0] = a;
	bytes[1] = b;
	bytes[2] = c;
	bytes[3] = d;
}

void IPv4Address::setBytes(const sl_uint8* bytes)
{
	a = bytes[0];
	b = bytes[1];
	c = bytes[2];
	d = bytes[3];
}

void IPv4Address::setZero()
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
}

sl_bool IPv4Address::isLoopback() const
{
	return a == 127;
}

sl_bool IPv4Address::isMulticast() const
{
	return a >= 224 && a <= 239;
}

sl_bool IPv4Address::isBroadcast() const
{
	return getInt() == 0xFFFFFFFF;
}

sl_bool IPv4Address::isHost() const
{
	sl_uint32 n = getInt();
	return n != 0 && a < 224 && a != 127;
}

sl_bool IPv4Address::isPrivate() const
{
	sl_uint32 n = getInt();
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

int IPv4Address::compare(const IPv4Address& other) const
{
	sl_uint32 p1 = getInt();
	sl_uint32 p2 = getInt();
	return (p1 < p2) ? -1 : (p1 > p2);
}

sl_uint32 IPv4Address::hashCode() const
{
	return sl_hash(getInt());
}

String IPv4Address::toString() const
{
	String ret = String::fromUint32(a) + "." + String::fromUint32(b) + "." + String::fromUint32(c) + "." + String::fromUint32(d);
	return ret;
}

template <class CT>
SLIB_INLINE sl_reg _IPv4Address_parse(IPv4Address* obj, const CT* sz, sl_size i, sl_size n)
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
		obj->a = (sl_uint8)(v[0]);
		obj->b = (sl_uint8)(v[1]);
		obj->c = (sl_uint8)(v[2]);
		obj->d = (sl_uint8)(v[3]);
	}
	return i;
}

sl_reg IPv4Address::parse(IPv4Address* out, const char* sz, sl_size posBegin, sl_size n)
{
	return _IPv4Address_parse(out, sz, posBegin, n);
}

sl_reg IPv4Address::parse(IPv4Address* out, const sl_char16* sz, sl_size posBegin, sl_size n)
{
	return _IPv4Address_parse(out, sz, posBegin, n);
}

sl_bool IPv4Address::parse(const String& s, IPv4Address* out)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv4Address_parse(out, s.getData(), 0, n) == n;
}

sl_bool IPv4Address::parse(const String& s)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv4Address_parse(this, s.getData(), 0, n) == n;
}

void IPv4Address::makeNetworkMask(sl_uint32 networkPrefixLength)
{
	int p = networkPrefixLength;
	if (p >= 32) {
		a = 255;
		b = 255;
		c = 255;
		d = 255;
	} else if (p == 31) {
		a = 255;
		b = 255;
		c = 255;
		d = 254;
	} else if (p == 0) {
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	} else {
		int t = ((1 << (p + 1)) - 1) << (32 - p);
		a = (t >> 24) & 255;
		b = (t >> 16) & 255;
		c = (t >> 8) & 255;
		d = t & 255;
	}
}

sl_uint32 IPv4Address::getNetworkPrefixLengthFromMask() const
{
	sl_uint32 t = getInt();
	if (t == 0) {
		return 0;
	}
	return 32 - Math::getLeastSignificantBits(t);
}

sl_bool IPv4Address::setHostName(const String& hostName)
{
	*this = Network::getIPv4AddressFromHostName(hostName);
	return isNotZero();
}

IPv4Address& IPv4Address::operator=(const String& address)
{
	if (! parse(address)) {
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	}
	return *this;
}

sl_bool IPv4Address::operator>=(const IPv4Address& other) const
{
	return getInt() >= other.getInt();
}

sl_bool IPv4Address::operator>(const IPv4Address& other) const
{
	return getInt() > other.getInt();
}

sl_bool IPv4Address::operator<=(const IPv4Address& other) const
{
	return getInt() <= other.getInt();
}

sl_bool IPv4Address::operator<(const IPv4Address& other) const
{
	return getInt() < other.getInt();
}

template <>
int Compare<IPv4Address>::compare(const IPv4Address& a, const IPv4Address& b)
{
	return a.compare(b);
}

template <>
sl_bool Compare<IPv4Address>::equals(const IPv4Address& a, const IPv4Address& b)
{
	return a == b;
}

template <>
sl_uint32 Hash<IPv4Address>::hash(const IPv4Address& a)
{
	return a.hashCode();
}


SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPv4AddressInfo)

IPv4Address IPv4AddressInfo::getNetworkMask() const
{
	IPv4Address ret;
	ret.makeNetworkMask(networkPrefixLength);
	return ret;
}

void IPv4AddressInfo::setNetworkMask(const IPv4Address& mask)
{
	networkPrefixLength = mask.getNetworkPrefixLengthFromMask();
}

template <>
int Compare<IPv4AddressInfo>::compare(const IPv4AddressInfo& a, const IPv4AddressInfo& b)
{
	return a.address.compare(b.address);
}

template <>
sl_bool Compare<IPv4AddressInfo>::equals(const IPv4AddressInfo& a, const IPv4AddressInfo& b)
{
	return a.address == b.address;
}


SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPv6Address)

const sl_uint8 IPv6Address::_zero[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const sl_uint8 IPv6Address::_loopback[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

IPv6Address::IPv6Address() = default;

IPv6Address::IPv6Address(const IPv6Address& other) = default;

IPv6Address::IPv6Address(const sl_uint16* s)
{
	for (int i = 0; i < 8; i++) {
		m[i * 2] = (sl_uint8)(s[i] >> 8);
		m[i * 2 + 1] = (sl_uint8)(s[i]);
	}
}

IPv6Address::IPv6Address(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7)
{
	setElements(s0, s1, s2, s3, s4, s5, s6, s7);
}

IPv6Address::IPv6Address(const sl_uint8* b)
{
	Base::copyMemory(m, b, 16);
}

IPv6Address::IPv6Address(const String& address)
{
	if (! parse(address)) {
		Base::resetMemory(m, 0, 16);
	}
}

IPv6Address::IPv6Address(const IPv4Address& ip)
{
	setIPv4Transition(ip);
}

sl_uint16 IPv6Address::getElement(int index) const
{
	return (sl_uint16)((((sl_uint32)m[index * 2]) << 8) | ((sl_uint32)m[index * 2 + 1]));
}

void IPv6Address::setElement(int index, sl_uint16 s)
{
	m[index * 2] = (sl_uint8)(s >> 8);
	m[index * 2 + 1] = (sl_uint8)(s);
}

void IPv6Address::getElements(sl_uint16* s) const
{
	for (int i = 0; i < 8; i++) {
		s[i] = (sl_uint16)((((sl_uint32)m[i * 2]) << 8) | ((sl_uint32)m[i * 2 + 1]));
	}
}

void IPv6Address::setElements(const sl_uint16* s)
{
	for (int i = 0; i < 8; i++) {
		m[i * 2] = (sl_uint8)(s[i] >> 8);
		m[i * 2 + 1] = (sl_uint8)(s[i]);
	}
}

void IPv6Address::setElements(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7)
{
	m[0] = (sl_uint8)(s0 >> 8);
	m[1] = (sl_uint8)(s0);
	m[2] = (sl_uint8)(s1 >> 8);
	m[3] = (sl_uint8)(s1);
	m[4] = (sl_uint8)(s2 >> 8);
	m[5] = (sl_uint8)(s2);
	m[6] = (sl_uint8)(s3 >> 8);
	m[7] = (sl_uint8)(s3);
	m[8] = (sl_uint8)(s4 >> 8);
	m[9] = (sl_uint8)(s4);
	m[10] = (sl_uint8)(s5 >> 8);
	m[11] = (sl_uint8)(s5);
	m[12] = (sl_uint8)(s6 >> 8);
	m[13] = (sl_uint8)(s6);
	m[14] = (sl_uint8)(s7 >> 8);
	m[15] = (sl_uint8)(s7);
}

void IPv6Address::getBytes(sl_uint8* bytes) const
{
	Base::copyMemory(bytes, m, 16);
}

void IPv6Address::setBytes(const sl_uint8* bytes)
{
	Base::copyMemory(m, bytes, 16);
}

void IPv6Address::setZero()
{
	Base::zeroMemory(m, 16);
}

sl_bool IPv6Address::isZero() const
{
	return Base::compareZero(m, 16) == 0;
}

sl_bool IPv6Address::isNotZero() const
{
	return Base::compareZero(m, 16) != 0;
}

sl_bool IPv6Address::isLoopback() const
{
	return Base::compareMemory(_loopback, m, 16) == 0;
}

IPv4Address IPv6Address::getIPv4Transition() const
{
	if (isIPv4Transition()) {
		return {m[12], m[13], m[14], m[15]};
	} else {
		return {0, 0, 0, 0};
	}
}

void IPv6Address::setIPv4Transition(const IPv4Address& ip)
{
	setElements(0, 0, 0, 0, 0, 0xFFFF, (sl_uint16)((ip.a << 8) | ip.b), (sl_uint16)((ip.c << 8) | ip.d));
}

sl_bool IPv6Address::isIPv4Transition() const
{
	return m[0] == 0 && m[1] == 0 && m[2] == 0 && m[3] == 0 && m[4] == 0 && m[5] == 0 && m[6] == 0 && m[7] == 0 && m[8] == 0 && m[9] == 0 && m[10] == 255 && m[11] == 255;
}

int IPv6Address::compare(const IPv6Address& other) const
{
	return Base::compareMemory(m, other.m, 16);
}

sl_uint32 IPv6Address::hashCode() const
{
	return sl_hash(m, 16);
}

String IPv6Address::toString() const
{
	String ret;
	for (int i = 0; i < 8; i++) {
		if (i > 0) {
			ret += ":";
		}
		ret += String::fromUint32(getElement(i), 16, 4);
	}
	return ret;
}

template <class CT>
SLIB_INLINE sl_reg _IPv6Address_parse(IPv6Address* obj, const CT* sz, sl_size i, sl_size n)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	int k = 0;
	sl_uint16 v[8];
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
				v[k] = (sl_uint16)s;
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
			v[k] = (sl_uint16)s;
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
					obj->setElement(q, v[q]);
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
					obj->setElement(q, v[q]);
				}
				int x = skip_START + 8 - k;
				for (; q < x; q++) {
					obj->setElement(q, 0);
				}
				for (; q < 8; q++) {
					obj->setElement(q, v[q - 8 + k]);
				}
			}
		}
	}
	return i;
}

sl_reg IPv6Address::parse(IPv6Address* out, const char* sz, sl_size posStart, sl_size n)
{
	return _IPv6Address_parse(out, sz, posStart, n);
}

sl_reg IPv6Address::parse(IPv6Address* out, const sl_char16* sz, sl_size posStart, sl_size n)
{
	return _IPv6Address_parse(out, sz, posStart, n);
}

sl_bool IPv6Address::parse(const String& s, IPv6Address* out)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv6Address_parse(out, s.getData(), 0, n) == n;
}

sl_bool IPv6Address::parse(const String& s)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv6Address_parse(this, s.getData(), 0, n) == n;
}

sl_bool IPv6Address::setHostName(const String& hostName)
{
	*this = Network::getIPv6AddressFromHostName(hostName);
	return isNotZero();
}

IPv6Address& IPv6Address::operator=(const IPv6Address& other) = default;

IPv6Address& IPv6Address::operator=(const String& address)
{
	if (! parse(address)) {
		Base::resetMemory(m, 0, 16);
	}
	return *this;
}

sl_bool IPv6Address::operator==(const IPv6Address& other) const
{
	return Base::compareMemory(m, other.m, 16) == 0;
}

sl_bool IPv6Address::operator!=(const IPv6Address& other) const
{
	return !(*this == other);
}

sl_bool IPv6Address::operator<=(const IPv6Address& other) const
{
	return Base::compareMemory(m, other.m, 16) <= 0;
}

sl_bool IPv6Address::operator<(const IPv6Address& other) const
{
	return Base::compareMemory(m, other.m, 16) < 0;
}

sl_bool IPv6Address::operator>=(const IPv6Address& other) const
{
	return Base::compareMemory(m, other.m, 16) >= 0;
}

sl_bool IPv6Address::operator>(const IPv6Address& other) const
{
	return Base::compareMemory(m, other.m, 16) > 0;
}

template <>
int Compare<IPv6Address>::compare(const IPv6Address& a, const IPv6Address& b)
{
	return a.compare(b);
}

template <>
sl_bool Compare<IPv6Address>::equals(const IPv6Address& a, const IPv6Address& b)
{
	return a == b;
}

template <>
sl_uint32 Hash<IPv6Address>::hash(const IPv6Address& a)
{
	return a.hashCode();
}


SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(IPAddress)

const IPAddress::_IPAddress IPAddress::_none = { IPAddressType::None, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

IPAddress::IPAddress(const IPAddress& other) = default;

IPAddress::IPAddress(const IPv4Address& other)
{
	setIPv4(other);
}

IPAddress::IPAddress(const IPv6Address& other)
{
	setIPv6(other);
}

sl_bool IPAddress::isIPv4() const
{
	return type == IPAddressType::IPv4;
}

const IPv4Address& IPAddress::getIPv4() const
{
	return *((IPv4Address*)(void*)(m));
}

void IPAddress::setIPv4(const IPv4Address& addr)
{
	type = IPAddressType::IPv4;
	*((IPv4Address*)(void*)(m)) = addr;
}

sl_bool IPAddress::isIPv6() const
{
	return type == IPAddressType::IPv6;
}

const IPv6Address& IPAddress::getIPv6() const
{
	return *((IPv6Address*)(void*)(m));
}

void IPAddress::setIPv6(const IPv6Address& addr)
{
	type = IPAddressType::IPv6;
	*((IPv6Address*)(void*)(m)) = addr;
}

int IPAddress::compare(const IPAddress& other) const
{
	if (type < other.type) {
		return -1;
	}
	if (type > other.type) {
		return 1;
	}
	switch (type) {
		case IPAddressType::None:
			return 0;
		case IPAddressType::IPv4:
			return ((IPv4Address*)(void*)(m))->compare(*((IPv4Address*)(void*)(other.m)));
		case IPAddressType::IPv6:
			return ((IPv6Address*)(void*)(m))->compare(*((IPv6Address*)(void*)(other.m)));
	}
	return 0;
}

sl_uint32 IPAddress::hashCode() const
{
	switch (type) {
		case IPAddressType::None:
			return 0;
		case IPAddressType::IPv4:
			return ((IPv4Address*)(void*)(m))->hashCode();
		case IPAddressType::IPv6:
			return ((IPv6Address*)(void*)(m))->hashCode();
	}
	return 0;
}

String IPAddress::toString() const
{
	switch (type) {
		case IPAddressType::IPv4:
			return (*((IPv4Address*)(void*)(m))).toString();
		case IPAddressType::IPv6:
			return (*((IPv6Address*)(void*)(m))).toString();
		default:
			return String::null();
	}
}

template <class CT>
SLIB_INLINE sl_reg _IPAddress_parse(IPAddress* obj, const CT* sz, sl_size posStart, sl_size len)
{
	if (posStart >= len) {
		return SLIB_PARSE_ERROR;
	}
	sl_reg index;
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

sl_reg IPAddress::parse(IPAddress* out, const char* sz, sl_size posStart, sl_size len)
{
	return _IPAddress_parse(out, sz, posStart, len);
}

sl_reg IPAddress::parse(IPAddress* out, const sl_char16* sz, sl_size posStart, sl_size len)
{
	return _IPAddress_parse(out, sz, posStart, len);
}

sl_bool IPAddress::parse(const String& s, IPAddress* out)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPAddress_parse(out, s.getData(), 0, n) == n;
}

sl_bool IPAddress::parse(const String& s)
{
	sl_size n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPAddress_parse(this, s.getData(), 0, n) == n;
}

sl_bool IPAddress::setHostName(const String& hostName)
{
	*this = Network::getIPAddressFromHostName(hostName);
	return isNotNone();
}

IPAddress& IPAddress::operator=(const IPAddress& other) = default;

IPAddress& IPAddress::operator=(const IPv4Address& other)
{
	type = IPAddressType::IPv4;
	*((IPv4Address*)(void*)(m)) = other;
	return *this;
}

IPAddress& IPAddress::operator=(const IPv6Address& other)
{
	type = IPAddressType::IPv6;
	*((IPv6Address*)(void*)(m)) = other;
	return *this;
}

sl_bool IPAddress::operator==(const IPAddress& other) const
{
	if (type != other.type) {
		return sl_false;
	}
	switch (type) {
		case IPAddressType::None:
			return sl_true;
		case IPAddressType::IPv4:
			return (*((IPv4Address*)(void*)(m)) == *((IPv4Address*)(void*)(other.m)));
		case IPAddressType::IPv6:
			return (*((IPv6Address*)(void*)(m)) == *((IPv6Address*)(void*)(other.m)));
	}
	return sl_false;
}

sl_bool IPAddress::operator!=(const IPAddress& other) const
{
	return !(*this == other);
}

template <>
int Compare<IPAddress>::compare(const IPAddress& a, const IPAddress& b)
{
	return a.compare(b);
}

template <>
sl_bool Compare<IPAddress>::equals(const IPAddress& a, const IPAddress& b)
{
	return a == b;
}

template <>
sl_uint32 Hash<IPAddress>::hash(const IPAddress& a)
{
	return a.hashCode();
}

SLIB_NETWORK_NAMESPACE_END
