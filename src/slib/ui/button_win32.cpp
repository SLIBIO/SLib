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

#include "slib/ui/button.h"

#include "button_win32.h"

#include <commctrl.h>

namespace slib
{

	namespace priv
	{
		namespace button
		{

			SLIB_DEFINE_OBJECT(ButtonInstance, Win32_ViewInstance)

			ButtonInstance::ButtonInstance()
			{
			}

			ButtonInstance::~ButtonInstance()
			{
			}

			sl_bool ButtonInstance::processCommand(SHORT code, LRESULT& result)
			{
				/*
					Click events are processed by subclassing
				-------------------------------------------------
				if (code == BN_CLICKED) {
					onClick();
					return sl_true;
				}
				*/
				return sl_false;
			}

			void ButtonInstance::setPadding(View* view, const UIEdgeInsets& padding)
			{
				HWND handle = m_handle;
				if (handle) {
					RECT rc;
					rc.left = (LONG)(padding.left);
					rc.top = (LONG)(padding.top);
					rc.right = (LONG)(padding.top);
					rc.bottom = (LONG)(padding.bottom);
					SendMessageW(handle, BCM_SETTEXTMARGIN, 0, (LPARAM)&rc);
				}
			}

			void ButtonInstance::setText(Button* view, const String& text)
			{
				Win32_ViewInstance::setText(text);
			}

			void ButtonInstance::setDefaultButton(Button* view, sl_bool flag)
			{
				Windows::setWindowStyle(m_handle, BS_DEFPUSHBUTTON, flag);
			}

			sl_bool ButtonInstance::measureSize(Button* view, UISize& _out)
			{
				HWND handle = m_handle;
				if (handle) {
					SIZE size = { 0, 0 };
					SendMessageW(handle, BCM_GETIDEALSIZE, 0, (LPARAM)&size);
					_out.x = (sl_ui_len)(size.cx);
					_out.y = (sl_ui_len)(size.cy);
					return sl_true;
				}
				return sl_false;
			}

		}
	}

	using namespace priv::button;

	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* parent)
	{
		DWORD style = WS_TABSTOP;
		if (m_flagDefaultButton) {
			style |= BS_DEFPUSHBUTTON;
		}
		Ref<ButtonInstance> ret = Win32_ViewInstance::create<ButtonInstance>(this, parent, L"BUTTON", getText(), style, 0);
		if (ret.isNotNull()) {
			ret->setPadding(this, getPadding());
			return ret;
		}
		return sl_null;
	}

	Ptr<IButtonInstance> Button::getButtonInstance()
	{
		return CastRef<ButtonInstance>(getViewInstance());
	}

}

#endif
