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

#include "slib/ui/label_view.h"

#include "view_win32.h"

namespace slib
{

	class _priv_Win32_LabelViewInstance : public Win32_ViewInstance
	{
		SLIB_DECLARE_OBJECT

	public:
		Color m_colorText;
		Color m_colorBackground;
		HBRUSH m_hBrushBackground;

	public:
		_priv_Win32_LabelViewInstance()
		{
			m_hBrushBackground = NULL;
			m_colorText = Color::zero();
			m_colorBackground = Color::zero();
		}

		~_priv_Win32_LabelViewInstance()
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
				m_hBrushBackground = ::CreateSolidBrush(GraphicsPlatform::getColorRef(color));
			}
			::InvalidateRect(handle, NULL, TRUE);
		}

		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			return sl_false;
		}

		sl_bool processCommand(SHORT code, LRESULT& result) override
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

	SLIB_DEFINE_OBJECT(_priv_Win32_LabelViewInstance, Win32_ViewInstance)

	Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		int style = SS_NOTIFY;
		if (isBorder()) {
			style |= WS_BORDER;
		}
		Alignment align = m_textAlignment & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			style |= SS_CENTER;
		} else if (align == Alignment::Right) {
			style |= SS_RIGHT;
		}
		String16 text = m_text;
		Ref<_priv_Win32_LabelViewInstance> ret = Win32_ViewInstance::create<_priv_Win32_LabelViewInstance>(this, parent, L"STATIC", (LPCWSTR)(text.getData()), style, 0);
		if (ret.isNotNull()) {

			HWND handle = ret->getHandle();

			Ref<Font> font = getFont();
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}

			ret->setTextColor(m_textColor);
			ret->setBackgroundColor(getBackgroundColor());
		}
		return ret;
	}

	void LabelView::_setText_NW(const String& text)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			Windows::setWindowText(handle, text);
		}
	}

	void LabelView::_setTextAlignment_NW(Alignment _align)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			LONG style = ::GetWindowLongW(handle, GWL_STYLE) & (~(SS_RIGHT | SS_CENTER));
			Alignment align = _align & Alignment::HorizontalMask;
			if (align == Alignment::Center) {
				style |= SS_CENTER;
			} else if (align == Alignment::Right) {
				style |= SS_RIGHT;
			}
			::SetWindowLongW(handle, GWL_STYLE, style);
			::SetWindowPos(handle, NULL, 0, 0, 0, 0
				, SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
		}
	}

	void LabelView::_setTextColor_NW(const Color& color)
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_priv_Win32_LabelViewInstance* instance = CastInstance<_priv_Win32_LabelViewInstance>(_instance.get())) {
			instance->setTextColor(color); 
		}
	}

	void LabelView::_setFont_NW(const Ref<Font>& font)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
		}
	}

	void LabelView::_setBorder_NW(sl_bool flag)
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

	void LabelView::_setBackgroundColor_NW(const Color& color)
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_priv_Win32_LabelViewInstance* instance = CastInstance<_priv_Win32_LabelViewInstance>(_instance.get())) {
			instance->setBackgroundColor(color);
		}
	}

}

#endif
