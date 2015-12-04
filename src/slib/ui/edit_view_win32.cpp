#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/edit_view.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_EditViewInstance : public Win32_ViewInstance
{
	SLIB_DECLARE_OBJECT(_Win32_EditViewInstance, Win32_ViewInstance)
public:
	Color m_colorText;
	Color m_colorBackground;
	HBRUSH m_hBrushBackground;

	_Win32_EditViewInstance()
	{
		m_hBrushBackground = NULL;
		m_colorText = Color::zero();
		m_colorBackground = Color::zero();
	}

	~_Win32_EditViewInstance()
	{
		if (m_hBrushBackground) {
			::DeleteObject(m_hBrushBackground);
		}
	}

	void setTextColor(const Color& color)
	{
		HWND handle = getHandle();
		m_colorText = color;
		::InvalidateRect(handle, NULL, TRUE);
	}

	void setBackgroundColor(const Color& color)
	{
		HWND handle = getHandle();
		m_colorBackground = color;
		if (m_hBrushBackground) {
			::DeleteObject(m_hBrushBackground);
			m_hBrushBackground = NULL;
		}
		if (color.getAlpha() != 0) {
			m_hBrushBackground = ::CreateSolidBrush(UIPlatform::getColorRef(color));
		}
		::InvalidateRect(handle, NULL, TRUE);
	}

	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		switch (code) {
		case EN_CHANGE:
			{
				Ref<View> _view = getView();
				if (EditView::checkInstance(_view)) {
					EditView* view = (EditView*)(_view.get());
					String text = Windows::getWindowText(m_handle);
					String textNew = view->onChange(text);
					if (text != textNew) {
						Windows::setWindowText(m_handle, textNew);
					}
				}
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool processControlColor(UINT msg, HDC hDC, HBRUSH& result)
	{
		HBRUSH hbr = m_hBrushBackground;
		if (hbr) {
			::SetBkMode(hDC, OPAQUE);
			::SetBkColor(hDC, UIPlatform::getColorRef(m_colorBackground));
			result = hbr;
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void processPostControlColor(UINT msg, HDC hDC, HBRUSH& result)
	{
		Color c = m_colorText;
		::SetTextColor(hDC, UIPlatform::getColorRef(c));
	}
};

class _EditView : public EditView
{
public:
	/*
		type
			0 - EditView
			1 - PasswordView
			2 - TextArea
	*/
	Ref<ViewInstance> __createInstance(ViewInstance* parent, int type)
	{
		int style = WS_TABSTOP;
		if (m_flagBorder) {
			style |= WS_BORDER;
		}
		sl_uint32 align = m_textAlignment & alignHorizontalMask;
		if (align == alignCenter) {
			style |= ES_CENTER;
		} else if (align == alignRight) {
			style |= ES_RIGHT;
		}
		if (type == 0) {
			if (m_flagMultiLine) {
				style |= ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
			}
		} else if (type == 1) {
			style |= ES_PASSWORD;
		} else if (type == 2) {
			style |= WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN;
		}
		if (m_flagReadOnly) {
			style |= ES_READONLY;
		}		
		style |= ES_AUTOHSCROLL;
		String16 text = m_text;
		Ref<_Win32_EditViewInstance> ret = Win32_ViewInstance::create<_Win32_EditViewInstance>(this, parent, L"EDIT", (LPCWSTR)(text.getBuf()), style, 0);
		if (ret.isNotNull()) {
			
			HWND handle = ret->getHandle();
			
			Ref<Font> font = m_font;
			Ref<FontInstance> fontInstance;
			HFONT hFont = UIPlatform::getGdiFont(font.get(), fontInstance);
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
			m_fontInstance = fontInstance;

			String16 hintText = m_hintText;
			::SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(hintText.getBuf()));
			ret->setTextColor(m_textColor);
			ret->setBackgroundColor(m_backgroundColor);
		}
		return Ref<ViewInstance>::from(ret);
	}

};

Ref<ViewInstance> EditView::createInstance(ViewInstance* parent)
{
	return ((_EditView*)this)->__createInstance(parent, 0);
}

Ref<ViewInstance> PasswordView::createInstance(ViewInstance* parent)
{
	return ((_EditView*)this)->__createInstance(parent, 1);
}

Ref<ViewInstance> TextArea::createInstance(ViewInstance* parent)
{
	return ((_EditView*)this)->__createInstance(parent, 2);
}

String EditView::getText()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = Windows::getWindowText(handle);
	}
	return m_text;
}

void EditView::setText(const String& _text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Windows::setWindowText(handle, _text);
	}
	m_text = _text;
}

sl_bool EditView::isBorder()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		m_flagBorder = (style & WS_BORDER) ? sl_true : sl_false;
	}
	return m_flagBorder;
}

void EditView::setBorder(sl_bool flag)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG old = ::GetWindowLongW(handle, GWL_STYLE);
		if (flag) {
			::SetWindowLongW(handle, GWL_STYLE, old | WS_BORDER);
		} else {
			::SetWindowLongW(handle, GWL_STYLE, old & (~WS_BORDER));
		}
		::SetWindowPos(handle, NULL, 0, 0, 0, 0
			, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}
	m_flagBorder = flag;
}

Alignment EditView::getTextAlignment()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		if (style & ES_CENTER) {
			m_textAlignment = alignCenter;
		} else if (style & ES_RIGHT) {
			m_textAlignment = alignRight;
		} else {
			m_textAlignment = alignLeft;
		}
	}
	return m_textAlignment;
}

void EditView::setTextAlignment(Alignment _align)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE) & (~(ES_RIGHT | ES_CENTER));
		sl_uint32 align = _align & alignHorizontalMask;
		if (align == alignCenter) {
			style |= ES_CENTER;
		} else if (align == alignRight) {
			style |= ES_RIGHT;
		}
		::SetWindowLongW(handle, GWL_STYLE, style);
		::SetWindowPos(handle, NULL, 0, 0, 0, 0
			, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}
	m_textAlignment = _align;
}

String EditView::getHintText()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		WCHAR buf[1024] = { 0 };
		::SendMessageW(handle, 0x1502 /*EM_GETCUEBANNER*/, (WPARAM)buf, 1024);
		m_hintText = String16(buf, 1024);
	}
	return m_hintText;
}

void EditView::setHintText(const String& _value)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		String16 value = _value;
		::SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(value.getBuf()));
	}
	m_hintText = _value;
}

sl_bool EditView::isReadOnly()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		m_flagReadOnly = (style & ES_READONLY) ? sl_true : sl_false;
	}
	return m_flagReadOnly;
}

void EditView::setReadOnly(sl_bool flag)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		::SendMessageW(handle, EM_SETREADONLY, (WPARAM)(flag ? TRUE : FALSE), 0);
	}
	m_flagReadOnly = flag;
}

sl_bool EditView::isMultiLine()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		m_flagMultiLine = (style & ES_MULTILINE) ? sl_true : sl_false;
	}
	return m_flagMultiLine;
}

void EditView::setMultiLine(sl_bool flag)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG old = ::GetWindowLongW(handle, GWL_STYLE);
		if (flag) {
			::SetWindowLongW(handle, GWL_STYLE, old | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN);
		} else {
			::SetWindowLongW(handle, GWL_STYLE, old & (~(ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN)));
		}
		::SetWindowPos(handle, NULL, 0, 0, 0, 0
			, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}
	m_flagMultiLine = flag;
}

Color EditView::getTextColor()
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_EditViewInstance::checkInstance(_instance)) {
		_Win32_EditViewInstance* instance = ((_Win32_EditViewInstance*)(_instance.get()));
		m_textColor = instance->m_colorText;
	}
	return m_textColor;
}

void EditView::setTextColor(const Color& color)
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_EditViewInstance::checkInstance(_instance)) {
		_Win32_EditViewInstance* instance = ((_Win32_EditViewInstance*)(_instance.get()));
		instance->setTextColor(color); 
	}
	m_textColor = color;
}

Color EditView::getBackgroundColor()
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_EditViewInstance::checkInstance(_instance)) {
		_Win32_EditViewInstance* instance = ((_Win32_EditViewInstance*)(_instance.get()));
		m_backgroundColor = instance->m_colorBackground;
	}
	return m_backgroundColor;
}

void EditView::setBackgroundColor(const Color& color)
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_EditViewInstance::checkInstance(_instance)) {
		_Win32_EditViewInstance* instance = ((_Win32_EditViewInstance*)(_instance.get()));
		instance->setBackgroundColor(color);
	}
	m_backgroundColor = color;
}

void EditView::setFont(const Ref<Font>& _font)
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
SLIB_UI_NAMESPACE_END

#endif
