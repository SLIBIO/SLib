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
	
	class IScrollViewInstance;

	class SLIB_EXPORT ScrollView : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ScrollView();

		~ScrollView();
		
	public:
		Ref<View> getContentView();
		
		virtual void setContentView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		using ViewGroup::setContentSize;
		
		void setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode = UIUpdateMode::UpdateLayout) override;
		
		ScrollPoint getScrollPosition() override;
		
		ScrollPoint getScrollRange() override;
		
		UIRect getBounds() override;
		
	protected:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		virtual Ptr<IScrollViewInstance> getScrollViewInstance();
		
	public:
		void dispatchScroll(sl_scroll_pos x, sl_scroll_pos y) override;
		
	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		void onResizeChild(View* child, sl_ui_len width, sl_ui_len height) override;
		
	protected:
		void _refreshSize();
		
	protected:
		AtomicRef<View> m_viewContent;
		
	};
	
	class SLIB_EXPORT HorizontalScrollView : public ScrollView
	{
	public:
		HorizontalScrollView();
		
		~HorizontalScrollView();

	};
	
	class SLIB_EXPORT VerticalScrollView : public ScrollView
	{
	public:
		VerticalScrollView();
		
		~VerticalScrollView();

	};

	class SLIB_EXPORT IScrollViewInstance
	{
	public:
		virtual void refreshContentSize(ScrollView* view) = 0;
		
		virtual void setContentView(ScrollView* view, const Ref<View>& content) = 0;
		
		virtual sl_bool getScrollPosition(ScrollView* view, ScrollPoint& _out) = 0;
		
		virtual sl_bool getScrollRange(ScrollView* view, ScrollPoint& _out) = 0;
		
		virtual sl_bool getBounds(ScrollView* view, UIRect& _out);
		
	};
	
}

#endif
