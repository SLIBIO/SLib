/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/cursor.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(Cursor, Object)

	Cursor::Cursor()
	{
	}

	Cursor::~Cursor()
	{
	}
	
#if !defined(SLIB_PLATFORM_IS_WIN32) && !defined(SLIB_PLATFORM_IS_OSX)
	
	Ref<Cursor> Cursor::getArrow()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getIBeam()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getCross()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getHand()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getResizeLeftRight()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getResizeUpDown()
	{
		return sl_null;
	}
	
	void Cursor::setCurrent(const Ref<Cursor>& cursor)
	{
	}
	
	Ref<Cursor> Cursor::getCurrent()
	{
		return sl_null;
	}
	
	void Cursor::show()
	{
	}
	
	void Cursor::hide()
	{
	}
	
#endif
}
