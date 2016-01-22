#include "../../../inc/slib/ui/tab_view.h"

#if defined(SLIB_UI_SUPPORT_NATIVE_TAB_VIEW)

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

TabView::TabView()
{
	m_items.setCount(1);
	m_indexSelected = 0;
}

sl_uint32 TabView::getTabsCount()
{
	return (sl_uint32)(m_items.count());
}

void TabView::setTabsCount(sl_uint32 nCount)
{
	ObjectLocker lock(this);
	m_items.setCount(nCount);
	_refreshTabsCount();
}

String TabView::getTabLabel(sl_uint32 index)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.count()) {
		TabViewItem* item = m_items.getItemPtr(index);
		return item->label;
	}
	return String::null();
}

void TabView::setTabLabel(sl_uint32 index, const String& text)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.count()) {
		TabViewItem* item = m_items.getItemPtr(index);
		item->label = text;
		_setTabLabel(index, text);
	}
}

Ref<View> TabView::getTabContentView(sl_uint32 index)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.count()) {
		TabViewItem* item = m_items.getItemPtr(index);
		return item->contentView;
	}
	return Ref<View>::null();
}

void TabView::setTabContentView(sl_uint32 index, const Ref<View>& view)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.count()) {
		TabViewItem* item = m_items.getItemPtr(index);
		if (item->contentView != view) {
			Ref<View> viewOld = item->contentView;
			if (viewOld.isNotNull()) {
				viewOld->detach();
				viewOld->removeParent(this);
			}
			item->contentView = view;
			view->setParent(this);
			if (UI::isUIThread()) {
				_setTabContentView(index, view);
			} else {
				UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(TabView, _setTabContentView_, this, index, view));
			}
		}
	}
}

void TabView::_setTabContentView_(sl_uint32 index, Ref<View> view)
{
	_setTabContentView(index, view);
}

void TabView::selectTab(sl_uint32 index)
{
	if (index < m_items.count()) {
		_selectTab(index);
		m_indexSelected = index;
	} else {
		if (index == 0) {
			m_indexSelected = 0;
		}
	}
}

Ref<Font> TabView::getFont()
{
	return m_font;
}

void TabView::onSelectTab(sl_uint32 index)
{
	PtrLocker<ITabViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectTab(this, index);
	}
}

SLIB_UI_NAMESPACE_END

#endif
