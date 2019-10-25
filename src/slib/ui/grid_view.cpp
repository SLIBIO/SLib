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

#include "slib/ui/grid_view.h"

#include "slib/core/math.h"
#include "slib/core/scoped.h"

namespace slib
{

	namespace priv
	{
		namespace grid_view
		{

			class Cell
			{
			public:
				Ref<View> view;
				sl_uint32 rowspan;
				sl_uint32 colspan;

			public:
				Cell()
				{
					rowspan = 1;
					colspan = 1;
				}
			};

			class Column
			{
			public:
				SizeMode widthMode;
				sl_ui_len widthLayout;
				sl_ui_len widthFixed;
				sl_real widthWeight;

				sl_ui_len minWidth;
				sl_ui_len maxWidth;
				sl_bool flagMaxWidthDefined;

			public:
				Column()
				{
					widthMode = SizeMode::Filling;
					widthLayout = 0;
					widthFixed = 0;
					widthWeight = 1;

					minWidth = 0;
					maxWidth = 0;
					flagMaxWidthDefined = sl_false;
				}

			public:
				SLIB_INLINE sl_ui_len restrictWidth(sl_ui_len width)
				{
					if (width < minWidth) {
						return minWidth;
					}
					if (flagMaxWidthDefined) {
						if (width > maxWidth) {
							return maxWidth;
						}
					}
					return width;
				}

				SLIB_INLINE sl_ui_len getFixedWidth()
				{
					return restrictWidth(widthFixed);
				}

				SLIB_INLINE sl_ui_len getWeightWidth(sl_ui_len widthParent)
				{
					return restrictWidth((sl_ui_len)(widthParent * widthWeight));
				}

			};

			class Row
			{
			public:
				SizeMode heightMode;
				sl_ui_len heightLayout;
				sl_ui_len heightFixed;
				sl_real heightWeight;

				sl_ui_len minHeight;
				sl_ui_len maxHeight;
				sl_bool flagMaxHeightDefined;

				CList<Cell> cells;

			public:
				Row()
				{
					heightMode = SizeMode::Filling;
					heightLayout = 0;
					heightFixed = 0;
					heightWeight = 1;

					minHeight = 0;
					maxHeight = 0;
					flagMaxHeightDefined = sl_false;
				}

			public:
				SLIB_INLINE sl_ui_len restrictHeight(sl_ui_len height)
				{
					if (height < minHeight) {
						return minHeight;
					}
					if (flagMaxHeightDefined) {
						if (height > maxHeight) {
							return maxHeight;
						}
					}
					return height;
				}

				SLIB_INLINE sl_ui_len getFixedHeight()
				{
					return restrictHeight(heightFixed);
				}

				SLIB_INLINE sl_ui_len getWeightHeight(sl_ui_len heightParent)
				{
					return restrictHeight((sl_ui_len)(heightParent * heightWeight));
				}

			};

		}
	}

	using namespace priv::grid_view;

	SLIB_DEFINE_OBJECT(GridView, ViewGroup)

	GridView::GridView()
	{
		setCustomLayout(sl_true);
		setSavingCanvasState(sl_false);
	}

	GridView::~GridView()
	{
	}

	void GridView::init()
	{
		ViewGroup::init();
	}

	sl_uint32 GridView::getColumnsCount()
	{
		ObjectLocker lock(this);
		return (sl_uint32)(m_columns.getCount());
	}

	void GridView::setColumnsCount(sl_uint32 nColumns, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		sl_uint32 nColumnsOld = (sl_uint32)(m_columns.getCount());
		if (nColumnsOld == nColumns) {
			return;
		}
		if (nColumnsOld < nColumns) {
			m_columns.setCount_NoLock(nColumns);
			return;
		}
		sl_bool flagRemovedChild = sl_false;
		UIUpdateMode modeNone = (mode == UIUpdateMode::Init) ? (UIUpdateMode::Init) : (UIUpdateMode::None);
		ListElements<Row> rows(m_rows);
		for (sl_size i = 0; i < rows.count; i++) {
			Row& row = rows[i];
			ListElements<Cell> cells(row.cells);
			if (cells.count > nColumns) {
				for (sl_size k = nColumns; k < cells.count; k++) {
					Ref<View>& view = cells[k].view;
					if (view.isNotNull()) {
						removeChild(view, modeNone);
						flagRemovedChild = sl_true;
					}
				}
				row.cells.setCount_NoLock(nColumns);
			}
		}
		m_columns.setCount_NoLock(nColumns);
		if (flagRemovedChild) {
			invalidateLayout(mode);
		}
	}

	SizeMode GridView::getColumnWidthMode(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->widthMode;
		}
		return SizeMode::Filling;
	}

	sl_ui_len GridView::getColumnWidth(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			if (col->widthMode == SizeMode::Fixed) {
				return col->getFixedWidth();
			} else {
				return col->widthLayout;
			}
		}
		return 0;
	}

	void GridView::setColumnWidth(sl_uint32 iCol, sl_ui_len width, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			if (width < 0) {
				width = 0;
			}
			col->widthFixed = width;
			col->widthMode = SizeMode::Fixed;
			invalidateLayout(mode);
		}
	}

	sl_bool GridView::isColumnWidthFixed(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->widthMode == SizeMode::Fixed;
		}
		return sl_false;
	}

	sl_real GridView::getColumnWidthWeight(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->widthWeight;
		}
		return sl_false;
	}
	
	sl_bool GridView::isColumnWidthFilling(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->widthMode == SizeMode::Filling;
		}
		return sl_false;
	}
	
	void GridView::setColumnWidthFilling(sl_uint32 iCol, sl_real weight, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			if (weight < 0) {
				weight = 0;
			}
			col->widthWeight = weight;
			col->widthMode = SizeMode::Filling;
			invalidateLayout(mode);
		}
	}
	
	sl_bool GridView::isColumnWidthWrapping(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->widthMode == SizeMode::Wrapping;
		}
		return sl_false;
	}
	
	void GridView::setColumnWidthWrapping(sl_uint32 iCol, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			col->widthMode = SizeMode::Wrapping;
			invalidateLayout(mode);
		}
	}
	
	sl_bool GridView::isColumnWidthWeight(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->widthMode == SizeMode::Weight;
		}
		return sl_false;
	}
	
	void GridView::setColumnWidthWeight(sl_uint32 iCol, sl_real weight, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			if (weight < 0) {
				weight = 0;
			}
			col->widthWeight = weight;
			col->widthMode = SizeMode::Weight;
			invalidateLayout(mode);
		}
	}
	
	sl_ui_len GridView::getColumnMinimumWidth(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->minWidth;
		}
		return 0;
	}
	
	void GridView::setColumnMinimumWidth(sl_uint32 iCol, sl_ui_len width, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			if (width < 0) {
				width = 0;
			}
			col->minWidth = width;
			invalidateLayout(mode);
		}
	}
	
	sl_bool GridView::isColumnMaximumWidthDefined(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->flagMaxWidthDefined;
		}
		return 0;
	}

	sl_ui_len GridView::getColumnMaximumWidth(sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			return col->maxWidth;
		}
		return 0;
	}
	
	void GridView::setColumnMaximumWidth(sl_uint32 iCol, sl_ui_len width, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Column* col = m_columns.getPointerAt(iCol);
		if (col) {
			if (width < 0) {
				width = 0;
			}
			col->maxWidth = width;
			col->flagMaxWidthDefined = sl_true;
			invalidateLayout(mode);
		}
	}
	
	sl_uint32 GridView::getRowsCount()
	{
		ObjectLocker lock(this);
		return (sl_uint32)(m_rows.getCount());
	}
	
	void GridView::setRowsCount(sl_uint32 nRows, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		sl_uint32 nRowsOld = (sl_uint32)(m_rows.getCount());
		if (nRowsOld == nRows) {
			return;
		}
		if (nRowsOld < nRows) {
			m_rows.setCount_NoLock(nRows);
			return;
		}
		sl_bool flagRemovedChild = sl_false;
		UIUpdateMode modeNone = (mode == UIUpdateMode::Init) ? (UIUpdateMode::Init) : (UIUpdateMode::None);
		ListElements<Row> rows(m_rows);
		for (sl_size i = nRows; i < rows.count; i++) {
			Row& row = rows[i];
			ListElements<Cell> cells(row.cells);
			for (sl_size k = 0; k < cells.count; k++) {
				Ref<View>& view = cells[k].view;
				if (view.isNotNull()) {
					removeChild(view, modeNone);
					flagRemovedChild = sl_true;
				}
			}
		}
		m_rows.setCount_NoLock(nRows);
		if (flagRemovedChild) {
			invalidateLayout(mode);
		}
	}

	SizeMode GridView::getRowHeightMode(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->heightMode;
		}
		return SizeMode::Filling;
	}

	sl_ui_len GridView::getRowHeight(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			if (row->heightMode == SizeMode::Fixed) {
				return row->getFixedHeight();
			} else {
				return row->heightLayout;
			}
		}
		return 0;
	}

	void GridView::setRowHeight(sl_uint32 iRow, sl_ui_len height, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			if (height < 0) {
				height = 0;
			}
			row->heightFixed = height;
			row->heightMode = SizeMode::Fixed;
			invalidateLayout(mode);
		}
	}

	sl_bool GridView::isRowHeightFixed(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->heightMode == SizeMode::Fixed;
		}
		return sl_false;
	}
	
	sl_real GridView::getRowHeightWeight(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->heightWeight;
		}
		return 0;
	}
	
	sl_bool GridView::isRowHeightFilling(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->heightMode == SizeMode::Filling;
		}
		return sl_false;
	}
	
	void GridView::setRowHeightFilling(sl_uint32 iRow, sl_real weight, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			if (weight < 0) {
				weight = 0;
			}
			row->heightWeight = weight;
			row->heightMode = SizeMode::Filling;
			invalidateLayout(mode);
		}
	}

	sl_bool GridView::isRowHeightWrapping(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->heightMode == SizeMode::Wrapping;
		}
		return sl_false;
	}
	
	void GridView::setRowHeightWrapping(sl_uint32 iRow, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			row->heightMode = SizeMode::Wrapping;
			invalidateLayout(mode);
		}
	}
	
	sl_bool GridView::isRowHeightWeight(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->heightMode == SizeMode::Weight;
		}
		return sl_false;
	}
	
	void GridView::setRowHeightWeight(sl_uint32 iRow, sl_real weight, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			if (weight < 0) {
				weight = 0;
			}
			row->heightWeight = weight;
			row->heightMode = SizeMode::Weight;
			invalidateLayout(mode);
		}
	}
	
	sl_ui_len GridView::getRowMinimumHeight(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->minHeight;
		}
		return 0;
	}
	
	void GridView::setRowMinimumHeight(sl_uint32 iRow, sl_ui_len height, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			if (height < 0) {
				height = 0;
			}
			row->minHeight = height;
			invalidateLayout(mode);
		}
	}

	sl_bool GridView::isRowMaximumHeightDefined(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->flagMaxHeightDefined;
		}
		return sl_false;
	}

	sl_ui_len GridView::getRowMaximumHeight(sl_uint32 iRow)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->maxHeight;
		}
		return 0;
	}
	
	void GridView::setRowMaximumHeight(sl_uint32 iRow, sl_ui_len height, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			if (height < 0) {
				height = 0;
			}
			row->maxHeight = height;
			row->flagMaxHeightDefined = sl_true;
			invalidateLayout(mode);
		}
	}

	Cell* GridView::_getCell(sl_uint32 iRow, sl_uint32 iCol)
	{
		Row* row = m_rows.getPointerAt(iRow);
		if (row) {
			return row->cells.getPointerAt(iCol);
		}
		return sl_null;
	}

	Ref<View> GridView::getCell(sl_uint32 iRow, sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Cell* cell = _getCell(iRow, iCol);
		if (cell) {
			return cell->view.get();
		}
		return sl_null;
	}

	sl_uint32 GridView::getRowspan(sl_uint32 iRow, sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Cell* cell = _getCell(iRow, iCol);
		if (cell) {
			return cell->rowspan;
		}
		return 1;
	}

	sl_uint32 GridView::getColspan(sl_uint32 iRow, sl_uint32 iCol)
	{
		ObjectLocker lock(this);
		Cell* cell = _getCell(iRow, iCol);
		if (cell) {
			return cell->colspan;
		}
		return 1;
	}

	void GridView::setCell(sl_uint32 iRow, sl_uint32 iCol, const Ref<View>& view, UIUpdateMode mode)
	{
		setCell(iRow, iCol, view, 1, 1, mode);
	}

	void GridView::setCell(sl_uint32 iRow, sl_uint32 iCol, const Ref<View>& view, sl_uint32 rowspan, sl_uint32 colspan, UIUpdateMode mode)
	{
		if (rowspan < 1) {
			rowspan = 1;
		}
		if (colspan < 1) {
			colspan = 1;
		}
		ObjectLocker lock(this);
		if (iCol >= m_columns.getCount() || iRow >= m_rows.getCount()) {
			return;
		}
		Row* row = m_rows.getData() + iRow;
		Cell* cell;
		if (iCol < row->cells.getCount()) {
			cell = row->cells.getData() + iCol;
		} else {
			if (view.isNotNull()) {
				if (row->cells.setCount_NoLock(iCol + 1)) {
					cell = row->cells.getData() + iCol;
				} else {
					return;
				}
			} else {
				return;
			}
		}
		if (cell->view.isNotNull()) {
			removeChild(cell->view, (mode == UIUpdateMode::Init) ? (UIUpdateMode::Init) : (UIUpdateMode::None));
		}
		cell->view = view;
		cell->colspan = colspan;
		cell->rowspan = rowspan;
		if (view.isNotNull()) {
			addChild(view, mode);
		}
	}
	
	void GridView::onUpdateLayout()
	{
		ObjectLocker lock(this);

		sl_uint32 nRows = (sl_uint32)(m_rows.getCount());
		sl_uint32 nCols = (sl_uint32)(m_columns.getCount());
		sl_bool flagWidthWrapping = isWidthWrapping();
		sl_bool flagHeightWrapping = isHeightWrapping();
		if (!nRows && !nCols) {
			if (flagWidthWrapping) {
				setLayoutWidth(getPaddingLeft() + getPaddingRight());
			}
			if (flagHeightWrapping) {
				setLayoutHeight(getPaddingTop() + getPaddingBottom());
			}
			return;
		}

		UIRect layoutFrameContainer = getLayoutFrame();
		sl_ui_len widthLayout = layoutFrameContainer.getWidth();
		sl_ui_len heightLayout = layoutFrameContainer.getHeight();
		sl_ui_len widthContainer = widthLayout - getPaddingLeft() - getPaddingTop();
		sl_ui_len heightContainer = heightLayout - getPaddingTop() - getPaddingBottom();

		sl_uint32 iRow, iCol;
		Row* rows = m_rows.getData();
		Column* cols = m_columns.getData();

		sl_uint32 nFillRows = 0;
		sl_uint32 nFillCols = 0;
		sl_ui_len sumWidth = 0;
		sl_ui_len sumHeight = 0;
		sl_real sumRowFillWeights = 0;
		sl_real sumColFillWeights = 0;
		sl_bool flagWrappingRows = sl_false;
		sl_bool flagWrappingCols = sl_false;
		SLIB_SCOPED_BUFFER(SizeMode, 64, colWidthModes, nCols)
		SLIB_SCOPED_BUFFER(SizeMode, 64, rowHeightModes, nRows)

		for (iRow = 0; iRow < nRows; iRow++) {
			Row& row = rows[iRow];
			if (flagHeightWrapping) {
				if (row.heightMode == SizeMode::Fixed) {
					rowHeightModes[iRow] = SizeMode::Fixed;
					row.heightLayout = row.getFixedHeight();
					sumHeight += row.heightLayout;
				} else {
					rowHeightModes[iRow] = SizeMode::Wrapping;
					row.heightLayout = 0;
					flagWrappingRows = sl_true;
				}
			} else {
				rowHeightModes[iRow] = row.heightMode;
				switch (row.heightMode) {
				case SizeMode::Fixed:
					row.heightLayout = row.getFixedHeight();
					sumHeight += row.heightLayout;
					break;
				case SizeMode::Weight:
					row.heightLayout = row.getWeightHeight(heightContainer);
					sumHeight += row.heightLayout;
					break;
				case SizeMode::Filling:
					nFillRows++;
					sumRowFillWeights += row.heightWeight;
					row.heightLayout = 0;
					break;
				case SizeMode::Wrapping:
					row.heightLayout = 0;
					flagWrappingRows = sl_true;
					break;
				}
			}
		}
		for (iCol = 0; iCol < nCols; iCol++) {
			Column& col = cols[iCol];
			if (flagWidthWrapping) {
				if (col.widthMode == SizeMode::Fixed) {
					colWidthModes[iCol] = SizeMode::Fixed;
					col.widthLayout = col.getFixedWidth();
					sumWidth += col.widthLayout;
				} else {
					colWidthModes[iCol] = SizeMode::Wrapping;
					col.widthLayout = 0;
					flagWrappingCols = sl_true;
				}
			} else {
				colWidthModes[iCol] = col.widthMode;
				switch (col.widthMode) {
				case SizeMode::Fixed:
					col.widthLayout = col.getFixedWidth();
					sumWidth += col.widthLayout;
					break;
				case SizeMode::Weight:
					col.widthLayout = col.getWeightWidth(widthContainer);
					sumWidth += col.widthLayout;
					break;
				case SizeMode::Filling:
					nFillCols++;
					sumColFillWeights += col.widthWeight;
					col.widthLayout = 0;
					break;
				case SizeMode::Wrapping:
					col.widthLayout = 0;
					flagWrappingCols = sl_true;
					break;
				}
			}
		}

		if (flagWrappingCols) {
			UpdateLayoutFrameParam updateLayoutParam;
			updateLayoutParam.parentContentFrame.left = 0;
			updateLayoutParam.parentContentFrame.top = 0;
			updateLayoutParam.flagUseLayout = sl_true;
			updateLayoutParam.flagHorizontal = sl_true;
			updateLayoutParam.flagVertical = sl_false;
			for (iRow = 0; iRow < nRows; iRow++) {
				Row& row = rows[iRow];
				Cell* cells = row.cells.getData();
				sl_uint32 nCells = Math::min((sl_uint32)(row.cells.getCount()), nCols);
				for (iCol = 0; iCol < nCells; iCol++) {
					if (colWidthModes[iCol] == SizeMode::Wrapping) {
						Cell& cell = cells[iCol];
						Column& col = cols[iCol];
						View* view = cell.view.get();
						if (view && cell.colspan == 1) {
							SizeMode mode = view->getWidthMode();
							if (mode == SizeMode::Fixed || mode == SizeMode::Wrapping) {
								updateLayoutParam.parentContentFrame.right = col.widthLayout;
								updateLayoutParam.parentContentFrame.bottom = row.heightLayout;
								view->setInvalidateLayoutFrameInParent();
								view->updateLayoutFrameInParent(updateLayoutParam);
								sl_ui_len w = col.restrictWidth(view->getLayoutWidth() + view->getMarginLeft() + view->getMarginRight());
								if (w > col.widthLayout) {
									col.widthLayout = w;
								}
							}
						}
					}
				}
			}
			for (iCol = 0; iCol < nCols; iCol++) {
				Column& col = cols[iCol];
				if (colWidthModes[iCol] == SizeMode::Wrapping) {					
					sumWidth += col.widthLayout;
				}
			}
		}
		if (nFillCols) {
			if (sumWidth < 0) {
				sumWidth = 0;
			}
			sl_ui_len widthRemain;
			if (widthContainer > sumWidth) {
				widthRemain = widthContainer - sumWidth;
				if (widthRemain < 0) {
					widthRemain = 0;
				}
			} else {
				widthRemain = 0;
			}
			if (sumColFillWeights < SLIB_EPSILON) {
				sumColFillWeights = 1;
			}
			for (iCol = 0; iCol < nCols; iCol++) {
				Column& col = cols[iCol];
				if (colWidthModes[iCol] == SizeMode::Filling) {
					col.widthLayout = col.restrictWidth((sl_ui_len)(widthRemain * col.widthWeight / sumColFillWeights));
				}
			}
		}

		if (flagWrappingRows) {
			UpdateLayoutFrameParam updateLayoutParam;
			updateLayoutParam.parentContentFrame.left = 0;
			updateLayoutParam.parentContentFrame.top = 0;
			updateLayoutParam.flagUseLayout = sl_true;
			updateLayoutParam.flagHorizontal = sl_false;
			updateLayoutParam.flagVertical = sl_true;
			for (iRow = 0; iRow < nRows; iRow++) {
				Row& row = rows[iRow];
				if (rowHeightModes[iRow] == SizeMode::Wrapping) {
					Cell* cells = row.cells.getData();
					sl_uint32 nCells = Math::min((sl_uint32)(row.cells.getCount()), nCols);
					for (iCol = 0; iCol < nCells; iCol++) {
						Cell& cell = cells[iCol];
						Column& col = cols[iCol];
						View* view = cell.view.get();
						if (view && cell.rowspan == 1) {
							SizeMode mode = view->getHeightMode();
							if (mode == SizeMode::Fixed || mode == SizeMode::Wrapping) {
								updateLayoutParam.parentContentFrame.right = col.widthLayout;
								updateLayoutParam.parentContentFrame.bottom = row.heightLayout;
								view->setInvalidateLayoutFrameInParent();
								view->updateLayoutFrameInParent(updateLayoutParam);
								sl_ui_len h = row.restrictHeight(view->getLayoutHeight() + view->getMarginTop() + view->getMarginBottom());
								if (h > row.heightLayout) {
									row.heightLayout = h;
								}
							}
						}
					}
				}
			}
			for (iRow = 0; iRow < nRows; iRow++) {
				Row& row = rows[iRow];
				if (rowHeightModes[iRow] == SizeMode::Wrapping) {
					sumHeight += row.heightLayout;
				}
			}
		}
		if (nFillRows) {
			if (sumHeight < 0) {
				sumHeight = 0;
			}
			sl_ui_len heightRemain;
			if (heightContainer > sumHeight) {
				heightRemain = heightContainer - sumHeight;
				if (heightRemain < 0) {
					heightRemain = 0;
				}
			} else {
				heightRemain = 0;
			}
			if (sumRowFillWeights < SLIB_EPSILON) {
				sumRowFillWeights = 1;
			}
			for (iRow = 0; iRow < nRows; iRow++) {
				Row& row = rows[iRow];
				if (rowHeightModes[iRow] == SizeMode::Filling) {
					row.heightLayout = row.restrictHeight((sl_ui_len)(heightRemain * row.heightWeight / sumRowFillWeights));
				}
			}
		}

		UpdateLayoutFrameParam updateLayoutParam;
		updateLayoutParam.flagUseLayout = sl_true;
		updateLayoutParam.flagHorizontal = sl_true;
		updateLayoutParam.flagVertical = sl_true;
		sl_ui_len y = 0;
		for (iRow = 0; iRow < nRows; iRow++) {
			Row& row = rows[iRow];
			Cell* cells = row.cells.getData();
			sl_uint32 nCells = Math::min((sl_uint32)(row.cells.getCount()), nCols);
			sl_ui_len x = 0;
			for (iCol = 0; iCol < nCells; iCol++) {
				Cell& cell = cells[iCol];
				Column& col = cols[iCol];
				View* view = cell.view.get();
				if (view) {
					updateLayoutParam.parentContentFrame.left = x;
					updateLayoutParam.parentContentFrame.top = y;
					updateLayoutParam.parentContentFrame.right = x + col.widthLayout;
					updateLayoutParam.parentContentFrame.bottom = y + row.heightLayout;
					if (cell.colspan > 1) {
						for (sl_uint32 k = 1; k < cell.colspan; k++) {
							if (iCol + k >= nCols) {
								break;
							}
							updateLayoutParam.parentContentFrame.right += cols[iCol + k].widthLayout;
						}
					}
					if (cell.rowspan > 1) {
						for (sl_uint32 k = 1; k < cell.rowspan; k++) {
							if (iRow + k >= nRows) {
								break;
							}
							updateLayoutParam.parentContentFrame.bottom += rows[iRow + k].heightLayout;
						}
					}
					view->setInvalidateLayoutFrameInParent();
					view->updateLayoutFrameInParent(updateLayoutParam);
				}
				x += col.widthLayout;
			}
			y += row.heightLayout;
		}
		if (isWidthWrapping()) {
			sl_ui_len x = 0;
			for (iCol = 0; iCol < nCols; iCol++) {
				Column& col = cols[iCol];
				x += col.widthLayout;
			}
			setLayoutWidth(x);
		}
		if (isHeightWrapping()) {
			setLayoutHeight(y);
		}
	}
	
}
