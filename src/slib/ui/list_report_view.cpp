#include "../../../inc/slib/ui/list_report_view.h"

SLIB_UI_NAMESPACE_BEGIN

void IListReportViewListener::onSelectRow(ListReportView* view, sl_uint32 row)
{
}

void IListReportViewListener::onClickRow(ListReportView* view, sl_uint32 row, const Point& pt)
{
}

void IListReportViewListener::onRightButtonClickRow(ListReportView* view, sl_uint32 row, const UIPoint& pt)
{
}

void IListReportViewListener::onDoubleClickRow(ListReportView* view, sl_uint32 row, const UIPoint& pt)
{
}

ListReportViewColumn::ListReportViewColumn()
{
	width = 40;
	align = Alignment::MiddleCenter;
	headerAlign = Alignment::MiddleCenter;
}


SLIB_DEFINE_OBJECT(ListReportView, View)

ListReportView::ListReportView()
{
	setCreatingNativeWidget(sl_true);
	setUsingFont(sl_true);
	
	m_columns.setCount(1);
	m_nRows = 0;
	m_selectedRow = -1;
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
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
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
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
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
		if (mode == UIUpdateMode::Redraw) {
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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

void ListReportView::onSelectRow(sl_uint32 row)
{
}

void ListReportView::onClickRow(sl_uint32 row, const UIPoint& pt)
{
}

void ListReportView::onRightButtonClickRow(sl_uint32 row, const UIPoint& pt)
{
}

void ListReportView::onDoubleClickRow(sl_uint32 row, const UIPoint& pt)
{
}

void ListReportView::dispatchSelectRow(sl_uint32 row)
{
	onSelectRow(row);
	PtrLocker<IListReportViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectRow(this, row);
	}
}

void ListReportView::dispatchClickRow(sl_uint32 row, const UIPoint& pt)
{
	Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::Unknown, (sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), Time::zero());
	if (ev.isNotNull()) {
		dispatchClick(ev.get());
	}
	onClickRow(row, pt);
	PtrLocker<IListReportViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onClickRow(this, row, pt);
	}
}

void ListReportView::dispatchRightButtonClickRow(sl_uint32 row, const UIPoint& pt)
{
	PtrLocker<IListReportViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onRightButtonClickRow(this, row, pt);
	}
}

void ListReportView::dispatchDoubleClickRow(sl_uint32 row, const UIPoint& pt)
{
	Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::LeftButtonDoubleClick, (sl_real)(pt.x), (sl_real)(pt.y), Time::zero());
	if (ev.isNotNull()) {
		dispatchMouseEvent(ev.get());
	}
	onDoubleClickRow(row, pt);
	PtrLocker<IListReportViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onDoubleClickRow(this, row, pt);
	}
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_WIN32))

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

SLIB_UI_NAMESPACE_END
