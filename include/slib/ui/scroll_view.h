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

#ifndef CHECKHEADER_SLIB_UI_SCROLL_VIEW
#define CHECKHEADER_SLIB_UI_SCROLL_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT ScrollView : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ScrollView();

		~ScrollView();
		
	public:
		Ref<View> getContentView();
		
		virtual void setContentView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode = UIUpdateMode::UpdateLayout) override;
		
		void setContentSize(const ScrollPoint& size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		ScrollPoint getScrollPosition() override;
		
		ScrollPoint getScrollRange() override;
		
		void scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void scrollTo(const ScrollPoint& position, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void smoothScrollTo(const ScrollPoint& position, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		void dispatchScroll(sl_scroll_pos x, sl_scroll_pos y) override;
		
	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		void onResizeChild(View* child, sl_ui_len width, sl_ui_len height) override;
		
		void onUpdatePaging() override;
		
	private:
		void _refreshContentSize_NW();
		
		void _setContentView_NW(const Ref<View>& view);
		
		void _scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate);
		
		ScrollPoint _getScrollPosition_NW();
		
		ScrollPoint _getScrollRange_NW();
		
		void _setBorder_NW(sl_bool flag) override;
		
		void _setBackgroundColor_NW(const Color& color) override;
		
		void _setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical) override;
		
		void _setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight);
		
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
