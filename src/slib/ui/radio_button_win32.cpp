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

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/radio_button.h"

#include "view_win32.h"

namespace slib
{

	class _priv_Win32_RadioButtonViewInstance : public Win32_ViewInstance
	{
	public:
		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			return sl_false;
		}

		sl_bool processCommand(SHORT code, LRESULT& result) override
		{
			if (code == BN_CLICKED) {
				::SendMessageW(getHandle(), BM_SETCHECK, BST_CHECKED, 0);
				onClick();
				return sl_true;
			}
			return sl_false;
		}
	};

	Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		String16 text = getText();
		UINT style = BS_RADIOBUTTON | WS_TABSTOP;
		Ref<_priv_Win32_RadioButtonViewInstance> ret = Win32_ViewInstance::create<_priv_Win32_RadioButtonViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getData()), style, 0);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			if (isChecked()) {
				::SendMessageW(handle, BM_SETCHECK, BST_CHECKED, 0);
			} else {
				::SendMessageW(handle, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			Ref<Font> font = getFont();
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
		}
		return ret;
	}

}

#endif
