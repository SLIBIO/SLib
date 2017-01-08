#ifndef CHECKHEADER_SLIB_UI_LIST_REPORT_VIEW
#define CHECKHEADER_SLIB_UI_LIST_REPORT_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class ListReportView;

class SLIB_EXPORT IListReportViewListener
{
public:
	virtual void onSelectRow(ListReportView* view, sl_uint32 row);
	
	virtual void onClickRow(ListReportView* view, sl_uint32 row, const Point& pt);
	
	virtual void onRightButtonClickRow(ListReportView* view, sl_uint32 row, const UIPoint& pt);
	
	virtual void onDoubleClickRow(ListReportView* view, sl_uint32 row, const UIPoint& pt);

};

class ListReportViewColumn
{
public:
	AtomicString title;
	sl_ui_len width;
	Alignment align;
	Alignment headerAlign;

public:
	ListReportViewColumn();
};

class ListReportViewCell
{
public:
	AtomicString text;
};

class SLIB_EXPORT ListReportView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ListReportView();
	
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
	SLIB_PROPERTY(AtomicPtr<IListReportViewListener>, Listener)
	
protected:
	virtual void onSelectRow(sl_uint32 row);
	
	virtual void onClickRow(sl_uint32 row, const UIPoint& pt);
	
	virtual void onRightButtonClickRow(sl_uint32 row, const UIPoint& pt);
	
	virtual void onDoubleClickRow(sl_uint32 row, const UIPoint& pt);
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	void dispatchSelectRow(sl_uint32 row);
	
	void dispatchClickRow(sl_uint32 row, const UIPoint& pt);
	
	void dispatchRightButtonClickRow(sl_uint32 row, const UIPoint& pt);
	
	void dispatchDoubleClickRow(sl_uint32 row, const UIPoint& pt);

protected:
	void _refreshColumnsCount_NW();
	
	void _refreshRowsCount_NW();
	
	void _setHeaderText_NW(sl_uint32 col, const String& text);
	
	void _setColumnWidth_NW(sl_uint32 col, sl_ui_len width);
	
	void _setHeaderAlignment_NW(sl_uint32 col, Alignment align);
	
	void _setColumnAlignment_NW(sl_uint32 col, Alignment align);
	
	void _getSelectedRow_NW();
	
	// override
	void _setFont_NW(const Ref<Font>& font);

protected:
	CList<ListReportViewColumn> m_columns;
	sl_uint32 m_nRows;
	CList< List<ListReportViewCell> > m_cells;
	sl_int32 m_selectedRow;
	
};

SLIB_UI_NAMESPACE_END

#endif
