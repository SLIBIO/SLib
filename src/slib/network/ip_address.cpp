/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/ip_address.h"

#include "slib/network/os.h"
#include "slib/core/math.h"

namespace slib
{
	
	SLIB_ALIGN(8) const sl_uint8 IPv4Address::_zero[4] = {0, 0, 0, 0};

	IPv4Address::IPv4Address(const String& address) noexcept
	{
		setString(address);
	}

	void IPv4Address::setElements(sl_uint8 _a, sl_uint8 _b, sl_uint8 _c, sl_uint8 _d) noexcept
	{
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}

	void IPv4Address::getBytes(sl_uint8* bytes) const noexcept
	{
		bytes[0] = a;
		bytes[1] = b;
		bytes[2] = c;
		bytes[3] = d;
	}

	void IPv4Address::setBytes(const sl_uint8* bytes) noexcept
	{
		a = bytes[0];
		b = bytes[1];
		c = bytes[2];
		d = bytes[3];
	}

	void IPv4Address::setZero() noexcept
	{
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	}

	sl_bool IPv4Address::isLoopback() const noexcept
	{
		return a == 127;
	}

	sl_bool IPv4Address::isMulticast() const noexcept
	{
		return a >= 224 && a <= 239;
	}

	sl_bool IPv4Address::isBroadcast() const noexcept
	{
		return getInt() == 0xFFFFFFFF;
	}

	sl_bool IPv4Address::isHost() const noexcept
	{
		sl_uint32 n = getInt();
		return n != 0 && a < 224 && a != 127;
	}

	sl_bool IPv4Address::isPrivate() const noexcept
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

	int IPv4Address::compare(const IPv4Address& other) const noexcept
	{
		sl_uint32 p1 = getInt();
		sl_uint32 p2 = getInt();
		return (p1 < p2) ? -1 : (p1 > p2);
	}

	sl_size IPv4Address::hashCode() const noexcept
	{
		return Rehash32(getInt());
	}

	String IPv4Address::toString() const noexcept
	{
		String ret = String::fromUint32(a) + "." + String::fromUint32(b) + "." + String::fromUint32(c) + "." + String::fromUint32(d);
		return ret;
	}

	sl_bool IPv4Address::setString(const String& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	void IPv4Address::makeNetworkMask(sl_uint32 networkPrefixLength) noexcept
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

	sl_uint32 IPv4Address::getNetworkPrefixLengthFromMask() const noexcept
	{
		sl_uint32 t = getInt();
		if (t == 0) {
			return 0;
		}
		return 32 - Math::getLeastSignificantBits(t);
	}

	sl_bool IPv4Address::setHostName(const String& hostName) noexcept
	{
		*this = Network::getIPv4AddressFromHostName(hostName);
		return isNotZero();
	}


	template <class CT>
	SLIB_INLINE static sl_reg _priv_IPv4Address_parse(IPv4Address* obj, const CT* sz, sl_size i, sl_size n) noexcept
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

	template <>
	sl_reg Parser<IPv4Address, sl_char8>::parse(IPv4Address* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_IPv4Address_parse(_out, sz, posBegin, posEnd);
	}

	template <>
	sl_reg Parser<IPv4Address, sl_char16>::parse(IPv4Address* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_IPv4Address_parse(_out, sz, posBegin, posEnd);
	}


	IPv4Address& IPv4Address::operator=(const String& address) noexcept
	{
		setString(address);
		return *this;
	}

	sl_bool IPv4Address::operator>=(const IPv4Address& other) const noexcept
	{
		return getInt() >= other.getInt();
	}

	sl_bool IPv4Address::operator>(const IPv4Address& other) const noexcept
	{
		return getInt() > other.getInt();
	}

	sl_bool IPv4Address::operator<=(const IPv4Address& other) const noexcept
	{
		return getInt() <= other.getInt();
	}

	sl_bool IPv4Address::operator<(const IPv4Address& other) const noexcept
	{
		return getInt() < other.getInt();
	}

	int Compare<IPv4Address>::operator()(const IPv4Address& a, const IPv4Address& b) const noexcept
	{
		return a.compare(b);
	}

	sl_bool Equals<IPv4Address>::operator()(const IPv4Address& a, const IPv4Address& b) const noexcept
	{
		return a == b;
	}

	sl_size Hash<IPv4Address>::operator()(const IPv4Address& a) const noexcept
	{
		return a.hashCode();
	}


	IPv4Address IPv4AddressInfo::getNetworkMask() const noexcept
	{
		IPv4Address ret;
		ret.makeNetworkMask(networkPrefixLength);
		return ret;
	}

	void IPv4AddressInfo::setNetworkMask(const IPv4Address& mask) noexcept
	{
		networkPrefixLength = mask.getNetworkPrefixLengthFromMask();
	}

	int Compare<IPv4AddressInfo>::operator()(const IPv4AddressInfo& a, const IPv4AddressInfo& b) const noexcept
	{
		return a.address.compare(b.address);
	}

	sl_bool Equals<IPv4AddressInfo>::operator()(const IPv4AddressInfo& a, const IPv4AddressInfo& b) const noexcept
	{
		return a.address == b.address;
	}


	SLIB_ALIGN(8) const sl_uint8 IPv6Address::_zero[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	SLIB_ALIGN(8) const sl_uint8 IPv6Address::_loopback[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

	IPv6Address::IPv6Address(const IPv6Address& other) noexcept = default;

	IPv6Address::IPv6Address(const sl_uint16* s) noexcept
	{
		for (int i = 0; i < 8; i++) {
			m[i * 2] = (sl_uint8)(s[i] >> 8);
			m[i * 2 + 1] = (sl_uint8)(s[i]);
		}
	}

	IPv6Address::IPv6Address(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7) noexcept
	{
		setElements(s0, s1, s2, s3, s4, s5, s6, s7);
	}

	IPv6Address::IPv6Address(const sl_uint8* b) noexcept
	{
		Base::copyMemory(m, b, 16);
	}

	IPv6Address::IPv6Address(const String& address) noexcept
	{
		setString(address);
	}

	IPv6Address::IPv6Address(const IPv4Address& ip) noexcept
	{
		setIPv4Transition(ip);
	}

	sl_uint16 IPv6Address::getElement(int index) const noexcept
	{
		return (sl_uint16)((((sl_uint32)m[index * 2]) << 8) | ((sl_uint32)m[index * 2 + 1]));
	}

	void IPv6Address::setElement(int index, sl_uint16 s) noexcept
	{
		m[index * 2] = (sl_uint8)(s >> 8);
		m[index * 2 + 1] = (sl_uint8)(s);
	}

	void IPv6Address::getElements(sl_uint16* s) const noexcept
	{
		for (int i = 0; i < 8; i++) {
			s[i] = (sl_uint16)((((sl_uint32)m[i * 2]) << 8) | ((sl_uint32)m[i * 2 + 1]));
		}
	}

	void IPv6Address::setElements(const sl_uint16* s) noexcept
	{
		for (int i = 0; i < 8; i++) {
			m[i * 2] = (sl_uint8)(s[i] >> 8);
			m[i * 2 + 1] = (sl_uint8)(s[i]);
		}
	}

	void IPv6Address::setElements(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7) noexcept
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

	void IPv6Address::getBytes(sl_uint8* bytes) const noexcept
	{
		Base::copyMemory(bytes, m, 16);
	}

	void IPv6Address::setBytes(const sl_uint8* bytes) noexcept
	{
		Base::copyMemory(m, bytes, 16);
	}

	void IPv6Address::setZero() noexcept
	{
		Base::zeroMemory(m, 16);
	}

	sl_bool IPv6Address::isZero() const noexcept
	{
		return Base::equalsMemoryZero(m, 16);
	}

	sl_bool IPv6Address::isNotZero() const noexcept
	{
		return !(Base::equalsMemoryZero(m, 16));
	}

	sl_bool IPv6Address::isLoopback() const noexcept
	{
		return Base::equalsMemory(_loopback, m, 16);
	}

	IPv4Address IPv6Address::getIPv4Transition() const noexcept
	{
		if (isIPv4Transition()) {
			return {m[12], m[13], m[14], m[15]};
		} else {
			return {0, 0, 0, 0};
		}
	}

	void IPv6Address::setIPv4Transition(const IPv4Address& ip) noexcept
	{
		setElements(0, 0, 0, 0, 0, 0xFFFF, (sl_uint16)((ip.a << 8) | ip.b), (sl_uint16)((ip.c << 8) | ip.d));
	}

	sl_bool IPv6Address::isIPv4Transition() const noexcept
	{
		return m[0] == 0 && m[1] == 0 && m[2] == 0 && m[3] == 0 && m[4] == 0 && m[5] == 0 && m[6] == 0 && m[7] == 0 && m[8] == 0 && m[9] == 0 && m[10] == 255 && m[11] == 255;
	}

	int IPv6Address::compare(const IPv6Address& other) const noexcept
	{
		return Base::compareMemory(m, other.m, 16);
	}

	sl_size IPv6Address::hashCode() const noexcept
	{
		return Rehash64ToSize(SLIB_MAKE_QWORD(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7]) ^ SLIB_MAKE_QWORD(m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]));
	}

	String IPv6Address::toString() const noexcept
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

	sl_bool IPv6Address::setString(const String& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	sl_bool IPv6Address::setHostName(const String& hostName) noexcept
	{
		*this = Network::getIPv6AddressFromHostName(hostName);
		return isNotZero();
	}


	template <class CT>
	SLIB_INLINE static sl_reg _priv_IPv6Address_parse(IPv6Address* obj, const CT* sz, sl_size i, sl_size n) noexcept
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

	template <>
	sl_reg Parser<IPv6Address, sl_char8>::parse(IPv6Address* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_IPv6Address_parse(_out, sz, posBegin, posEnd);
	}

	template <>
	sl_reg Parser<IPv6Address, sl_char16>::parse(IPv6Address* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_IPv6Address_parse(_out, sz, posBegin, posEnd);
	}

	IPv6Address& IPv6Address::operator=(const IPv6Address& other) noexcept = default;

	IPv6Address& IPv6Address::operator=(const String& address) noexcept
	{
		setString(address);
		return *this;
	}

	sl_bool IPv6Address::operator==(const IPv6Address& other) const noexcept
	{
		return Base::compareMemory(m, other.m, 16) == 0;
	}

	sl_bool IPv6Address::operator!=(const IPv6Address& other) const noexcept
	{
		return !(*this == other);
	}

	sl_bool IPv6Address::operator<=(const IPv6Address& other) const noexcept
	{
		return Base::compareMemory(m, other.m, 16) <= 0;
	}

	sl_bool IPv6Address::operator<(const IPv6Address& other) const noexcept
	{
		return Base::compareMemory(m, other.m, 16) < 0;
	}

	sl_bool IPv6Address::operator>=(const IPv6Address& other) const noexcept
	{
		return Base::compareMemory(m, other.m, 16) >= 0;
	}

	sl_bool IPv6Address::operator>(const IPv6Address& other) const noexcept
	{
		return Base::compareMemory(m, other.m, 16) > 0;
	}

	int Compare<IPv6Address>::operator()(const IPv6Address& a, const IPv6Address& b) const noexcept
	{
		return a.compare(b);
	}

	sl_bool Equals<IPv6Address>::operator()(const IPv6Address& a, const IPv6Address& b) const noexcept
	{
		return a == b;
	}

	sl_size Hash<IPv6Address>::operator()(const IPv6Address& a) const noexcept
	{
		return a.hashCode();
	}


	const IPAddress::_ipaddress IPAddress::_none = { IPAddressType::None, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

	IPAddress::IPAddress(const IPAddress& other) noexcept = default;

	IPAddress::IPAddress(const IPv4Address& other) noexcept
	{
		setIPv4(other);
	}

	IPAddress::IPAddress(const IPv6Address& other) noexcept
	{
		setIPv6(other);
	}

	IPAddress::IPAddress(const String& address) noexcept
	{
		setString(address);
	}

	sl_bool IPAddress::isIPv4() const noexcept
	{
		return type == IPAddressType::IPv4;
	}

	const IPv4Address& IPAddress::getIPv4() const noexcept
	{
		return *(reinterpret_cast<IPv4Address const*>(m));
	}

	void IPAddress::setIPv4(const IPv4Address& addr) noexcept
	{
		type = IPAddressType::IPv4;
		*(reinterpret_cast<IPv4Address*>(m)) = addr;
	}

	sl_bool IPAddress::isIPv6() const noexcept
	{
		return type == IPAddressType::IPv6;
	}

	const IPv6Address& IPAddress::getIPv6() const noexcept
	{
		return *(reinterpret_cast<IPv6Address const*>(m));
	}

	void IPAddress::setIPv6(const IPv6Address& addr) noexcept
	{
		type = IPAddressType::IPv6;
		*(reinterpret_cast<IPv6Address*>(m)) = addr;
	}

	int IPAddress::compare(const IPAddress& other) const noexcept
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
				return (reinterpret_cast<IPv4Address const*>(m))->compare(*(reinterpret_cast<IPv4Address const*>(other.m)));
			case IPAddressType::IPv6:
				return (reinterpret_cast<IPv6Address const*>(m))->compare(*(reinterpret_cast<IPv6Address const*>(other.m)));
		}
		return 0;
	}

	sl_size IPAddress::hashCode() const noexcept
	{
		switch (type) {
			case IPAddressType::None:
				return 0;
			case IPAddressType::IPv4:
				return (reinterpret_cast<IPv4Address const*>(m))->hashCode();
			case IPAddressType::IPv6:
				return (reinterpret_cast<IPv6Address const*>(m))->hashCode();
		}
		return 0;
	}

	String IPAddress::toString() const noexcept
	{
		switch (type) {
			case IPAddressType::IPv4:
				return (*(reinterpret_cast<IPv4Address const*>(m))).toString();
			case IPAddressType::IPv6:
				return (*(reinterpret_cast<IPv6Address const*>(m))).toString();
			default:
				return sl_null;
		}
	}

	sl_bool IPAddress::setString(const String& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setNone();
			return sl_false;
		}
	}

	sl_bool IPAddress::setHostName(const String& hostName) noexcept
	{
		*this = Network::getIPAddressFromHostName(hostName);
		return isNotNone();
	}


	template <class CT>
	SLIB_INLINE static sl_reg _priv_IPAddress_parse(IPAddress* obj, const CT* sz, sl_size posStart, sl_size posEnd) noexcept
	{
		if (posStart >= posEnd) {
			return SLIB_PARSE_ERROR;
		}
		sl_reg index;
		IPv4Address a4;
		index = Parser<IPv4Address, CT>::parse(&a4, sz, posStart, posEnd);
		if (index != SLIB_PARSE_ERROR) {
			if (obj) {
				*obj = a4;
			}
			return index;
		}
		IPv6Address a6;
		index = Parser<IPv6Address, CT>::parse(&a6, sz, posStart, posEnd);
		if (index != SLIB_PARSE_ERROR) {
			if (obj) {
				*obj = a6;
			}
			return index;
		}
		return SLIB_PARSE_ERROR;
	}

	template <>
	sl_reg Parser<IPAddress, sl_char8>::parse(IPAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_IPAddress_parse(_out, sz, posBegin, posEnd);
	}

	template <>
	sl_reg Parser<IPAddress, sl_char16>::parse(IPAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_IPAddress_parse(_out, sz, posBegin, posEnd);
	}

	IPAddress& IPAddress::operator=(const IPAddress& other) noexcept = default;

	IPAddress& IPAddress::operator=(const IPv4Address& other) noexcept
	{
		type = IPAddressType::IPv4;
		*(reinterpret_cast<IPv4Address*>(m)) = other;
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPv6Address& other) noexcept
	{
		type = IPAddressType::IPv6;
		*(reinterpret_cast<IPv6Address*>(m)) = other;
		return *this;
	}

	IPAddress& IPAddress::operator=(const String& address) noexcept
	{
		setString(address);
		return *this;
	}

	sl_bool IPAddress::operator==(const IPAddress& other) const noexcept
	{
		if (type != other.type) {
			return sl_false;
		}
		switch (type) {
			case IPAddressType::None:
				return sl_true;
			case IPAddressType::IPv4:
				return (*(reinterpret_cast<IPv4Address const*>(m)) == *(reinterpret_cast<IPv4Address const*>(other.m)));
			case IPAddressType::IPv6:
				return (*(reinterpret_cast<IPv6Address const*>(m)) == *(reinterpret_cast<IPv6Address const*>(other.m)));
		}
		return sl_false;
	}

	sl_bool IPAddress::operator!=(const IPAddress& other) const noexcept
	{
		return !(*this == other);
	}

	int Compare<IPAddress>::operator()(const IPAddress& a, const IPAddress& b) const noexcept
	{
		return a.compare(b);
	}

	sl_bool Equals<IPAddress>::operator()(const IPAddress& a, const IPAddress& b) const noexcept
	{
		return a == b;
	}

	sl_size Hash<IPAddress>::operator()(const IPAddress& a) const noexcept
	{
		return a.hashCode();
	}

}
