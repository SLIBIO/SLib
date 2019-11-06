/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_UI_TAB_VIEW
#define CHECKHEADER_SLIB_UI_TAB_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class TabViewItem
	{
	public:
		AtomicString label;
		AtomicRef<Drawable> icon;
		AtomicRef<View> contentView;
		
	public:
		TabViewItem();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TabViewItem)
		
	};
	
	class ITabViewInstance;
	
	class SLIB_EXPORT TabView : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TabView();
		
		~TabView();

	public:
		sl_uint32 getTabsCount();
		
		virtual void setTabsCount(sl_uint32 count, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getTabLabel(sl_uint32 index);
		
		virtual void setTabLabel(sl_uint32 index, const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getTabIcon(sl_uint32 index);
		
		virtual void setTabIcon(sl_uint32 index, const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<View> getTabContentView(sl_uint32 index);
		
		virtual void setTabContentView(sl_uint32 index, const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_uint32 getSelectedTabIndex();
		
		virtual void selectTab(sl_uint32 index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		UISize getContentViewSize();
		
		LayoutOrientation getOrientation();
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getTabWidth();
		
		virtual void setTabWidth(sl_real width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getTabHeight();
		
		virtual void setTabHeight(sl_real height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getBarBackground();
		
		void setBarBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setBarBackground(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getContentBackground();
		
		void setContentBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setContentBackground(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getTabBackground();
		
		void setTabBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTabBackground(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getSelectedTabBackground();
		
		void setSelectedTabBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setSelectedTabBackground(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getHoverTabBackground();
		
		void setHoverTabBackground(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setHoverTabBackground(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getLabelColor();
		
		void setLabelColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getSelectedLabelColor();
		
		void setSelectedLabelColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getHoverLabelColor();
		
		void setHoverLabelColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getTabAlignment();
		
		virtual void setTabAlignment(const Alignment& align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void setTabPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTabPadding(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTabPaddingLeft();
		
		void setTabPaddingLeft(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTabPaddingTop();
		
		void setTabPaddingTop(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTabPaddingRight();
		
		void setTabPaddingRight(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTabPaddingBottom();
		
		void setTabPaddingBottom(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTabSpaceSize();
		
		virtual void setTabSpaceSize(sl_ui_pos size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		UISize getIconSize();

		virtual void setIconSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconSize(sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getIconWidth();
		
		void setIconWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getIconHeight();
		
		void setIconHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual UIRect getTabBarRegion();
		
		virtual UIRect getTabRegion(sl_uint32 index);
		
		virtual UIRect getWholeContentRegion();
		
		virtual UIRect getTabContentRegion();
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(TabView, SelectTab, sl_uint32 index)
		
	protected:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		virtual Ptr<ITabViewInstance> getTabViewInstance();
		
	protected:
		void onClickEvent(UIEvent* ev) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onDraw(Canvas* canvas) override;
		
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		virtual void onDrawTab(Canvas* canvas, const UIRect& rect, sl_uint32 index, const Ref<Drawable>& icon, const String& label);
		
	private:
		void _selectTab(sl_bool flagEvent, sl_uint32 index, UIUpdateMode mode = UIUpdateMode::Redraw);

		void _invalidateTabBar(UIUpdateMode mode);
		
		void _relayout(UIUpdateMode mode);
		
		void _refreshSize();
		
	protected:
		CList<TabViewItem> m_items;
		sl_uint32 m_indexSelected;
		sl_int32 m_indexHover;
		
		LayoutOrientation m_orientation;
		sl_real m_tabWidth;
		sl_real m_tabHeight;
		
		AtomicRef<Drawable> m_barBackground;
		AtomicRef<Drawable> m_contentBackground;
		AtomicRef<Drawable> m_tabBackground;
		AtomicRef<Drawable> m_selectedTabBackground;
		AtomicRef<Drawable> m_hoverTabBackground;
		Color m_labelColor;
		Color m_selectedLabelColor;
		Color m_hoverLabelColor;
		Alignment m_tabAlignment;
		sl_ui_pos m_tabPaddingLeft;
		sl_ui_pos m_tabPaddingTop;
		sl_ui_pos m_tabPaddingRight;
		sl_ui_pos m_tabPaddingBottom;
		sl_ui_pos m_tabSpaceSize;
		sl_ui_len m_iconWidth;
		sl_ui_len m_iconHeight;
		
	};
	
	class SLIB_EXPORT ITabViewInstance
	{
	public:
		virtual void refreshTabsCount(TabView* view) = 0;
		
		virtual void refreshSize(TabView* view) = 0;
		
		virtual void setTabLabel(TabView* view, sl_uint32 index, const String& text) = 0;
		
		virtual void setTabContentView(TabView* view, sl_uint32 index, const Ref<View>& content) = 0;
		
		virtual sl_bool getContentViewSize(TabView* view, UISize& _out) = 0;
		
		virtual void selectTab(TabView* view, sl_uint32 index) = 0;
		
	};

}

#endif
