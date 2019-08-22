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

#include "slib/ui/select_view.h"

#include "view_win32.h"

namespace slib
{

	namespace priv
	{
		namespace select_view
		{

			class SelectViewHelper : public SelectView
			{
			public:
				void _applyItemsCount(HWND hWnd)
				{
					ObjectLocker lock(this);
					sl_uint32 nOrig = (sl_uint32)(::SendMessageW(hWnd, CB_GETCOUNT, 0, 0));
					sl_uint32 nNew = (sl_uint32)(m_titles.getCount());
					if (nOrig == nNew) {
						return;
					}
					if (nOrig > nNew) {
						if (nNew > 0) {
							for (sl_uint32 i = nOrig; i > nNew; i--) {
								::SendMessageW(hWnd, CB_DELETESTRING, (WPARAM)(i - 1), 0);
							}
						} else {
							::SendMessageW(hWnd, CB_RESETCONTENT, 0, 0);
						}
					} else {
						for (sl_uint32 i = nOrig; i < nNew; i++) {
							String16 s = m_titles.getValueAt(i);
							::SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)(s.getData()));
						}
					}
				}

				void _copyItems(HWND hWnd)
				{
					::SendMessageW(hWnd, CB_RESETCONTENT, 0, 0);
					_applyItemsCount(hWnd);
					sl_uint32 n = (sl_uint32)(m_titles.getCount());
					if (m_indexSelected >= n) {
						m_indexSelected = 0;
					}
					if (n > 0) {
						::SendMessageW(hWnd, CB_SETCURSEL, (WPARAM)m_indexSelected, 0);
					}
				}

				void _setItemTitle(HWND hWnd, sl_uint32 index, const String& title)
				{
					String16 s = title;
					::SendMessageW(hWnd, CB_DELETESTRING, (WPARAM)index, 0);
					::SendMessageW(hWnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)(s.getData()));
				}

				sl_uint32 _getSelectedIndex(HWND hWnd)
				{
					return (sl_uint32)(::SendMessageW(hWnd, CB_GETCURSEL, 0, 0));
				}

				void _onSelectItem(HWND hWnd)
				{
					dispatchSelectItem(_getSelectedIndex(hWnd));
				}

			};

			class SelectViewInstance : public Win32_ViewInstance
			{
			public:
				sl_bool processCommand(SHORT code, LRESULT& result) override
				{
					if (code == CBN_SELCHANGE) {
						Ref<View> view = getView();
						if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
							_view->_onSelectItem(m_handle);
						}
						return sl_true;
					}
					return sl_false;
				}
			};

		}
	}

	using namespace priv::select_view;

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		UINT style = CBS_DROPDOWNLIST | WS_TABSTOP;
		Ref<SelectViewInstance> ret = Win32_ViewInstance::create<SelectViewInstance>(this, parent, L"COMBOBOX", sl_null, style, 0);
		
		if (ret.isNotNull()) {

			HWND handle = ret->getHandle();

			Ref<Font> font = getFont();
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}

			((SelectViewHelper*)this)->_copyItems(handle);
		}
		return ret;
	}
	
	void SelectView::_select_NW(sl_uint32 index)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			::SendMessageW(handle, CB_SETCURSEL, (WPARAM)index, 0);
		}
	}

	void SelectView::_refreshItemsCount_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((SelectViewHelper*)this)->_applyItemsCount(handle);
		}
	}

	void SelectView::_refreshItemsContent_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((SelectViewHelper*)this)->_copyItems(handle);
		}
	}

	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((SelectViewHelper*)this)->_setItemTitle(handle, index, title);
		}
	}

	void SelectView::_setFont_NW(const Ref<Font>& font)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
		}
	}

}

#endif
