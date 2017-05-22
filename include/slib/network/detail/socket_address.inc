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
	
	SLIB_INLINE SocketAddress::SocketAddress(const SocketAddress& other) noexcept = default;

	SLIB_INLINE SocketAddress& SocketAddress::operator=(const SocketAddress& other) noexcept = default;
	
	SLIB_INLINE SocketAddress::SocketAddress() noexcept
	 : port(0)
	 {}
	
	SLIB_INLINE SocketAddress::SocketAddress(sl_int32 _port) noexcept
	 : port(_port)
	 {}
	
	SLIB_INLINE SocketAddress::SocketAddress(const IPAddress& _ip, sl_int32 _port) noexcept
	 : ip(_ip), port(_port)
	 {}

	SLIB_INLINE const SocketAddress& SocketAddress::none() noexcept
	{
		return *(reinterpret_cast<SocketAddress const*>(&_none));
	}

}
