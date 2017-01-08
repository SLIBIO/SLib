#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/scroll_view.h"
#include "../../../inc/slib/ui/core.h"

#include "view_win32.h"

#define _SCROLL_LINE_SIZE 20
#define _SCROLL_WHEEL_SIZE 40

SLIB_UI_NAMESPACE_BEGIN

class _ScrollView : public ScrollView
{
public:
	friend class _Win32_ScrollViewInstance;
};
    
class _Win32_ScrollViewInstance : public Win32_ViewInstance
{
public:
	Color m_backgroundColor;

public:
    // override
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		HWND handle = getHandle();
		if (msg == WM_ERASEBKGND) {
			Ref<View> view = getView();
			if (_ScrollView* sv = CastInstance<_ScrollView>(view.get())) {
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
			if (_ScrollView* sv = CastInstance<_ScrollView>(view.get())) {
				if (sv->m_flagHorizontalScroll) {
					if (Windows::processWindowHorizontalScrollEvents(handle, msg, wParam, lParam, _SCROLL_LINE_SIZE, _SCROLL_WHEEL_SIZE)) {
						__refreshContentPosition(sv, sl_true);
						return sl_true;
					}
				}
				if (sv->m_flagVerticalScroll) {
					if (Windows::processWindowVerticalScrollEvents(handle, msg, wParam, lParam, _SCROLL_LINE_SIZE, _SCROLL_WHEEL_SIZE)) {
						__refreshContentPosition(sv, sl_true);
						return sl_true;
					}
				}
			}
		}
		return sl_false;
	}

    // override
	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		return sl_false;
	}

    // override
	sl_bool processNotify(NMHDR* nmhdr, LRESULT& result)
	{
		return sl_false;
	}

	void __setContentView(const Ref<View>& viewChild, _ScrollView* viewParent)
	{
		if (viewChild.isNotNull()) {
			viewChild->attachToNewInstance(this);
		}
		__refreshContentSize(viewParent);
		__refreshContentPosition(viewParent, sl_false);
	}

	void __refreshContentSize(_ScrollView* view)
	{
		HWND handle = getHandle();
		if (handle) {
			Ref<View> viewContent = view->getContentView();
			Sizei sizeContent = view->getContentSize();
			Sizei sizeParent = view->getSize();
			if (view->m_flagHorizontalScroll) {
				Windows::setWindowHorizontalScrollParam(handle, 0, sizeContent.x, sizeParent.x);
			}
			if (view->m_flagVerticalScroll) {
				Windows::setWindowVerticalScrollParam(handle, 0, sizeContent.y, sizeParent.y);
			}
			__refreshContentPosition(view, sl_false);
		}
	}

	void __refreshContentPosition(_ScrollView* view, sl_bool flagFromEvent)
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
				viewContent->setPosition((sl_ui_pos)(-x), (sl_ui_pos)(-y));
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
	if (m_flagHorizontalScroll) {
		style = WS_HSCROLL;
	}
	if (m_flagVerticalScroll) {
		style |= WS_VSCROLL;
	}

	DWORD styleEx = WS_EX_CONTROLPARENT;

	if (isBorder()) {
		style |= WS_BORDER;
	}
	Ref<_Win32_ScrollViewInstance> ret = Win32_ViewInstance::create<_Win32_ScrollViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
	if (ret.isNotNull()) {
		ret->__setContentView(m_viewContent, (_ScrollView*)this);
		ret->m_backgroundColor = getBackgroundColor();
	}
	return ret;
}

void ScrollView::_refreshContentSize_NW()
{
	Ref<ViewInstance> instance = getViewInstance();
	if (_Win32_ScrollViewInstance* _instance = CastInstance<_Win32_ScrollViewInstance>(instance.get())) {
		_instance->__refreshContentSize((_ScrollView*)this);
	}
}

void ScrollView::_setContentView_NW(const Ref<View>& view)
{
	Ref<ViewInstance> instance = getViewInstance();
	if (_Win32_ScrollViewInstance* _instance = CastInstance<_Win32_ScrollViewInstance>(instance.get())) {
		_instance->__setContentView(view, (_ScrollView*)this);
	}
}

void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		SCROLLINFO si;
		Base::zeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		if (m_flagHorizontalScroll) {
			si.nPos = (int)x;
			::SetScrollInfo(handle, SB_HORZ, &si, TRUE);
		}
		if (m_flagVerticalScroll) {
			si.nPos = (int)y;
			::SetScrollInfo(handle, SB_VERT, &si, TRUE);
		}
		Ref<ViewInstance> instance = getViewInstance();
		if (_Win32_ScrollViewInstance* _instance = CastInstance<_Win32_ScrollViewInstance>(instance.get())) {
			_instance->__refreshContentPosition((_ScrollView*)this, sl_false);
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
	if (_Win32_ScrollViewInstance* _instance = CastInstance<_Win32_ScrollViewInstance>(instance.get())) {
		_instance->m_backgroundColor = color;
	}
}

SLIB_UI_NAMESPACE_END

#endif
