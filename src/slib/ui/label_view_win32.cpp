#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/label_view.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_LabelViewInstance : public Win32_ViewInstance
{
	SLIB_DECLARE_OBJECT

public:
	Color m_colorText;
	Color m_colorBackground;
	HBRUSH m_hBrushBackground;

public:
	_Win32_LabelViewInstance()
	{
		m_hBrushBackground = NULL;
		m_colorText = Color::zero();
		m_colorBackground = Color::zero();
	}

	~_Win32_LabelViewInstance()
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
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		return sl_false;
	}

    // override
	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		switch (code) {
		case STN_CLICKED:
			{
				onClick();
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

SLIB_DEFINE_OBJECT(_Win32_LabelViewInstance, Win32_ViewInstance)

Ref<ViewInstance> LabelView::createInstance(ViewInstance* parent)
{
	int style = SS_NOTIFY;
	if (m_flagBorder) {
		style |= WS_BORDER;
	}
	Alignments align = m_textAlignment & Alignment::HorizontalMask;
	if (align == Alignment::Center) {
		style |= SS_CENTER;
	} else if (align == Alignment::Right) {
		style |= SS_RIGHT;
	}
	String16 text = m_text;
	Ref<_Win32_LabelViewInstance> ret = Win32_ViewInstance::create<_Win32_LabelViewInstance>(this, parent, L"STATIC", (LPCWSTR)(text.getData()), style, 0);
	if (ret.isNotNull()) {

		HWND handle = ret->getHandle();

		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		m_fontInstance = fontInstance;

		ret->setTextColor(m_textColor);
		ret->setBackgroundColor(m_backgroundColor);
	}
	return ret;
}

String LabelView::getText()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = Windows::getWindowText(handle);
	}
	return m_text;
}

void LabelView::setText(const String& text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Windows::setWindowText(handle, text);
	}
	m_text = text;
}

sl_bool LabelView::isBorder()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		m_flagBorder = (style & WS_BORDER) ? sl_true : sl_false;
	}
	return m_flagBorder;
}

void LabelView::setBorder(sl_bool flag)
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

Alignments LabelView::getTextAlignment()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE);
		if (style & SS_CENTER) {
			m_textAlignment = Alignment::Center;
		} else if (style & SS_RIGHT) {
			m_textAlignment = Alignment::Right;
		} else {
			m_textAlignment = Alignment::Left;
		}
	}
	return m_textAlignment;
}

void LabelView::setTextAlignment(Alignments _align)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LONG style = ::GetWindowLongW(handle, GWL_STYLE) & (~(SS_RIGHT | SS_CENTER));
		Alignments align = _align & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			style |= SS_CENTER;
		} else if (align == Alignment::Right) {
			style |= SS_RIGHT;
		}
		::SetWindowLongW(handle, GWL_STYLE, style);
		::SetWindowPos(handle, NULL, 0, 0, 0, 0
			, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}
	m_textAlignment = _align;
}

Color LabelView::getTextColor()
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_LabelViewInstance::checkInstance(_instance.ptr)) {
		_Win32_LabelViewInstance* instance = ((_Win32_LabelViewInstance*)(_instance.ptr));
		m_textColor = instance->m_colorText;
	}
	return m_textColor;
}

void LabelView::setTextColor(const Color& color)
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_LabelViewInstance::checkInstance(_instance.ptr)) {
		_Win32_LabelViewInstance* instance = ((_Win32_LabelViewInstance*)(_instance.ptr));
		instance->setTextColor(color); 
	}
	m_textColor = color;
}

Color LabelView::getBackgroundColor()
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_LabelViewInstance::checkInstance(_instance.ptr)) {
		_Win32_LabelViewInstance* instance = ((_Win32_LabelViewInstance*)(_instance.ptr));
		m_backgroundColor = instance->m_colorBackground;
	}
	return m_backgroundColor;
}

void LabelView::setBackgroundColor(const Color& color)
{
	Ref<ViewInstance> _instance = getViewInstance();
	if (_Win32_LabelViewInstance::checkInstance(_instance.ptr)) {
		_Win32_LabelViewInstance* instance = ((_Win32_LabelViewInstance*)(_instance.ptr));
		instance->setBackgroundColor(color);
	}
	m_backgroundColor = color;
}

void LabelView::setFont(const Ref<Font>& font)
{
	Ref<FontInstance> fontInstance;
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
	}
	m_font = font;
	m_fontInstance = fontInstance;
}

SLIB_UI_NAMESPACE_END

#endif
