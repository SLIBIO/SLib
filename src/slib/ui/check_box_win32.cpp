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

#include "button_win32.h"

namespace slib
{

	namespace priv
	{
		namespace button
		{

			SLIB_DEFINE_OBJECT(CheckBoxInstance, ButtonInstance)

			CheckBoxInstance::CheckBoxInstance()
			{
			}

			CheckBoxInstance::~CheckBoxInstance()
			{
			}

			sl_bool CheckBoxInstance::getChecked(CheckBox* view, sl_bool& _out)
			{
				HWND handle = m_handle;
				if (handle) {
					LRESULT lr = SendMessageW(handle, BM_GETCHECK, 0, 0);
					_out = (lr == BST_CHECKED);
					return sl_true;
				}
				return sl_false;
			}

			void CheckBoxInstance::setChecked(CheckBox* view, sl_bool flag)
			{
				HWND handle = m_handle;
				if (handle) {
					if (flag) {
						SendMessageW(handle, BM_SETCHECK, BST_CHECKED, 0);
					} else {
						SendMessageW(handle, BM_SETCHECK, BST_UNCHECKED, 0);
					}
				}
			}

			sl_bool CheckBoxInstance::measureSize(Button* view, UISize& _out)
			{
				if (m_font.isNotNull()) {
					sl_ui_len cx = (sl_ui_len)(GetSystemMetrics(SM_CXMENUCHECK));
					sl_ui_len cy = (sl_ui_len)(GetSystemMetrics(SM_CYMENUCHECK));
					String16 text = m_text;
					if (text.isNotEmpty()) {
						text = SLIB_UNICODE("  ") + text;
					}
					UISize size = m_font->measureText16(text);
					size.x += cx;
					size.y = SLIB_MAX(size.y, cy);
					_out = size;
					return sl_true;
				}
				return sl_false;
			}

		}
	}

	using namespace priv::button;

	Ref<ViewInstance> CheckBox::createNativeWidget(ViewInstance* parent)
	{
		UINT style = BS_AUTOCHECKBOX | WS_TABSTOP;
		Ref<CheckBoxInstance> ret = Win32_ViewInstance::create<CheckBoxInstance>(this, parent, L"BUTTON", getText(), style, 0);
		if (ret.isNotNull()) {
			ret->setPadding(this, getPadding());
			ret->setChecked(this, m_flagChecked);
			return ret;
		}
		return sl_null;
	}

	Ptr<ICheckBoxInstance> CheckBox::getCheckBoxInstance()
	{
		return CastRef<CheckBoxInstance>(getViewInstance());
	}

}

#endif
