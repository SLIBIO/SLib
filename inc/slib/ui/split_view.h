/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHorizontal();
		
		void setHorizontal(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isVertical();
		
		void setVertical(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_size getItemsCount();
		
		void setItemsCount(sl_size count, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<View> getItemView(sl_size index);
		
		void setItemView(sl_size index, const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getItemSize(sl_size index);
		
		void setItemSize(sl_size index, sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getItemWeight(sl_size index);
		
		void setItemWeight(sl_size index, sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getItemMinimumWeight(sl_size index);
		
		void setItemMinimumWeight(sl_size index, sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_real getItemMaximumWeight(sl_size index);
		
		void setItemMaximumWeight(sl_size index, sl_real weight, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getItemMinimumSize(sl_size index);
		
		void setItemMinimumSize(sl_size index, sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getItemMaximumSize(sl_size index);
		
		void setItemMaximumSize(sl_size index, sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getItemDividerWidth(sl_size index);
		
		void setItemDividerWidth(sl_size index, sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getItemDividerBackground(sl_size index);
		
		void setItemDividerBackground(sl_size index, const Ref<Drawable>& background, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getItemDividerColor(sl_size index);
		
		void setItemDividerColor(sl_size index, const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getDividerWidth();
		
		void setDividerWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getDividerBackground();
		
		void setDividerBackground(const Ref<Drawable>& background, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getDividerColor();
		
		void setDividerColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getCursorMargin();
		
		void setCursorMargin(sl_ui_len margin);
		
		void relayout(UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		// override
		void onResize(sl_ui_len width, sl_ui_len height);
		
		// override
		void onDraw(Canvas* canvas);
		
		// override
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
		
		// override
		void onMakeLayout();
		
	public:
		// override
		void dispatchMouseEvent(UIEvent* ev);
		
		// override
		void dispatchSetCursor(UIEvent* ev);
		
		// override
		sl_bool hitTestForCapturingChildInstanceEvents(const UIPoint& pt);
		
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
