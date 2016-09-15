#include "../../../inc/slib/ui/tab_view.h"

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(TabView, View)

#define MAX_TABS_COUNT 100

TabView::TabView()
{
	setCreatingNativeWidget(sl_true);
	setCreatingChildInstances(sl_true);
	setUsingFont(sl_true);
	
	m_items.setCount(1);
	m_indexSelected = 0;
	m_indexHover = -1;
	
	setBackgroundColor(Color::White, sl_false);
	setBarBackgroundColor(Color(230, 230, 230));
	setSelectedTabBackgroundColor(Color(150, 150, 150));
	setHoverTabBackgroundColor(Color(210, 210, 210));
	m_labelColor = Color(50, 50, 50);
	m_selectedLabelColor = Color::Black;
	m_hoverLabelColor = Color(0, 20, 250);
	
	m_orientation = LayoutOrientation::Horizontal;
	m_tabWidth = 90;
	m_tabHeight = 24;
	
	m_labelAlignment = Alignment::MiddleCenter;
	m_labelMarginLeft = 0;
	m_labelMarginTop = 0;
	m_labelMarginRight = 0;
	m_labelMarginBottom = 0;
	
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
				view->setAttachMode(UIAttachMode::NotAttachInNativeWidget);
				addChild(view, sl_false);
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

UISize TabView::getContentViewSize()
{
	if (isNativeWidget()) {
		UISize size = _getContentViewSize_NW();
		if (size.x < 0) {
			size.x = 0;
		}
		if (size.y < 0) {
			size.y = 0;
		}
		return size;
	}
	return getTabContentRegion().getSize();
}

LayoutOrientation TabView::getOrientation()
{
	return m_orientation;
}

void TabView::setOrientation(LayoutOrientation orientation, sl_bool flagRedraw)
{
	m_orientation = orientation;
	if (!(isNativeWidget())) {
		_relayout(flagRedraw);
	}
}

sl_ui_len TabView::getTabWidth()
{
	return m_tabWidth;
}

void TabView::setTabWidth(sl_ui_len width, sl_bool flagRedraw)
{
	m_tabWidth = width;
	if (!(isNativeWidget())) {
		_relayout(flagRedraw);
	}
}

sl_ui_len TabView::getTabHeight()
{
	return m_tabHeight;
}

void TabView::setTabHeight(sl_ui_len height, sl_bool flagRedraw)
{
	m_tabHeight = height;
	if (!(isNativeWidget())) {
		_relayout(flagRedraw);
	}
}

Ref<Drawable> TabView::getBarBackground()
{
	return m_barBackground;
}

void TabView::setBarBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_barBackground = drawable;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

void TabView::setBarBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	setBarBackground(Drawable::createColorDrawable(color), flagRedraw);
}

Ref<Drawable> TabView::getContentBackground()
{
	return m_contentBackground;
}

void TabView::setContentBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_contentBackground = drawable;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

void TabView::setContentBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	setContentBackground(Drawable::createColorDrawable(color));
}

Ref<Drawable> TabView::getTabBackground()
{
	return m_tabBackground;
}

void TabView::setTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_tabBackground = drawable;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

void TabView::setTabBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	setTabBackground(Drawable::createColorDrawable(color), flagRedraw);
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

Color TabView::getLabelColor()
{
	return m_labelColor;
}

void TabView::setLabelColor(const Color& color, sl_bool flagRedraw)
{
	m_labelColor = color;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

Color TabView::getSelectedLabelColor()
{
	return m_selectedLabelColor;
}

void TabView::setSelectedLabelColor(const Color& color, sl_bool flagRedraw)
{
	m_selectedLabelColor = color;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

Color TabView::getHoverLabelColor()
{
	return m_hoverLabelColor;
}

void TabView::setHoverLabelColor(const Color& color, sl_bool flagRedraw)
{
	m_hoverLabelColor = color;
	if (flagRedraw) {
		_invalidateTabBar();
	}
}

Alignment TabView::getLabelAlignment()
{
	return m_labelAlignment;
}

void TabView::setLabelAlignment(Alignment align, sl_bool flagRedraw)
{
	m_labelAlignment = align;
	if (flagRedraw) {
		invalidate();
	}
}

void TabView::setLabelMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw)
{
	m_labelMarginLeft = left;
	m_labelMarginTop = top;
	m_labelMarginRight = right;
	m_labelMarginBottom = bottom;
	if (flagRedraw) {
		invalidate();
	}
}

void TabView::setLabelMargin(sl_ui_pos margin, sl_bool flagRedraw)
{
	setLabelMargin(margin, margin, margin, margin, flagRedraw);
}

sl_ui_pos TabView::getLabelMarginLeft()
{
	return m_labelMarginLeft;
}

void TabView::setLabelMarginLeft(sl_ui_pos margin, sl_bool flagRedraw)
{
	setLabelMargin(margin, m_labelMarginTop, m_labelMarginRight, m_labelMarginBottom, flagRedraw);
}

sl_ui_pos TabView::getLabelMarginTop()
{
	return m_labelMarginTop;
}

void TabView::setLabelMarginTop(sl_ui_pos margin, sl_bool flagRedraw)
{
	setLabelMargin(m_labelMarginLeft, margin, m_labelMarginRight, m_labelMarginBottom, flagRedraw);
}

sl_ui_pos TabView::getLabelMarginRight()
{
	return m_labelMarginRight;
}

void TabView::setLabelMarginRight(sl_ui_pos margin, sl_bool flagRedraw)
{
	setLabelMargin(m_labelMarginLeft, m_labelMarginTop, margin, m_labelMarginBottom, flagRedraw);
}

sl_ui_pos TabView::getLabelMarginBottom()
{
	return m_labelMarginBottom;
}

void TabView::setLabelMarginBottom(sl_ui_pos margin, sl_bool flagRedraw)
{
	setLabelMargin(m_labelMarginLeft, m_labelMarginTop, m_labelMarginRight, margin, flagRedraw);
}

UIRect TabView::getTabBarRegion()
{
	UISize size = getSize();
	UIRect ret;
	if (m_orientation == LayoutOrientation::Vertical) {
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
	ret.fixSizeError();
	return ret;
}

UIRect TabView::getTabRegion(sl_uint32 index)
{
	UIRect ret;
	if (m_orientation == LayoutOrientation::Vertical) {
		ret.left = 0;
		ret.top = (sl_ui_len)index * m_tabHeight;
		ret.right = m_tabWidth;
		ret.bottom = (sl_ui_len)(index + 1) * m_tabHeight;
	} else {
		ret.left = (sl_ui_len)index * m_tabWidth;
		ret.top = 0;
		ret.right = (sl_ui_len)(index + 1) * m_tabWidth;
		ret.bottom = m_tabHeight;
	}
	ret.fixSizeError();
	return ret;
}

UIRect TabView::getWholeContentRegion()
{
	UIRect ret = getBounds();
	if (m_orientation == LayoutOrientation::Vertical) {
		ret.left += m_tabWidth;
	} else {
		ret.top += m_tabHeight;
	}
	ret.fixSizeError();
	return ret;
}

UIRect TabView::getTabContentRegion()
{
	UIRect ret = getBounds();
	if (m_orientation == LayoutOrientation::Vertical) {
		ret.left += m_tabWidth + getPaddingLeft();
		ret.top += getPaddingTop();
		ret.right -= getPaddingRight();
		ret.bottom -= getPaddingBottom();
	} else {
		ret.left += getPaddingLeft();
		ret.top += m_tabHeight + getPaddingTop();
		ret.right -= getPaddingRight();
		ret.bottom -= getPaddingBottom();
	}
	ret.fixSizeError();
	return ret;
}

void TabView::_invalidateTabBar()
{
	invalidate(getTabBarRegion());
}

void TabView::_relayout(sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	UIRect bound = getTabContentRegion();
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

void TabView::onClick(UIEvent* ev)
{
	UIPoint pt = ev->getPoint();
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
		UIPoint pt = ev->getPoint();
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
	canvas->draw(getTabBarRegion(), m_barBackground);
	canvas->draw(getWholeContentRegion(), m_contentBackground);
	ObjectLocker lock(this);
	ListLocker<TabViewItem> items(m_items);
	sl_uint32 n = (sl_uint32)(items.count);
	for (sl_uint32 i = 0; i < n; i++) {
		onDrawTab(canvas, getTabRegion(i), i, items[i].label);
	}
}

void TabView::onDrawTab(Canvas* canvas, const UIRect& rect, sl_uint32 index, const String& label)
{
	UIRect rc = getTabRegion(index);
	Color labelColor;
	if (m_indexSelected == index) {
		canvas->draw(rc, m_selectedTabBackground);
		labelColor = m_selectedLabelColor;
	} else if (m_indexHover == index) {
		canvas->draw(rc, m_hoverTabBackground);
		labelColor = m_hoverLabelColor;
	} else {
		canvas->draw(rc, m_tabBackground);
		labelColor = m_labelColor;
	}
	if (label.isNotEmpty()) {
		Ref<Font> font = getFont();
		rc.left += m_labelMarginLeft;
		rc.top += m_labelMarginTop;
		rc.right -= m_labelMarginRight;
		rc.bottom -= m_labelMarginBottom;
		rc.fixSizeError();
		if (m_orientation == LayoutOrientation::Vertical) {
			canvas->drawText(label, rc, font, labelColor, m_labelAlignment);
		} else {
			canvas->drawText(label, rc, font, labelColor, m_labelAlignment);
		}
	}
}

void TabView::onResize(sl_ui_len width, sl_ui_len height)
{
	if (isNativeWidget()) {
		_refreshSize_NW();
	} else {
		_relayout(sl_true);
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

UISize TabView::_getContentViewSize_NW()
{
	return UISize::zero();
}

void TabView::_selectTab_NW(sl_uint32 index)
{
}

void TabView::_setFont_NW(const Ref<Font>& font)
{
}

#endif

SLIB_UI_NAMESPACE_END
