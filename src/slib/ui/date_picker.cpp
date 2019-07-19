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

#include "slib/ui/date_picker.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(DatePicker, View)

	DatePicker::DatePicker()
	{
		setCreatingNativeWidget(sl_true);
		m_date = Time::now();
	}

	DatePicker::~DatePicker()
	{
	}

	Time DatePicker::getDate()
	{
		if (isNativeWidget()) {
			_getDate_NW();
		}
		return m_date;
	}
	
	void DatePicker::setDate(const Time& date)
	{
		m_date = date;
		if (isNativeWidget()) {
			_setDate_NW(date);
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(DatePicker, Change, Time&)
	
	void DatePicker::dispatchChange(Time& date)
	{
		SLIB_INVOKE_EVENT_HANDLER(Change, date)
		m_date = date;
	}
	
	void DatePicker::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		if (isNativeWidget()) {
			UISize size;
			if (_measureSize_NW(size)) {
				if (flagHorizontal) {
					setLayoutWidth(size.x);
				}
				if (flagVertical) {
					setLayoutHeight(size.y);
				}
				return;
			}
		}
		
		Ref<Font> font = getFont();
		if (flagHorizontal) {
			sl_ui_pos width = getPaddingLeft() + getPaddingRight();
			if (font.isNotNull()) {
				sl_ui_pos t = (sl_ui_pos)(font->getFontHeight());
				if (t > 0) {
					width += t * 4;
				}
			}
			if (width < 0) {
				width = 0;
			}
			setLayoutWidth(width);
		}
		if (flagVertical) {
			sl_ui_pos height = 0;
			if (font.isNotNull()) {
				height = (sl_ui_pos)(font->getFontHeight() * 1.5f);
				if (height < 0) {
					height = 0;
				}
			}
			height += getPaddingTop() + getPaddingBottom();
			if (height < 0) {
				height = 0;
			}
			setLayoutHeight(height);
		}
	}
	
#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<ViewInstance> DatePicker::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}

	void DatePicker::_getDate_NW()
	{
	}

	void DatePicker::_setDate_NW(const Time& date)
	{
	}

	sl_bool DatePicker::_measureSize_NW(UISize& _out)
	{
		return sl_false;
	}

	void DatePicker::_setFont_NW(const Ref<Font>& font)
	{
	}
#endif

}
