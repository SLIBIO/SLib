/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/ui/cursor.h"
#include "slib/ui/platform.h"

namespace slib
{

	class _priv_Win32_Cursor : public Cursor
	{
	public:
		HCURSOR m_hCursor;
		sl_bool m_flagDestroyOnRelease;

	public:
		_priv_Win32_Cursor()
		{
		}

		~_priv_Win32_Cursor()
		{
			if (m_flagDestroyOnRelease) {
				::DestroyCursor(m_hCursor);
			}
		}

	public:
		static Ref<_priv_Win32_Cursor> create(HCURSOR hCursor, sl_bool flagFreeOnDestroy)
		{
			Ref<_priv_Win32_Cursor> ret;
			if (hCursor) {
				ret = new _priv_Win32_Cursor;
				if (ret.isNotNull()) {
					ret->m_hCursor = hCursor;
					ret->m_flagDestroyOnRelease = flagFreeOnDestroy;
					return ret;
				}
				if (flagFreeOnDestroy) {
					::DestroyCursor(hCursor);
				}
			}
			return ret;
		}
	};

	Ref<Cursor> UIPlatform::createCursor(HCURSOR hCursor, sl_bool flagDestroyOnRelease)
	{
		return _priv_Win32_Cursor::create(hCursor, flagDestroyOnRelease);
	}

	HCURSOR UIPlatform::getCursorHandle(const Ref<Cursor>& cursor)
	{
		if (cursor.isNull()) {
			return NULL;
		}
		_priv_Win32_Cursor* c = (_priv_Win32_Cursor*)(cursor.get());
		return c->m_hCursor;
	}

	Ref<Cursor> Cursor::getArrow()
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
		return UIPlatform::createCursor(hCursor, sl_false);
	}

	Ref<Cursor> Cursor::getIBeam()
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_IBEAM);
		return UIPlatform::createCursor(hCursor, sl_false);
	}

	Ref<Cursor> Cursor::getCross()
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_CROSS);
		return UIPlatform::createCursor(hCursor, sl_false);
	}

	Ref<Cursor> Cursor::getHand()
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_HAND);
		return UIPlatform::createCursor(hCursor, sl_false);
	}

	Ref<Cursor> Cursor::getResizeLeftRight()
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_SIZEWE);
		return UIPlatform::createCursor(hCursor, sl_false);
	}

	Ref<Cursor> Cursor::getResizeUpDown()
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_SIZENS);
		return UIPlatform::createCursor(hCursor, sl_false);
	}

	void Cursor::setCurrent(const Ref<Cursor>& cursor)
	{
		if (cursor.isNull()) {
			return;
		}
		_priv_Win32_Cursor* c = (_priv_Win32_Cursor*)(cursor.get());
		::SetCursor(c->m_hCursor);
	}

	Ref<Cursor> Cursor::getCurrent()
	{
		HCURSOR hCursor = ::GetCursor();
		return UIPlatform::createCursor(hCursor);
	}

	void Cursor::show()
	{
		::ShowCursor(TRUE);
	}

	void Cursor::hide()
	{
		::ShowCursor(FALSE);
	}

}

#endif
