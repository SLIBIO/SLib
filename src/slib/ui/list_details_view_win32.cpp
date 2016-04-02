#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/list_details_view.h"

#include "view_win32.h"

#include <commctrl.h>

SLIB_UI_NAMESPACE_BEGIN

class _ListDetailsView : public ListDetailsView
{
public:
	static sl_uint32 getColumnsCountFromListView(HWND hWnd)
	{
		HWND hWndHeader = (HWND)(::SendMessageW(hWnd, LVM_GETHEADER, 0, 0));
		if (hWndHeader) {
			return (sl_uint32)(::SendMessageW(hWndHeader, HDM_GETITEMCOUNT, 0, 0));
		}
		return 0;
	}

	void __applyColumnsCount(HWND hWnd)
	{
		ObjectLocker lock(this);
		sl_uint32 nNew = (sl_uint32)(m_columns.getCount());
		sl_uint32 nOrig = getColumnsCountFromListView(hWnd);
		if (nOrig == nNew) {
			return;
		}
		if (nOrig > nNew) {
			for (sl_uint32 i = nOrig; i > nNew; i--) {
				::SendMessageW(hWnd, LVM_DELETECOLUMN, (WPARAM)(i - 1), 0);
			}
		} else {
			LVCOLUMNW lvc;
			Base::zeroMemory(&lvc, sizeof(lvc));
			lvc.mask = LVCF_SUBITEM;
			for (sl_uint32 i = nOrig; i < nNew; i++) {
				lvc.iSubItem = i;
				::SendMessageW(hWnd, LVM_INSERTCOLUMNW, (WPARAM)i, (LPARAM)&lvc);
			}
		}
	}

	void __copyColumns(HWND hWnd)
	{
		__applyColumnsCount(hWnd);
		LVCOLUMNW lvc;
		Base::zeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		ListLocker<ListDetailsViewColumn> columns(m_columns);
		for (sl_size i = 0; i < columns.count; i++) {
			ListDetailsViewColumn& column = columns[i];
			String16 title = column.title;
			lvc.pszText = (LPWSTR)(title.getData());
			int width = (int)(column.width);
			if (width < 0) {
				width = 0;
			}
			lvc.cx = width;
			lvc.fmt = translateAlignment(column.align);
			::SendMessageW(hWnd, LVM_SETCOLUMNW, (WPARAM)i, (LPARAM)(&lvc));
		}
	}

	void __applyRowsCount(HWND hWnd)
	{
		sl_uint32 nNew = m_nRows;
		::SendMessageW(hWnd, LVM_SETITEMCOUNT, (WPARAM)nNew, LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
	}

	static int translateAlignment(Alignments _align)
	{
		Alignments align = _align & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			return LVCFMT_CENTER;
		} else if (align == Alignment::Right) {
			return LVCFMT_RIGHT;
		}
		return LVCFMT_LEFT;
	}

};

class _Win32_ListDetailsViewInstance : public Win32_ViewInstance
{
public:
    // override
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
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
		Ref<View> _view = getView();
		if (ListDetailsView::checkInstance(_view.ptr)) {
			ListDetailsView* view = (ListDetailsView*)(_view.ptr);
			NMITEMACTIVATE* nm = (NMITEMACTIVATE*)nmhdr;
			UINT code = nmhdr->code;
			if (code == LVN_GETDISPINFOW) {
				NMLVDISPINFOW* disp = (NMLVDISPINFOW*)nmhdr;
				String16 s = view->onGetCellText(disp->item.iItem, disp->item.iSubItem);
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
						view->onSelectRow(v->iItem);
					}
				}
				return sl_true;
			} else if (code == NM_CLICK || code == NM_DBLCLK || code == NM_RCLICK) {
				LVHITTESTINFO lvhi;
				Base::zeroMemory(&lvhi, sizeof(lvhi));
				lvhi.pt.x = (LONG)(nm->ptAction.x);
				lvhi.pt.y = (LONG)(nm->ptAction.y);
				Point pt((sl_real)(nm->ptAction.x), (sl_real)(nm->ptAction.y));
				sl_int32 n = (sl_int32)(::SendMessageW(getHandle(), LVM_HITTEST, 0, (LPARAM)(&lvhi)));
				if (n >= 0) {
					if (code == NM_CLICK) {
						view->onClickRow(n, pt);
					} else if (code == NM_RCLICK) {
						view->onRightButtonClickRow(n, pt);
					} else if (code == NM_DBLCLK) {
						view->onDoubleClickRow(n, pt);
					}
				}
				return sl_true;
			}
		}
		return sl_false;
	}
};

Ref<ViewInstance> ListDetailsView::createInstance(ViewInstance* parent)
{
	DWORD style = LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDATA | WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER;
	DWORD styleEx = 0;
#if defined(_SLIB_UI_WIN32_USE_COMPOSITE_VIEWS)
	styleEx |= WS_EX_COMPOSITED;
#endif
	Ref<_Win32_ListDetailsViewInstance> ret = Win32_ViewInstance::create<_Win32_ListDetailsViewInstance>(this, parent, L"SysListView32", L"", style, styleEx);
	
	if (ret.isNotNull()) {

		HWND handle = ret->getHandle();

		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			// You should send this message before inserting any items
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		m_fontInstance = fontInstance;

		UINT exStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE;
		::SendMessageW(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, exStyle, exStyle);

		((_ListDetailsView*)this)->__copyColumns(handle);
		((_ListDetailsView*)this)->__applyRowsCount(handle);
	}
	return ret;
}

void ListDetailsView::_refreshColumnsCount()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_ListDetailsView*)this)->__applyColumnsCount(handle);
	}
}

void ListDetailsView::_refreshRowsCount()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_ListDetailsView*)this)->__applyRowsCount(handle);
		::InvalidateRect(handle, NULL, TRUE);
	}
}

void ListDetailsView::_setHeaderText(sl_uint32 iCol, const String& _text)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LVCOLUMNW lvc;
		Base::zeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT;
		String16 text = _text;
		lvc.pszText = (LPWSTR)(text.getData());
		::SendMessageW(handle, LVM_SETCOLUMNW, (WPARAM)iCol, (LPARAM)(&lvc));
	}
}

void ListDetailsView::_setColumnWidth(sl_uint32 iCol, sl_real width)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		if (width < 0) {
			width = 0;
		}
		::SendMessageW(handle, LVM_SETCOLUMNWIDTH, (WPARAM)iCol, (LPARAM)(width));
	}
}

void ListDetailsView::_setHeaderAlignment(sl_uint32 iCol, Alignments align)
{
}

void ListDetailsView::_setColumnAlignment(sl_uint32 iCol, Alignments align)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		LVCOLUMNW lvc;
		Base::zeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_FMT;
		lvc.fmt = _ListDetailsView::translateAlignment(align);
		::SendMessageW(handle, LVM_SETCOLUMNW, (WPARAM)iCol, (LPARAM)(&lvc));
	}
}

sl_int32 ListDetailsView::getSelectedRow()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		sl_int32 n = (sl_int32)(::SendMessageW(handle, LVM_GETNEXTITEM, (WPARAM)(-1), LVNI_SELECTED));
		return n;
	}
	return -1;
}

void ListDetailsView::setFont(const Ref<Font>& font)
{
	Ref<FontInstance> fontInstance;
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
	}
	m_font = font;
	m_fontInstance = fontInstance;
}

SLIB_UI_NAMESPACE_END

#endif
