#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/radio_button.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN

class _Win32_RadioButtonViewInstance : public Win32_ViewInstance
{
public:
    // override
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		return sl_false;
	}

    // override
	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		if (code == BN_CLICKED) {
			::SendMessageW(getHandle(), BM_SETCHECK, BST_CHECKED, 0);
			onClick();
			return sl_true;
		}
		return sl_false;
	}
};

Ref<ViewInstance> RadioButton::createInstance(ViewInstance* parent)
{
	String16 text = m_text;
	UINT style = BS_RADIOBUTTON | WS_TABSTOP;
	Ref<_Win32_RadioButtonViewInstance> ret = Win32_ViewInstance::create<_Win32_RadioButtonViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getBuf()), style, 0);
	if (ret.isNotNull()) {
		HWND handle = ret->getHandle();
		if (m_flagSelected) {
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
	return ret;
}

String RadioButton::getText()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = Windows::getWindowText(handle);
	}
	return m_text;
}

void RadioButton::setText(const String& text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Windows::setWindowText(handle, text);
	}
	m_text = text;
}

void RadioButton::setFont(const Ref<Font>& font)
{
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

sl_bool RadioButton::isChecked()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LRESULT lr = ::SendMessageW(handle, BM_GETCHECK, 0, 0);
		m_flagSelected = (lr == BST_CHECKED);
	}
	return m_flagSelected;
}

void RadioButton::setChecked(sl_bool flag)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		if (flag) {
			::SendMessageW(handle, BM_SETCHECK, BST_CHECKED, 0);
		} else {
			::SendMessageW(handle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
	}
	m_flagSelected = flag;
}

SLIB_UI_NAMESPACE_END

#endif
