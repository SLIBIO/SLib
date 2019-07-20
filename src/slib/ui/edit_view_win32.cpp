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

#include "slib/ui/edit_view.h"

#include "view_win32.h"

#include <Richedit.h>

namespace slib
{

	namespace priv
	{
		namespace edit_view
		{

			class EditViewInstance : public Win32_ViewInstance
			{
				SLIB_DECLARE_OBJECT
			public:
				Color m_colorText;
				Color m_colorBackground;
				HBRUSH m_hBrushBackground;

				sl_uint32 m_heightRequested;

			public:
				EditViewInstance()
				{
					m_hBrushBackground = NULL;
					m_colorText = Color::zero();
					m_colorBackground = Color::zero();
					m_heightRequested = 0;
				}

				~EditViewInstance()
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

				sl_bool processCommand(SHORT code, LRESULT& result) override
				{
					switch (code) {
					case EN_CHANGE:
						{
							Ref<View> _view = getView();
							if (EditView* view = CastInstance<EditView>(_view.get())) {
								String text = Windows::getWindowText(m_handle);
								String textNew = text;
								view->dispatchChange(&textNew);
								if (text != textNew) {
									Windows::setWindowText(m_handle, textNew);
								}
							}
							return sl_true;
						}
					}
					return sl_false;
				}

				sl_bool processNotify(NMHDR* nmhdr, LRESULT& result) override
				{
					switch (nmhdr->code) {
					case EN_REQUESTRESIZE:
						{
							Ref<View> _view = getView();
							if (EditView* view = CastInstance<EditView>(_view.get())) {
								REQRESIZE* req = (REQRESIZE*)nmhdr;
								m_heightRequested = req->rc.bottom - req->rc.top;
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

			SLIB_DEFINE_OBJECT(EditViewInstance, Win32_ViewInstance)

			class EditViewHelper : public EditView
			{
			public:
				Ref<ViewInstance> _createInstance(ViewInstance* parent, sl_bool flagTextArea)
				{
					Win32_UI_Shared* shared = Win32_UI_Shared::get();
					if (!shared) {
						return sl_null;
					}

					LPCWSTR className = L"Edit";

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
					if (flagTextArea) {
						HMODULE hModule = LoadLibraryW(L"Msftedit.dll");
						if (hModule) {
							className = MSFTEDIT_CLASS;
						} else {
							LoadLibraryW(L"Riched20.dll");
							className = RICHEDIT_CLASSW;
						}
						style |= ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
						if (isVerticalScrollBarVisible()) {
							style |= WS_VSCROLL;
						}
						if (m_multiLine != MultiLineMode::WordWrap && m_multiLine != MultiLineMode::BreakWord) {
							style |= ES_AUTOHSCROLL;
							if (isHorizontalScrollBarVisible()) {
								style |= WS_HSCROLL;
							}
						}
					} else {
						if (m_multiLine != MultiLineMode::Single) {
							style |= ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
						}
						style |= ES_AUTOHSCROLL;
					}
					if (m_flagReadOnly) {
						style |= ES_READONLY;
					}
					if (m_flagPassword) {
						style |= ES_PASSWORD;
					}
					String16 text = m_text;
					Ref<EditViewInstance> ret = Win32_ViewInstance::create<EditViewInstance>(this, parent, className, (LPCWSTR)(text.getData()), style, 0);
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

						if (flagTextArea) {
							SendMessageW(handle, EM_SETEVENTMASK, 0, ENM_REQUESTRESIZE | ENM_CHANGE);
						}
					}
					return ret;
				}

			};

		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		return ((EditViewHelper*)this)->_createInstance(parent, sl_false);
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		return ((EditViewHelper*)this)->_createInstance(parent, sl_true);
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

	void EditView::_setPassword_NW(sl_bool flag)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::SendMessageW(handle, EM_SETPASSWORDCHAR, (WPARAM)(flag ? TRUE : FALSE), 0);
			::InvalidateRect(handle, NULL, TRUE);
		}
	}

	void EditView::_setMultiLine_NW(MultiLineMode multiple)
	{
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setTextColor(color); 
		}
	}

	void EditView::_setHintTextColor_NW(const Color& color)
	{
	}

	sl_ui_len EditView::_measureHeight_NW()
	{
		if (IsInstanceOf<TextArea>(this)) {
			Ref<ViewInstance> _instance = getViewInstance();
			if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
				SendMessageW(instance->getHandle(), EM_REQUESTRESIZE, 0, 0);
				sl_ui_len height = instance->m_heightRequested;
				if (height > 0) {
					if (isBorder()) {
						height += 8;
					}
					return height;
				}
			}
		} else {
			HWND handle = UIPlatform::getViewHandle(this);
			if (handle) {
				int nLines = 1;
				if (getMultiLine() != MultiLineMode::Single) {
					nLines = (int)(SendMessageW(handle, EM_GETLINECOUNT, 0, 0));
					if (nLines < 1) {
						nLines = 1;
					}
				}
				
				Ref<Font> font = getFont();
				if (font.isNotNull()) {
					sl_ui_len height = (sl_ui_len)(nLines * font->getFontHeight());
					height += 2;
					if (isBorder()) {
						height += 2;
					}
					return height;
				}
			}
		}
		return 0;
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
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setBackgroundColor(color);
		}
	}

	void EditView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			WINAPI_ShowScrollBar func = Windows::getAPI_ShowScrollBar();
			if (func) {
				func(handle, SB_HORZ, flagHorizontal);
				func(handle, SB_VERT, flagVertical);
			}
		}
	}

}

#endif
