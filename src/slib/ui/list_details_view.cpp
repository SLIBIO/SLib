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

sl_bool IListDetailsViewListener::onGetCellText(ListDetailsView* view, sl_uint32 row, sl_uint32 col, String& outText)
{
	return sl_false;
}
SLIB_UI_NAMESPACE_END

SLIB_UI_NAMESPACE_BEGIN
ListDetailsViewColumn::ListDetailsViewColumn()
{
	width = 40;
	align = alignMiddleCenter;
	headerAlign = alignMiddleCenter;
}

ListDetailsView::ListDetailsView()
{
	m_columns.setCount(1);
	m_nRows = 0;
}

sl_uint32 ListDetailsView::getColumnsCount()
{
	return (sl_uint32)(m_columns.count());
}

void ListDetailsView::setColumnsCount(sl_uint32 nCount)
{
	ObjectLocker lock(this);
	m_columns.setCount(nCount);
	_refreshColumnsCount();
}

sl_uint32 ListDetailsView::getRowsCount()
{
	return m_nRows;
}

void ListDetailsView::setRowsCount(sl_uint32 nCount)
{
	ObjectLocker lock(this);
	if (nCount < m_cells.count()) {
		m_cells.setCount(nCount);
	}
	m_nRows = nCount;
	_refreshRowsCount();
}

String ListDetailsView::getItemText(sl_uint32 iRow, sl_uint32 iCol)
{
	List<ListDetailsViewCell> row = m_cells.getItemValue(iRow, List<ListDetailsViewCell>::null());
	if (row.isNotNull()) {
		MutexLocker lock(row.getLocker());
		if (iCol < row.count()) {
			ListDetailsViewCell* cell = row.getItemPtr(iCol);
			return cell->text;
		}
	}
	return String::null();
}

void ListDetailsView::setItemText(sl_uint32 iRow, sl_uint32 iCol, const String& text)
{
	ObjectLocker lock(this);
	if (iRow < m_nRows) {
		if (iRow >= m_cells.count()) {
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
			if (iCol >= row.count()) {
				if (!(row.setCount(iCol + 1))) {
					return;
				}
			}
			ListDetailsViewCell* cell = row.getItemPtr(iCol);
			cell->text = text;
		}
		_refreshRowsCount();
	}
}

String ListDetailsView::getHeaderText(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->title;
	}
	return String::null();
}

void ListDetailsView::setHeaderText(sl_uint32 iCol, const String& text)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->title = text;
		_setHeaderText(iCol, text);
	}
}

sl_real ListDetailsView::getColumnWidth(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->width;
	}
	return 0;
}

void ListDetailsView::setColumnWidth(sl_uint32 iCol, sl_real width)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->width = width;
		_setColumnWidth(iCol, width);
	}
}

Alignment ListDetailsView::getHeaderAlignment(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->headerAlign;
	}
	return alignCenter;
}

void ListDetailsView::setHeaderAlignment(sl_uint32 iCol, Alignment align)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->headerAlign = align;
		_setHeaderAlignment(iCol, align);
	}
}

Alignment ListDetailsView::getColumnAlignment(sl_uint32 iCol)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		return col->align;
	}
	return alignCenter;
}

void ListDetailsView::setColumnAlignment(sl_uint32 iCol, Alignment align)
{
	MutexLocker lock(m_columns.getLocker());
	if (iCol < m_columns.count()) {
		ListDetailsViewColumn* col = m_columns.getItemPtr(iCol);
		col->align = align;
		_setColumnAlignment(iCol, align);
	}
}

void ListDetailsView::addRow()
{
	ObjectLocker lock(this);
	setRowsCount(m_nRows+1);
}

void ListDetailsView::insertRow(sl_uint32 iRow)
{
	ObjectLocker lock(this);
	if (iRow < m_cells.count()) {
		m_cells.insert(iRow, List<ListDetailsViewCell>::null());
	}
	setRowsCount(m_nRows+1);
}

void ListDetailsView::removeRow(sl_uint32 iRow)
{
	ObjectLocker lock(this);
	if (iRow < m_nRows) {
		if (iRow < m_cells.count()) {
			m_cells.remove(iRow);
		}
		setRowsCount(m_nRows - 1);
	}
}

void ListDetailsView::removeAllRows()
{
	setRowsCount(0);
}

Ref<Font> ListDetailsView::getFont()
{
	return m_font;
}

void ListDetailsView::onSelectRow(sl_uint32 row)
{
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectRow(this, row);
	}
}

void ListDetailsView::onClickRow(sl_uint32 row, const Point& pt)
{
	onClick();
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onClickRow(this, row, pt);
	}
}

void ListDetailsView::onRightButtonClickRow(sl_uint32 row, const Point& pt)
{
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onRightButtonClickRow(this, row, pt);
	}
}

void ListDetailsView::onDoubleClickRow(sl_uint32 row, const Point& pt)
{
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onDoubleClickRow(this, row, pt);
	}
}

String ListDetailsView::onGetCellText(sl_uint32 row, sl_uint32 col)
{
	PtrLocker<IListDetailsViewListener> listener(getListener());
	if (listener.isNotNull()) {
		String text;
		if (listener->onGetCellText(this, row, col, text)) {
			return text;
		}
	}
	return getItemText(row, col);
}
SLIB_UI_NAMESPACE_END

#endif
