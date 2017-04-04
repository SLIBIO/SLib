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

#include "slib/ui/button.h"

#include "view_win32.h"

namespace slib
{

	class _Win32_ButtonViewInstance : public Win32_ViewInstance
	{
	public:
		// override
		sl_bool preprocessWindowMessage(MSG& msg)
		{
			if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
				LRESULT lr;
				sl_bool flag = Win32_ViewInstance::processWindowMessage(msg.message, msg.wParam, msg.lParam, lr);
				if (flag) {
					return sl_true;
				}
			}
			return sl_false;
		}

		// override
		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
		{
			return sl_false;
		}

		// override
		sl_bool processCommand(SHORT code, LRESULT& result)
		{
			if (code == BN_CLICKED) {
				onClick();
				return sl_true;
			}
			return sl_false;
		}
	};

	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}
		String16 text = m_text;
		DWORD style = WS_TABSTOP;
		if (m_flagDefaultButton) {
			style |= BS_DEFPUSHBUTTON;
		}
		Ref<_Win32_ButtonViewInstance> ret = Win32_ViewInstance::create<_Win32_ButtonViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getData()), style, 0);
		if (ret.isNotNull()) {
			Ref<Font> font = getFont();
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(ret->getHandle(), WM_SETFONT, (WPARAM)hFont, TRUE);
			}
		}
		return ret;
	}

	void Button::_setText_NW(const String& text)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			Windows::setWindowText(handle, text);
		}
	}

	void Button::_setDefaultButton_NW(sl_bool flag)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			LONG old = ::GetWindowLongW(handle, GWL_STYLE);
			if (flag) {
				::SetWindowLongW(handle, GWL_STYLE, old | BS_DEFPUSHBUTTON);
			} else {
				::SetWindowLongW(handle, GWL_STYLE, old & (~(BS_DEFPUSHBUTTON)));
			}
			::SetWindowPos(handle, NULL, 0, 0, 0, 0
				, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
		}
	}

	void Button::_setFont_NW(const Ref<Font>& font)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
		}
	}

}

#endif
