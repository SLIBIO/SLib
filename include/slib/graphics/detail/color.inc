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
	
	SLIB_INLINE const Color& Color::zero() noexcept
	{
		return *(reinterpret_cast<Color const*>(&_zero));
	}
	
	SLIB_INLINE void Color::setZero() noexcept
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
	
	SLIB_INLINE void Color::setRGBA(sl_uint8 _r, sl_uint8 _g, sl_uint8 _b, sl_uint8 _a) noexcept
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	
	SLIB_INLINE void Color::setARGB(sl_uint32 v) noexcept
	{
		b = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		r = (sl_uint8)((v >> 16) & 0xFF);
		a = (sl_uint8)((v >> 24) & 0xFF);
	}
	
	SLIB_INLINE void Color::setABGR(sl_uint32 v) noexcept
	{
		r = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		b = (sl_uint8)((v >> 16) & 0xFF);
		a = (sl_uint8)((v >> 24) & 0xFF);
	}
	
	SLIB_INLINE void Color::setRGB(sl_uint8 _r, sl_uint8 _g, sl_uint8 _b) noexcept
	{
		r = _r;
		g = _g;
		b = _b;
	}
	
	SLIB_INLINE void Color::setRGB(sl_uint32 v) noexcept
	{
		b = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		r = (sl_uint8)((v >> 16) & 0xFF);
	}
	
	SLIB_INLINE void Color::setBGR(sl_uint32 v) noexcept
	{
		r = (sl_uint8)(v & 0xFF);
		g = (sl_uint8)((v >> 8) & 0xFF);
		b = (sl_uint8)((v >> 16) & 0xFF);
	}

	SLIB_INLINE Color& Color::operator=(const Color& other) noexcept
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
		return *this;
	}

}
