#include "../../../inc/slib/ui/tab_view.h"

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(TabView, View)

#define MAX_TABS_COUNT 100

TabView::TabView()
{
	setCreatingNativeWidget(sl_true);
	setCreatingChildInstances(sl_true);
	
	setOpaque(sl_true, sl_false);
	
	m_items.setCount(1);
	m_indexSelected = 0;
	m_indexHover = -1;
	
	setBackgroundColor(Color::White, sl_false);
	setTabBarBackgroundColor(Color(230, 230, 230));
	setSelectedTabBackgroundColor(Color(150, 150, 150));
	setHoverTabBackgroundColor(Color(210, 210, 210));
	m_tabLabelColor = Color(50, 50, 50);
	m_selectedTabLabelColor = Color::Black;
	m_hoverTabLabelColor = Color(0, 20, 250);
	
	m_flagVertical = sl_false;
	m_tabWidth = 90;
	m_tabHeight = 24;
	
	setOccurringClick(sl_true);
	
}

sl_uint32 TabView::getTabsCount()
{
	return (sl_uint32)(m_items.getCount());
}

void TabView::setTabsCount(sl_uint32 nNew, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	ListLocker<TabViewItem> items(m_items);
	if (nNew < 1) {
		nNew = 1;
	}
	if (nNew > MAX_TABS_COUNT) {
		nNew = MAX_TABS_COUNT;
	}
	sl_uint32 nOrig = (sl_uint32)(items.count);
	if (nOrig == nNew) {
		return;
	}
	if (nOrig > nNew) {
		for (sl_uint32 i = nOrig; i > nNew; i--) {
			TabViewItem& item = items[i-1];
			removeChild(item.contentView, sl_false);
		}
	}
	m_items.setCount(nNew);
	if (isNativeWidget()) {
		_refreshTabsCount_NW();
	}
	selectTab(m_indexSelected, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

String TabView::getTabLabel(sl_uint32 index)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.getCount()) {
		TabViewItem* item = m_items.getItemPtr(index);
		return item->label;
	}
	return String::null();
}

void TabView::setTabLabel(sl_uint32 index, const String& text, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	if (index < m_items.getCount()) {
		TabViewItem* item = m_items.getItemPtr(index);
		item->label = text;
		if (isNativeWidget()) {
			_setTabLabel_NW(index, text);
		} else {
			if (flagRedraw) {
				_invalidateTabBar();
			}
		}
	}
}

Ref<View> TabView::getTabContentView(sl_uint32 index)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.getCount()) {
		TabViewItem* item = m_items.getItemPtr(index);
		return item->contentView;
	}
	return Ref<View>::null();
}

void TabView::setTabContentView(sl_uint32 index, const Ref<View>& view, sl_bool flagRedraw)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.getCount()) {
		TabViewItem* item = m_items.getItemPtr(index);
		if (item->contentView != view) {
			removeChild(item->contentView, sl_false);
			if (view.isNotNull()) {
				view->setFrame(getTabContentRegion(), sl_false);
				view->setCreatingInstance(sl_true);
				addChildNotAttach(view, sl_false);
			}
			item->contentView = view;
			if (isNativeWidget()) {
				if (UI::isUiThread()) {
					_setTabContentView_NW_OnUiThread(index, view);
				} else {
					UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(TabView, _setTabContentView_NW_OnUiThread, this, index, view));
				}
			} else {
				selectTab(m_indexSelected, sl_false);
				if (flagRedraw) {
					invalidate();
				}
			}
		}
	}
}

void TabView::_setTabContentView_NW_OnUiThread(sl_uint32 index, Ref<View> view)
{
	_setTabContentView_NW(index, view);
}

sl_uint32 TabView::getSelectedTabIndex()
{
	if (isNativeWidget()) {
		_getSelectedTabIndex_NW();
	}
	return m_indexSelected;
}

void TabView::selectTab(sl_uint32 index, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	ListLocker<TabViewItem> items(m_items);
	if (index >= items.count) {
		index = 0;
	}
	m_indexSelected = index;
	if (isNativeWidget()) {
		if (index < items.count) {
			_selectTab_NW(index);
		}
	} else {
		for (sl_size i = 0; i < items.count; i++) {
			Ref<View> view = items[i].contentView;
			if (view.isNotNull()) {
				if (i == index) {
					view->setVisible(sl_true, sl_false);
				} else {
					view->setVisible(sl_false, sl_false);
				}
			}
		}
		if (flagRedraw) {
			invalidate();
		}
	}
}

Size TabView::getContentViewSize()
{
	if (isNativeWidget()) {
		return _getContentViewSize_NW();
	}
	return getTabContentRegion().getSize();
}

sl_bool TabView::isVerticalTabBar()
{
	return m_flagVertical;
}

void TabView::setVerticalTabBar(sl_bool flag, sl_bool flagRedraw)
{
	m_flagVertical = flag;
	_relayout(flagRedraw);
}

sl_real TabView::getTabWidth()
{
	return m_tabWidth;
}

void TabView::setTabWidth(sl_real width, sl_bool flagRedraw)
{
	m_tabWidth = width;
	_relayout(flagRedraw);
}

sl_real TabView::getTabHeight()
{
	return m_tabHeight;
}

void TabView::setTabHeight(sl_real height, sl_bool flagRedraw)
{
	m_tabHeight = height;
	_relayout(flagRedraw);
}

Ref<Drawable> TabView::getTabBarBackground()
{
	return m_tabBarBackground;
}

void TabView::setTabBarBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_tabBarBackground = drawable;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

void TabView::setTabBarBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	setTabBarBackground(Drawable::createColorDrawable(color), flagRedraw);
}

Ref<Drawable> TabView::getSelectedTabBackground()
{
	return m_selectedTabBackground;
}

void TabView::setSelectedTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_selectedTabBackground = drawable;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

void TabView::setSelectedTabBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	setSelectedTabBackground(Drawable::createColorDrawable(color), flagRedraw);
}

Ref<Drawable> TabView::getHoverTabBackground()
{
	return m_hoverTabBackground;
}

void TabView::setHoverTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_hoverTabBackground = drawable;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

void TabView::setHoverTabBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	setHoverTabBackground(Drawable::createColorDrawable(color), flagRedraw);
}

Color TabView::getTabLabelColor()
{
	return m_tabLabelColor;
}

void TabView::setTabLabelColor(const Color& color, sl_bool flagRedraw)
{
	m_tabLabelColor = color;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

Color TabView::getSelectedTabLabelColor()
{
	return m_selectedTabLabelColor;
}

void TabView::setSelectedTabLabelColor(const Color& color, sl_bool flagRedraw)
{
	m_selectedTabLabelColor = color;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

Color TabView::getHoverTabLabelColor()
{
	return m_hoverTabLabelColor;
}

void TabView::setHoverTabLabelColor(const Color& color, sl_bool flagRedraw)
{
	m_hoverTabLabelColor = color;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

Rectangle TabView::getTabBarRegion()
{
	Size size = getSize();
	Rectangle ret;
	if (m_flagVertical) {
		ret.left = 0;
		ret.top = 0;
		ret.right = m_tabWidth;
		ret.bottom = size.y;
	} else {
		ret.left = 0;
		ret.top = 0;
		ret.right = size.x;
		ret.bottom = m_tabHeight;
	}
	return ret;
}

Rectangle TabView::getTabRegion(sl_uint32 index)
{
	Rectangle ret;
	sl_real f = (sl_real)index;
	if (m_flagVertical) {
		ret.left = 0;
		ret.top = f * m_tabHeight;
		ret.right = m_tabWidth;
		ret.bottom = (f + 1) * m_tabHeight;
	} else {
		ret.left = f * m_tabWidth;
		ret.top = 0;
		ret.right = (f + 1) * m_tabWidth;
		ret.bottom = m_tabHeight;
	}
	return ret;
}

Rectangle TabView::getTabContentRegion()
{
	Rectangle ret = getBounds();
	if (m_flagVertical) {
		ret.left += m_tabWidth + getPaddingLeft();
		ret.top += getPaddingTop();
		ret.right -= getPaddingRight();
		ret.bottom -= getPaddingBottom();
		return ret;
	} else {
		ret.left += getPaddingLeft();
		ret.top += m_tabHeight + getPaddingTop();
		ret.right -= getPaddingRight();
		ret.bottom -= getPaddingBottom();
		return ret;
	}
}

void TabView::_invalidateTabBar()
{
	invalidate(getTabBarRegion());
}

void TabView::_relayout(sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	Rectangle bound = getTabContentRegion();
	ListLocker<TabViewItem> items(m_items);
	for (sl_size i = 0; i < items.count; i++) {
		Ref<View> view = items[i].contentView;
		if (view.isNotNull()) {
			view->setFrame(bound, flagRedraw);
		}
	}
	if (flagRedraw) {
		invalidate();
	}
}

void TabView::dispatchSelectTab(sl_uint32 index)
{
	onSelectTab(index);
	PtrLocker<ITabViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectTab(this, index);
	}
}

void TabView::onSelectTab(sl_uint32 index)
{
}

void TabView::onClickView(UIEvent* ev)
{
	Point pt = ev->getPoint();
	ObjectLocker lock(this);
	ListLocker<TabViewItem> items(m_items);
	sl_uint32 n = (sl_uint32)(items.count);
	for (sl_uint32 i = 0; i < n; i++) {
		if (getTabRegion(i).containsPoint(pt)) {
			selectTab(i);
			onSelectTab(i);
			return;
		}
	}
}

void TabView::onMouseEvent(UIEvent* ev)
{
	if (ev->getAction() == UIAction::MouseLeave) {
		m_indexHover = -1;
		_invalidateTabBar();
	} else if (ev->getAction() == UIAction::MouseMove) {
		Point pt = ev->getPoint();
		ObjectLocker lock(this);
		ListLocker<TabViewItem> items(m_items);
		sl_uint32 n = (sl_uint32)(items.count);
		for (sl_uint32 i = 0; i < n; i++) {
			if (getTabRegion(i).containsPoint(pt)) {
				if (i != m_indexHover) {
					m_indexHover = i;
					_invalidateTabBar();
				}
				return;
			}
		}
		if (-1 != m_indexHover) {
			m_indexHover = -1;
			_invalidateTabBar();
		}
	}
}

void TabView::onDraw(Canvas* canvas)
{
	onDrawTabBarBackground(canvas);
	ObjectLocker lock(this);
	ListLocker<TabViewItem> items(m_items);
	sl_uint32 n = (sl_uint32)(items.count);
	for (sl_uint32 i = 0; i < n; i++) {
		onDrawTab(canvas, getTabRegion(i), i, items[i].label);
	}
}

void TabView::onDrawTabBarBackground(Canvas* canvas)
{
	canvas->draw(getTabBarRegion(), m_tabBarBackground);
}

void TabView::onDrawTab(Canvas* canvas, const Rectangle& rect, sl_uint32 index, const String& label)
{
	Rectangle rc = getTabRegion(index);
	Color labelColor;
	if (m_indexSelected == index) {
		canvas->draw(rc, m_selectedTabBackground);
		labelColor = m_selectedTabLabelColor;
	} else if (m_indexHover == index) {
		canvas->draw(rc, m_hoverTabBackground);
		labelColor = m_hoverTabLabelColor;
	} else {
		labelColor = m_tabLabelColor;
	}
	if (label.isNotEmpty()) {
		Ref<Font> font = getFont();
		if (m_flagVertical) {
			canvas->drawText(label, rc, font, labelColor, Alignment::MiddleLeft);
		} else {
			canvas->drawText(label, rc, font, labelColor, Alignment::MiddleCenter);
		}
	}
}

void TabView::onResize(sl_real width, sl_real height)
{
	if (isNativeWidget()) {
		_refreshSize_NW();
	}
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_WIN32))

Ref<ViewInstance> TabView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void TabView::_refreshTabsCount_NW()
{
}

void TabView::_refreshSize_NW()
{
}

void TabView::_setTabLabel_NW(sl_uint32 index, const String& text)
{
}

void TabView::_setTabContentView_NW(sl_uint32 index, const Ref<View>& view)
{
}

void TabView::_getSelectedTabIndex_NW()
{
}

Size TabView::_getContentViewSize_NW()
{
	return Size::zero();
}

void TabView::_selectTab_NW(sl_uint32 index)
{
}

void TabView::_setFont_NW(const Ref<Font>& font)
{
}

#endif

SLIB_UI_NAMESPACE_END
