#include "../../../inc/slib/ui/s_tab_view.h"

#define MAX_TABS_COUNT 100

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(STabView, SView)

STabView::STabView()
{
	setGroup(sl_true);
	setOpaque(sl_true);

	m_items.setCount(1);
	m_indexSelected = 0;
	m_indexHover = -1;
	
	setBackgroundColor(Color::White);
	setTabBarBackgroundColor(Color(230, 230, 230));
	setSelectedTabBackgroundColor(Color(150, 150, 150));
	setHoverTabBackgroundColor(Color(210, 210, 210));
	m_tabLabelColor = Color(50, 50, 50);
	m_selectedTabLabelColor = Color::Black;
	m_hoverTabLabelColor = Color(0, 20, 250);

	m_flagVertical = sl_false;
	m_tabWidth = 90;
	m_tabHeight = 24;
	
	setClickEnabled(sl_true);
}

sl_uint32 STabView::getTabsCount()
{
	return (sl_uint32)(m_items.getCount());
}

void STabView::setTabsCount(sl_uint32 nNew)
{
	ObjectLocker lock(this);
	ListLocker<STabViewItem> items(m_items);
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
			STabViewItem& item = items[i-1];
			removeChild(item.contentView);
		}
	}
	m_items.setCount(nNew);
	selectTab(m_indexSelected);
}

String STabView::getTabLabel(sl_uint32 index)
{
	ObjectLocker lock(this);
	if (index < m_items.getCount()) {
		STabViewItem* item = m_items.getItemPtr(index);
		return item->label;
	}
	return String::null();
}

void STabView::setTabLabel(sl_uint32 index, const String& text)
{
	ObjectLocker lock(this);
	if (index < m_items.getCount()) {
		STabViewItem* item = m_items.getItemPtr(index);
		item->label = text;
		invalidateTabBar();
	}
}

Ref<View> STabView::getTabContentView(sl_uint32 index)
{
	ObjectLocker lock(this);
	if (index < m_items.getCount()) {
		STabViewItem* item = m_items.getItemPtr(index);
		return item->contentView;
	}
	return Ref<View>::null();
}

void STabView::setTabContentView(sl_uint32 index, const Ref<View>& view)
{
	MutexLocker lock(m_items.getLocker());
	if (index < m_items.getCount()) {
		STabViewItem* item = m_items.getItemPtr(index);
		if (item->contentView != view) {
			removeChild(item->contentView);
			addChild(view);
			item->contentView = view;
			view->setFrame(getContentBounds());
			selectTab(m_indexSelected);
		}
	}
}

sl_uint32 STabView::getSelectedTabIndex()
{
	return m_indexSelected;
}

void STabView::selectTab(sl_uint32 index)
{
	ObjectLocker lock(this);
	ListLocker<STabViewItem> items(m_items);
	if (index >= items.count) {
		index = 0;
	}
	m_indexSelected = index;
	for (sl_size i = 0; i < items.count; i++) {
		Ref<View> view = items[i].contentView;
		if (view.isNotNull()) {
			if (i == index) {
				view->setVisible(sl_true);
			} else {
				view->setVisible(sl_false);
			}
		}
	}
	invalidate();
}

Size STabView::getContentViewSize()
{
	return getContentBounds().getSize();
}

Ref<Font> STabView::getFont()
{
	return m_font;
}

void STabView::setFont(const Ref<Font>& font)
{
	m_font = font;
	invalidateTabBar();
}

sl_bool STabView::isVerticalTabBar()
{
	return m_flagVertical;
}

void STabView::setVerticalTabBar(sl_bool flag)
{
	m_flagVertical = flag;
	relayout();
}

sl_real STabView::getTabWidth()
{
	return m_tabWidth;
}

void STabView::setTabWidth(sl_real width)
{
	m_tabWidth = width;
	relayout();
}

sl_real STabView::getTabHeight()
{
	return m_tabHeight;
}

void STabView::setTabHeight(sl_real height)
{
	m_tabHeight = height;
	relayout();
}

Ref<Drawable> STabView::getTabBarBackground()
{
	return m_tabBarBackground;
}

void STabView::setTabBarBackground(const Ref<Drawable>& drawable)
{
	m_tabBarBackground = drawable;
	invalidateTabBar();
}

void STabView::setTabBarBackgroundColor(const Color& color)
{
	setTabBarBackground(Drawable::createColorDrawable(color));
}

Ref<Drawable> STabView::getSelectedTabBackground()
{
	return m_selectedTabBackground;
}

void STabView::setSelectedTabBackground(const Ref<Drawable>& drawable)
{
	m_selectedTabBackground = drawable;
	invalidateTabBar();
}

void STabView::setSelectedTabBackgroundColor(const Color& color)
{
	setSelectedTabBackground(Drawable::createColorDrawable(color));
}

Ref<Drawable> STabView::getHoverTabBackground()
{
	return m_hoverTabBackground;
}

void STabView::setHoverTabBackground(const Ref<Drawable>& drawable)
{
	m_hoverTabBackground = drawable;
	invalidateTabBar();
}

void STabView::setHoverTabBackgroundColor(const Color& color)
{
	setHoverTabBackground(Drawable::createColorDrawable(color));
}

Color STabView::getTabLabelColor()
{
	return m_tabLabelColor;
}

void STabView::setTabLabelColor(const Color& color)
{
	m_tabLabelColor = color;
	invalidateTabBar();
}

Color STabView::getSelectedTabLabelColor()
{
	return m_selectedTabLabelColor;
}

void STabView::setSelectedTabLabelColor(const Color& color)
{
	m_selectedTabLabelColor = color;
	invalidateTabBar();
}

Color STabView::getHoverTabLabelColor()
{
	return m_hoverTabLabelColor;
}

void STabView::setHoverTabLabelColor(const Color& color)
{
	m_hoverTabLabelColor = color;
	invalidateTabBar();
}

Rectangle STabView::getTabBarRegion()
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

Rectangle STabView::getTabRegion(sl_uint32 index)
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

Rectangle STabView::getContentBounds()
{
	Rectangle ret = getFrameBounds();
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

void STabView::invalidateTabBar()
{
	invalidate(getTabBarRegion());
}

void STabView::relayout()
{
	ObjectLocker lock(this);
	Rectangle bound = getContentBounds();
	ListLocker<STabViewItem> items(m_items);
	for (sl_size i = 0; i < items.count; i++) {
		Ref<View> view = items[i].contentView;
		if (view.isNotNull()) {
			view->setFrame(bound);
		}
	}
	invalidate();
}

void STabView::onSelectTab(sl_uint32 index)
{
	PtrLocker<STabViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectTab(this, index);
	}
}

void STabView::onClickView(UIEvent* ev)
{
	Point pt = ev->getPoint();
	ObjectLocker lock(this);
	ListLocker<STabViewItem> items(m_items);
	sl_uint32 n = (sl_uint32)(items.count);
	for (sl_uint32 i = 0; i < n; i++) {
		if (getTabRegion(i).containsPoint(pt)) {
			selectTab(i);
			onSelectTab(i);
			return;
		}
	}
}

void STabView::onMouseEvent(UIEvent* ev)
{
	SView::onMouseEvent(ev);
	if (ev->getAction() == UIAction::MouseLeave) {
		m_indexHover = -1;
		invalidateTabBar();
	} else if (ev->getAction() == UIAction::MouseMove) {
		Point pt = ev->getPoint();
		ObjectLocker lock(this);
		ListLocker<STabViewItem> items(m_items);
		sl_uint32 n = (sl_uint32)(items.count);
		for (sl_uint32 i = 0; i < n; i++) {
			if (getTabRegion(i).containsPoint(pt)) {
				if (i != m_indexHover) {
					m_indexHover = i;
					invalidateTabBar();
				}
				return;
			}
		}
		if (-1 != m_indexHover) {
			m_indexHover = -1;
			invalidateTabBar();
		}
	}
}

void STabView::onDraw(Canvas* canvas)
{
	onDrawTabBarBackground(canvas);
	ObjectLocker lock(this);
	ListLocker<STabViewItem> items(m_items);
	sl_uint32 n = (sl_uint32)(items.count);
	for (sl_uint32 i = 0; i < n; i++) {
		onDrawTab(canvas, getTabRegion(i), i, items[i].label);
	}
}

void STabView::onDrawTabBarBackground(Canvas* canvas)
{
	canvas->draw(getTabBarRegion(), m_tabBarBackground);
}

void STabView::onDrawTab(Canvas* canvas, const Rectangle& rect, sl_uint32 index, const String& label)
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
		Ref<Font> font = _getFont();
		if (m_flagVertical) {
			canvas->drawText(label, rc, font, labelColor, Alignment::MiddleLeft);
		} else {
			canvas->drawText(label, rc, font, labelColor, Alignment::MiddleCenter);
		}
	}
}

Ref<Font> STabView::_getFont()
{
	Ref<Font> ret = m_font;
	if (ret.isNull()) {
		ret = Font::getDefault();
	}
	return ret;
}

SLIB_UI_NAMESPACE_END
