/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_SCROLL_VIEW
#define CHECKHEADER_SLIB_UI_SCROLL_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT ScrollView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ScrollView();

		~ScrollView();
		
	public:
		Ref<View> getContentView();
		
		virtual void setContentView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void setContentSize(const ScrollPoint& size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		ScrollPoint getScrollPosition() override;
		
		ScrollPoint getScrollRange() override;
		
		void scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void scrollTo(const ScrollPoint& position, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void smoothScrollTo(const ScrollPoint& position, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		void dispatchScroll(sl_scroll_pos x, sl_scroll_pos y) override;
		
	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		void onResizeChild(View* child, sl_ui_len width, sl_ui_len height) override;
		
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical, const UIRect& currentFrame) override;
		
		void onUpdatePaging() override;
		
	private:
		void _refreshContentSize_NW();
		
		void _setContentView_NW(const Ref<View>& view);
		
		void _scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate);
		
		ScrollPoint _getScrollPosition_NW();
		
		ScrollPoint _getScrollRange_NW();
		
		void _setBorder_NW(sl_bool flag) override;
		
		void _setBackgroundColor_NW(const Color& color) override;
		
		void _setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight);
		
		void _setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical);
		
	protected:
		void _onScroll_NW(sl_scroll_pos x, sl_scroll_pos y);
		
	protected:
		AtomicRef<View> m_viewContent;
		
	};
	
	class HorizontalScrollView : public ScrollView
	{
	public:
		HorizontalScrollView();
		
		~HorizontalScrollView();

	};
	
	class VerticalScrollView : public ScrollView
	{
	public:
		VerticalScrollView();
		
		~VerticalScrollView();

	};

}

#endif
