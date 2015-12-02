#ifndef CHECKHEADER_SLIB_UI_LIST_DETAILS_VIEW
#define CHECKHEADER_SLIB_UI_LIST_DETAILS_VIEW

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_OSX)
#define SLIB_UI_SUPPORT_NATIVE_LIST_DETAILS_VIEW
#endif

#if defined(SLIB_UI_SUPPORT_NATIVE_LIST_DETAILS_VIEW)

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class ListDetailsView;

class SLIB_EXPORT IListDetailsViewListener
{
public:
	virtual void onSelectRow(ListDetailsView* view, sl_uint32 row);
	virtual void onClickRow(ListDetailsView* view, sl_uint32 row, const Point& pt);
	virtual void onRightButtonClickRow(ListDetailsView* view, sl_uint32 row, const Point& pt);
	virtual void onDoubleClickRow(ListDetailsView* view, sl_uint32 row, const Point& pt);
	virtual sl_bool onGetCellText(ListDetailsView* view, sl_uint32 row, sl_uint32 col, String& outText);
};

struct ListDetailsViewColumn
{
	String title;
	sl_real width;
	Alignment align;
	Alignment headerAlign;

	ListDetailsViewColumn();
};

struct ListDetailsViewCell
{
	String text;
};

class SLIB_EXPORT ListDetailsView : public View
{
	SLIB_DECLARE_OBJECT(ListDetailsView, View)
public:
	ListDetailsView();
	
public:
	sl_uint32 getColumnsCount();
	virtual void setColumnsCount(sl_uint32 nCount);

	sl_uint32 getRowsCount();
	virtual void setRowsCount(sl_uint32 nCount);
	
	String getItemText(sl_uint32 row, sl_uint32 col);
	virtual void setItemText(sl_uint32 row, sl_uint32 col, const String& text);
	
	String getHeaderText(sl_uint32 col);
	virtual void setHeaderText(sl_uint32 col, const String& text);
	
	sl_real getColumnWidth(sl_uint32 col);
	virtual void setColumnWidth(sl_uint32 col, sl_real width);
	
	Alignment getHeaderAlignment(sl_uint32 col);
	virtual void setHeaderAlignment(sl_uint32 col, Alignment align);
	
	Alignment getColumnAlignment(sl_uint32 col);
	virtual void setColumnAlignment(sl_uint32 col, Alignment align);
	
	sl_int32 getSelectedRow();
	
	virtual void addRow();
	
	virtual void insertRow(sl_uint32 row);
	
	virtual void removeRow(sl_uint32 row);
	
	void removeAllRows();
	
	Ref<Font> getFont();
	virtual void setFont(const Ref<Font>& font);

public:
	SLIB_PROPERTY_INLINE(Ptr<IListDetailsViewListener>, Listener)
	
public:
	virtual void onSelectRow(sl_uint32 row);
	virtual void onClickRow(sl_uint32 row, const Point& pt);
	virtual void onRightButtonClickRow(sl_uint32 row, const Point& pt);
	virtual void onDoubleClickRow(sl_uint32 row, const Point& pt);
	virtual String onGetCellText(sl_uint32 row, sl_uint32 col);
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
protected:
	void _setHeaderText(sl_uint32 col, const String& text);
	void _setColumnWidth(sl_uint32 col, sl_real width);
	void _setHeaderAlignment(sl_uint32 col, Alignment align);
	void _setColumnAlignment(sl_uint32 col, Alignment align);
	void _refreshColumnsCount();
	void _refreshRowsCount();

protected:
	List<ListDetailsViewColumn> m_columns;
	sl_uint32 m_nRows;
	List< List<ListDetailsViewCell> > m_cells;
	
	Ref<Font> m_font;
	Ref<FontInstance> m_fontInstance;
};
SLIB_UI_NAMESPACE_END

#endif

#endif
