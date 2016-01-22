#include "../../../inc/slib/ui/drop_down_list.h"

SLIB_UI_NAMESPACE_BEGIN

DropDownList::DropDownList()
{
	m_indexSelected = 0;
}

sl_uint32 DropDownList::getItemsCount()
{
	return (sl_uint32)(m_titles.count());
}

void DropDownList::setItemsCount(sl_uint32 n)
{
	m_values.setCount(n);
	m_titles.setCount(n);
	_refreshItemsCount();
	if (m_indexSelected >= n) {
		selectIndex(0);
	}
}

void DropDownList::removeAllItems()
{
	setItemsCount(0);
}

String DropDownList::getItemValue(sl_uint32 index)
{
	return m_values.getItemValue(index, String::null());
}

void DropDownList::setItemValue(sl_uint32 index, const String& value)
{
	m_values.setItem(index, value);
}

List<String> DropDownList::getValues()
{
	return m_values;
}

void DropDownList::setValues(const List<String>& list)
{
	m_values = list;
}

String DropDownList::getItemTitle(sl_uint32 index)
{
	return m_titles.getItemValue(index, String::null());
}

void DropDownList::setItemTitle(sl_uint32 index, const String& title)
{
	if (index < m_titles.count()) {
		m_titles.setItem(index, title);
		_setItemTitle(index, title);
	}
}

List<String> DropDownList::getTitles()
{
	return m_titles;
}

void DropDownList::setTitles(const List<String>& list)
{
	m_titles = list;
	_refreshItemsContent();
	sl_uint32 n = (sl_uint32)(m_titles.count());
	if (m_indexSelected >= n) {
		selectIndex(0);
	}
}

void DropDownList::selectIndex(sl_uint32 index)
{
	if (index < m_titles.count()) {
		_select(index);
		m_indexSelected = index;
	} else {
		if (index == 0) {
			m_indexSelected = 0;
		}
	}
}

void DropDownList::selectValue(const String& value)
{
	sl_int32 m = (sl_int32)(m_values.indexOf(value));
	if (m > 0) {
		selectIndex(m);
	}
}

String DropDownList::getSelectedValue()
{
	return m_values.getItemValue(m_indexSelected, String::null());
}

String DropDownList::getSelectedTitle()
{
	return m_titles.getItemValue(m_indexSelected, String::null());
}

Ref<Font> DropDownList::getFont()
{
	return m_font;
}

void DropDownList::onSelectItem(sl_uint32 index)
{
	PtrLocker<IDropDownListListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectItem(this, index);
	}
}

SLIB_UI_NAMESPACE_END
