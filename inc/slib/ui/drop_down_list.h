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
	SLIB_DECLARE_OBJECT(DropDownList, View)
public:
	DropDownList();
	
public:
	sl_uint32 getItemsCount();
	
	virtual void setItemsCount(sl_uint32 n);
	
	void removeAllItems();
	
	
	String getItemValue(sl_uint32 index);
	
	virtual void setItemValue(sl_uint32 index, const String& value);
	
	List<String> getValues();
	
	virtual void setValues(const List<String>& values);
	
	
	String getItemTitle(sl_uint32 index);
	
	virtual void setItemTitle(sl_uint32 index, const String& title);
	
	List<String> getTitles();
	
	virtual void setTitles(const List<String>& values);
	
	
	virtual void selectIndex(sl_uint32 index);
	
	virtual void selectValue(const String& value);
	
	sl_uint32 getSelectedIndex();
	
	String getSelectedValue();
	
	String getSelectedTitle();
	
	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);

public:
	SLIB_PTR_PROPERTY(IDropDownListListener, Listener)
	
public:
	virtual void onSelectItem(sl_uint32 index);
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
private:
	void _select(sl_uint32 index);
	
	void _refreshItemsCount();
	
	void _refreshItemsContent();
	
	void _setItemTitle(sl_uint32 index, const String& title);

protected:
	SafeList<String> m_values;
	SafeList<String> m_titles;
	sl_uint32 m_indexSelected;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

SLIB_UI_NAMESPACE_END

#endif
