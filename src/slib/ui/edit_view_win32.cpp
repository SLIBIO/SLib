/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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
			
			class EditViewHelper : public EditView
			{
			public:
				void onChange(HWND handle)
				{
					String textOld = m_text;
					String text = Windows::getWindowText(handle);
					String textNew = text;
					dispatchChange(&textNew);
					if (text != textNew) {
						Windows::setWindowText(handle, textNew);
					}
					if (textOld.isEmpty() || textNew.isEmpty()) {
						InvalidateRect(handle, NULL, TRUE);
					}
				}

			};

			static void SetGravity(HWND handle, const Alignment& gravity)
			{
				LONG style = 0;
				Alignment align = gravity & Alignment::HorizontalMask;
				if (align == Alignment::Center) {
					style = ES_CENTER;
				} else if (align == Alignment::Right) {
					style = ES_RIGHT;
				}
				Windows::removeAndAddWindowStyle(handle, ES_RIGHT | ES_CENTER, style);
			}
			
			class EditViewInstance : public Win32_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT

			protected:
				Color m_colorText;
				Color m_colorBackground;
				HBRUSH m_hBrushBackground;

			public:
				EditViewInstance()
				{
					m_hBrushBackground = NULL;
					m_colorText = Color::zero();
					m_colorBackground = Color::zero();
				}

				~EditViewInstance()
				{
					if (m_hBrushBackground) {
						DeleteObject(m_hBrushBackground);
					}
				}

			public:
				Ref<EditView> getView()
				{
					return CastRef<EditView>(Win32_ViewInstance::getView());
				}

				sl_bool getText(EditView* view, String& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						_out = Windows::getWindowText(handle);
						return sl_true;
					}
					return sl_false;
				}

				void setText(EditView* view, const String& text) override
				{
					Win32_ViewInstance::setText(text);
				}

				void setGravity(EditView* view, const Alignment& gravity) override
				{
					HWND handle = m_handle;
					if (handle) {
						SetGravity(handle, gravity);
					}
				}

				void setTextColor(const Color& color)
				{
					m_colorText = color;
				}

				void setTextColor(EditView* view, const Color& color) override
				{
					m_colorText = color;
					HWND handle = m_handle;
					if (handle) {
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setHintText(EditView* view, const String& text) override
				{
					HWND handle = m_handle;
					if (handle) {
						String16 s = String16::from(text);
						SendMessageW(handle, 0x1501 /*EM_SETCUEBANNER*/, FALSE, (LPARAM)(LPCWSTR)(s.getData()));
					}
				}

				void setHintGravity(EditView* view, const Alignment& gravity) override
				{
				}

				void setHintTextColor(EditView* view, const Color& color) override
				{
				}

				void setHintFont(EditView* view, const Ref<Font>& font) override
				{
				}

				void setReadOnly(EditView* view, sl_bool flag) override
				{
					HWND handle = m_handle;
					if (handle) {
						SendMessageW(handle, EM_SETREADONLY, (WPARAM)(flag ? TRUE : FALSE), 0);
					}
				}

				void setPassword(EditView* view, sl_bool flag) override
				{
					HWND handle = m_handle;
					if (handle) {
						SendMessageW(handle, EM_SETPASSWORDCHAR, (WPARAM)(flag ? TRUE : FALSE), 0);
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
					Windows::setWindowStyle(m_handle, ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN, mode != MultiLineMode::Single);
				}

				sl_ui_len measureHeight(EditView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						int nLines = 1;
						if (view->getMultiLine() != MultiLineMode::Single) {
							nLines = (int)(SendMessageW(handle, EM_GETLINECOUNT, 0, 0));
							if (nLines < 1) {
								nLines = 1;
							}
						}
						Ref<Font> font = m_font;
						if (font.isNotNull()) {
							sl_ui_len height = nLines * (sl_ui_len)(font->getFontHeight());
							height += 4;
							if (view->isBorder()) {
								height += 2;
							}
							return height;
						}
					}
					return 0;
				}

				void setBackgroundColor(const Color& color)
				{
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
				}

				void setBackgroundColor(View* view, const Color& color) override
				{
					HWND handle = m_handle;
					if (handle) {
						setBackgroundColor(color);
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				sl_bool processCommand(SHORT code, LRESULT& result) override
				{
					switch (code) {
					case EN_CHANGE:
						{
							Ref<EditViewHelper> helper = CastRef<EditViewHelper>(getView());
							if (helper.isNotNull()) {
								helper->onChange(m_handle);
								result = 0;
								return sl_true;
							}
							break;
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

			class TextAreaInstance: public Win32_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				String16 m_hintText;
				Alignment m_hintGravity;
				Color m_hintTextColor;
				Ref<Font> m_hintFont;
				sl_uint32 m_heightRequested;

			public:
				TextAreaInstance()
				{
					m_hintTextColor = Color(120, 120, 120);
					m_heightRequested = 0;
				}

			public:
				Ref<TextArea> getView()
				{
					return CastRef<TextArea>(Win32_ViewInstance::getView());
				}

				sl_bool getText(EditView* view, String& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						_out = Windows::getWindowText(handle);
						return sl_true;
					}
					return sl_false;
				}

				void setText(EditView* view, const String& text) override
				{
					Win32_ViewInstance::setText(text);
				}

				void setGravity(EditView* view, const Alignment& gravity) override
				{
					HWND handle = m_handle;
					if (handle) {
						SetGravity(handle, gravity);
					}
				}

				void setTextColor(EditView* view, const Color& color) override
				{
					HWND handle = m_handle;
					if (handle) {
						CHARFORMAT2W cf;
						Base::zeroMemory(&cf, sizeof(cf));
						cf.cbSize = sizeof(cf);
						cf.dwMask = CFM_COLOR;
						SendMessageW(handle, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
						cf.crTextColor = GraphicsPlatform::getColorRef(color);
						cf.dwEffects &= ~CFE_AUTOCOLOR;
						SendMessageW(handle, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
					}
				}

				void setHintText(EditView* view, const String& text) override
				{
					m_hintText = String16::from(text);
					HWND handle = m_handle;
					if (handle) {
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setHintGravity(EditView* view, const Alignment& gravity) override
				{
					m_hintGravity = gravity;
					HWND handle = m_handle;
					if (handle) {
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setHintTextColor(EditView* view, const Color& color) override
				{
					m_hintTextColor = color;
					HWND handle = m_handle;
					if (handle) {
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setHintFont(EditView* view, const Ref<Font>& font) override
				{
					m_hintFont = font;
					HWND handle = m_handle;
					if (handle) {
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setReadOnly(EditView* view, sl_bool flag) override
				{
					HWND handle = m_handle;
					if (handle) {
						SendMessageW(handle, EM_SETREADONLY, (WPARAM)(flag ? TRUE : FALSE), 0);
					}
				}

				void setPassword(EditView* view, sl_bool flag) override
				{
				}

				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
				}

				sl_ui_len measureHeight(EditView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						SendMessageW(handle, EM_REQUESTRESIZE, 0, 0);
						sl_ui_len height = m_heightRequested;
						if (height > 0) {
							if (view->isBorder()) {
								height += 8;
							}
							return height;
						}
					}
					return 0;
				}

				void setBackgroundColor(View* view, const Color& color) override
				{
					HWND handle = m_handle;
					if (handle) {
						if (color.a == 0) {
							SendMessageW(handle, EM_SETBKGNDCOLOR, 0, (LPARAM)(0xFFFFFF));
						} else {
							SendMessageW(handle, EM_SETBKGNDCOLOR, 0, (LPARAM)(GraphicsPlatform::getColorRef(color)));
						}
					}
				}

				void setPadding(View* view, const UIEdgeInsets& inset) override
				{
					HWND handle = getHandle();
					if (handle) {
						RECT rc;
						GetClientRect(handle, &rc);
						rc.left += (LONG)(inset.left);
						rc.top += (LONG)(inset.top);
						rc.right -= (LONG)(inset.right);
						rc.bottom -= (LONG)(inset.bottom);
						SendMessageW(handle, EM_SETRECT, 0, (LPARAM)&rc);
					}
				}

				LRESULT processSubclassMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
				{
					HWND handle = m_handle;
					if (!handle) {
						return 0;
					}
					if (msg == WM_PAINT) {
						if (m_hintText.isNotEmpty() && SendMessageW(handle, WM_GETTEXTLENGTH, 0, 0) == 0) {
							Win32_ViewInstance::processSubclassMessage(msg, wParam, lParam);
							HDC hDC = GetDC(handle);
							if (hDC) {
								HFONT hFont = GraphicsPlatform::getGdiFont(m_hintFont.get());
								if (hFont) {
									HFONT hFontOld = (HFONT)(SelectObject(hDC, hFont));
									RECT rc;
									SendMessageW(handle, EM_GETRECT, 0, (LPARAM)&rc);
									UINT format = DT_EXPANDTABS | DT_WORDBREAK;
									Alignment halign = m_hintGravity & Alignment::HorizontalMask;
									Alignment valign = m_hintGravity & Alignment::VerticalMask;
									if (halign == Alignment::Right) {
										format |= DT_RIGHT;
									} else if (halign == Alignment::Center) {
										format |= DT_CENTER;
									}
									if (valign != Alignment::Top) {
										RECT rcCalc = rc;
										DrawTextW(hDC, (LPCWSTR)(m_hintText.getData()), (int)(m_hintText.getLength()), &rcCalc, format | DT_CALCRECT);
										if (valign == Alignment::Bottom) {
											rc.top += rc.bottom - rcCalc.bottom;
										} else {
											rc.top += (rc.bottom - rcCalc.bottom) / 2;
										}
									}
									SetTextColor(hDC, GraphicsPlatform::getColorRef(m_hintTextColor));
									SetBkMode(hDC, TRANSPARENT);
									DrawTextW(hDC, (LPCWSTR)(m_hintText.getData()), (int)(m_hintText.getLength()), &rc, format);
									SelectObject(hDC, hFontOld);
								}
								ReleaseDC(handle, hDC);
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
							Ref<EditViewHelper> helper = CastRef<EditViewHelper>(getView());
							if (helper.isNotNull()) {
								helper->onChange(m_handle);
								result = 0;
								return sl_true;
							}
							break;
						}
					}
					return sl_false;
				}

				sl_bool processNotify(NMHDR* nmhdr, LRESULT& result) override
				{
					switch (nmhdr->code) {
					case EN_REQUESTRESIZE:
						{
							REQRESIZE* req = (REQRESIZE*)nmhdr;
							m_heightRequested = req->rc.bottom - req->rc.top;
							return sl_true;
						}
					}
					return sl_false;
				}

			};

			SLIB_DEFINE_OBJECT(TextAreaInstance, Win32_ViewInstance)

		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		int style = WS_TABSTOP | ES_AUTOHSCROLL;
		Alignment align = m_gravity & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			style |= ES_CENTER;
		} else if (align == Alignment::Right) {
			style |= ES_RIGHT;
		}
		if (m_multiLine != MultiLineMode::Single) {
			style |= ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
		}
		if (m_flagReadOnly) {
			style |= ES_READONLY;
		}
		if (m_flagPassword) {
			style |= ES_PASSWORD;
		}
		Ref<EditViewInstance> ret = Win32_ViewInstance::create<EditViewInstance>(this, parent, L"Edit", getText(), style, 0);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			ret->setTextColor(m_textColor);
			ret->setBackgroundColor(getBackgroundColor());
			ret->setHintText(this, m_hintText);
			return ret;
		}
		return sl_null;
	}

	Ptr<IEditViewInstance> EditView::getEditViewInstance()
	{
		return CastRef<EditViewInstance>(getViewInstance());
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		LPCWSTR className;
		HMODULE hModule = LoadLibraryW(L"Msftedit.dll");
		if (hModule) {
			className = MSFTEDIT_CLASS;
		} else {
			LoadLibraryW(L"Riched20.dll");
			className = RICHEDIT_CLASSW;
		}
		int style = WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
		Alignment align = m_gravity & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			style |= ES_CENTER;
		} else if (align == Alignment::Right) {
			style |= ES_RIGHT;
		}
		if (m_multiLine != MultiLineMode::WordWrap && m_multiLine != MultiLineMode::BreakWord) {
			style |= ES_AUTOHSCROLL;
		}
		if (m_flagReadOnly) {
			style |= ES_READONLY;
		}
		Ref<TextAreaInstance> ret = Win32_ViewInstance::create<TextAreaInstance>(this, parent, className, getText(), style, 0);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			ret->m_hintText = String16::from(m_hintText);
			ret->m_hintGravity = m_hintGravity;
			ret->m_hintTextColor = m_hintTextColor;
			ret->m_hintFont = getHintFont();
			SendMessageW(handle, EM_SETEVENTMASK, 0, ENM_REQUESTRESIZE | ENM_CHANGE);
			ret->setTextColor(this, m_textColor);
			ret->setBackgroundColor(this, getBackgroundColor());
			ret->setPadding(this, getPadding());
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IEditViewInstance> TextArea::getEditViewInstance()
	{
		return CastRef<TextAreaInstance>(getViewInstance());
	}

}

#endif
