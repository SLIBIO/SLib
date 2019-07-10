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

#include "slib/ui/radio_button.h"

#include "view_win32.h"

namespace slib
{

	namespace priv
	{
		namespace radio_button
		{

			class RadioButtonInstance : public Win32_ViewInstance
			{
			public:
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

		}
	}

	using namespace priv::radio_button;

	Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		String16 text = getText();
		UINT style = BS_RADIOBUTTON | WS_TABSTOP;
		Ref<RadioButtonInstance> ret = Win32_ViewInstance::create<RadioButtonInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getData()), style, 0);
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
