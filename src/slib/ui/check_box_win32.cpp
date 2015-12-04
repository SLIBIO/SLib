#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/check_box.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_CheckBoxViewInstance : public Win32_ViewInstance
{
public:
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		if (code == BN_CLICKED) {
			onClick();
			return sl_true;
		}
		return sl_false;
	}
};

Ref<ViewInstance> CheckBox::createInstance(ViewInstance* parent)
{
	String16 text = m_text;
	UINT style = BS_AUTOCHECKBOX | WS_TABSTOP;
	Ref<_Win32_CheckBoxViewInstance> ret = Win32_ViewInstance::create<_Win32_CheckBoxViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getBuf()), style, 0);
	if (ret.isNotNull()) {
		HWND handle = ret->getHandle();
		if (m_flagChecked) {
			::SendMessageW(handle, BM_SETCHECK, BST_CHECKED, 0);
		} else {
			::SendMessageW(handle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		HFONT hFont = UIPlatform::getGdiFont(font.get(), fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		m_fontInstance = fontInstance;
	}
	return Ref<ViewInstance>::from(ret);
}

String CheckBox::getText()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = Windows::getWindowText(handle);
	}
	return m_text;
}

void CheckBox::setText(const String& _text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Windows::setWindowText(handle, _text);
	}
	m_text = _text;
}

void CheckBox::setFont(const Ref<Font>& _font)
{
	Ref<Font> font = _font;
	Ref<FontInstance> fontInstance;
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		HFONT hFont = UIPlatform::getGdiFont(font.get(), fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
	}
	m_font = font;
	m_fontInstance = fontInstance;
}

sl_bool CheckBox::isChecked()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LRESULT lr = ::SendMessageW(handle, BM_GETCHECK, 0, 0);
		m_flagChecked = (lr == BST_CHECKED);
	}
	return m_flagChecked;
}

void CheckBox::setChecked(sl_bool flag)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		if (flag) {
			::SendMessageW(handle, BM_SETCHECK, BST_CHECKED, 0);
		} else {
			::SendMessageW(handle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
	}
	m_flagChecked = flag;
}
SLIB_UI_NAMESPACE_END

#endif
