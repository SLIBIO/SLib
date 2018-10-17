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

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/edit_view.h"

#include "view_win32.h"

namespace slib
{

	class Win32_EditViewInstance : public Win32_ViewInstance
	{
		SLIB_DECLARE_OBJECT
	public:
		Color m_colorText;
		Color m_colorBackground;
		HBRUSH m_hBrushBackground;
		sl_bool m_flagMultiLine;
		
	public:
		Win32_EditViewInstance()
		{
			m_hBrushBackground = NULL;
			m_colorText = Color::zero();
			m_colorBackground = Color::zero();
			m_flagMultiLine = sl_false;
		}

		~Win32_EditViewInstance()
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

		void setMultiLine(sl_bool flag)
		{
			HWND handle = m_handle;
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

		void setBackgroundColor(const Color& color)
		{
			HWND handle = getHandle();
			m_colorBackground = color;
			if (m_hBrushBackground) {
				::DeleteObject(m_hBrushBackground);
				m_hBrushBackground = NULL;
			}
			if (color.a != 0) {
				m_hBrushBackground = ::CreateSolidBrush(GraphicsPlatform::getColorRef(color));
			}
			::InvalidateRect(handle, NULL, TRUE);
		}

		sl_bool preprocessWindowMessage(MSG& msg)
		{
			if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
				if (msg.wParam == VK_ESCAPE) {
					return onEventKey(msg.message == WM_KEYDOWN, msg.wParam, msg.lParam);
				}
			}
			if (m_flagMultiLine) {
				return sl_false;
			}
			return Win32_ViewInstance::preprocessWindowMessage(msg);
		}

		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			return sl_false;
		}

		sl_bool processCommand(SHORT code, LRESULT& result) override
		{
			switch (code) {
			case EN_CHANGE:
				{
					Ref<View> _view = getView();
					if (EditView* view = CastInstance<EditView>(_view.get())) {
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

		sl_bool processControlColor(UINT msg, HDC hDC, HBRUSH& result) override
		{
			HBRUSH hbr = m_hBrushBackground;
			if (hbr) {
				::SetBkMode(hDC, OPAQUE);
				::SetBkColor(hDC, GraphicsPlatform::getColorRef(m_colorBackground));
				result = hbr;
				return sl_true;
			} else {
				return sl_false;
			}
		}

		void processPostControlColor(UINT msg, HDC hDC, HBRUSH& result) override
		{
			Color c = m_colorText;
			::SetTextColor(hDC, GraphicsPlatform::getColorRef(c));
		}
	};

	class EditView_Impl : public EditView
	{
	public:
		/*
			type
				0 - EditView
				1 - PasswordView
				2 - TextArea
		*/
		Ref<ViewInstance> _createInstance(ViewInstance* parent, int type)
		{
			Win32_UI_Shared* shared = Win32_UI_Shared::get();
			if (!shared) {
				return sl_null;
			}

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
			Ref<Win32_EditViewInstance> ret = Win32_ViewInstance::create<Win32_EditViewInstance>(this, parent, L"EDIT", (LPCWSTR)(text.getData()), style, 0);
			if (ret.isNotNull()) {
				
				HWND handle = ret->getHandle();
				
				Ref<Font> font = getFont();
				HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
				if (hFont) {
					::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
				}

				String16 hintText = m_hintText;
				::SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(hintText.getData()));
				ret->setTextColor(m_textColor);
				ret->setBackgroundColor(getBackgroundColor());
				ret->m_flagMultiLine = m_flagMultiLine;
			}
			return ret;
		}

	};

	SLIB_DEFINE_OBJECT(Win32_EditViewInstance, Win32_ViewInstance)

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 0);
	}

	Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 1);
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		return ((EditView_Impl*)this)->_createInstance(parent, 2);
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

	void EditView::_setHintText_NW(const String& _value)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			String16 value = _value;
			::SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(value.getData()));
		}
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::SendMessageW(handle, EM_SETREADONLY, (WPARAM)(flag ? TRUE : FALSE), 0);
		}
	}

	void EditView::_setMultiLine_NW(sl_bool flag)
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (Win32_EditViewInstance* instance = CastInstance<Win32_EditViewInstance>(_instance.get())) {
			instance->setMultiLine(flag);
		}
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (Win32_EditViewInstance* instance = CastInstance<Win32_EditViewInstance>(_instance.get())) {
			instance->setTextColor(color); 
		}
	}

	void EditView::_setHintTextColor_NW(const Color& color)
	{
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
		}
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
		if (Win32_EditViewInstance* instance = CastInstance<Win32_EditViewInstance>(_instance.get())) {
			instance->setBackgroundColor(color);
		}
	}

}

#endif
