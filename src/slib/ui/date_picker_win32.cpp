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

#include "slib/ui/date_picker.h"

#include "view_win32.h"

#include <commctrl.h>

namespace slib
{

	namespace priv
	{
		namespace date_picker
		{

			class DatePickerInstance : public Win32_ViewInstance, public IDatePickerInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				sl_bool getDate(DatePicker* view, Time& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						SYSTEMTIME st;
						if (GDT_VALID == SendMessageW(handle, DTM_GETSYSTEMTIME, 0, (LPARAM)&st)) {
							_out = Windows::getTime(&st, sl_false);
							return sl_true;
						}
					}
					return sl_false;
				}

				void setDate(DatePicker* view, const Time& time) override
				{
					HWND handle = m_handle;
					if (handle) {
						SYSTEMTIME st;
						Windows::getSYSTEMTIME(time, sl_false, &st);
						SendMessageW(handle, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&st);
					}
				}

				sl_bool measureSize(DatePicker* view, UISize& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						SIZE size = { 0, 0 };
						SendMessageW(handle, (DTM_FIRST + 15) /*DTM_GETIDEALSIZE*/, 0, (LPARAM)&size);
						if (size.cx > 0 && size.cy > 0) {
							_out.x = (sl_ui_len)(size.cx);
							_out.y = (sl_ui_len)(size.cy);
							return sl_true;
						} else {
							if (m_font.isNotNull()) {
								Size size = m_font->measureText("0000-00-00");
								_out.x = (sl_ui_len)(size.x + size.y * 2);
								_out.y = (sl_ui_len)(size.y * 1.5f);
								return sl_true;
							}
						}
					}
					return sl_false;
				}

			};

			SLIB_DEFINE_OBJECT(DatePickerInstance, Win32_ViewInstance)

		}
	}

	using namespace priv::date_picker;

	Ref<ViewInstance> DatePicker::createNativeWidget(ViewInstance* parent)
	{
		SLIB_STATIC_STRING16(text, "DateTime")
		Ref<DatePickerInstance> ret = Win32_ViewInstance::create<DatePickerInstance>(this, parent, L"SysDateTimePick32", text, 0, 0);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			ret->setDate(this, m_date);
		}
		return ret;
	}

	Ptr<IDatePickerInstance> DatePicker::getDatePickerInstance()
	{
		return CastRef<DatePickerInstance>(getViewInstance());
	}

}

#endif
