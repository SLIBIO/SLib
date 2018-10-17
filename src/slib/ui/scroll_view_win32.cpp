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

#include "slib/ui/scroll_view.h"
#include "slib/ui/core.h"

#include "view_win32.h"

#define PRIV_SCROLL_LINE_SIZE 20
#define PRIV_SCROLL_WHEEL_SIZE 40

namespace slib
{

	class _priv_ScrollView : public ScrollView
	{
	public:
		friend class _priv_Win32_ScrollViewInstance;
	};
		
	class _priv_Win32_ScrollViewInstance : public Win32_ViewInstance
	{
	public:
		Color m_backgroundColor;

	public:
		sl_bool preprocessWindowMessage(MSG& msg) override
		{
			return sl_false;
		}

		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			HWND handle = getHandle();
			if (msg == WM_ERASEBKGND) {
				Ref<View> view = getView();
				if (_priv_ScrollView* sv = CastInstance<_priv_ScrollView>(view.get())) {
					Ref<View> cv = sv->getContentView();
					if (cv.isNotNull()) {
						if (cv->getWidth() >= sv->getWidth() && cv->getHeight() >= sv->getHeight()) {
							result = TRUE;
							return sl_true;
						}
					}
				}
				Color color = m_backgroundColor;
				if (color.a < 255) {
					Color c = GraphicsPlatform::getColorFromColorRef(::GetSysColor(COLOR_MENU));
					c.blend_PA_NPA(color);
					color = c;
				}
				HBRUSH hbr = ::CreateSolidBrush(RGB(color.r, color.g, color.b));
				if (hbr) {
					HDC hDC = (HDC)(wParam);
					RECT rc;
					::GetClientRect(handle, &rc);
					::FillRect(hDC, &rc, hbr);
					::DeleteObject(hbr);
					result = TRUE;
					return sl_true;
				}
			} else {
				Ref<View> view = getView();
				if (_priv_ScrollView* sv = CastInstance<_priv_ScrollView>(view.get())) {
					if (sv->isHorizontalScrolling()) {
						if (Windows::processWindowHorizontalScrollEvents(handle, msg, wParam, lParam, PRIV_SCROLL_LINE_SIZE, PRIV_SCROLL_WHEEL_SIZE)) {
							_refreshContentPosition(sv, sl_true);
							return sl_true;
						}
					}
					if (sv->isVerticalScrolling()) {
						if (Windows::processWindowVerticalScrollEvents(handle, msg, wParam, lParam, PRIV_SCROLL_LINE_SIZE, PRIV_SCROLL_WHEEL_SIZE)) {
							_refreshContentPosition(sv, sl_true);
							return sl_true;
						}
					}
				}
			}
			return sl_false;
		}

		sl_bool processCommand(SHORT code, LRESULT& result) override
		{
			return sl_false;
		}

		sl_bool processNotify(NMHDR* nmhdr, LRESULT& result) override
		{
			return sl_false;
		}

		void _setContentView(const Ref<View>& viewChild, _priv_ScrollView* viewParent)
		{
			if (viewChild.isNotNull()) {
				viewChild->attachToNewInstance(this);
			}
			_refreshContentSize(viewParent);
			_refreshContentPosition(viewParent, sl_false);
		}

		void _refreshContentSize(_priv_ScrollView* view)
		{
			HWND handle = getHandle();
			if (handle) {
				Sizei sizeContent = view->getContentSize();
				Sizei sizeParent = view->getSize();
				if (view->isHorizontalScrolling()) {
					Windows::setWindowHorizontalScrollParam(handle, 0, sizeContent.x - 1, sizeParent.x);
				}
				if (view->isVerticalScrolling()) {
					Windows::setWindowVerticalScrollParam(handle, 0, sizeContent.y - 1, sizeParent.y);
				}
				_refreshContentPosition(view, sl_false);
			}
		}

		void _refreshContentPosition(_priv_ScrollView* view, sl_bool flagFromEvent)
		{
			HWND handle = getHandle();
			if (handle) {
				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.fMask = SIF_POS;
				::GetScrollInfo(handle, SB_HORZ, &si);
				int x = si.nPos;
				::GetScrollInfo(handle, SB_VERT, &si);
				int y = si.nPos;
				Ref<View> viewContent = view->getContentView();
				if (viewContent.isNotNull()) {
					viewContent->setLocation((sl_ui_pos)(-x), (sl_ui_pos)(-y));
				}
				if (flagFromEvent) {
					view->_onScroll_NW((sl_scroll_pos)x, (sl_scroll_pos)y);
				}
			}
		}
	};

	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		DWORD style = WS_CLIPCHILDREN;
		if (isHorizontalScrolling()) {
			style = WS_HSCROLL;
		}
		if (isVerticalScrolling()) {
			style |= WS_VSCROLL;
		}

		DWORD styleEx = WS_EX_CONTROLPARENT;

		if (isBorder()) {
			style |= WS_BORDER;
		}
		Ref<_priv_Win32_ScrollViewInstance> ret = Win32_ViewInstance::create<_priv_Win32_ScrollViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
		if (ret.isNotNull()) {
			ret->_setContentView(m_viewContent, (_priv_ScrollView*)this);
			ret->m_backgroundColor = getBackgroundColor();
		}
		return ret;
	}

	void ScrollView::_refreshContentSize_NW()
	{
		Ref<ViewInstance> instance = getViewInstance();
		if (_priv_Win32_ScrollViewInstance* _instance = CastInstance<_priv_Win32_ScrollViewInstance>(instance.get())) {
			_instance->_refreshContentSize((_priv_ScrollView*)this);
		}
	}

	void ScrollView::_setContentView_NW(const Ref<View>& view)
	{
		Ref<ViewInstance> instance = getViewInstance();
		if (_priv_Win32_ScrollViewInstance* _instance = CastInstance<_priv_Win32_ScrollViewInstance>(instance.get())) {
			_instance->_setContentView(view, (_priv_ScrollView*)this);
		}
	}

	void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			SCROLLINFO si;
			Base::zeroMemory(&si, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			if (isHorizontalScrolling()) {
				si.nPos = (int)x;
				::SetScrollInfo(handle, SB_HORZ, &si, TRUE);
			}
			if (isVerticalScrolling()) {
				si.nPos = (int)y;
				::SetScrollInfo(handle, SB_VERT, &si, TRUE);
			}
			Ref<ViewInstance> instance = getViewInstance();
			if (_priv_Win32_ScrollViewInstance* _instance = CastInstance<_priv_Win32_ScrollViewInstance>(instance.get())) {
				_instance->_refreshContentPosition((_priv_ScrollView*)this, sl_false);
			}
		}
	}

	ScrollPoint ScrollView::_getScrollPosition_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			SCROLLINFO si;
			Base::zeroMemory(&si, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			::GetScrollInfo(handle, SB_HORZ, &si);
			int x = si.nPos;
			::GetScrollInfo(handle, SB_VERT, &si);
			int y = si.nPos;
			return ScrollPoint((sl_scroll_pos)x, (sl_scroll_pos)y);
		}
		return ScrollPoint::zero();
	}

	ScrollPoint ScrollView::_getScrollRange_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			SCROLLINFO si;
			Base::zeroMemory(&si, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE;
			::GetScrollInfo(handle, SB_HORZ, &si);
			int w = si.nMax;
			::GetScrollInfo(handle, SB_VERT, &si);
			int h = si.nMax;
			if (w < 0) {
				w = 0;
			}
			if (h < 0) {
				h = 0;
			}
			return ScrollPoint((sl_scroll_pos)w, (sl_scroll_pos)h);
		}
		return ScrollPoint::zero();
	}

	void ScrollView::_setBorder_NW(sl_bool flag)
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

	void ScrollView::_setBackgroundColor_NW(const Color& color)
	{
		Ref<ViewInstance> instance = getViewInstance();
		if (_priv_Win32_ScrollViewInstance* _instance = CastInstance<_priv_Win32_ScrollViewInstance>(instance.get())) {
			_instance->m_backgroundColor = color;
		}
	}

}

#endif
