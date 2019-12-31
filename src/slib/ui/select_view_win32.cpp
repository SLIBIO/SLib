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
				void applyItemsCount(HWND hWnd)
				{
					ObjectLocker lock(this);
					sl_uint32 nOrig = (sl_uint32)(SendMessageW(hWnd, CB_GETCOUNT, 0, 0));
					sl_uint32 nNew = (sl_uint32)(m_titles.getCount());
					if (nOrig == nNew) {
						return;
					}
					if (nOrig > nNew) {
						if (nNew > 0) {
							for (sl_uint32 i = nOrig; i > nNew; i--) {
								SendMessageW(hWnd, CB_DELETESTRING, (WPARAM)(i - 1), 0);
							}
						} else {
							SendMessageW(hWnd, CB_RESETCONTENT, 0, 0);
						}
					} else {
						for (sl_uint32 i = nOrig; i < nNew; i++) {
							String16 s = String16::from(m_titles.getValueAt(i));
							SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)(s.getData()));
						}
					}
				}

				void copyItems(HWND hWnd)
				{
					SendMessageW(hWnd, CB_RESETCONTENT, 0, 0);
					applyItemsCount(hWnd);
					sl_uint32 n = (sl_uint32)(m_titles.getCount());
					if (m_indexSelected >= n) {
						m_indexSelected = 0;
					}
					if (n > 0) {
						SendMessageW(hWnd, CB_SETCURSEL, (WPARAM)m_indexSelected, 0);
					}
				}

			};

			class SelectViewInstance : public Win32_ViewInstance, public ISelectViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				void select(SelectView* view, sl_uint32 index) override
				{
					HWND handle = m_handle;
					if (handle) {
						SendMessageW(handle, CB_SETCURSEL, (WPARAM)index, 0);
					}
				}

				void refreshItemsCount(SelectView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<SelectViewHelper*>(view))->applyItemsCount(handle);
					}
				}

				void refreshItemsContent(SelectView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<SelectViewHelper*>(view))->copyItems(handle);
					}
				}

				void setItemTitle(SelectView* view, sl_uint32 index, const String& title) override
				{
					HWND handle = m_handle;
					if (handle) {
						String16 s = String16::from(title);
						SendMessageW(handle, CB_DELETESTRING, (WPARAM)index, 0);
						SendMessageW(handle, CB_INSERTSTRING, (WPARAM)index, (LPARAM)(s.getData()));
					}
				}

				sl_bool processCommand(SHORT code, LRESULT& result) override
				{
					if (code == CBN_SELCHANGE) {
						Ref<SelectViewHelper> helper = CastRef<SelectViewHelper>(getView());
						if (helper.isNotNull()) {
							sl_uint32 index = (sl_uint32)(SendMessageW(m_handle, CB_GETCURSEL, 0, 0));
							helper->dispatchSelectItem(index);							
						}
						return sl_true;
					}
					return sl_false;
				}

			};

			SLIB_DEFINE_OBJECT(SelectViewInstance, Win32_ViewInstance)

		}
	}

	using namespace priv::select_view;

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		UINT style = CBS_DROPDOWNLIST | WS_TABSTOP;
		Ref<SelectViewInstance> ret = Win32_ViewInstance::create<SelectViewInstance>(this, parent, L"COMBOBOX", sl_null, style, 0);		
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			(static_cast<SelectViewHelper*>(this))->copyItems(handle);
			return ret;
		}
		return sl_null;
	}

	Ptr<ISelectViewInstance> SelectView::getSelectViewInstance()
	{
		return CastRef<SelectViewInstance>(getViewInstance());
	}

}

#endif
