/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_IP_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_IP_ADDRESS

#include "definition.h"

#include "../core/list.h"
#include "../core/string.h"
#include "../core/parse.h"

namespace slib
{

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
		constexpr IPv4Address(): a(0), b(0), c(0), d(0) {}
		
		constexpr IPv4Address(const IPv4Address& other): a(other.a), b(other.b), c(other.c), d(other.d) {}
		
		constexpr IPv4Address(sl_uint8 const addr[4]): a(addr[0]), b(addr[1]), c(addr[2]), d(addr[3]) {}
		
		constexpr IPv4Address(sl_uint8 _a, sl_uint8 _b, sl_uint8 _c, sl_uint8 _d): a(_a), b(_b), c(_c), d(_d) {}
		
		constexpr IPv4Address(sl_uint32 addr): a((sl_uint8)(addr >> 24)), b((sl_uint8)(addr >> 16)), c((sl_uint8)(addr >> 8)), d((sl_uint8)(addr)) {}
		
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
		
		
		template <class ST>
		static sl_bool parse(const ST& str, IPv4Address* _out)
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str)
		{
			return Parse(str, this);
		}
		
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
	sl_reg Parser<IPv4Address, sl_char8>::parse(IPv4Address* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg Parser<IPv4Address, sl_char16>::parse(IPv4Address* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	class Compare<IPv4Address>
	{
	public:
		int operator()(const IPv4Address& a, const IPv4Address& b) const;
	};
	
	template <>
	class Equals<IPv4Address>
	{
	public:
		sl_bool operator()(const IPv4Address& a, const IPv4Address& b) const;
	};
	
	template <>
	class Hash<IPv4Address>
	{
	public:
		sl_uint32 operator()(const IPv4Address& a) const;
	};
	
	
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
	class Compare<IPv4AddressInfo>
	{
	public:
		int operator()(const IPv4AddressInfo& a, const IPv4AddressInfo& b) const;
	};
	
	template <>
	class Equals<IPv4AddressInfo>
	{
	public:
		sl_bool operator()(const IPv4AddressInfo& a, const IPv4AddressInfo& b) const;
	};
	
	
	class SLIB_EXPORT IPv6Address
	{
	public:
		sl_uint8 m[16];
		
	public:
		SLIB_INLINE IPv6Address() = default;
		
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
		
		
		template <class ST>
		static sl_bool parse(const ST& str, IPv6Address* _out)
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str)
		{
			return Parse(str, this);
		}
		
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
	sl_reg Parser<IPv6Address, sl_char8>::parse(IPv6Address* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg Parser<IPv6Address, sl_char16>::parse(IPv6Address* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	class Compare<IPv6Address>
	{
	public:
		int operator()(const IPv6Address& a, const IPv6Address& b) const;
	};
	
	template <>
	class Equals<IPv6Address>
	{
	public:
		sl_bool operator()(const IPv6Address& a, const IPv6Address& b) const;
	};
	
	template <>
	class Hash<IPv6Address>
	{
	public:
		sl_uint32 operator()(const IPv6Address& a) const;
	};
	
	
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
		SLIB_INLINE IPAddress(): type(IPAddressType::None) {}
		
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
		
		
		template <class ST>
		static sl_bool parse(const ST& str, IPAddress* _out)
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str)
		{
			return Parse(str, this);
		}
		
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
	sl_reg Parser<IPAddress, sl_char8>::parse(IPAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg Parser<IPAddress, sl_char16>::parse(IPAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	class Compare<IPAddress>
	{
	public:
		int operator()(const IPAddress& a, const IPAddress& b) const;
	};
	
	template <>
	class Equals<IPAddress>
	{
	public:
		sl_bool operator()(const IPAddress& a, const IPAddress& b) const;
	};
	
	template <>
	class Hash<IPAddress>
	{
	public:
		sl_uint32 operator()(const IPAddress& a) const;
	};
	
}

#include "detail/ip_address.inc"

#endif
