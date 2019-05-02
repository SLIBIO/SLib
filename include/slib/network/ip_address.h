/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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
		SLIB_INLINE constexpr IPv4Address() noexcept: a(0), b(0), c(0), d(0) {}
		
		SLIB_INLINE constexpr IPv4Address(const IPv4Address& other) noexcept: a(other.a), b(other.b), c(other.c), d(other.d) {}
		
		SLIB_INLINE constexpr IPv4Address(sl_uint8 const addr[4]) noexcept: a(addr[0]), b(addr[1]), c(addr[2]), d(addr[3]) {}
		
		SLIB_INLINE constexpr IPv4Address(sl_uint8 _a, sl_uint8 _b, sl_uint8 _c, sl_uint8 _d) noexcept: a(_a), b(_b), c(_c), d(_d) {}
		
		SLIB_INLINE constexpr IPv4Address(sl_uint32 addr) noexcept: a((sl_uint8)(addr >> 24)), b((sl_uint8)(addr >> 16)), c((sl_uint8)(addr >> 8)), d((sl_uint8)(addr)) {}
		
		IPv4Address(const String& address) noexcept;
		
	public:
		void setElements(sl_uint8 a, sl_uint8 b, sl_uint8 c, sl_uint8 d) noexcept;
		
		sl_uint32 getInt() const noexcept;
		
		void setInt(sl_uint32 addr) noexcept;
		
		void getBytes(void* bytes) const noexcept;
		
		void setBytes(const void* bytes) noexcept;
		
		static const IPv4Address& zero() noexcept;
		
		sl_bool isZero() const noexcept;
		
		sl_bool isNotZero() const noexcept;
		
		void setZero() noexcept;
		
		sl_bool isLoopback() const noexcept;
		
		sl_bool isLinkLocal() const noexcept;
		
		sl_bool isMulticast() const noexcept;
		
		sl_bool isBroadcast() const noexcept;
		
		sl_bool isHost() const noexcept;
		
		sl_bool isPrivate() const noexcept;
		
		int compare(const IPv4Address& other) const noexcept;
		
		sl_size hashCode() const noexcept;
		
		// "a.b.c.d"
		String toString() const noexcept;
		
		sl_bool setString(const String& str) noexcept;
		
		void makeNetworkMask(sl_uint32 networkPrefixLength) noexcept;
		
		sl_uint32 getNetworkPrefixLengthFromMask() const noexcept;
		
		sl_bool setHostName(const String& hostName) noexcept;
		
		
		template <class ST>
		static sl_bool parse(const ST& str, IPv4Address* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}
		
	public:
		SLIB_INLINE IPv4Address& operator=(const IPv4Address& other) = default;
		
		IPv4Address& operator=(sl_uint32 addr) noexcept;
		
		IPv4Address& operator=(const String& address) noexcept;
		
		sl_bool operator==(const IPv4Address& other) const noexcept;
		
		sl_bool operator==(sl_uint32 addr) const noexcept;
		
		sl_bool operator!=(const IPv4Address& other) const noexcept;
		
		sl_bool operator!=(sl_uint32 addr) const noexcept;
		
		sl_bool operator>=(const IPv4Address& other) const noexcept;
		
		sl_bool operator>(const IPv4Address& other) const noexcept;
		
		sl_bool operator<=(const IPv4Address& other) const noexcept;
		
		sl_bool operator<(const IPv4Address& other) const noexcept;
		
	private:
		static const sl_uint8 _zero[4];
		
	};
	
	template <>
	sl_reg Parser<IPv4Address, sl_char8>::parse(IPv4Address* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<IPv4Address, sl_char16>::parse(IPv4Address* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	class Compare<IPv4Address>
	{
	public:
		int operator()(const IPv4Address& a, const IPv4Address& b) const noexcept;
	};
	
	template <>
	class Equals<IPv4Address>
	{
	public:
		sl_bool operator()(const IPv4Address& a, const IPv4Address& b) const noexcept;
	};
	
	template <>
	class Hash<IPv4Address>
	{
	public:
		sl_size operator()(const IPv4Address& a) const noexcept;
	};
	
	
	class SLIB_EXPORT IPv4AddressInfo
	{
	public:
		IPv4Address address;
		sl_uint32 networkPrefixLength;
		
	public:
		IPv4Address getNetworkMask() const noexcept;
		
		void setNetworkMask(const IPv4Address& mask) noexcept;
		
	};
	
	template <>
	class Compare<IPv4AddressInfo>
	{
	public:
		int operator()(const IPv4AddressInfo& a, const IPv4AddressInfo& b) const noexcept;
	};
	
	template <>
	class Equals<IPv4AddressInfo>
	{
	public:
		sl_bool operator()(const IPv4AddressInfo& a, const IPv4AddressInfo& b) const noexcept;
	};
	
	
	class SLIB_EXPORT IPv6Address
	{
	public:
		sl_uint8 m[16];
		
	public:
		SLIB_INLINE IPv6Address() noexcept {}
		
		IPv6Address(const IPv6Address& other) noexcept;
		
		// 8 elements
		IPv6Address(const sl_uint16* s) noexcept;
		
		IPv6Address(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7) noexcept;
		
		// 16 elements
		IPv6Address(const sl_uint8* b) noexcept;
		
		IPv6Address(const IPv4Address& ipv4) noexcept;
		
		IPv6Address(const String& address) noexcept;
		
	public:
		// not checking index bound (0~7)
		sl_uint16 getElement(int index) const noexcept;
		
		// not checking index bound (0~7)
		void setElement(int index, sl_uint16 s) noexcept;
		
		// 8 elements
		void getElements(sl_uint16* s) const noexcept;
		
		// 8 elements
		void setElements(const sl_uint16* s) noexcept;
		
		void setElements(sl_uint16 s0, sl_uint16 s1, sl_uint16 s2, sl_uint16 s3, sl_uint16 s4, sl_uint16 s5, sl_uint16 s6, sl_uint16 s7) noexcept;
		
		// 16 elements
		void getBytes(void* bytes) const noexcept;
		
		// 16 elements
		void setBytes(const void* bytes) noexcept;
		
		static const IPv6Address& zero() noexcept;
		
		void setZero() noexcept;
		
		sl_bool isZero() const noexcept;
		
		sl_bool isNotZero() const noexcept;
		
		static const IPv6Address& getLoopback() noexcept;
		
		sl_bool isLoopback() const noexcept;
		
		sl_bool isLinkLocal() const noexcept;
		
		IPv4Address getIPv4Transition() const noexcept;
		
		void setIPv4Transition(const IPv4Address& ipv4) noexcept;
		
		sl_bool isIPv4Transition() const noexcept;
		
		int compare(const IPv6Address& other) const noexcept;
		
		sl_size hashCode() const noexcept;
		
		// "s0:s1:s2:s3:s4:s5:s6:s7"
		String toString() const noexcept;
		
		sl_bool setString(const String& str) noexcept;
		
		sl_bool setHostName(const String& hostName) noexcept;
		
		
		template <class ST>
		static sl_bool parse(const ST& str, IPv6Address* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}
		
	public:
		IPv6Address& operator=(const IPv6Address& other) noexcept;
		
		IPv6Address& operator=(const String& address) noexcept;
		
		sl_bool operator==(const IPv6Address& other) const noexcept;
		
		sl_bool operator!=(const IPv6Address& other) const noexcept;
		
		sl_bool operator<=(const IPv6Address& other) const noexcept;
		
		sl_bool operator<(const IPv6Address& other) const noexcept;
		
		sl_bool operator>=(const IPv6Address& other) const noexcept;
		
		sl_bool operator>(const IPv6Address& other) const noexcept;
		
	private:
		static const sl_uint8 _zero[16];
		static const sl_uint8 _loopback[16];
		static const sl_uint8 _loopback_linkLocal[16];

	};
	
	template <>
	sl_reg Parser<IPv6Address, sl_char8>::parse(IPv6Address* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<IPv6Address, sl_char16>::parse(IPv6Address* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	class Compare<IPv6Address>
	{
	public:
		int operator()(const IPv6Address& a, const IPv6Address& b) const noexcept;
	};
	
	template <>
	class Equals<IPv6Address>
	{
	public:
		sl_bool operator()(const IPv6Address& a, const IPv6Address& b) const noexcept;
	};
	
	template <>
	class Hash<IPv6Address>
	{
	public:
		sl_size operator()(const IPv6Address& a) const noexcept;
	};
	
	
#define PRIV_SLIB_NET_IPADDRESS_SIZE 16
	
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
		sl_uint8 m[PRIV_SLIB_NET_IPADDRESS_SIZE];
		
	public:
		SLIB_INLINE IPAddress() noexcept: type(IPAddressType::None) {}
		
		IPAddress(const IPAddress& other) noexcept;
		
		IPAddress(const IPv4Address& other) noexcept;
		
		IPAddress(const IPv6Address& other) noexcept;
		
		IPAddress(const String& address) noexcept;
		
	public:
		static const IPAddress& none() noexcept;
		
		void setNone() noexcept;
		
		sl_bool isNone() const noexcept;
		
		sl_bool isNotNone() const noexcept;
		
		sl_bool isIPv4() const noexcept;
		
		const IPv4Address& getIPv4() const noexcept;
		
		void setIPv4(const IPv4Address& addr) noexcept;
		
		sl_bool isIPv6() const noexcept;
		
		const IPv6Address& getIPv6() const noexcept;
		
		void setIPv6(const IPv6Address& addr) noexcept;
		
		int compare(const IPAddress& other) const noexcept;
		
		sl_size hashCode() const noexcept;
		
		String toString() const noexcept;
		
		sl_bool setString(const String& str) noexcept;
		
		sl_bool setHostName(const String& hostName) noexcept;
		
		
		template <class ST>
		static sl_bool parse(const ST& str, IPAddress* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}
		
	public:
		IPAddress& operator=(const IPAddress& other) noexcept;
		
		IPAddress& operator=(const IPv4Address& other) noexcept;
		
		IPAddress& operator=(const IPv6Address& other) noexcept;
		
		IPAddress& operator=(const String& address) noexcept;
		
		sl_bool operator==(const IPAddress& other) const noexcept;
		
		sl_bool operator!=(const IPAddress& other) const noexcept;
		
	private:
		struct _ipaddress
		{
			IPAddressType type;
			sl_uint8 m[PRIV_SLIB_NET_IPADDRESS_SIZE];
		};
		static const _ipaddress _none;
		
	};
	
	template <>
	sl_reg Parser<IPAddress, sl_char8>::parse(IPAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<IPAddress, sl_char16>::parse(IPAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	class Compare<IPAddress>
	{
	public:
		int operator()(const IPAddress& a, const IPAddress& b) const noexcept;
	};
	
	template <>
	class Equals<IPAddress>
	{
	public:
		sl_bool operator()(const IPAddress& a, const IPAddress& b) const noexcept;
	};
	
	template <>
	class Hash<IPAddress>
	{
	public:
		sl_size operator()(const IPAddress& a) const noexcept;
	};
	
}

#include "detail/ip_address.inc"

#endif
