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

#include "slib/ui/check_box.h"

#include "view_win32.h"

namespace slib
{

	class _priv_Win32_CheckBoxViewInstance : public Win32_ViewInstance
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
		Ref<_priv_Win32_CheckBoxViewInstance> ret = Win32_ViewInstance::create<_priv_Win32_CheckBoxViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getData()), style, 0);
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

	UISize _priv_CheckBox_Win32_measureSize(Button* view)
	{
		Ref<Font> font = view->getFont();
		if (font.isNotNull()) {
			UISize size = font->measureText(view->getText());
			size.x += (sl_ui_len)(::GetSystemMetrics(SM_CXMENUCHECK));
			sl_ui_len cy = (sl_ui_len)(::GetSystemMetrics(SM_CYMENUCHECK));
			size.y = SLIB_MAX(size.y, cy);
			return size;
		}
		return UISize::zero();
	}

}

#endif
