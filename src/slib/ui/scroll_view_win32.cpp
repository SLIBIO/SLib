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

#include "slib/ui/scroll_view.h"

#include "view_win32.h"

#define PRIV_SCROLL_LINE_SIZE 20
#define PRIV_SCROLL_WHEEL_SIZE 40

namespace slib
{

	namespace priv
	{
		namespace scroll_view
		{

			static void ScrollTo(HWND handle, sl_scroll_pos x, sl_scroll_pos y)
			{
				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS;
				si.nPos = (int)x;
				SetScrollInfo(handle, SB_HORZ, &si, TRUE);
				si.nPos = (int)y;
				SetScrollInfo(handle, SB_VERT, &si, TRUE);
			}

			class ScrollViewHelper : public ScrollView
			{
			public:
				friend class ScrollViewInstance;
			};
				
			class ScrollViewInstance : public Win32_ViewInstance, public IScrollViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				Color m_backgroundColor;

			public:
				void refreshContentPosition(ScrollView* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagFromEvent)
				{
					HWND handle = m_handle;
					if (handle) {
						Ref<View> viewContent = view->getContentView();
						if (viewContent.isNotNull()) {
							viewContent->setLocation((sl_ui_pos)(-x), (sl_ui_pos)(-y), UIUpdateMode::Redraw);
						}
						if (flagFromEvent) {
							(static_cast<ScrollViewHelper*>(view))->_onScroll_NW(x, y);
						}
					}
				}

				void refreshContentSize(ScrollView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						Sizei sizeContent = view->getContentSize();
						Sizei sizeParent = view->getSize();
						if (view->isHorizontalScrolling()) {
							Windows::setWindowHorizontalScrollParam(handle, 0, sizeContent.x - 1, sizeParent.x);
						}
						if (view->isVerticalScrolling()) {
							Windows::setWindowVerticalScrollParam(handle, 0, sizeContent.y - 1, sizeParent.y);
						}
						refreshContentPosition(view, view->getScrollX(), view->getScrollY(), sl_false);
					}
				}

				void setContentView(ScrollView* view, const Ref<View>& content) override
				{
					refreshContentSize(view);
				}

				sl_bool getScrollPosition(ScrollView* view, ScrollPoint& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						SCROLLINFO si;
						Base::zeroMemory(&si, sizeof(si));
						si.cbSize = sizeof(si);
						si.fMask = SIF_POS;
						::GetScrollInfo(handle, SB_HORZ, &si);
						_out.x = (sl_scroll_pos)(si.nPos);
						::GetScrollInfo(handle, SB_VERT, &si);
						_out.y = (sl_scroll_pos)(si.nPos);
						return sl_true;
					}
					return sl_false;
				}

				sl_bool getScrollRange(ScrollView* view, ScrollPoint& _out) override
				{
					HWND handle = m_handle;
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
						_out.x = (sl_scroll_pos)w;
						_out.y =(sl_scroll_pos)h;
						return sl_true;
					}
					return sl_false;
				}

				sl_bool getBounds(ScrollView* view, UIRect& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						RECT rc;
						GetClientRect(handle, &rc);
						_out.left = 0;
						_out.top = 0;
						_out.right = (sl_ui_len)(rc.right);
						_out.bottom = (sl_ui_len)(rc.bottom);
						return sl_true;
					}
					return sl_false;
				}

				void setBackgroundColor(View* view, const Color& color) override
				{
					HWND handle = m_handle;
					if (handle) {
						m_backgroundColor = color;
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void scrollTo(View* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate) override
				{
					HWND handle = m_handle;
					if (handle) {
						ScrollTo(handle, x, y);
						if (IsInstanceOf<ScrollView>(view)) {
							refreshContentPosition(static_cast<ScrollView*>(view), x, y, sl_false);
						}
					}
				}

				LRESULT processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
				{
					HWND handle = getHandle();
					Ref<ScrollViewHelper> helper = CastRef<ScrollViewHelper>(getView());
					if (helper.isNotNull()) {
						sl_bool flagUpdateScroll = sl_false;
						if (helper->isHorizontalScrolling()) {
							if (Windows::processWindowHorizontalScrollEvents(handle, msg, wParam, lParam, PRIV_SCROLL_LINE_SIZE, PRIV_SCROLL_WHEEL_SIZE)) {
								flagUpdateScroll = sl_true;
							}
						}
						if (helper->isVerticalScrolling()) {
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
							refreshContentPosition(helper.get(), (sl_scroll_pos)x, (sl_scroll_pos)y, sl_true);
							return 0;
						}
					}
					return Win32_ViewInstance::processWindowMessage(msg, wParam, lParam);
				}

			};

			SLIB_DEFINE_OBJECT(ScrollViewInstance, Win32_ViewInstance)
			
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
		DWORD styleEx = WS_EX_CONTROLPARENT;
		Ref<ScrollViewInstance> ret = Win32_ViewInstance::create<ScrollViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), sl_null, style, styleEx);
		if (ret.isNotNull()) {
			ret->m_backgroundColor = getBackgroundColor();
			ret->setContentView(this, getContentView());
			return ret;
		}
		return sl_null;
	}

	Ptr<IScrollViewInstance> ScrollView::getScrollViewInstance()
	{
		return CastRef<ScrollViewInstance>(getViewInstance());
	}

}

#endif
