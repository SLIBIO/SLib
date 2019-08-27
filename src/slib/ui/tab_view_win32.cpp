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

#include "slib/ui/tab_view.h"
#include "slib/ui/core.h"

#include "view_win32.h"

#include <commctrl.h>

namespace slib
{

	namespace priv
	{
		namespace tab_view
		{

			class TabViewHelper : public TabView
			{
			public:
				void applyTabsCount(HWND hWnd)
				{
					ObjectLocker lock(this);
					sl_uint32 nOrig = (sl_uint32)(SendMessageW(hWnd, TCM_GETITEMCOUNT, 0, 0));
					sl_uint32 nNew = (sl_uint32)(m_items.getCount());
					if (nOrig == nNew) {
						return;
					}
					if (nOrig > nNew) {
						if (nNew > 0) {
							for (sl_uint32 i = nOrig; i > nNew; i--) {
								SendMessageW(hWnd, TCM_GETITEMCOUNT, (WPARAM)(i - 1), 0);
							}
						} else {
							SendMessageW(hWnd, TCM_DELETEALLITEMS, 0, 0);
						}
					} else {
						for (sl_uint32 i = nOrig; i < nNew; i++) {
							TCITEMW tci;
							Base::zeroMemory(&tci, sizeof(tci));
							SendMessageW(hWnd, TCM_INSERTITEM, (WPARAM)i, (LPARAM)(&tci));
						}
					}
				}

				void copyTabs(ViewInstance* instance, HWND hWnd)
				{
					applyTabsCount(hWnd);
					ListLocker<TabViewItem> items(m_items);
					for (sl_size i = 0; i < items.count; i++) {
						TCITEMW tci;
						Base::zeroMemory(&tci, sizeof(tci));
						tci.mask = TCIF_TEXT;
						String16 label = items[i].label;
						tci.pszText = (LPWSTR)(label.getData());
						SendMessageW(hWnd, TCM_SETITEMW, (WPARAM)i, (LPARAM)&tci);
					}
					selectTab(instance, hWnd, m_indexSelected);
				}

				void selectTab(ViewInstance* instance, HWND hWnd, sl_uint32 index)
				{
					sl_uint32 n = (sl_uint32)(m_items.getCount());
					if (index >= n) {
						index = 0;
					}
					SendMessageW(hWnd, TCM_SETCURSEL, (WPARAM)index, 0);
					applyTabContents(instance, hWnd);
				}

				void applyTabContents(ViewInstance* viewInstance, HWND hWnd)
				{
					UIRect rc = getClientBounds(hWnd);
					sl_size sel = m_indexSelected;
					ListLocker<TabViewItem> items(m_items);
					for (sl_size i = 0; i < items.count; i++) {
						Ref<View> view = items[i].contentView;
						if (view.isNotNull()) {
							view->setFrame(rc);
							if (i == sel) {
								if (!(view->isInstance())) {
									view->setVisible(sl_true, UIUpdateMode::None);
									view->attachToNewInstance(viewInstance);
								} else {
									view->setVisible(sl_true);
								}
							} else {
								view->setVisible(sl_false);
							}
						}
					}
				}

				void applyClientBounds(HWND hWnd)
				{
					UIRect rc = getClientBounds(hWnd);
					ListLocker<TabViewItem> items(m_items);
					for (sl_size i = 0; i < items.count; i++) {
						Ref<View> view = items[i].contentView;
						if (view.isNotNull()) {
							view->setFrame(rc);
						}
					}
				}

				UIRect getClientBounds(HWND hWnd)
				{
					RECT rc;
					rc.left = -2;
					rc.top = 0;
					rc.right = (int)(getWidth());
					rc.bottom = (int)(getHeight()) + 1;
					SendMessageW(hWnd, TCM_ADJUSTRECT, FALSE, (LPARAM)(&rc));
					return UIRect((sl_ui_pos)(rc.left), (sl_ui_pos)(rc.top), (sl_ui_pos)(rc.right), (sl_ui_pos)(rc.bottom));
				}

			};

			class TabViewInstance : public Win32_ViewInstance, public ITabViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				void refreshTabsCount(TabView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<TabViewHelper*>(view))->applyTabsCount(handle);
					}
				}

				void refreshSize(TabView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<TabViewHelper*>(view))->applyClientBounds(handle);
					}
				}

				void setTabLabel(TabView* view, sl_uint32 index, const String& _text) override
				{
					HWND handle = m_handle;
					if (handle) {
						TCITEMW tci;
						Base::zeroMemory(&tci, sizeof(tci));
						tci.mask = TCIF_TEXT;
						String16 text = _text;
						tci.pszText = (LPWSTR)(text.getData());
						SendMessageW(handle, TCM_SETITEMW, (WPARAM)index, (LPARAM)(&tci));
					}
				}

				void setTabContentView(TabView* view, sl_uint32 index, const Ref<View>& content) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<TabViewHelper*>(view))->applyTabContents(this, handle);
					}
				}

				void selectTab(TabView* view, sl_uint32 index) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<TabViewHelper*>(view))->selectTab(this, handle, index);
					}
				}

				sl_bool getContentViewSize(TabView* view, UISize& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						_out = (static_cast<TabViewHelper*>(view))->getClientBounds(handle).getSize();
						return sl_true;
					}
					return sl_false;
				}

				sl_bool processNotify(NMHDR* nmhdr, LRESULT& result) override
				{
					HWND handle = getHandle();
					if (handle) {
						Ref<TabViewHelper> helper = CastRef<TabViewHelper>(getView());
						if (helper.isNotNull()) {
							UINT code = nmhdr->code;
							if (code == TCN_SELCHANGE) {
								sl_uint32 index = (sl_uint32)(SendMessageW(handle, TCM_GETCURSEL, 0, 0));
								helper->dispatchSelectTab(index);
								helper->applyTabContents(this, handle);
								return sl_true;
							}
						}
					}
					return sl_false;
				}
			};

			SLIB_DEFINE_OBJECT(TabViewInstance, Win32_ViewInstance)

		}
	}

	using namespace priv::tab_view;

	Ref<ViewInstance> TabView::createNativeWidget(ViewInstance* parent)
	{
		DWORD style = WS_CLIPCHILDREN;
		DWORD styleEx = WS_EX_CONTROLPARENT;
		Ref<TabViewInstance> ret = Win32_ViewInstance::create<TabViewInstance>(this, parent, L"SysTabControl32", sl_null, style, styleEx);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			(static_cast<TabViewHelper*>(this))->copyTabs(ret.get(), handle);
			return ret;
		}
		return sl_null;
	}

	Ptr<ITabViewInstance> TabView::getTabViewInstance()
	{
		return CastRef<TabViewInstance>(getViewInstance());
	}

}

#endif
