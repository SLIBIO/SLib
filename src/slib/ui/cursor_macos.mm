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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/cursor.h"
#include "slib/ui/platform.h"

namespace slib
{

	class _priv_macOS_Cursor : public Cursor
	{
	public:
		NSCursor* m_cursor;
		
	public:
		static Ref<_priv_macOS_Cursor> create(NSCursor* cursor)
		{
			Ref<_priv_macOS_Cursor> ret;
			if (cursor != nil) {
				ret = new _priv_macOS_Cursor;
				if (ret.isNotNull()) {
					ret->m_cursor = cursor;
				}
			}
			return ret;
		}
	};

	Ref<Cursor> UIPlatform::createCursor(NSCursor* cursor)
	{
		return _priv_macOS_Cursor::create(cursor);
	}

	NSCursor* UIPlatform::getCursorHandle(const Ref<Cursor>& cursor)
	{
		if (cursor.isNull()) {
			return nil;
		}
		_priv_macOS_Cursor* c = (_priv_macOS_Cursor*)(cursor.get());
		return c->m_cursor;
	}

	Ref<Cursor> Cursor::getArrow()
	{
		return UIPlatform::createCursor([NSCursor arrowCursor]);
	}

	Ref<Cursor> Cursor::getIBeam()
	{
		return UIPlatform::createCursor([NSCursor IBeamCursor]);
	}

	Ref<Cursor> Cursor::getCross()
	{
		return UIPlatform::createCursor([NSCursor crosshairCursor]);
	}

	Ref<Cursor> Cursor::getHand()
	{
		return UIPlatform::createCursor([NSCursor pointingHandCursor]);
	}

	Ref<Cursor> Cursor::getResizeLeftRight()
	{
		return UIPlatform::createCursor([NSCursor resizeLeftRightCursor]);
	}

	Ref<Cursor> Cursor::getResizeUpDown()
	{
		return UIPlatform::createCursor([NSCursor resizeUpDownCursor]);
	}

	void Cursor::setCurrent(const Ref<Cursor>& cursor)
	{
		if (cursor.isNull()) {
			return;
		}
		_priv_macOS_Cursor* c = (_priv_macOS_Cursor*)(cursor.get());
		[c->m_cursor set];
	}

	Ref<Cursor> Cursor::getCurrent()
	{
		return UIPlatform::createCursor([NSCursor currentCursor]);
	}

	void Cursor::show()
	{
		[NSCursor unhide];
	}

	void Cursor::hide()
	{
		[NSCursor hide];
	}

}

#endif
