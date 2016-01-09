#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/scroll_view.h"
#include "../../../inc/slib/ui/core.h"

#include "view_win32.h"

#define _SCROLL_LINE_SIZE 20
#define _SCROLL_WHEEL_SIZE 40

SLIB_UI_NAMESPACE_BEGIN
class _Win32_ScrollViewInstance : public Win32_ViewInstance
{
public:
	Color m_backgroundColor;

	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		HWND handle = getHandle();
		if (msg == WM_ERASEBKGND) {
			Color color = m_backgroundColor;
			if (color.getAlpha() > 0) {
				HBRUSH hbr = ::CreateSolidBrush(RGB(color.getRed(), color.getGreen(), color.getBlue()));
				if (hbr) {
					HDC hDC = (HDC)(wParam);
					RECT rc;
					::GetClientRect(handle, &rc);
					::FillRect(hDC, &rc, hbr);
					::DeleteObject(hbr);
					result = TRUE;
					return sl_true;
				}
			}
		} else if (Windows::processWindowScrollEvents(handle, msg, wParam, lParam, _SCROLL_LINE_SIZE, _SCROLL_WHEEL_SIZE)) {
			Ref<View> view = getView();
			if (ScrollView::checkInstance(view)) {
				__refreshContentPosition((ScrollView*)(view.get()));
			}
			return sl_true;
		}
		return sl_false;
	}

	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool processNotify(NMHDR* nmhdr, LRESULT& result)
	{
		return sl_false;
	}

	void __setContentView(const Ref<View>& viewChild, ScrollView* viewParent)
	{
		if (viewChild.isNotNull()) {
			Ref<ViewInstance> instance = viewChild->attachToNewInstance(this);
			if (instance.isNotNull()) {
				__refreshContentSize(viewParent);
				__refreshContentPosition(viewParent);
			}
		}
	}

	void __refreshContentSize(ScrollView* view)
	{
		HWND handle = getHandle();
		if (handle) {
			Ref<View> viewContent = view->getContentView();
			Sizei sizeContent;
			if (viewContent.isNotNull()) {
				sizeContent = viewContent->getSize();
			} else {
				sizeContent = Sizei::zero();
			}
			Sizei sizeParent = view->getSize();
			Windows::setWindowHorizontalScrollParam(handle, 0, sizeContent.x, sizeParent.x);
			Windows::setWindowVerticalScrollParam(handle, 0, sizeContent.y, sizeParent.y);
			__refreshContentPosition(view);
		}
	}

	void __refreshContentPosition(ScrollView* view)
	{
		HWND handle = getHandle();
		if (handle) {
			Ref<View> viewContent = view->getContentView();
			if (viewContent.isNotNull()) {
				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.fMask = SIF_POS;
				::GetScrollInfo(handle, SB_HORZ, &si);
				int x = si.nPos;
				::GetScrollInfo(handle, SB_VERT, &si);
				int y = si.nPos;
				viewContent->setPosition(-(sl_real)x, -(sl_real)y);
			}
		}
	}
};

Ref<ViewInstance> ScrollView::createInstance(ViewInstance* parent)
{
	Win32_View_Shared* shared = Win32_View_Shared::get();
	DWORD style = WS_HSCROLL | WS_VSCROLL;
	DWORD styleEx = WS_EX_CONTROLPARENT;
#if defined(_SLIB_UI_WIN32_USE_COMPOSITE_VIEWS)
	styleEx |= WS_EX_COMPOSITED;
#endif
#if defined(_SLIB_UI_WIN32_USE_CLIP_CHILDREN)
	style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
#endif
	if (m_flagBorder) {
		style |= WS_BORDER;
	}
	Ref<_Win32_ScrollViewInstance> ret = Win32_ViewInstance::create<_Win32_ScrollViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClass)), L"", style, styleEx);
	if (ret.isNotNull()) {
		ret->__setContentView(m_viewContent, this);
		ret->m_backgroundColor = m_backgroundColor;
	}
	return Ref<ViewInstance>::from(ret);
}

void ScrollView::onResize()
{
	View::onResize();
	Ref<ViewInstance> instance = getViewInstance();
	if (_Win32_ScrollViewInstance::checkInstance(instance)) {
		((_Win32_ScrollViewInstance*)(instance.get()))->__refreshContentSize(this);
	}
}

void ScrollView::onResizeChild(View* child)
{
	Ref<ViewInstance> instance = getViewInstance();
	if (_Win32_ScrollViewInstance::checkInstance(instance)) {
		((_Win32_ScrollViewInstance*)(instance.get()))->__refreshContentSize(this);
	}
}

void ScrollView::_setBorder(sl_bool flag)
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

void ScrollView::_setBackgroundColor(const Color& color)
{
	Ref<ViewInstance> instance = getViewInstance();
	if (_Win32_ScrollViewInstance::checkInstance(instance)) {
		((_Win32_ScrollViewInstance*)(instance.get()))->m_backgroundColor = m_backgroundColor;
	}
}

void ScrollView::_setContentView(const Ref<View>& view)
{
	Ref<ViewInstance> instance = getViewInstance();
	if (_Win32_ScrollViewInstance::checkInstance(instance)) {
		((_Win32_ScrollViewInstance*)(instance.get()))->__setContentView(view, this);
	}
}

void ScrollView::scrollTo(sl_real x, sl_real y)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		SCROLLINFO si;
		Base::zeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = (int)x;
		::SetScrollInfo(handle, SB_HORZ, &si, TRUE);
		si.nPos = (int)y;
		::SetScrollInfo(handle, SB_VERT, &si, TRUE);
		Ref<ViewInstance> instance = getViewInstance();
		if (_Win32_ScrollViewInstance::checkInstance(instance)) {
			((_Win32_ScrollViewInstance*)(instance.get()))->__refreshContentPosition(this);
		}
	}
}

Point ScrollView::getScrollPosition()
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
		return Point((sl_real)x, (sl_real)y);
	}
	return Point::zero();
}

Size ScrollView::getScrollRange()
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
		return Size((sl_real)w, (sl_real)h);
	}
	return Size::zero();
}
SLIB_UI_NAMESPACE_END

#endif
