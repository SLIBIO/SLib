/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		
		
		template <class ST>
		static sl_bool parse(const ST& str, SocketAddress* _out)
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str)
		{
			return Parse(str, this);
		}
		
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
	sl_reg Parser<SocketAddress, sl_char8>::parse(SocketAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg Parser<SocketAddress, sl_char16>::parse(SocketAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	class Compare<SocketAddress>
	{
	public:
		int operator()(const SocketAddress& a, const SocketAddress& b) const;
	};
	
	template <>
	class Equals<SocketAddress>
	{
	public:
		sl_bool operator()(const SocketAddress& a, const SocketAddress& b) const;
	};
	
	template <>
	class Hash<SocketAddress>
	{
	public:
		sl_uint32 operator()(const SocketAddress& a) const;
	};

}

#endif
