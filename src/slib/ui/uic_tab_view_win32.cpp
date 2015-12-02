#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/tab_view.h"
#include "../../../inc/slib/ui/core.h"

#include "ui_view_win32.h"

#include <commctrl.h>

SLIB_UI_NAMESPACE_BEGIN
class _TabView : public TabView
{
public:
	void __applyTabsCount(HWND hWnd)
	{
		MutexLocker lock(getLocker());
		sl_uint32 nOrig = (sl_uint32)(::SendMessageW(hWnd, TCM_GETITEMCOUNT, 0, 0));
		sl_uint32 nNew = (sl_uint32)(m_items.count());
		if (nOrig == nNew) {
			return;
		}
		if (nOrig > nNew) {
			if (nNew > 0) {
				for (sl_uint32 i = nOrig; i > nNew; i--) {
					::SendMessageW(hWnd, TCM_GETITEMCOUNT, (WPARAM)(i - 1), 0);
				}
			} else {
				::SendMessageW(hWnd, TCM_DELETEALLITEMS, 0, 0);
			}
		} else {
			for (sl_uint32 i = nOrig; i < nNew; i++) {
				TCITEMW tci;
				Base::zeroMemory(&tci, sizeof(tci));
				::SendMessageW(hWnd, TCM_INSERTITEM, (WPARAM)i, (LPARAM)(&tci));
			}
		}
	}

	void __copyTabs(HWND hWnd, ViewInstance* viewInstance)
	{
		__applyTabsCount(hWnd);
		ListLocker<TabViewItem> items(m_items);
		for (sl_size i = 0; i < items.count(); i++) {
			TCITEMW tci;
			Base::zeroMemory(&tci, sizeof(tci));
			tci.mask = TCIF_TEXT;
			String16 label = items[i].label;
			tci.pszText = (LPWSTR)(label.getBuf());
			::SendMessageW(hWnd, TCM_SETITEMW, (WPARAM)i, (LPARAM)&tci);
		}
		__selectTab(hWnd, viewInstance, m_indexSelected);
	}

	void __setTabLabel(HWND hWnd, sl_uint32 index, const String& _label)
	{
		TCITEMW tci;
		Base::zeroMemory(&tci, sizeof(tci));
		tci.mask = TCIF_TEXT;
		String16 label = _label;
		tci.pszText = (LPWSTR)(label.getBuf());
		::SendMessageW(hWnd, TCM_SETITEMW, (WPARAM)index, (LPARAM)(&tci));
	}

	void __selectTab(HWND hWnd, ViewInstance* viewInstance, sl_uint32 index)
	{
		sl_uint32 n = (sl_uint32)(m_items.count());
		if (index >= n) {
			index = 0;
		}
		m_indexSelected = index;
		::SendMessageW(hWnd, TCM_SETCURSEL, (WPARAM)m_indexSelected, 0);
		__applyTabContents(hWnd, viewInstance);
	}

	sl_uint32 __getSelectedIndex(HWND hWnd)
	{
		return (sl_uint32)(::SendMessageW(hWnd, TCM_GETCURSEL, 0, 0));
	}

	void __onSelectTab(HWND hWnd, ViewInstance* viewInstance)
	{
		sl_uint32 index = __getSelectedIndex(hWnd);
		m_indexSelected = index;
		onSelectTab(index);
		__applyTabContents(hWnd, viewInstance);
	}

	void __applyTabContents(HWND hWnd, ViewInstance* viewInstance)
	{
		Rectangle rc = __getClientBounds(hWnd);
		sl_size sel = m_indexSelected;
		ListLocker<TabViewItem> items(m_items);
		for (sl_size i = 0; i < items.count(); i++) {
			Ref<View> view = items[i].contentView;
			if (view.isNotNull()) {
				view->setFrame(rc);
				if (i == sel) {
					view->setVisible(sl_true);
					if (view->getViewInstance().isNull()) {
						view->attachToNewInstance(viewInstance);
					}
				} else {
					view->setVisible(sl_false);
				}
			}
		}
	}

	void __applyClientBounds(HWND hWnd)
	{
		Rectangle rc = __getClientBounds(hWnd);
		ListLocker<TabViewItem> items(m_items);
		for (sl_size i = 0; i < items.count(); i++) {
			Ref<View> view = items[i].contentView;
			if (view.isNotNull()) {
				view->setFrame(rc);
			}
		}
	}

	Rectangle __getClientBounds(HWND hWnd)
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = (int)(getWidth());
		rc.bottom = (int)(getHeight());
		::SendMessageW(hWnd, TCM_ADJUSTRECT, FALSE, (LPARAM)(&rc));
		return Rectangle((sl_real)(rc.left), (sl_real)(rc.top), (sl_real)(rc.right), (sl_real)(rc.bottom));
	}
};

class _Win32_TabViewInstance : public Win32_ViewInstance
{
public:
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool processNotify(NMHDR* nmhdr, LRESULT& result)
	{
		HWND handle = getHandle();
		if (handle) {
			Ref<View> _view = getView();
			if (TabView::checkInstance(_view)) {
				_TabView* view = (_TabView*)(_view.get());
				UINT code = nmhdr->code;
				if (code == TCN_SELCHANGE) {
					view->__onSelectTab(handle, this);
					return sl_true;
				}
			}
		}
		return sl_false;
	}
};

Ref<ViewInstance> TabView::createInstance(ViewInstance* parent)
{
	DWORD style = 0;
	DWORD styleEx = WS_EX_CONTROLPARENT;
#if defined(_SLIB_UI_WIN32_USE_COMPOSITE_VIEWS)
	styleEx |= WS_EX_COMPOSITED;
#endif
#if defined(_SLIB_UI_WIN32_USE_CLIP_CHILDREN)
	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
#endif

	Ref<_Win32_TabViewInstance> ret = Win32_ViewInstance::create<_Win32_TabViewInstance>(this, parent, L"SysTabControl32", L"", style, styleEx);
	
	if (ret.isNotNull()) {

		HWND handle = ret->getHandle();

		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		HFONT hFont = UIPlatform::getGdiFont(font.get(), fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		m_fontInstance = fontInstance;

		((_TabView*)this)->__copyTabs(handle, ret.get());
	}
	return Ref<ViewInstance>::from(ret);
}

void TabView::_refreshTabsCount()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_TabView*)this)->__applyTabsCount(handle);
	}
}

void TabView::_setTabLabel(sl_uint32 index, const String& text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_TabView*)this)->__setTabLabel(handle, index, text);
	}
}

void TabView::_setTabContentView(sl_uint32 index, const Ref<View>& view)
{
	Ref<ViewInstance> viewInstance = getViewInstance();
	if (viewInstance.isNotNull()) {
		HWND handle = UIPlatform::getViewHandle(viewInstance.get());
		((_TabView*)this)->__applyTabContents(handle, viewInstance.get());
	}
}

sl_uint32 TabView::getSelectedTabIndex()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_indexSelected = (sl_uint32)(((_TabView*)this)->__getSelectedIndex(handle));
	}
	return m_indexSelected;
}

void TabView::_selectTab(sl_uint32 index)
{
	Ref<ViewInstance> viewInstance = getViewInstance();
	if (viewInstance.isNotNull()) {
		HWND handle = UIPlatform::getViewHandle(viewInstance.get());
		((_TabView*)this)->__selectTab(handle, viewInstance.get(), index);
	}
}

Size TabView::getContentViewSize()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		return ((_TabView*)this)->__getClientBounds(handle).getSize();
	}
	return Size::zero();
}

void TabView::setFont(const Ref<Font>& _font)
{
	Ref<Font> font = _font;
	Ref<FontInstance> fontInstance;
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		HFONT hFont = UIPlatform::getGdiFont(font.get(), fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
	}
	m_font = font;
	m_fontInstance = fontInstance;
}

void TabView::onResize()
{
	View::onResize();
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_TabView*)this)->__applyClientBounds(handle);
	}
}
SLIB_UI_NAMESPACE_END

#endif
