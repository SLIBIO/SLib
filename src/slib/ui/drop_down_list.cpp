#include "../../../inc/slib/ui/drop_down_list.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(DropDownList, View)

DropDownList::DropDownList()
{
	setCreatingNativeWidget(sl_true);
	m_indexSelected = 0;
}

sl_uint32 DropDownList::getItemsCount()
{
	return (sl_uint32)(m_titles.getCount());
}

void DropDownList::setItemsCount(sl_uint32 n, sl_bool flagRedraw)
{
	m_values.setCount(n);
	m_titles.setCount(n);
	if (isNativeWidget()) {
		_refreshItemsCount_NW();
		if (m_indexSelected >= n) {
			selectIndex(0, sl_false);
		}
	} else {
		if (m_indexSelected >= n) {
			selectIndex(0, sl_false);
		}
		if (flagRedraw) {
			invalidate();
		}
	}
}

void DropDownList::removeAllItems(sl_bool flagRedraw)
{
	setItemsCount(0, flagRedraw);
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

void DropDownList::setItemTitle(sl_uint32 index, const String& title, sl_bool flagRedraw)
{
	if (index < m_titles.getCount()) {
		m_titles.setItem(index, title);
		if (isNativeWidget()) {
			_setItemTitle_NW(index, title);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

List<String> DropDownList::getTitles()
{
	return m_titles;
}

void DropDownList::setTitles(const List<String>& list, sl_bool flagRedraw)
{
	m_titles = list;
	if (isNativeWidget()) {
		_refreshItemsContent_NW();
		sl_uint32 n = (sl_uint32)(m_titles.getCount());
		if (m_indexSelected >= n) {
			selectIndex(0, sl_false);
		}
	} else {
		sl_uint32 n = (sl_uint32)(m_titles.getCount());
		if (m_indexSelected >= n) {
			selectIndex(0, sl_false);
		}
		if (flagRedraw) {
			invalidate();
		}
	}
}

void DropDownList::selectIndex(sl_uint32 index, sl_bool flagRedraw)
{
	if (index < m_titles.getCount()) {
		m_indexSelected = index;
		if (isNativeWidget()) {
			_select_NW(index);
		} else {
			if (flagRedraw) {
				invalidate();
			}
		}
	} else {
		if (index == 0) {
			m_indexSelected = 0;
		}
	}
}

void DropDownList::selectValue(const String& value, sl_bool flagRedraw)
{
	sl_int32 m = (sl_int32)(m_values.indexOf(value));
	if (m > 0) {
		selectIndex(m, flagRedraw);
	}
}

sl_uint32 DropDownList::getSelectedIndex()
{
	if (isNativeWidget()) {
		_getSelectedIndex_NW();
	}
	return m_indexSelected;
}

String DropDownList::getSelectedValue()
{
	return m_values.getItemValue(m_indexSelected, String::null());
}

String DropDownList::getSelectedTitle()
{
	return m_titles.getItemValue(m_indexSelected, String::null());
}

void DropDownList::onSelectItem(sl_uint32 index)
{
	PtrLocker<IDropDownListListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectItem(this, index);
	}
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> DropDownList::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void DropDownList::_getSelectedIndex_NW()
{
}

void DropDownList::_select_NW(sl_uint32 index)
{
}

void DropDownList::_refreshItemsCount_NW()
{
}

void DropDownList::_refreshItemsContent_NW()
{
}

void DropDownList::_setItemTitle_NW(sl_uint32 index, const String& title)
{
}

void DropDownList::_setFont_NW(const Ref<Font>& font)
{
}

#endif

SLIB_UI_NAMESPACE_END
