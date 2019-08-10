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

#include "slib/ui/scroll_view.h"
#include "slib/ui/core.h"

#include "view_win32.h"

#define PRIV_SCROLL_LINE_SIZE 20
#define PRIV_SCROLL_WHEEL_SIZE 40

namespace slib
{

	namespace priv
	{
		namespace scroll_view
		{

			class ScrollViewHelper : public ScrollView
			{
			public:
				friend class ScrollViewInstance;
			};
				
			class ScrollViewInstance : public Win32_ViewInstance
			{
			public:
				Color m_backgroundColor;

			public:
				LRESULT processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
				{
					HWND handle = getHandle();
					Ref<View> view = getView();
					if (ScrollViewHelper* sv = CastInstance<ScrollViewHelper>(view.get())) {
						sl_bool flagUpdateScroll = sl_false;
						if (sv->isHorizontalScrolling()) {
							if (Windows::processWindowHorizontalScrollEvents(handle, msg, wParam, lParam, PRIV_SCROLL_LINE_SIZE, PRIV_SCROLL_WHEEL_SIZE)) {
								flagUpdateScroll = sl_true;
							}
						}
						if (sv->isVerticalScrolling()) {
							if (Windows::processWindowVerticalScrollEvents(handle, msg, wParam, lParam, PRIV_SCROLL_LINE_SIZE, PRIV_SCROLL_WHEEL_SIZE)) {
								flagUpdateScroll = sl_true;
							}
						}
						if (flagUpdateScroll) {
							SCROLLINFO si;
							Base::zeroMemory(&si, sizeof(si));
							si.cbSize = sizeof(si);
							si.fMask = SIF_POS;
							::GetScrollInfo(handle, SB_HORZ, &si);
							int x = si.nPos;
							::GetScrollInfo(handle, SB_VERT, &si);
							int y = si.nPos;
							_refreshContentPosition(sv, (sl_scroll_pos)x, (sl_scroll_pos)y, sl_true);
							return 0;
						}
					}
					return Win32_ViewInstance::processWindowMessage(msg, wParam, lParam);
				}

				void _setContentView(const Ref<View>& viewChild, ScrollViewHelper* viewParent)
				{
					_refreshContentSize(viewParent);
				}

				void _refreshContentSize(ScrollViewHelper* view)
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
						_refreshContentPosition(view, view->getScrollX(), view->getScrollY(), sl_false);
					}
				}

				void _refreshContentPosition(ScrollViewHelper* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagFromEvent)
				{
					HWND handle = getHandle();
					if (handle) {
						Ref<View> viewContent = view->getContentView();
						if (viewContent.isNotNull()) {
							viewContent->setLocation((sl_ui_pos)(-x), (sl_ui_pos)(-y), UIUpdateMode::Redraw);
						}
						if (flagFromEvent) {
							view->_onScroll_NW(x, y);
						}
					}
				}

				static void _scrollTo(HWND handle, sl_scroll_pos x, sl_scroll_pos y)
				{
					SCROLLINFO si;
					Base::zeroMemory(&si, sizeof(si));
					si.cbSize = sizeof(si);
					si.fMask = SIF_POS;
					si.nPos = (int)x;
					::SetScrollInfo(handle, SB_HORZ, &si, TRUE);
					si.nPos = (int)y;
					::SetScrollInfo(handle, SB_VERT, &si, TRUE);
				}
			};
			
		}
	}

	using namespace priv::scroll_view;

	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		DWORD style = WS_CLIPCHILDREN;
		if (isHorizontalScrollBarVisible()) {
			style |= WS_HSCROLL;
		}
		if (isVerticalScrollBarVisible()) {
			style |= WS_VSCROLL;
		}
		
		DWORD styleEx = WS_EX_CONTROLPARENT;

		if (isBorder()) {
			style |= WS_BORDER;
		}
		Ref<ScrollViewInstance> ret = Win32_ViewInstance::create<ScrollViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
		if (ret.isNotNull()) {
			ret->_setContentView(m_viewContent, (ScrollViewHelper*)this);
			ScrollViewInstance::_scrollTo(ret->getHandle(), getScrollX(), getScrollY());
			ret->m_backgroundColor = getBackgroundColor();
		}
		return ret;
	}

	void ScrollView::_refreshContentSize_NW()
	{
		Ref<ViewInstance> instance = getViewInstance();
		if (ScrollViewInstance* _instance = CastInstance<ScrollViewInstance>(instance.get())) {
			_instance->_refreshContentSize((ScrollViewHelper*)this);
		}
	}

	void ScrollView::_setContentView_NW(const Ref<View>& view)
	{
		Ref<ViewInstance> instance = getViewInstance();
		if (ScrollViewInstance* _instance = CastInstance<ScrollViewInstance>(instance.get())) {
			_instance->_setContentView(view, (ScrollViewHelper*)this);
		}
	}

	void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			ScrollViewInstance::_scrollTo(handle, x, y);
			Ref<ViewInstance> instance = getViewInstance();
			if (ScrollViewInstance* _instance = CastInstance<ScrollViewInstance>(instance.get())) {
				_instance->_refreshContentPosition((ScrollViewHelper*)this, x, y, sl_false);
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

	UIRect ScrollView::_getBounds_NW()
	{
		HWND handle = UIPlatform::getViewHandle(this);
		if (handle) {
			RECT rc;
			GetClientRect(handle, &rc);
			return UIRect(0, 0, rc.right, rc.bottom);
		}
		return ViewGroup::getBounds();
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
		if (ScrollViewInstance* _instance = CastInstance<ScrollViewInstance>(instance.get())) {
			_instance->m_backgroundColor = color;
		}
	}

	void ScrollView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
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
