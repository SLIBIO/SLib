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

#include "slib/ui/list_report_view.h"

#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_WIN32)
#	define HAS_NATIVE_WIDGET_IMPL
#endif


namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ListReportViewColumn)

	ListReportViewColumn::ListReportViewColumn()
	{
		width = 40;
		align = Alignment::MiddleCenter;
		headerAlign = Alignment::MiddleCenter;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ListReportViewCell)
	
	ListReportViewCell::ListReportViewCell()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(ListReportView, View)
	
	ListReportView::ListReportView()
	{
#ifdef HAS_NATIVE_WIDGET_IMPL
		setCreatingNativeWidget(sl_true);
#endif
		setUsingFont(sl_true);
		
		m_columns.setCount(1);
		m_nRows = 0;
		m_selectedRow = -1;
	}
	
	ListReportView::~ListReportView()
	{
	}

	sl_uint32 ListReportView::getColumnsCount()
	{
		return (sl_uint32)(m_columns.getCount());
	}
	
	void ListReportView::setColumnsCount(sl_uint32 nCount, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		m_columns.setCount(nCount);
		if (isNativeWidget()) {
			_refreshColumnsCount_NW();
		} else {
			invalidate(mode);
		}
	}
	
	sl_uint32 ListReportView::getRowsCount()
	{
		return m_nRows;
	}
	
	void ListReportView::setRowsCount(sl_uint32 nCount, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (nCount < m_cells.getCount()) {
			m_cells.setCount(nCount);
		}
		m_nRows = nCount;
		if (isNativeWidget()) {
			_refreshRowsCount_NW();
		} else {
			invalidate(mode);
		}
	}
	
	String ListReportView::getItemText(sl_uint32 iRow, sl_uint32 iCol)
	{
		List<ListReportViewCell> row = m_cells.getValueAt(iRow);
		if (row.isNotNull()) {
			MutexLocker lock(row.getLocker());
			if (iCol < row.getCount()) {
				ListReportViewCell* cell = row.getPointerAt(iCol);
				return cell->text;
			}
		}
		return sl_null;
	}
	
	void ListReportView::setItemText(sl_uint32 iRow, sl_uint32 iCol, const String& text, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (iRow < m_nRows) {
			if (iRow >= m_cells.getCount()) {
				if (!(m_cells.setCount(iRow + 1))) {
					return;
				}
			}
			List<ListReportViewCell> row = m_cells.getValueAt(iRow);
			if (row.isNull()) {
				row.setCount(iCol + 1);
				m_cells.setAt(iRow, row);
			}
			if (row.isNotNull()) {
				MutexLocker lock(row.getLocker());
				if (iCol >= row.getCount()) {
					if (!(row.setCount(iCol + 1))) {
						return;
					}
				}
				ListReportViewCell* cell = row.getPointerAt(iCol);
				cell->text = text;
			}
			if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
				if (isNativeWidget()) {
					_refreshRowsCount_NW();
				} else {
					invalidate();
				}
			}
		}
	}
	
	String ListReportView::getHeaderText(sl_uint32 iCol)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			return col->title;
		}
		return sl_null;
	}
	
	void ListReportView::setHeaderText(sl_uint32 iCol, const String& text, UIUpdateMode mode)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			col->title = text;
			if (isNativeWidget()) {
				_setHeaderText_NW(iCol, text);
			} else {
				invalidate(mode);
			}
		}
	}
	
	sl_ui_len ListReportView::getColumnWidth(sl_uint32 iCol)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			return col->width;
		}
		return 0;
	}
	
	void ListReportView::setColumnWidth(sl_uint32 iCol, sl_ui_len width, UIUpdateMode mode)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			col->width = width;
			if (isNativeWidget()) {
				_setColumnWidth_NW(iCol, width);
			} else {
				invalidate(mode);
			}
		}
	}
	
	Alignment ListReportView::getHeaderAlignment(sl_uint32 iCol)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			return col->headerAlign;
		}
		return Alignment::Center;
	}
	
	void ListReportView::setHeaderAlignment(sl_uint32 iCol, Alignment align, UIUpdateMode mode)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			col->headerAlign = align;
			if (isNativeWidget()) {
				_setHeaderAlignment_NW(iCol, align);
			} else {
				invalidate(mode);
			}
		}
	}
	
	Alignment ListReportView::getColumnAlignment(sl_uint32 iCol)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			return col->align;
		}
		return Alignment::Center;
	}
	
	void ListReportView::setColumnAlignment(sl_uint32 iCol, Alignment align, UIUpdateMode mode)
	{
		MutexLocker lock(m_columns.getLocker());
		if (iCol < m_columns.getCount()) {
			ListReportViewColumn* col = m_columns.getPointerAt(iCol);
			col->align = align;
			if (isNativeWidget()) {
				_setColumnAlignment_NW(iCol, align);
			} else {
				invalidate(mode);
			}
		}
	}
	
	sl_int32 ListReportView::getSelectedRow()
	{
		if (isNativeWidget()) {
			_getSelectedRow_NW();
		}
		return m_selectedRow;
	}
	
	void ListReportView::addRow(UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		setRowsCount(m_nRows+1, mode);
	}
	
	void ListReportView::insertRow(sl_uint32 iRow, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (iRow < m_cells.getCount()) {
			m_cells.insert(iRow, List<ListReportViewCell>::null());
		}
		setRowsCount(m_nRows+1, mode);
	}
	
	void ListReportView::removeRow(sl_uint32 iRow, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (iRow < m_nRows) {
			if (iRow < m_cells.getCount()) {
				m_cells.removeAt(iRow);
			}
			setRowsCount(m_nRows - 1, mode);
		}
	}
	
	void ListReportView::removeAllRows(UIUpdateMode mode)
	{
		setRowsCount(0, mode);
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ListReportView, SelectRow, sl_uint32 row)

	void ListReportView::dispatchSelectRow(sl_uint32 row)
	{
		SLIB_INVOKE_EVENT_HANDLER(SelectRow, row)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ListReportView, ClickRow, sl_uint32 row, const UIPoint& pt)

	void ListReportView::dispatchClickRow(sl_uint32 row, const UIPoint& pt)
	{
		Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::Unknown, (sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), Time::zero());
		if (ev.isNotNull()) {
			dispatchClickEvent(ev.get());
		}
		
		SLIB_INVOKE_EVENT_HANDLER(ClickRow, row, pt)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ListReportView, RightButtonClickRow, sl_uint32 row, const UIPoint& pt)

	void ListReportView::dispatchRightButtonClickRow(sl_uint32 row, const UIPoint& pt)
	{
		SLIB_INVOKE_EVENT_HANDLER(RightButtonClickRow, row, pt)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ListReportView, DoubleClickRow, sl_uint32 row, const UIPoint& pt)

	void ListReportView::dispatchDoubleClickRow(sl_uint32 row, const UIPoint& pt)
	{
		Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::LeftButtonDoubleClick, (sl_real)(pt.x), (sl_real)(pt.y), Time::zero());
		if (ev.isNotNull()) {
			dispatchMouseEvent(ev.get());
		}

		SLIB_INVOKE_EVENT_HANDLER(DoubleClickRow, row, pt)
	}
	
	
#if !defined(HAS_NATIVE_WIDGET_IMPL)
	Ref<ViewInstance> ListReportView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	void ListReportView::_refreshColumnsCount_NW()
	{
	}
	
	void ListReportView::_refreshRowsCount_NW()
	{
	}
	
	void ListReportView::_setHeaderText_NW(sl_uint32 col, const String& text)
	{
	}
	
	void ListReportView::_setColumnWidth_NW(sl_uint32 col, sl_ui_len width)
	{
	}
	
	void ListReportView::_setHeaderAlignment_NW(sl_uint32 col, Alignment align)
	{
	}
	
	void ListReportView::_setColumnAlignment_NW(sl_uint32 col, Alignment align)
	{
	}
	
	void ListReportView::_getSelectedRow_NW()
	{
	}
	
	void ListReportView::_setFont_NW(const Ref<Font>& font)
	{
	}
#endif

}
