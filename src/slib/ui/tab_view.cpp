/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/tab_view.h"

#include "slib/ui/core.h"

#define MAX_TABS_COUNT 100

namespace slib
{

	SLIB_DEFINE_OBJECT(TabView, ViewGroup)
	

	TabViewItem::TabViewItem()
	{
	}

	TabViewItem::~TabViewItem()
	{
	}


	TabView::TabView()
	{
		
		SLIB_REFERABLE_CONSTRUCTOR
		
		setCreatingNativeWidget(sl_true);
		setCreatingChildInstances(sl_true);
		setUsingFont(sl_true);
		setDrawing(sl_true, UIUpdateMode::Init);
		setSavingCanvasState(sl_false);
		
		m_items.setCount(1);
		m_indexSelected = 0;
		m_indexHover = -1;
		
		setBackgroundColor(Color::White, UIUpdateMode::Init);
		setBarBackground(Color(230, 230, 230), UIUpdateMode::Init);
		setSelectedTabBackground(Color(150, 150, 150), UIUpdateMode::Init);
		setHoverTabBackground(Color(210, 210, 210), UIUpdateMode::Init);
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
		
	}
	
	TabView::~TabView()
	{
	}

	sl_uint32 TabView::getTabsCount()
	{
		return (sl_uint32)(m_items.getCount());
	}
	
	void TabView::setTabsCount(sl_uint32 nNew, UIUpdateMode mode)
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
				removeChild(item.contentView, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
			}
		}
		m_items.setCount(nNew);
		if (isNativeWidget()) {
			_refreshTabsCount_NW();
		}
		selectTab(m_indexSelected, UIUpdateMode::None);
		invalidate(mode);
	}
	
	String TabView::getTabLabel(sl_uint32 index)
	{
		MutexLocker lock(m_items.getLocker());
		if (index < m_items.getCount()) {
			TabViewItem* item = m_items.getPointerAt(index);
			return item->label;
		}
		return sl_null;
	}
	
	void TabView::setTabLabel(sl_uint32 index, const String& text, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (index < m_items.getCount()) {
			TabViewItem* item = m_items.getPointerAt(index);
			item->label = text;
			if (isNativeWidget()) {
				_setTabLabel_NW(index, text);
			} else {
				_invalidateTabBar(mode);
			}
		}
	}
	
	Ref<View> TabView::getTabContentView(sl_uint32 index)
	{
		MutexLocker lock(m_items.getLocker());
		if (index < m_items.getCount()) {
			TabViewItem* item = m_items.getPointerAt(index);
			return item->contentView;
		}
		return sl_null;
	}
	
	void TabView::setTabContentView(sl_uint32 index, const Ref<View>& view, UIUpdateMode mode)
	{
		MutexLocker lock(m_items.getLocker());
		if (index < m_items.getCount()) {
			TabViewItem* item = m_items.getPointerAt(index);
			if (item->contentView != view) {
				removeChild(item->contentView, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
				if (view.isNotNull()) {
					view->setCreatingInstance(sl_true);
					view->setAttachMode(UIAttachMode::NotAttachInNativeWidget);
					view->setFrame(getTabContentRegion(), SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::UpdateLayout);
					addChild(view, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
				}
				item->contentView = view;
				if (isNativeWidget()) {
					if (UI::isUiThread()) {
						_setTabContentView_NW(index, view);
					} else {
						UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), TabView, _setTabContentView_NW, this, index, view));
					}
				} else {
					selectTab(m_indexSelected, UIUpdateMode::None);
					invalidate(mode);
				}
			}
		}
	}
	
	sl_uint32 TabView::getSelectedTabIndex()
	{
		if (isNativeWidget()) {
			_getSelectedTabIndex_NW();
		}
		return m_indexSelected;
	}
	
	void TabView::selectTab(sl_uint32 index, UIUpdateMode mode)
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
						view->setVisible(sl_true, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
					} else {
						view->setVisible(sl_false, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
					}
				}
			}
			invalidate(mode);
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
	
	void TabView::setOrientation(LayoutOrientation orientation, UIUpdateMode mode)
	{
		m_orientation = orientation;
		if (!(isNativeWidget())) {
			_relayout(mode);
		}
	}
	
	sl_ui_len TabView::getTabWidth()
	{
		return m_tabWidth;
	}
	
	void TabView::setTabWidth(sl_ui_len width, UIUpdateMode mode)
	{
		m_tabWidth = width;
		if (!(isNativeWidget())) {
			_relayout(mode);
		}
	}
	
	sl_ui_len TabView::getTabHeight()
	{
		return m_tabHeight;
	}
	
	void TabView::setTabHeight(sl_ui_len height, UIUpdateMode mode)
	{
		m_tabHeight = height;
		if (!(isNativeWidget())) {
			_relayout(mode);
		}
	}
	
	Ref<Drawable> TabView::getBarBackground()
	{
		return m_barBackground;
	}
	
	void TabView::setBarBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_barBackground = drawable;
		_invalidateTabBar(mode);
	}
	
	void TabView::setBarBackground(const Color& color, UIUpdateMode mode)
	{
		setBarBackground(Drawable::createColorDrawable(color), mode);
	}
	
	Ref<Drawable> TabView::getContentBackground()
	{
		return m_contentBackground;
	}
	
	void TabView::setContentBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_contentBackground = drawable;
		invalidate(mode);
	}
	
	void TabView::setContentBackground(const Color& color, UIUpdateMode mode)
	{
		setContentBackground(Drawable::createColorDrawable(color), mode);
	}
	
	Ref<Drawable> TabView::getTabBackground()
	{
		return m_tabBackground;
	}
	
	void TabView::setTabBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_tabBackground = drawable;
		_invalidateTabBar(mode);
	}
	
	void TabView::setTabBackground(const Color& color, UIUpdateMode mode)
	{
		setTabBackground(Drawable::createColorDrawable(color), mode);
	}
	
	Ref<Drawable> TabView::getSelectedTabBackground()
	{
		return m_selectedTabBackground;
	}
	
	void TabView::setSelectedTabBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_selectedTabBackground = drawable;
		_invalidateTabBar(mode);
	}
	
	void TabView::setSelectedTabBackground(const Color& color, UIUpdateMode mode)
	{
		setSelectedTabBackground(Drawable::createColorDrawable(color), mode);
	}
	
	Ref<Drawable> TabView::getHoverTabBackground()
	{
		return m_hoverTabBackground;
	}
	
	void TabView::setHoverTabBackground(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_hoverTabBackground = drawable;
		_invalidateTabBar(mode);
	}
	
	void TabView::setHoverTabBackground(const Color& color, UIUpdateMode mode)
	{
		setHoverTabBackground(Drawable::createColorDrawable(color), mode);
	}
	
	Color TabView::getLabelColor()
	{
		return m_labelColor;
	}
	
	void TabView::setLabelColor(const Color& color, UIUpdateMode mode)
	{
		m_labelColor = color;
		_invalidateTabBar(mode);
	}

	Color TabView::getSelectedLabelColor()
	{
		return m_selectedLabelColor;
	}
	
	void TabView::setSelectedLabelColor(const Color& color, UIUpdateMode mode)
	{
		m_selectedLabelColor = color;
		_invalidateTabBar(mode);
	}
	
	Color TabView::getHoverLabelColor()
	{
		return m_hoverLabelColor;
	}
	
	void TabView::setHoverLabelColor(const Color& color, UIUpdateMode mode)
	{
		m_hoverLabelColor = color;
		_invalidateTabBar(mode);
	}
	
	Alignment TabView::getLabelAlignment()
	{
		return m_labelAlignment;
	}
	
	void TabView::setLabelAlignment(Alignment align, UIUpdateMode mode)
	{
		m_labelAlignment = align;
		invalidate(mode);
	}
	
	void TabView::setLabelMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		m_labelMarginLeft = left;
		m_labelMarginTop = top;
		m_labelMarginRight = right;
		m_labelMarginBottom = bottom;
		invalidate(mode);
	}
	
	void TabView::setLabelMargin(sl_ui_pos margin, UIUpdateMode mode)
	{
		setLabelMargin(margin, margin, margin, margin, mode);
	}
	
	sl_ui_pos TabView::getLabelMarginLeft()
	{
		return m_labelMarginLeft;
	}
	
	void TabView::setLabelMarginLeft(sl_ui_pos margin, UIUpdateMode mode)
	{
		setLabelMargin(margin, m_labelMarginTop, m_labelMarginRight, m_labelMarginBottom, mode);
	}
	
	sl_ui_pos TabView::getLabelMarginTop()
	{
		return m_labelMarginTop;
	}
	
	void TabView::setLabelMarginTop(sl_ui_pos margin, UIUpdateMode mode)
	{
		setLabelMargin(m_labelMarginLeft, margin, m_labelMarginRight, m_labelMarginBottom, mode);
	}
	
	sl_ui_pos TabView::getLabelMarginRight()
	{
		return m_labelMarginRight;
	}
	
	void TabView::setLabelMarginRight(sl_ui_pos margin, UIUpdateMode mode)
	{
		setLabelMargin(m_labelMarginLeft, m_labelMarginTop, margin, m_labelMarginBottom, mode);
	}
	
	sl_ui_pos TabView::getLabelMarginBottom()
	{
		return m_labelMarginBottom;
	}
	
	void TabView::setLabelMarginBottom(sl_ui_pos margin, UIUpdateMode mode)
	{
		setLabelMargin(m_labelMarginLeft, m_labelMarginTop, m_labelMarginRight, margin, mode);
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
	
	void TabView::_invalidateTabBar(UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			invalidate(getTabBarRegion());
		}
	}
	
	void TabView::_relayout(UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		UIRect bound = getTabContentRegion();
		ListLocker<TabViewItem> items(m_items);
		for (sl_size i = 0; i < items.count; i++) {
			Ref<View> view = items[i].contentView;
			if (view.isNotNull()) {
				view->setFrame(bound, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::UpdateLayout);
			}
		}
		invalidate(mode);
	}
	
	void TabView::dispatchSelectTab(sl_uint32 index)
	{
		onSelectTab(index);
		getOnSelectTab()(this, index);
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
				dispatchSelectTab(i);
				return;
			}
		}
	}
	
	void TabView::onMouseEvent(UIEvent* ev)
	{
		if (ev->getAction() == UIAction::MouseLeave) {
			m_indexHover = -1;
			_invalidateTabBar(UIUpdateMode::Redraw);
		} else if (ev->getAction() == UIAction::MouseMove) {
			UIPoint pt = ev->getPoint();
			ObjectLocker lock(this);
			ListLocker<TabViewItem> items(m_items);
			sl_int32 n = (sl_int32)(items.count);
			for (sl_int32 i = 0; i < n; i++) {
				if (getTabRegion(i).containsPoint(pt)) {
					if (i != m_indexHover) {
						m_indexHover = i;
						_invalidateTabBar(UIUpdateMode::Redraw);
					}
					return;
				}
			}
			if (-1 != m_indexHover) {
				m_indexHover = -1;
				_invalidateTabBar(UIUpdateMode::Redraw);
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
		Ref<Drawable> background;
		if (m_indexSelected == index) {
			background = m_selectedTabBackground;
			labelColor = m_selectedLabelColor;
		} else if (m_indexHover == (sl_int32)index) {
			background = m_hoverTabBackground;
			labelColor = m_hoverLabelColor;
		} else {
			background = m_tabBackground;
			labelColor = m_labelColor;
		}
		if (background.isNull()) {
			background = m_tabBackground;
		}
		if (labelColor.isZero()) {
			labelColor = m_labelColor;
		}
		if (background.isNotNull()) {
			canvas->draw(rc, background);
		}
		if (label.isNotEmpty() && labelColor.isNotZero()) {
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
			_relayout(UIUpdateMode::Redraw);
		}
	}
	
	
#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<ViewInstance> TabView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
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

}
