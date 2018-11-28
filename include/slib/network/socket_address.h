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

#ifndef CHECKHEADER_SLIB_NETWORK_SOCKET_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_SOCKET_ADDRESS

#include "definition.h"

#include "ip_address.h"

#include "../core/string.h"
#include "../core/parse.h"

namespace slib
{

	class SLIB_EXPORT SocketAddress
	{
	public:
		IPAddress ip;
		sl_uint32 port;
		
	public:
		SocketAddress() noexcept;
		
		SocketAddress(const SocketAddress& other) noexcept;
		
		SocketAddress(sl_int32 port) noexcept;
		
		SocketAddress(const IPAddress& ip, sl_int32 port) noexcept;
		
		SocketAddress(const String& str) noexcept;
		
	public:
		static const SocketAddress& none() noexcept;
		
		void setNone() noexcept;
		
		sl_bool isValid() const noexcept;
		
		sl_bool isInvalid() const noexcept;
		
		int compare(const SocketAddress& other) const noexcept;
		
		sl_size hashCode() const noexcept;
		
		/*
		 Address Format
			IPv4 - a.b.c.d:port
			Ipv6 - [s0:s1:s2:s3:s4:s5:s6:s7]:port
		 */
		String toString() const noexcept;
		
		sl_bool setString(const String& str) noexcept;
		
		sl_uint32 getSystemSocketAddress(void* addr) noexcept;
		
		sl_bool setSystemSocketAddress(const void* addr, sl_uint32 size = 0) noexcept;
		
		// HostName:port
		sl_bool setHostAddress(const String& address) noexcept;
		
		
		template <class ST>
		static sl_bool parse(const ST& str, SocketAddress* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}
		
		static sl_bool parseIPv4Range(const String& str, IPv4Address* from = sl_null, IPv4Address* to = sl_null) noexcept;
		
		static sl_bool parsePortRange(const String& str, sl_uint32* from = sl_null, sl_uint32* to = sl_null) noexcept;
		
	public:
		SocketAddress& operator=(const SocketAddress& other) noexcept;
		
		SocketAddress& operator=(const String& str) noexcept;
		
		sl_bool operator==(const SocketAddress& other) const noexcept;
		
		sl_bool operator!=(const SocketAddress& other) const noexcept;
		
	private:
		struct _socket_address
		{
			struct {
				IPAddressType type;
				sl_uint8 data[PRIV_SLIB_NET_IPADDRESS_SIZE];
			} ip;
			sl_uint32 port;
		};
		
		static const _socket_address _none;
		
	};
	
	template <>
	sl_reg Parser<SocketAddress, sl_char8>::parse(SocketAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<SocketAddress, sl_char16>::parse(SocketAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	class Compare<SocketAddress>
	{
	public:
		int operator()(const SocketAddress& a, const SocketAddress& b) const noexcept;
	};
	
	template <>
	class Equals<SocketAddress>
	{
	public:
		sl_bool operator()(const SocketAddress& a, const SocketAddress& b) const noexcept;
	};
	
	template <>
	class Hash<SocketAddress>
	{
	public:
		sl_size operator()(const SocketAddress& a) const noexcept;
	};

}

#include "detail/socket_address.inc"

#endif
