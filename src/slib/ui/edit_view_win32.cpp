#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/edit_view.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN

class _Win32_EditViewInstance : public Win32_ViewInstance
{
	SLIB_DECLARE_OBJECT
public:
	Color m_colorText;
	Color m_colorBackground;
	HBRUSH m_hBrushBackground;
    
public:
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

public:
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
		if (color.a != 0) {
			m_hBrushBackground = ::CreateSolidBrush(UIPlatform::getColorRef(color));
		}
		::InvalidateRect(handle, NULL, TRUE);
	}

    // override
	sl_bool preprocessWindowMessage(MSG& msg)
	{
		if (msg.message == WM_KEYUP) {
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
		switch (code) {
		case EN_CHANGE:
			{
				Ref<View> _view = getView();
				if (EditView::checkInstance(_view.ptr)) {
					EditView* view = (EditView*)(_view.ptr);
					String text = Windows::getWindowText(m_handle);
					String textNew = view->dispatchChange(text);
					if (text != textNew) {
						Windows::setWindowText(m_handle, textNew);
					}
				}
				return sl_true;
			}
		}
		return sl_false;
	}

    // override
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

    // override
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
		if (isBorder()) {
			style |= WS_BORDER;
		}
		Alignment align = m_textAlignment & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			style |= ES_CENTER;
		} else if (align == Alignment::Right) {
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
		Ref<_Win32_EditViewInstance> ret = Win32_ViewInstance::create<_Win32_EditViewInstance>(this, parent, L"EDIT", (LPCWSTR)(text.getData()), style, 0);
		if (ret.isNotNull()) {
			
			HWND handle = ret->getHandle();
			
			Ref<Font> font = getFont();
			Ref<FontInstance> fontInstance;
			HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
			_setFontInstance(fontInstance);

			String16 hintText = m_hintText;
			::SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(hintText.getData()));
			ret->setTextColor(m_textColor);
			ret->setBackgroundColor(getBackgroundColor());
		}
		return ret;
	}

};

SLIB_DEFINE_OBJECT(_Win32_EditViewInstance, Win32_ViewInstance)

Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
{
	return ((_EditView*)this)->__createInstance(parent, 0);
}

Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* parent)
{
	return ((_EditView*)this)->__createInstance(parent, 1);
}

Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
{
	return ((_EditView*)this)->__createInstance(parent, 2);
}

void EditView::_getText_NW()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = Windows::getWindowText(handle);
	}
}

void EditView::_setText_NW(const String& text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Windows::setWindowText(handle, text);
	}
}

void EditView::_getTextAlignment_NW()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		if (style & ES_CENTER) {
			m_textAlignment = Alignment::Center;
		} else if (style & ES_RIGHT) {
			m_textAlignment = Alignment::Right;
		} else {
			m_textAlignment = Alignment::Left;
		}
	}
}

void EditView::_setTextAlignment_NW(Alignment _align)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE) & (~(ES_RIGHT | ES_CENTER));
		Alignment align = _align & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			style |= ES_CENTER;
		} else if (align == Alignment::Right) {
			style |= ES_RIGHT;
		}
		::SetWindowLongW(handle, GWL_STYLE, style);
		::SetWindowPos(handle, NULL, 0, 0, 0, 0
			, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}
}

void EditView::_getHintText_NW()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		WCHAR buf[1024] = { 0 };
		::SendMessageW(handle, 0x1502 /*EM_GETCUEBANNER*/, (WPARAM)buf, 1024);
		m_hintText = String16(buf, 1024);
	}
}

void EditView::_setHintText_NW(const String& _value)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		String16 value = _value;
		::SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(value.getData()));
	}
}

void EditView::_isReadOnly_NW()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		m_flagReadOnly = (style & ES_READONLY) ? sl_true : sl_false;
	}
}

void EditView::_setReadOnly_NW(sl_bool flag)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		::SendMessageW(handle, EM_SETREADONLY, (WPARAM)(flag ? TRUE : FALSE), 0);
	}
}

void EditView::_isMultiLine_NW()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		m_flagMultiLine = (style & ES_MULTILINE) ? sl_true : sl_false;
	}
}

void EditView::_setMultiLine_NW(sl_bool flag)
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
}

void EditView::_setTextColor_NW(const Color& color)
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_EditViewInstance::checkInstance(_instance.ptr)) {
		_Win32_EditViewInstance* instance = ((_Win32_EditViewInstance*)(_instance.ptr));
		instance->setTextColor(color); 
	}
}

void EditView::_setFont_NW(const Ref<Font>& font)
{
	Ref<FontInstance> fontInstance;
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
	}
	_setFontInstance(fontInstance);
}

void EditView::_setBorder_NW(sl_bool flag)
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
}

void EditView::_setBackgroundColor_NW(const Color& color)
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_EditViewInstance::checkInstance(_instance.ptr)) {
		_Win32_EditViewInstance* instance = ((_Win32_EditViewInstance*)(_instance.ptr));
		instance->setBackgroundColor(color);
	}
}

SLIB_UI_NAMESPACE_END

#endif
