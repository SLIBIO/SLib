#include "../../../inc/slib/ui/list_details_view.h"

#if defined(SLIB_UI_SUPPORT_NATIVE_LIST_DETAILS_VIEW)

SLIB_UI_NAMESPACE_BEGIN

void IListDetailsViewListener::onSelectRow(ListDetailsView* view, sl_uint32 row)
{
}

void IListDetailsViewListener::onClickRow(ListDetailsView* view, sl_uint32 row, const Point& pt)
{
}

void IListDetailsViewListener::onRightButtonClickRow(ListDetailsView* view, sl_uint32 row, const Point& pt)
{
}

void IListDetailsViewListener::onDoubleClickRow(ListDetailsView* view, sl_uint32 row, const Point& pt)
{
}

ListDetailsViewColumn::ListDetailsViewColumn()
{
	width = 40;
	align = Alignment::MiddleCenter;
	headerAlign = Alignment::MiddleCenter;
}


SLIB_DEFINE_OBJECT(ListDetailsView, View)

ListDetailsView::ListDetailsView()
{
	setCreatingNativeWidget(sl_true);
	
	m_columns.setCount(1);
	m_nRows = 0;
	m_selectedRow = -1;
}

sl_uint32 ListDetailsView::getColumnsCount()
{
	return (sl_uint32)(m_columns.getCount());
}

void ListDetailsView::setColumnsCount(sl_uint32 nCount, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	m_columns.setCount(nCount);
	if (isNativeWidget()) {
		_refreshColumnsCount_NW();
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_uint32 ListDetailsView::getRowsCount()
{
	return m_nRows;
}

void ListDetailsView::setRowsCount(sl_uint32 nCount, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	if (nCount < m_cells.getCount()) {
		m_cells.setCount(nCount);
	}
	m_nRows = nCount;
	if (isNativeWidget()) {
		_refreshRowsCount_NW();
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

String ListDetailsView::getItemText(sl_uint32 iRow, sl_uint32 iCol)
{
	List<ListDetailsViewCell> row = m_cells.getItemValue(iRow, List<ListDetailsViewCell>::null());
	if (row.isNotNull()) {
		MutexLocker lock(row.getLocker());
		if (iCol < row.getCount()) {
			ListDetailsViewCell* cell = row.getItemPtr(iCol);
			return cell->text;
		}
	}
	return String::null();
}

void ListDetailsView::setItemText(sl_uint32 iRow, sl_uint32 iCol, const String& text, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	if (iRow < m_nRows) {
		if (iRow >= m_cells.getCount()) {
			if (!(m_cells.setCount(iRow + 1))) {
				return;
			}
		}
		List<ListDetailsViewCell> row = m_cells.getItemValue(iRow, List<ListDetailsViewCell>::null());
		if (row.isNull()) {
			row.setCount(iCol + 1);
			m_cells.setItem(iRow, row);
		}
		if (row.isNotNull()) {
			MutexLocker lock(row.getLocker());
			if (iCol >= row.getCount()) {
				if (!(row.setCount(iCol + 1))) {
					return;
				}
			}
			ListDetailsViewCell* cell = row.getItemPtr(iCol);
			cell->text = text;
		}
		if (flagRedraw) {
			if (isNativeWidget()) {
				_refreshRowsCount_NW();
			} else {
				invalidate();
			}
		}
	}
}

String ListDetailsView::getHeaderText(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->title;
	}
	return String::null();
}

void ListDetailsView::setHeaderText(sl_uint32 iCol, const String& text, sl_bool flagRedraw)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->title = text;
		if (isNativeWidget()) {
			_setHeaderText_NW(iCol, text);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

sl_real ListDetailsView::getColumnWidth(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->width;
	}
	return 0;
}

void ListDetailsView::setColumnWidth(sl_uint32 iCol, sl_real width, sl_bool flagRedraw)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->width = width;
		if (isNativeWidget()) {
			_setColumnWidth_NW(iCol, width);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

Alignment ListDetailsView::getHeaderAlignment(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->headerAlign;
	}
	return Alignment::Center;
}

void ListDetailsView::setHeaderAlignment(sl_uint32 iCol, Alignment align, sl_bool flagRedraw)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->headerAlign = align;
		if (isNativeWidget()) {
			_setHeaderAlignment_NW(iCol, align);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

Alignment ListDetailsView::getColumnAlignment(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->align;
	}
	return Alignment::Center;
}

void ListDetailsView::setColumnAlignment(sl_uint32 iCol, Alignment align, sl_bool flagRedraw)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.getCount()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->align = align;
		if (isNativeWidget()) {
			_setColumnAlignment_NW(iCol, align);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

sl_int32 ListDetailsView::getSelectedRow()
{
	if (isNativeWidget()) {
		_getSelectedRow_NW();
	}
	return m_selectedRow;
}

void ListDetailsView::addRow(sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	setRowsCount(m_nRows+1, flagRedraw);
}

void ListDetailsView::insertRow(sl_uint32 iRow, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	if (iRow < m_cells.getCount()) {
		m_cells.insert(iRow, List<ListDetailsViewCell>::null());
	}
	setRowsCount(m_nRows+1, flagRedraw);
}

void ListDetailsView::removeRow(sl_uint32 iRow, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	if (iRow < m_nRows) {
		if (iRow < m_cells.getCount()) {
			m_cells.remove(iRow);
		}
		setRowsCount(m_nRows - 1, flagRedraw);
	}
}

void ListDetailsView::removeAllRows(sl_bool flagRedraw)
{
	setRowsCount(0, flagRedraw);
}

void ListDetailsView::onSelectRow(sl_uint32 row)
{
}

void ListDetailsView::onClickRow(sl_uint32 row, const Point& pt)
{
}

void ListDetailsView::onRightButtonClickRow(sl_uint32 row, const Point& pt)
{
}

void ListDetailsView::onDoubleClickRow(sl_uint32 row, const Point& pt)
{
}

void ListDetailsView::dispatchSelectRow(sl_uint32 row)
{
	onSelectRow(row);
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectRow(this, row);
	}
}

void ListDetailsView::dispatchClickRow(sl_uint32 row, const Point& pt)
{
	Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::Unknown, pt.x, pt.y);
	if (ev.isNotNull()) {
		dispatchClick(ev.ptr);
	}
	onClickRow(row, pt);
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onClickRow(this, row, pt);
	}
}

void ListDetailsView::dispatchRightButtonClickRow(sl_uint32 row, const Point& pt)
{
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onRightButtonClickRow(this, row, pt);
	}
}

void ListDetailsView::dispatchDoubleClickRow(sl_uint32 row, const Point& pt)
{
	Ref<UIEvent> ev = UIEvent::createMouseEvent(UIAction::LeftButtonDoubleClick, pt.x, pt.y);
	if (ev.isNotNull()) {
		dispatchMouseEvent(ev.ptr);
	}
	onDoubleClickRow(row, pt);
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onDoubleClickRow(this, row, pt);
	}
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_WIN32))

Ref<ViewInstance> ListDetailsView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void ListDetailsView::_refreshColumnsCount_NW()
{
}

void ListDetailsView::_refreshRowsCount_NW()
{
}

void ListDetailsView::_setHeaderText_NW(sl_uint32 col, const String& text)
{
}

void ListDetailsView::_setColumnWidth_NW(sl_uint32 col, sl_real width)
{
}

void ListDetailsView::_setHeaderAlignment_NW(sl_uint32 col, Alignment align)
{
}

void ListDetailsView::_setColumnAlignment_NW(sl_uint32 col, Alignment align)
{
}

void ListDetailsView::_getSelectedRow_NW()
{
}

void ListDetailsView::_setFont_NW(const Ref<Font>& font)
{
}

#endif

SLIB_UI_NAMESPACE_END

#endif
