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

#ifndef CHECKHEADER_SLIB_UI_LIST_REPORT_VIEW
#define CHECKHEADER_SLIB_UI_LIST_REPORT_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{
	
	class ListReportViewColumn
	{
	public:
		AtomicString title;
		sl_ui_len width;
		Alignment align;
		Alignment headerAlign;
		
	public:
		ListReportViewColumn();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ListReportViewColumn)
		
	};
	
	class ListReportViewCell
	{
	public:
		AtomicString text;

	public:
		ListReportViewCell();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ListReportViewCell)

	};
	
	class SLIB_EXPORT ListReportView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ListReportView();
		
		~ListReportView();

	public:
		sl_uint32 getColumnsCount();
		
		virtual void setColumnsCount(sl_uint32 nCount, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_uint32 getRowsCount();
		
		virtual void setRowsCount(sl_uint32 nCount, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual String getItemText(sl_uint32 row, sl_uint32 col);
		
		virtual void setItemText(sl_uint32 row, sl_uint32 col, const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getHeaderText(sl_uint32 col);
		
		virtual void setHeaderText(sl_uint32 col, const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getColumnWidth(sl_uint32 col);
		
		virtual void setColumnWidth(sl_uint32 col, sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getHeaderAlignment(sl_uint32 col);
		
		virtual void setHeaderAlignment(sl_uint32 col, Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getColumnAlignment(sl_uint32 col);
		
		virtual void setColumnAlignment(sl_uint32 col, Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_int32 getSelectedRow();
		
		virtual void addRow(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void insertRow(sl_uint32 row, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void removeRow(sl_uint32 row, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void removeAllRows(UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(ListReportView, SelectRow, sl_uint32 row)
		SLIB_DECLARE_EVENT_HANDLER(ListReportView, ClickRow, sl_uint32 row, const UIPoint& pt)
		SLIB_DECLARE_EVENT_HANDLER(ListReportView, RightButtonClickRow, sl_uint32 row, const UIPoint& pt)
		SLIB_DECLARE_EVENT_HANDLER(ListReportView, DoubleClickRow, sl_uint32 row, const UIPoint& pt)
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
	protected:
		void _refreshColumnsCount_NW();
		
		void _refreshRowsCount_NW();
		
		void _setHeaderText_NW(sl_uint32 col, const String& text);
		
		void _setColumnWidth_NW(sl_uint32 col, sl_ui_len width);
		
		void _setHeaderAlignment_NW(sl_uint32 col, Alignment align);
		
		void _setColumnAlignment_NW(sl_uint32 col, Alignment align);
		
		void _getSelectedRow_NW();
		
		void _setFont_NW(const Ref<Font>& font) override;
		
	protected:
		CList<ListReportViewColumn> m_columns;
		sl_uint32 m_nRows;
		CList< List<ListReportViewCell> > m_cells;
		sl_int32 m_selectedRow;
		
	};	
}

#endif
