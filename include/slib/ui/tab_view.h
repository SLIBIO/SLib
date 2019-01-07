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
		
		AtomicRef<View> contentView;
		
	public:
		TabViewItem();

		~TabViewItem();

	};
	
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
		
		Ref<View> getTabContentView(sl_uint32 index);
		
		virtual void setTabContentView(sl_uint32 index, const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_uint32 getSelectedTabIndex();
		
		virtual void selectTab(sl_uint32 index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		UISize getContentViewSize();
		
		LayoutOrientation getOrientation();
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getTabWidth();
		
		virtual void setTabWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getTabHeight();
		
		virtual void setTabHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
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
		
		Alignment getLabelAlignment();
		
		virtual void setLabelAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void setLabelMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setLabelMargin(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getLabelMarginLeft();
		
		void setLabelMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getLabelMarginTop();
		
		void setLabelMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getLabelMarginRight();
		
		void setLabelMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getLabelMarginBottom();
		
		void setLabelMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual UIRect getTabBarRegion();
		
		virtual UIRect getTabRegion(sl_uint32 index);
		
		virtual UIRect getWholeContentRegion();
		
		virtual UIRect getTabContentRegion();
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(TabView, SelectTab, sl_uint32 index)
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
	protected:
		void onClickEvent(UIEvent* ev) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onDraw(Canvas* canvas) override;
		
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		virtual void onDrawTab(Canvas* canvas, const UIRect& rect, sl_uint32 index, const String& label);
		
	protected:
		void _invalidateTabBar(UIUpdateMode mode);
		
		void _relayout(UIUpdateMode mode);
		
	private:
		void _refreshTabsCount_NW();
		
		void _refreshSize_NW();
		
		void _setTabLabel_NW(sl_uint32 index, const String& text);
		
		void _setTabContentView_NW(sl_uint32 index, const Ref<View>& view);
				
		UISize _getContentViewSize_NW();
		
		void _selectTab_NW(sl_uint32 index);
		
		void _setFont_NW(const Ref<Font>& font) override;
		
	protected:
		CList<TabViewItem> m_items;
		sl_uint32 m_indexSelected;
		sl_int32 m_indexHover;
		
		LayoutOrientation m_orientation;
		sl_ui_len m_tabWidth;
		sl_ui_len m_tabHeight;
		
		AtomicRef<Drawable> m_barBackground;
		AtomicRef<Drawable> m_contentBackground;
		AtomicRef<Drawable> m_tabBackground;
		AtomicRef<Drawable> m_selectedTabBackground;
		AtomicRef<Drawable> m_hoverTabBackground;
		Color m_labelColor;
		Color m_selectedLabelColor;
		Color m_hoverLabelColor;
		Alignment m_labelAlignment;
		sl_ui_pos m_labelMarginLeft;
		sl_ui_pos m_labelMarginTop;
		sl_ui_pos m_labelMarginRight;
		sl_ui_pos m_labelMarginBottom;
		
	};

}

#endif
