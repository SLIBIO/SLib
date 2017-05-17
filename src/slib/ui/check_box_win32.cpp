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

#include "slib/ui/check_box.h"

#include "view_win32.h"

namespace slib
{

	class _Win32_CheckBoxViewInstance : public Win32_ViewInstance
	{
	public:
		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			return sl_false;
		}

		sl_bool processCommand(SHORT code, LRESULT& result) override
		{
			if (code == BN_CLICKED) {
				onClick();
				return sl_true;
			}
			return sl_false;
		}
	};

	Ref<ViewInstance> CheckBox::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}
		String16 text = getText();
		UINT style = BS_AUTOCHECKBOX | WS_TABSTOP;
		Ref<_Win32_CheckBoxViewInstance> ret = Win32_ViewInstance::create<_Win32_CheckBoxViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getData()), style, 0);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			if (m_flagChecked) {
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

	void CheckBox::_getChecked_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			LRESULT lr = ::SendMessageW(handle, BM_GETCHECK, 0, 0);
			m_flagChecked = (lr == BST_CHECKED);
		}
	}

	void CheckBox::_setChecked_NW(sl_bool flag)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			if (flag) {
				::SendMessageW(handle, BM_SETCHECK, BST_CHECKED, 0);
			} else {
				::SendMessageW(handle, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}
	}

}

#endif
