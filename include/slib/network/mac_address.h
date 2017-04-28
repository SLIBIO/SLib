/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_MAC_ADDRESS
#define CHECKHEADER_SLIB_NETWORK_MAC_ADDRESS

#include "definition.h"

#include "ip_address.h"

#include "../core/string.h"
#include "../core/parse.h"

namespace slib
{

	class SLIB_EXPORT MacAddress
	{
	public:
		sl_uint8 m[6];
		
	public:
		SLIB_INLINE MacAddress() = default;
		
		MacAddress(const MacAddress& other);
		
		MacAddress(const sl_uint8* m);
		
		MacAddress(sl_uint8 m0, sl_uint8 m1, sl_uint8 m2, sl_uint8 m3, sl_uint8 m4, sl_uint8 m5);
		
		MacAddress(const String& address);
		
	public:
		static const MacAddress& zero();
		
		sl_bool isZero() const;
		
		sl_bool isNotZero() const;
		
		void setZero();
		
		static const MacAddress& getBroadcast();
		
		sl_bool isBroadcast() const;
		
		sl_bool isNotBroadcast() const;
		
		void setBroadcast();
		
		sl_bool isMulticast() const;
		
		sl_bool isNotMulticast() const;
		
		void makeMulticast(const IPv4Address& addrMulticast);
		
		void makeMulticast(const IPv6Address& addrMulticast);
		
		void getBytes(sl_uint8* _m) const;
		
		void setBytes(const sl_uint8* _m);
		
		int compare(const MacAddress& other) const;
		
		sl_uint32 hashCode() const;
		
		// m0-m1-m2-m3-m4-m5, m0:m1:m2:m3:m4:m5
		String toString(sl_char8 sep = '-') const;
		
		sl_bool setString(const String& address);
		
		
		template <class ST>
		static sl_bool parse(const ST& str, MacAddress* _out)
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str)
		{
			return Parse(str, this);
		}
		
	public:
		MacAddress& operator=(const MacAddress& other);
		
		MacAddress& operator=(const String& address);
		
		sl_bool operator==(const MacAddress& other) const;
		
		sl_bool operator!=(const MacAddress& other) const;
		
	private:
		static const sl_uint8 _zero[6];
		static const sl_uint8 _broadcast[6];
		
	};
	
	template <>
	sl_reg Parser<MacAddress, sl_char8>::parse(MacAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg Parser<MacAddress, sl_char16>::parse(MacAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
	
	template <>
	class Compare<MacAddress>
	{
	public:
		int operator()(const MacAddress& a, const MacAddress& b) const;
	};
	
	template <>
	class Equals<MacAddress>
	{
	public:
		sl_bool operator()(const MacAddress& a, const MacAddress& b) const;
	};
	
	template <>
	class Hash<MacAddress>
	{
	public:
		sl_uint32 operator()(const MacAddress& a) const;
	};
	
}
	
#include "detail/mac_address.inc"

#endif
