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

#include "slib/ui/list_report_view.h"

#include "view_win32.h"

#include <commctrl.h>

namespace slib
{

	namespace priv
	{
		namespace list_report_view
		{

			static int TranslateAlignment(Alignment _align)
			{
				Alignment align = _align & Alignment::HorizontalMask;
				if (align == Alignment::Center) {
					return LVCFMT_CENTER;
				} else if (align == Alignment::Right) {
					return LVCFMT_RIGHT;
				}
				return LVCFMT_LEFT;
			}

			class ListReportViewHelper : public ListReportView
			{
			public:
				static sl_uint32 getColumnsCountFromListView(HWND hWnd)
				{
					HWND hWndHeader = (HWND)(SendMessageW(hWnd, LVM_GETHEADER, 0, 0));
					if (hWndHeader) {
						return (sl_uint32)(SendMessageW(hWndHeader, HDM_GETITEMCOUNT, 0, 0));
					}
					return 0;
				}

				void applyColumnsCount(HWND hWnd)
				{
					ObjectLocker lock(this);
					sl_uint32 nNew = (sl_uint32)(m_columns.getCount());
					sl_uint32 nOrig = getColumnsCountFromListView(hWnd);
					if (nOrig == nNew) {
						return;
					}
					if (nOrig > nNew) {
						for (sl_uint32 i = nOrig; i > nNew; i--) {
							SendMessageW(hWnd, LVM_DELETECOLUMN, (WPARAM)(i - 1), 0);
						}
					} else {
						LVCOLUMNW lvc;
						Base::zeroMemory(&lvc, sizeof(lvc));
						lvc.mask = LVCF_SUBITEM;
						for (sl_uint32 i = nOrig; i < nNew; i++) {
							lvc.iSubItem = i;
							SendMessageW(hWnd, LVM_INSERTCOLUMNW, (WPARAM)i, (LPARAM)&lvc);
						}
					}
				}

				void copyColumns(HWND hWnd)
				{
					applyColumnsCount(hWnd);
					LVCOLUMNW lvc;
					Base::zeroMemory(&lvc, sizeof(lvc));
					lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
					ListLocker<ListReportViewColumn> columns(m_columns);
					for (sl_size i = 0; i < columns.count; i++) {
						ListReportViewColumn& column = columns[i];
						String16 title = column.title;
						lvc.pszText = (LPWSTR)(title.getData());
						int width = (int)(column.width);
						if (width < 0) {
							width = 0;
						}
						lvc.cx = width;
						lvc.fmt = TranslateAlignment(column.align);
						SendMessageW(hWnd, LVM_SETCOLUMNW, (WPARAM)i, (LPARAM)(&lvc));
					}
				}

				void applyRowsCount(HWND hWnd)
				{
					sl_uint32 nNew = m_nRows;
					SendMessageW(hWnd, LVM_SETITEMCOUNT, (WPARAM)nNew, LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
				}

			};

			class ListReportViewInstance : public Win32_ViewInstance, public IListReportViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				void refreshColumnsCount(ListReportView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<ListReportViewHelper*>(view))->applyColumnsCount(handle);
					}
				}

				void refreshRowsCount(ListReportView* view) override
				{
					HWND handle = m_handle;
					if (handle) {
						(static_cast<ListReportViewHelper*>(view))->applyRowsCount(handle);
						InvalidateRect(handle, NULL, TRUE);
					}
				}

				void setHeaderText(ListReportView* view, sl_uint32 iCol, const String& _text) override
				{
					HWND handle = m_handle;
					if (handle) {
						LVCOLUMNW lvc;
						Base::zeroMemory(&lvc, sizeof(lvc));
						lvc.mask = LVCF_TEXT;
						String16 text = _text;
						lvc.pszText = (LPWSTR)(text.getData());
						SendMessageW(handle, LVM_SETCOLUMNW, (WPARAM)iCol, (LPARAM)(&lvc));
					}
				}

				void setColumnWidth(ListReportView* view, sl_uint32 iCol, sl_ui_len width) override
				{
					HWND handle = m_handle;
					if (handle) {
						if (width < 0) {
							width = 0;
						}
						SendMessageW(handle, LVM_SETCOLUMNWIDTH, (WPARAM)iCol, (LPARAM)(width));
					}
				}

				void setHeaderAlignment(ListReportView* view, sl_uint32 iCol, const Alignment& align) override
				{
				}

				void setColumnAlignment(ListReportView* view, sl_uint32 iCol, const Alignment& align) override
				{
					HWND handle = m_handle;
					if (handle) {
						LVCOLUMNW lvc;
						Base::zeroMemory(&lvc, sizeof(lvc));
						lvc.mask = LVCF_FMT;
						lvc.fmt = TranslateAlignment(align);
						SendMessageW(handle, LVM_SETCOLUMNW, (WPARAM)iCol, (LPARAM)(&lvc));
					}
				}

				sl_bool getSelectedRow(ListReportView* view, sl_int32& _out) override
				{
					HWND handle = m_handle;
					if (handle) {
						_out = (sl_int32)(::SendMessageW(handle, LVM_GETNEXTITEM, (WPARAM)(-1), LVNI_SELECTED));
						return sl_true;
					}
					return sl_false;
				}

				sl_bool processNotify(NMHDR* nmhdr, LRESULT& result) override
				{
					Ref<ListReportViewHelper> helper = CastRef<ListReportViewHelper>(getView());
					if (helper.isNotNull()) {
						NMITEMACTIVATE* nm = (NMITEMACTIVATE*)nmhdr;
						UINT code = nmhdr->code;
						if (code == LVN_GETDISPINFOW) {
							NMLVDISPINFOW* disp = (NMLVDISPINFOW*)nmhdr;
							String16 s = helper->getItemText(disp->item.iItem, disp->item.iSubItem);
							sl_uint32 n = (sl_uint32)(s.getLength());
							if (n > 0) {
								sl_uint32 m = (sl_uint32)(disp->item.cchTextMax);
								if (m > 0) {
									if (n >= m) {
										n = m - 1;
									}
									Base::copyMemory(disp->item.pszText, s.getData(), n * 2);
									(disp->item.pszText)[n] = 0;
								}
							}
							return sl_true;
						} else if (code == LVN_ITEMCHANGED) {
							NMLISTVIEW* v = (NMLISTVIEW*)(nm);
							if (v->hdr.hwndFrom == getHandle()) {
								if (!(v->uOldState & LVIS_SELECTED) && (v->uNewState & LVIS_SELECTED)) {
									helper->dispatchSelectRow(v->iItem);
								}
							}
							return sl_true;
						} else if (code == NM_CLICK || code == NM_DBLCLK || code == NM_RCLICK) {
							LVHITTESTINFO lvhi;
							Base::zeroMemory(&lvhi, sizeof(lvhi));
							lvhi.pt.x = (LONG)(nm->ptAction.x);
							lvhi.pt.y = (LONG)(nm->ptAction.y);
							UIPoint pt((sl_ui_pos)(nm->ptAction.x), (sl_ui_pos)(nm->ptAction.y));
							sl_int32 n = (sl_int32)(::SendMessageW(getHandle(), LVM_HITTEST, 0, (LPARAM)(&lvhi)));
							if (n >= 0) {
								if (code == NM_CLICK) {
									helper->dispatchClickRow(n, pt);
								} else if (code == NM_RCLICK) {
									helper->dispatchRightButtonClickRow(n, pt);
								} else if (code == NM_DBLCLK) {
									helper->dispatchDoubleClickRow(n, pt);
								}
							}
							return sl_true;
						}
					}
					return sl_false;
				}
			};

			SLIB_DEFINE_OBJECT(ListReportViewInstance, Win32_ViewInstance)

		}
	}

	using namespace priv::list_report_view;

	Ref<ViewInstance> ListReportView::createNativeWidget(ViewInstance* parent)
	{
		DWORD style = LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDATA | WS_TABSTOP | WS_BORDER;
		Ref<ListReportViewInstance> ret = Win32_ViewInstance::create<ListReportViewInstance>(this, parent, L"SysListView32", sl_null, style, 0);
		if (ret.isNotNull()) {
			HWND handle = ret->getHandle();
			ListReportViewHelper* helper = static_cast<ListReportViewHelper*>(this);
			UINT exStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE | LVS_EX_DOUBLEBUFFER;
			::SendMessageW(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, exStyle, exStyle);
			helper->copyColumns(handle);
			helper->applyRowsCount(handle);
			return ret;
		}
		return sl_null;
	}

	Ptr<IListReportViewInstance> ListReportView::getListReportViewInstance()
	{
		return CastRef<ListReportViewInstance>(getViewInstance());
	}

}

#endif
