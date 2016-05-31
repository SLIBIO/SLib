#ifndef CHECKHEADER_SLIB_UI_DROP_DOWN_LIST
#define CHECKHEADER_SLIB_UI_DROP_DOWN_LIST

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class DropDownList;

class SLIB_EXPORT IDropDownListListener
{
public:
	virtual void onSelectItem(DropDownList* view, sl_uint32 index) = 0;
	
};

class SLIB_EXPORT DropDownList : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	DropDownList();
	
public:
	sl_uint32 getItemsCount();
	
	virtual void setItemsCount(sl_uint32 n, sl_bool flagRedraw = sl_true);
	
	void removeAllItems(sl_bool flagRedraw = sl_true);
	
	
	String getItemValue(sl_uint32 index);
	
	virtual void setItemValue(sl_uint32 index, const String& value);
	
	List<String> getValues();
	
	virtual void setValues(const List<String>& values);
	
	
	String getItemTitle(sl_uint32 index);
	
	virtual void setItemTitle(sl_uint32 index, const String& title, sl_bool flagRedraw = sl_true);
	
	List<String> getTitles();
	
	virtual void setTitles(const List<String>& values, sl_bool flagRedraw = sl_true);
	
	
	virtual void selectIndex(sl_uint32 index, sl_bool flagRedraw = sl_true);
	
	virtual void selectValue(const String& value, sl_bool flagRedraw = sl_true);
	
	sl_uint32 getSelectedIndex();
	
	String getSelectedValue();
	
	String getSelectedTitle();

public:
	SLIB_PTR_PROPERTY(IDropDownListListener, Listener)
	
public:
	virtual void onSelectItem(sl_uint32 index);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
private:
	void _getSelectedIndex_NW();
	
	void _select_NW(sl_uint32 index);
	
	void _refreshItemsCount_NW();
	
	void _refreshItemsContent_NW();
	
	void _setItemTitle_NW(sl_uint32 index, const String& title);
	
	// override
	void _setFont_NW(const Ref<Font>& font);

protected:
	SafeList<String> m_values;
	SafeList<String> m_titles;
	sl_uint32 m_indexSelected;
	
};

SLIB_UI_NAMESPACE_END

#endif
