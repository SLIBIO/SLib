#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/button.h"

#include "ui_view_win32.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_ButtonViewInstance : public Win32_ViewInstance
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

Ref<ViewInstance> Button::createInstance(ViewInstance* parent)
{
	String16 text = m_text;
	DWORD style = WS_TABSTOP;
	if (m_flagDefaultButton) {
		style |= BS_DEFPUSHBUTTON;
	}
	Ref<_Win32_ButtonViewInstance> ret = Win32_ViewInstance::create<_Win32_ButtonViewInstance>(this, parent, L"BUTTON", (LPCWSTR)(text.getBuf()), style, 0);
	if (ret.isNotNull()) {
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		HFONT hFont = UIPlatform::getGdiFont(font.get(), fontInstance);
		if (hFont) {
			::SendMessageW(ret->getHandle(), WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		m_fontInstance = fontInstance;
	}
	return Ref<ViewInstance>::from(ret);
}

String Button::getText()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = Windows::getWindowText(handle);
	}
	return m_text;
}

void Button::setText(const String& _text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Windows::setWindowText(handle, _text);
	}
	m_text = _text;
}

void Button::setFont(const Ref<Font>& _font)
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

void Button::setDefaultButton(sl_bool flag)
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
	m_flagDefaultButton = flag;
}
SLIB_UI_NAMESPACE_END

#endif
