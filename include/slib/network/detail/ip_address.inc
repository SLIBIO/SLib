/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

	SLIB_INLINE sl_uint32 IPv4Address::getInt() const
	{
		return ((sl_uint32)(a) << 24) | ((sl_uint32)(b) << 16) | ((sl_uint32)(c) << 8) | ((sl_uint32)(d));
	}
	
	SLIB_INLINE void IPv4Address::setInt(sl_uint32 addr)
	{
		a = (sl_uint8)(addr >> 24);
		b = (sl_uint8)(addr >> 16);
		c = (sl_uint8)(addr >> 8);
		d = (sl_uint8)(addr);
	}
	
	SLIB_INLINE const IPv4Address& IPv4Address::zero()
	{
		return *(reinterpret_cast<IPv4Address const*>(&_zero));
	}
	
	SLIB_INLINE sl_bool IPv4Address::isZero() const
	{
		return getInt() == 0;
	}
	
	SLIB_INLINE sl_bool IPv4Address::isNotZero() const
	{
		return getInt() != 0;
	}
	
	SLIB_INLINE IPv4Address& IPv4Address::operator=(sl_uint32 addr)
	{
		a = (sl_uint8)(addr >> 24);
		b = (sl_uint8)(addr >> 16);
		c = (sl_uint8)(addr >> 8);
		d = (sl_uint8)(addr);
		return *this;
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator==(const IPv4Address& other) const
	{
		return getInt() == other.getInt();
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator==(sl_uint32 addr) const
	{
		return getInt() == addr;
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator!=(const IPv4Address& other) const
	{
		return getInt() != other.getInt();
	}
	
	SLIB_INLINE sl_bool IPv4Address::operator!=(sl_uint32 addr) const
	{
		return getInt() != addr;
	}
	
	
	SLIB_INLINE const IPv6Address& IPv6Address::zero()
	{
		return *(reinterpret_cast<IPv6Address const*>(&_zero));
	}
	
	SLIB_INLINE const IPv6Address& IPv6Address::getLoopback()
	{
		return *(reinterpret_cast<IPv6Address const*>(&_loopback));
	}
	
	
	SLIB_INLINE const IPAddress& IPAddress::none()
	{
		return *(reinterpret_cast<IPAddress const*>(&_none));
	}
	
	SLIB_INLINE void IPAddress::setNone()
	{
		type = IPAddressType::None;
	}
	
	SLIB_INLINE sl_bool IPAddress::isNone() const
	{
		return type == IPAddressType::None;
	}
	
	SLIB_INLINE sl_bool IPAddress::isNotNone() const
	{
		return type != IPAddressType::None;
	}

}
