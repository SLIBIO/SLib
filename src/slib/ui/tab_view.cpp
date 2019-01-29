/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/ui/tab_view.h"

#include "slib/ui/core.h"
#include "slib/graphics/util.h"

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
		setUsingLayout(sl_false);
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
		m_tabWidth = 0;
		m_tabHeight = 0;
		
		m_tabAlignment = Alignment::MiddleCenter;
		m_tabPaddingLeft = 0;
		m_tabPaddingTop = 0;
		m_tabPaddingRight = 0;
		m_tabPaddingBottom = 0;
		m_tabSpaceSize = 0;
		m_iconWidth = 0;
		m_iconHeight = 0;
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
	
	Ref<Drawable> TabView::getTabIcon(sl_uint32 index)
	{
		MutexLocker lock(m_items.getLocker());
		if (index < m_items.getCount()) {
			TabViewItem* item = m_items.getPointerAt(index);
			return item->icon;
		}
		return sl_null;
	}
	
	void TabView::setTabIcon(sl_uint32 index, const Ref<Drawable>& icon, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (index < m_items.getCount()) {
			TabViewItem* item = m_items.getPointerAt(index);
			item->icon = icon;
			if (!(isNativeWidget())) {
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
	
	sl_real TabView::getTabWidth()
	{
		sl_real width = m_tabWidth;
		if (width > 0) {
			return width;
		}
		if (m_orientation == LayoutOrientation::Horizontal) {
			sl_size n = m_items.getCount();
			if (n > 0) {
				return (sl_real)(getWidth()) / (sl_real)n;
			} else {
				return (sl_real)(getWidth());
			}
		} else {
			return getFontSize() * 2;
		}
	}
	
	void TabView::setTabWidth(sl_real width, UIUpdateMode mode)
	{
		m_tabWidth = width;
		if (!(isNativeWidget())) {
			_relayout(mode);
		}
	}
	
	sl_real TabView::getTabHeight()
	{
		sl_real height = m_tabHeight;
		if (height > 0) {
			return height;
		}
		if (m_orientation == LayoutOrientation::Vertical) {
			sl_size n = m_items.getCount();
			if (n > 0) {
				return (sl_real)(getHeight()) / (sl_real)n;
			} else {
				return (sl_real)(getHeight());
			}
		} else {
			return getFontSize() * 2;
		}
	}
	
	void TabView::setTabHeight(sl_real height, UIUpdateMode mode)
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
	
	Alignment TabView::getTabAlignment()
	{
		return m_tabAlignment;
	}
	
	void TabView::setTabAlignment(Alignment align, UIUpdateMode mode)
	{
		m_tabAlignment = align;
		invalidate(mode);
	}
	
	void TabView::setTabPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		m_tabPaddingLeft = left;
		m_tabPaddingTop = top;
		m_tabPaddingRight = right;
		m_tabPaddingBottom = bottom;
		invalidate(mode);
	}
	
	void TabView::setTabPadding(sl_ui_pos margin, UIUpdateMode mode)
	{
		setTabPadding(margin, margin, margin, margin, mode);
	}
	
	sl_ui_pos TabView::getTabPaddingLeft()
	{
		return m_tabPaddingLeft;
	}
	
	void TabView::setTabPaddingLeft(sl_ui_pos padding, UIUpdateMode mode)
	{
		setTabPadding(padding, m_tabPaddingTop, m_tabPaddingRight, m_tabPaddingBottom, mode);
	}
	
	sl_ui_pos TabView::getTabPaddingTop()
	{
		return m_tabPaddingTop;
	}
	
	void TabView::setTabPaddingTop(sl_ui_pos padding, UIUpdateMode mode)
	{
		setTabPadding(m_tabPaddingLeft, padding, m_tabPaddingRight, m_tabPaddingBottom, mode);
	}
	
	sl_ui_pos TabView::getTabPaddingRight()
	{
		return m_tabPaddingRight;
	}
	
	void TabView::setTabPaddingRight(sl_ui_pos padding, UIUpdateMode mode)
	{
		setTabPadding(m_tabPaddingLeft, m_tabPaddingTop, padding, m_tabPaddingBottom, mode);
	}
	
	sl_ui_pos TabView::getTabPaddingBottom()
	{
		return m_tabPaddingBottom;
	}
	
	void TabView::setTabPaddingBottom(sl_ui_pos padding, UIUpdateMode mode)
	{
		setTabPadding(m_tabPaddingLeft, m_tabPaddingTop, m_tabPaddingRight, padding, mode);
	}

	sl_ui_pos TabView::getTabSpaceSize()
	{
		return m_tabSpaceSize;
	}
	
	void TabView::setTabSpaceSize(sl_ui_pos size, UIUpdateMode mode)
	{
		m_tabSpaceSize = size;
		invalidate(mode);
	}
	
	UISize TabView::getIconSize()
	{
		return UISize(m_iconWidth, m_iconHeight);
	}
	
	void TabView::setIconSize(const UISize& size, UIUpdateMode mode)
	{
		m_iconWidth = size.x;
		m_iconHeight = size.y;
		invalidate(mode);
	}
	
	void TabView::setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
	{
		setIconSize(UISize(width, height), mode);
	}
	
	void TabView::setIconSize(sl_ui_len size, UIUpdateMode mode)
	{
		setIconSize(UISize(size, size), mode);
	}
	
	sl_ui_len TabView::getIconWidth()
	{
		return m_iconWidth;
	}
	
	void TabView::setIconWidth(sl_ui_len width, UIUpdateMode mode)
	{
		setIconSize(UISize(width, m_iconHeight), mode);
	}
	
	sl_ui_len TabView::getIconHeight()
	{
		return m_iconHeight;
	}
	
	void TabView::setIconHeight(sl_ui_len height, UIUpdateMode mode)
	{
		setIconSize(UISize(m_iconWidth, height), mode);
	}
	
	UIRect TabView::getTabBarRegion()
	{
		UISize size = getSize();
		UIRect ret;
		if (m_orientation == LayoutOrientation::Vertical) {
			ret.left = 0;
			ret.top = 0;
			ret.right = (sl_ui_len)(getTabWidth());
			ret.bottom = size.y;
		} else {
			ret.left = 0;
			ret.top = 0;
			ret.right = size.x;
			ret.bottom = (sl_ui_len)(getTabHeight());
		}
		ret.fixSizeError();
		return ret;
	}

	UIRect TabView::getTabRegion(sl_uint32 index)
	{
		UIRect ret;
		if (m_orientation == LayoutOrientation::Vertical) {
			ret.left = 0;
			ret.top = (sl_ui_len)(index * getTabHeight());
			ret.right = (sl_ui_len)(getTabWidth());
			ret.bottom = (sl_ui_len)((index + 1) * getTabHeight());
		} else {
			ret.left = (sl_ui_len)(index * getTabWidth());
			ret.top = 0;
			ret.right = (sl_ui_len)((index + 1) * getTabWidth());
			ret.bottom = (sl_ui_len)(getTabHeight());
		}
		ret.fixSizeError();
		return ret;
	}
	
	UIRect TabView::getWholeContentRegion()
	{
		UIRect ret = getBounds();
		if (m_orientation == LayoutOrientation::Vertical) {
			ret.left += (sl_ui_len)(getTabWidth());
		} else {
			ret.top += (sl_ui_len)(getTabHeight());
		}
		ret.fixSizeError();
		return ret;
	}

	UIRect TabView::getTabContentRegion()
	{
		UIRect ret = getBounds();
		if (m_orientation == LayoutOrientation::Vertical) {
			ret.left += (sl_ui_len)(getTabWidth()) + getPaddingLeft();
			ret.top += getPaddingTop();
			ret.right -= getPaddingRight();
			ret.bottom -= getPaddingBottom();
		} else {
			ret.left += getPaddingLeft();
			ret.top += (sl_ui_len)(getTabHeight()) + getPaddingTop();
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
	
	SLIB_DEFINE_EVENT_HANDLER(TabView, SelectTab, sl_uint32 index)

	void TabView::dispatchSelectTab(sl_uint32 index)
	{
		m_indexSelected = index;
		
		SLIB_INVOKE_EVENT_HANDLER(SelectTab, index)
	}

	void TabView::onClickEvent(UIEvent* ev)
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
		if (ev->getAction() == UIAction::MouseLeave || ev->getAction() == UIAction::LeftButtonUp || ev->getAction() == UIAction::TouchEnd || ev->getAction() == UIAction::TouchCancel) {
			m_indexHover = -1;
			_invalidateTabBar(UIUpdateMode::Redraw);
		} else if (ev->getAction() == UIAction::MouseMove || ev->getAction() == UIAction::LeftButtonDrag || ev->getAction() == UIAction::TouchBegin || ev->getAction() == UIAction::TouchMove) {
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
			onDrawTab(canvas, getTabRegion(i), i, items[i].icon, items[i].label);
		}
	}
	
	void TabView::onDrawTab(Canvas* canvas, const UIRect& rect, sl_uint32 index, const Ref<Drawable>& icon, const String& label)
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
		
		rc.left += m_tabPaddingLeft;
		rc.top += m_tabPaddingTop;
		rc.right -= m_tabPaddingRight;
		rc.bottom -= m_tabPaddingBottom;
		if (!(rc.isValidSize())) {
			return;
		}
		
		sl_ui_pos space = m_tabSpaceSize;
		sl_ui_len widthIcon = m_iconWidth;
		sl_ui_len heightIcon = m_iconHeight;
		if (icon.isNotNull()) {
			if (heightIcon <= 0) {
				if (widthIcon > 0) {
					sl_real w = icon->getDrawableWidth();
					if (w > 0.00001f) {
						heightIcon = (sl_ui_len)(icon->getDrawableHeight() * widthIcon / w);
					}
				} else {
					heightIcon = Math::min(rc.getWidth(), rc.getHeight());
				}
			}
			if (widthIcon <= 0 && heightIcon > 0) {
				sl_real h = icon->getDrawableHeight();
				if (h > 0.00001f) {
					widthIcon = (sl_ui_len)(icon->getDrawableWidth() * heightIcon / h);
				}
			}
		} else {
			space = 0;
		}
		sl_ui_len widthLabel = 0;
		sl_ui_len heightLabel = 0;
		Ref<Font> font;
		if (label.isNotEmpty()) {
			font = getFont();
			if (font.isNotNull()) {
				UISize size = canvas->measureText(font, label);
				widthLabel = size.x;
				heightLabel = size.y;
			}
		} else {
			space = 0;
		}
		
		sl_ui_len widthTotal = widthIcon + space + widthLabel;
		sl_ui_len heightTotal = Math::max(heightIcon, heightLabel);
		if (widthTotal <= 0 || heightTotal <= 0) {
			return;
		}
		
		UIPoint pt = GraphicsUtil::calculateAlignPosition(rc, (sl_real)widthTotal, (sl_real)heightTotal, m_tabAlignment);
		
		if (icon.isNotNull() && widthIcon > 0 && heightIcon > 0) {
			UIRect rcIcon;
			rcIcon.left = pt.x;
			rcIcon.right = rcIcon.left + widthIcon;
			rcIcon.top = pt.y + heightTotal / 2 - heightIcon / 2;
			rcIcon.bottom = rcIcon.top + heightIcon;
			canvas->draw(rcIcon, icon);
		}
		if (label.isNotEmpty() && labelColor.isNotZero() && font.isNotNull() && widthLabel > 0 && heightLabel > 0) {
			canvas->drawText(label, (sl_real)(pt.x + widthIcon + space), (sl_real)(pt.y + heightTotal / 2 - heightLabel / 2), font, labelColor);
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
