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
				sl_bool m_flagRichEdit;

				Color m_colorText;
				Color m_colorBackground;
				HBRUSH m_hBrushBackground;

				String16 m_hintText;
				Color m_colorHintText;
				Ref<Font> m_font;
				sl_uint32 m_heightRequested;

			public:
				EditViewInstance()
				{
					m_flagRichEdit = sl_false;
					m_hBrushBackground = NULL;
					m_colorText = Color::zero();
					m_colorBackground = Color::zero();
					m_colorHintText = Color(120, 120, 120);
					m_heightRequested = 0;
				}

				~EditViewInstance()
				{
					if (m_hBrushBackground) {
						DeleteObject(m_hBrushBackground);
					}
				}

			public:
				void setTextColor(const Color& color, sl_bool flagRedraw)
				{
					HWND hWnd = getHandle();
					if (!hWnd) {
						return;
					}
					if (m_flagRichEdit) {
						CHARFORMAT2W cf;
						Base::zeroMemory(&cf, sizeof(cf));
						cf.cbSize = sizeof(cf);
						cf.dwMask = CFM_COLOR;
						SendMessageW(hWnd, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
						cf.crTextColor = GraphicsPlatform::getColorRef(color);
						cf.dwEffects &= ~CFE_AUTOCOLOR;
						SendMessageW(hWnd, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
					} else {
						m_colorText = color;
						if (flagRedraw) {
							InvalidateRect(hWnd, NULL, TRUE);
						}
					}
				}

				void setBackgroundColor(const Color& color, sl_bool flagRedraw)
				{
					HWND hWnd = m_handle;
					if (!hWnd) {
						return;
					}
					if (m_flagRichEdit) {
						if (color.a == 0) {
							SendMessageW(hWnd, EM_SETBKGNDCOLOR, 0, (LPARAM)(0xFFFFFF));
						} else {
							SendMessageW(hWnd, EM_SETBKGNDCOLOR, 0, (LPARAM)(GraphicsPlatform::getColorRef(color)));
						}
					} else {
						if (m_colorBackground == color) {
							return;
						}
						m_colorBackground = color;
						if (m_hBrushBackground) {
							DeleteObject(m_hBrushBackground);
							m_hBrushBackground = NULL;
						}
						if (color.a != 0) {
							m_hBrushBackground = CreateSolidBrush(GraphicsPlatform::getColorRef(color));
						}
						if (flagRedraw) {
							InvalidateRect(hWnd, NULL, TRUE);
						}
					}
				}

				void setHintText(const String16& text, sl_bool flagRedraw)
				{
					HWND hWnd = m_handle;
					if (!hWnd) {
						return;
					}
					if (m_flagRichEdit) {
						m_hintText = text;
						if (flagRedraw) {
							InvalidateRect(hWnd, NULL, TRUE);
						}
					} else {
						SendMessageW(hWnd, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(text.getData()));
					}
				}
				
				void setHintTextColor(const Color& color, sl_bool flagRedraw)
				{
					HWND hWnd = m_handle;
					if (!hWnd) {
						return;
					}
					m_colorHintText = color;
					if (m_flagRichEdit) {
						if (flagRedraw) {
							InvalidateRect(hWnd, NULL, TRUE);
						}
					}
				}

				void setFont(const Ref<Font>& font, sl_bool flagRedraw)
				{
					HWND hWnd = m_handle;
					if (!hWnd) {
						return;
					}
					HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
					if (hFont) {
						SendMessageW(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
					}
					if (m_flagRichEdit) {
						m_font = font;
						if (flagRedraw) {
							InvalidateRect(hWnd, NULL, TRUE);
						}
					}
				}

				void setMultiLine(sl_bool flag)
				{
					HWND hWnd = getHandle();
					if (!hWnd) {
						return;
					}
					if (m_flagRichEdit) {
						return;
					}
					LONG old = GetWindowLongW(hWnd, GWL_STYLE);
					if (flag) {
						SetWindowLongW(hWnd, GWL_STYLE, old | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN);
					} else {
						SetWindowLongW(hWnd, GWL_STYLE, old & (~(ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN)));
					}
					SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
						SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
				}

				void setPadding(const UIEdgeInsets& inset)
				{
					HWND hWnd = getHandle();
					if (!hWnd) {
						return;
					}
					if (m_flagRichEdit) {
						RECT rc;
						GetClientRect(hWnd, &rc);
						rc.left += (LONG)(inset.left);
						rc.top += (LONG)(inset.top);
						rc.right -= (LONG)(inset.right);
						rc.bottom -= (LONG)(inset.bottom);
						SendMessageW(hWnd, EM_SETRECT, 0, (LPARAM)&rc);
					}
				}

				LRESULT processSubclassMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
				{
					HWND hWnd = m_handle;
					if (!hWnd) {
						return 0;
					}
					if (msg == WM_PAINT) {
						if (m_flagRichEdit && SendMessageW(hWnd, WM_GETTEXTLENGTH, 0, 0) == 0) {
							Win32_ViewInstance::processSubclassMessage(msg, wParam, lParam);
							HDC hDC = GetDC(hWnd);
							if (hDC) {
								HFONT hFont = GraphicsPlatform::getGdiFont(m_font.get());
								if (hFont) {
									HFONT hFontOld = (HFONT)(SelectObject(hDC, hFont));
									LONG style = GetWindowLongW(hWnd, GWL_STYLE);
									RECT rc;
									SendMessageW(hWnd, EM_GETRECT, 0, (LPARAM)&rc);
									UINT format = 0;
									if (style & ES_RIGHT) {
										format = DT_RIGHT;
									} else if (style & ES_CENTER) {
										format = DT_CENTER;
									}
									SetTextColor(hDC, GraphicsPlatform::getColorRef(m_colorHintText));
									SetBkMode(hDC, TRANSPARENT);
									DrawTextW(hDC, (LPCWSTR)(m_hintText.getData()), (int)(m_hintText.getLength()), &rc, format);
									SelectObject(hDC, hFontOld);
								}
								ReleaseDC(hWnd, hDC);
							}
							return 0;
						}
					}
					return Win32_ViewInstance::processSubclassMessage(msg, wParam, lParam);
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
						SetBkMode(hDC, OPAQUE);
						SetBkColor(hDC, GraphicsPlatform::getColorRef(m_colorBackground));
						result = hbr;
						return sl_true;
					} else {
						return sl_false;
					}
				}

				void processPostControlColor(UINT msg, HDC hDC, HBRUSH& result) override
				{
					Color c = m_colorText;
					SetTextColor(hDC, GraphicsPlatform::getColorRef(c));
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
						if (flagTextArea) {
							ret->m_flagRichEdit = sl_true;
							SendMessageW(handle, EM_SETEVENTMASK, 0, ENM_REQUESTRESIZE | ENM_CHANGE);
						}
						ret->setFont(getFont(), sl_false);
						ret->setTextColor(m_textColor, sl_false);
						ret->setBackgroundColor(getBackgroundColor(), sl_false);
						ret->setHintText(m_hintText, sl_false);
						ret->setPadding(getPadding());
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
		if (!(UI::isUiThread())) {
			return;
		}
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			m_text = Windows::getWindowText(handle);
		}
	}

	void EditView::_setText_NW(const String& text)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setText_NW, this, text));
			return;
		}
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			Windows::setWindowText(handle, text);
		}
	}

	void EditView::_setTextAlignment_NW(Alignment _align)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setTextAlignment_NW, this, _align));
			return;
		}
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			LONG style = GetWindowLongW(handle, GWL_STYLE) & (~(ES_RIGHT | ES_CENTER));
			Alignment align = _align & Alignment::HorizontalMask;
			if (align == Alignment::Center) {
				style |= ES_CENTER;
			} else if (align == Alignment::Right) {
				style |= ES_RIGHT;
			}
			SetWindowLongW(handle, GWL_STYLE, style);
			SetWindowPos(handle, NULL, 0, 0, 0, 0,
				SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
		}
	}

	void EditView::_setHintText_NW(const String& value)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintText_NW, this, value));
			return;
		}
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setHintText(value, sl_true);
		}
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setReadOnly_NW, this, flag));
			return;
		}
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			SendMessageW(handle, EM_SETREADONLY, (WPARAM)(flag ? TRUE : FALSE), 0);
		}
	}

	void EditView::_setPassword_NW(sl_bool flag)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setPassword_NW, this, flag));
			return;
		}
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			SendMessageW(handle, EM_SETPASSWORDCHAR, (WPARAM)(flag ? TRUE : FALSE), 0);
			InvalidateRect(handle, NULL, TRUE);
		}
	}

	void EditView::_setMultiLine_NW(MultiLineMode multiple)
	{
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setTextColor_NW, this, color));
			return;
		}
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setTextColor(color, sl_true); 
		}
	}

	void EditView::_setHintTextColor_NW(const Color& color)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintTextColor_NW, this, color));
			return;
		}
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setHintTextColor(color, sl_true);
		}
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
					sl_ui_len height = nLines * (sl_ui_len)(font->getFontHeight());
					height += 4;
					if (isBorder()) {
						height += 2;
					}
					return height;
				}
			}
		}
		return 0;
	}

	void EditView::_onChangePadding_NW()
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(EditView, _onChangePadding_NW, this));
			return;
		}
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setPadding(getPadding());
		}
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setFont_NW, this, font));
			return;
		}
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setFont(font, sl_true);
		}
	}

	void EditView::_setBorder_NW(sl_bool flag)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setBorder_NW, this, flag));
			return;
		}
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			LONG old = GetWindowLongW(handle, GWL_STYLE);
			if (flag) {
				SetWindowLongW(handle, GWL_STYLE, old | WS_BORDER);
			} else {
				SetWindowLongW(handle, GWL_STYLE, old & (~WS_BORDER));
			}
			SetWindowPos(handle, NULL, 0, 0, 0, 0,
				SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
		}
	}

	void EditView::_setBackgroundColor_NW(const Color& color)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setBackgroundColor_NW, this, color));
			return;
		}
		Ref<ViewInstance> _instance = getViewInstance();
		if (EditViewInstance* instance = CastInstance<EditViewInstance>(_instance.get())) {
			instance->setBackgroundColor(color, sl_true);
		}
	}

	void EditView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setScrollBarsVisible_NW, this, flagHorizontal, flagVertical));
			return;
		}
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
