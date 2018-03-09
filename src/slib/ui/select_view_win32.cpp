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

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/ui/select_view.h"

#include "view_win32.h"

namespace slib
{

	class _priv_SelectView : public SelectView
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

	class _priv_Win32_SelectViewInstance : public Win32_ViewInstance
	{
	public:
		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			return sl_false;
		}

		sl_bool processCommand(SHORT code, LRESULT& result) override
		{
			if (code == CBN_SELCHANGE) {
				Ref<View> view = getView();
				if (_priv_SelectView* _view = CastInstance<_priv_SelectView>(view.get())) {
					_view->_onSelectItem(m_handle);
				}
				return sl_true;
			}
			return sl_false;
		}
	};

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		UINT style = CBS_DROPDOWNLIST | WS_TABSTOP;
		Ref<_priv_Win32_SelectViewInstance> ret = Win32_ViewInstance::create<_priv_Win32_SelectViewInstance>(this, parent, L"COMBOBOX", L"", style, 0);
		
		if (ret.isNotNull()) {

			HWND handle = ret->getHandle();

			Ref<Font> font = getFont();
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
			}

			((_priv_SelectView*)this)->_copyItems(handle);
		}
		return ret;
	}

	void SelectView::_getSelectedIndex_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			m_indexSelected = ((_priv_SelectView*)this)->_getSelectedIndex(handle);
		}
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
			((_priv_SelectView*)this)->_applyItemsCount(handle);
		}
	}

	void SelectView::_refreshItemsContent_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_priv_SelectView*)this)->_copyItems(handle);
		}
	}

	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_priv_SelectView*)this)->_setItemTitle(handle, index, title);
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
