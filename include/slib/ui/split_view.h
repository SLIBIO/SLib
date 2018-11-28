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

#ifndef CHECKHEADER_SLIB_UI_SPLIT_VIEW
#define CHECKHEADER_SLIB_UI_SPLIT_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT SplitView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		SplitView();
		
		~SplitView();
		
	public:
		LayoutOrientation getOrientation();
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isHorizontal();
		
		void setHorizontal(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_bool isVertical();
		
		void setVertical(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_size getItemsCount();
		
		void setItemsCount(sl_size count, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<View> getItemView(sl_size index);
		
		void setItemView(sl_size index, const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getItemSize(sl_size index);
		
		void setItemSize(sl_size index, sl_ui_len size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getItemWeight(sl_size index);
		
		void setItemWeight(sl_size index, sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getItemMinimumWeight(sl_size index);
		
		void setItemMinimumWeight(sl_size index, sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getItemMaximumWeight(sl_size index);
		
		void setItemMaximumWeight(sl_size index, sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getItemMinimumSize(sl_size index);
		
		void setItemMinimumSize(sl_size index, sl_ui_len size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getItemMaximumSize(sl_size index);
		
		void setItemMaximumSize(sl_size index, sl_ui_len size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getItemDividerWidth(sl_size index);
		
		void setItemDividerWidth(sl_size index, sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<Drawable> getItemDividerBackground(sl_size index);
		
		void setItemDividerBackground(sl_size index, const Ref<Drawable>& background, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getItemDividerColor(sl_size index);
		
		void setItemDividerColor(sl_size index, const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getDividerWidth();
		
		void setDividerWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Ref<Drawable> getDividerBackground();
		
		void setDividerBackground(const Ref<Drawable>& background, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getDividerColor();
		
		void setDividerColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getCursorMargin();
		
		void setCursorMargin(sl_ui_len margin);
		
		void relayout(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		void onDraw(Canvas* canvas) override;
		
	public:
		void dispatchMouseEvent(UIEvent* ev) override;
		
		void dispatchSetCursor(UIEvent* ev) override;
		
		sl_bool hitTestForCapturingChildInstanceEvents(const UIPoint& pt) override;
		
	protected:
		sl_ui_len _getTotalSize();
		
		void _refreshItemFrames(UIUpdateMode mode);
		
		void _resetWeights(UIUpdateMode mode);
		
		sl_int32 _getDividerIndexAtPoint(const UIPoint& pt);
		
	private:
		LayoutOrientation m_orientation;
		
		class Item
		{
		public:
			Ref<View> view;
			sl_real weight;
			sl_real minWeight;
			sl_real maxWeight;
			sl_ui_len minSize;
			sl_ui_len maxSize;
			sl_ui_len dividerWidth;
			Ref<Drawable> dividerBackground;
			Color dividerColor;
			
			sl_ui_pos pos;
			sl_ui_len width;
			
		public:
			Item();
			
		};
		CList<Item> m_items;
		
		sl_ui_len m_dividerWidth;
		Ref<Drawable> m_dividerBackground;
		Color m_dividerColor;
		sl_ui_len m_cursorMargin;
		
		sl_int32 m_indexDividerDown;
		sl_ui_pos m_posDown;
		sl_real m_weightDown;
		
		Ref<Cursor> m_cursor;
		
	};
	
	class SLIB_EXPORT VerticalSplitView : public SplitView
	{
	public:
		VerticalSplitView();

		~VerticalSplitView();

	};
	
	class SLIB_EXPORT HorizontalSplitView : public SplitView
	{
	public:
		HorizontalSplitView();

		~HorizontalSplitView();

	};

}

#endif
