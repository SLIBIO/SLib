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

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/cursor.h"
#include "slib/ui/platform.h"

namespace slib
{

	namespace priv
	{
		namespace cursor
		{

			class NativeCursorImpl : public Cursor
			{
			public:
				HCURSOR m_hCursor;
				sl_bool m_flagDestroyOnRelease;

			public:
				NativeCursorImpl()
				{
				}

				~NativeCursorImpl()
				{
					if (m_flagDestroyOnRelease) {
						::DestroyCursor(m_hCursor);
					}
				}

			public:
				static Ref<NativeCursorImpl> create(HCURSOR hCursor, sl_bool flagFreeOnDestroy)
				{
					Ref<NativeCursorImpl> ret;
					if (hCursor) {
						ret = new NativeCursorImpl;
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

		}
	}

	using namespace priv::cursor;

	Ref<Cursor> UIPlatform::createCursor(HCURSOR hCursor, sl_bool flagDestroyOnRelease)
	{
		return NativeCursorImpl::create(hCursor, flagDestroyOnRelease);
	}

	HCURSOR UIPlatform::getCursorHandle(const Ref<Cursor>& cursor)
	{
		if (cursor.isNull()) {
			return NULL;
		}
		NativeCursorImpl* c = (NativeCursorImpl*)(cursor.get());
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
		NativeCursorImpl* c = (NativeCursorImpl*)(cursor.get());
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
