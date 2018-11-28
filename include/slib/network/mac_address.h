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
		SLIB_INLINE MacAddress() noexcept {}
		
		MacAddress(const MacAddress& other) noexcept;
		
		MacAddress(const sl_uint8* m) noexcept;
		
		MacAddress(sl_uint8 m0, sl_uint8 m1, sl_uint8 m2, sl_uint8 m3, sl_uint8 m4, sl_uint8 m5) noexcept;
		
		MacAddress(const String& address) noexcept;
		
	public:
		static const MacAddress& zero() noexcept;
		
		sl_bool isZero() const noexcept;
		
		sl_bool isNotZero() const noexcept;
		
		void setZero() noexcept;
		
		static const MacAddress& getBroadcast() noexcept;
		
		sl_bool isBroadcast() const noexcept;
		
		sl_bool isNotBroadcast() const noexcept;
		
		void setBroadcast() noexcept;
		
		sl_bool isMulticast() const noexcept;
		
		sl_bool isNotMulticast() const noexcept;
		
		void makeMulticast(const IPv4Address& addrMulticast) noexcept;
		
		void makeMulticast(const IPv6Address& addrMulticast) noexcept;
		
		void getBytes(sl_uint8* _m) const noexcept;
		
		void setBytes(const sl_uint8* _m) noexcept;
		
		int compare(const MacAddress& other) const noexcept;
		
		sl_size hashCode() const noexcept;
		
		// m0-m1-m2-m3-m4-m5, m0:m1:m2:m3:m4:m5
		String toString(sl_char8 sep = '-') const noexcept;
		
		sl_bool setString(const String& address) noexcept;
		
		
		template <class ST>
		static sl_bool parse(const ST& str, MacAddress* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}
		
	public:
		MacAddress& operator=(const MacAddress& other) noexcept;
		
		MacAddress& operator=(const String& address) noexcept;
		
		sl_bool operator==(const MacAddress& other) const noexcept;
		
		sl_bool operator!=(const MacAddress& other) const noexcept;
		
	private:
		static const sl_uint8 _zero[6];
		static const sl_uint8 _broadcast[6];
		
	};
	
	template <>
	sl_reg Parser<MacAddress, sl_char8>::parse(MacAddress* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<MacAddress, sl_char16>::parse(MacAddress* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	
	template <>
	class Compare<MacAddress>
	{
	public:
		int operator()(const MacAddress& a, const MacAddress& b) const noexcept;
	};
	
	template <>
	class Equals<MacAddress>
	{
	public:
		sl_bool operator()(const MacAddress& a, const MacAddress& b) const noexcept;
	};
	
	template <>
	class Hash<MacAddress>
	{
	public:
		sl_size operator()(const MacAddress& a) const noexcept;
	};
	
}
	
#include "detail/mac_address.inc"

#endif
