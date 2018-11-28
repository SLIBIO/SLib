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

namespace slib
{

	SLIB_INLINE sl_uint32 IPv4Address::getInt() const noexcept
	{
		return ((sl_uint32)(a) << 24) | ((sl_uint32)(b) << 16) | ((sl_uint32)(c) << 8) | ((sl_uint32)(d));
	}
	
	SLIB_INLINE void IPv4Address::setInt(sl_uint32 addr) noexcept
	{
		a = (sl_uint8)(addr >> 24);
		b = (sl_uint8)(addr >> 16);
		c = (sl_uint8)(addr >> 8);
		d = (sl_uint8)(addr);
	}
	
	SLIB_INLINE const IPv4Address& IPv4Address::zero() noexcept
	{
		return *(reinterpret_cast<IPv4Address const*>(&_zero));
	}
	
	SLIB_INLINE sl_bool IPv4Address::isZero() const noexcept
	{
		return getInt() == 0;
	}
	
	SLIB_INLINE sl_bool IPv4Address::isNotZero() const noexcept
	{
		return getInt() != 0;
	}
	
	SLIB_INLINE IPv4Address& IPv4Address::operator=(sl_uint32 addr) noexcept
	{
		a = (sl_uint8)(addr >> 24);
		b = (sl_uint8)(addr >> 16);
		c = (sl_uint8)(addr >> 8);
		d = (sl_uint8)(addr);
		return *this;
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator==(const IPv4Address& other) const noexcept
	{
		return getInt() == other.getInt();
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator==(sl_uint32 addr) const noexcept
	{
		return getInt() == addr;
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator!=(const IPv4Address& other) const noexcept
	{
		return getInt() != other.getInt();
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator!=(sl_uint32 addr) const noexcept
	{
		return getInt() != addr;
	}
	
	
	SLIB_INLINE const IPv6Address& IPv6Address::zero() noexcept
	{
		return *(reinterpret_cast<IPv6Address const*>(&_zero));
	}
	
	SLIB_INLINE const IPv6Address& IPv6Address::getLoopback() noexcept
	{
		return *(reinterpret_cast<IPv6Address const*>(&_loopback));
	}
	
	
	SLIB_INLINE const IPAddress& IPAddress::none() noexcept
	{
		return *(reinterpret_cast<IPAddress const*>(&_none));
	}
	
	SLIB_INLINE void IPAddress::setNone() noexcept
	{
		type = IPAddressType::None;
	}
	
	SLIB_INLINE sl_bool IPAddress::isNone() const noexcept
	{
		return type == IPAddressType::None;
	}
	
	SLIB_INLINE sl_bool IPAddress::isNotNone() const noexcept
	{
		return type != IPAddressType::None;
	}

}
