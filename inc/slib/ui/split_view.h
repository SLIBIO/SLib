#ifndef CHECKHEADER_SLIB_UI_SPLIT_VIEW
#define CHECKHEADER_SLIB_UI_SPLIT_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT SplitView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	SplitView();
	
public:
	LayoutOrientation getOrientation();
	
	void setOrientation(LayoutOrientation orientation, sl_bool flagRelayout = sl_true);
	
	sl_bool isHorizontal();
	
	void setHorizontal(sl_bool flagRelayout = sl_true);
	
	sl_bool isVertical();
	
	void setVertical(sl_bool flagRelayout = sl_true);
	
	sl_size getItemsCount();
	
	void setItemsCount(sl_size count, sl_bool flagRelayout = sl_true);
	
	Ref<View> getItemView(sl_size index);
	
	void setItemView(sl_size index, const Ref<View>& view, sl_bool flagRelayout = sl_true);
	
	sl_ui_len getItemSize(sl_size index);
	
	void setItemSize(sl_size index, sl_ui_len size, sl_bool flagRelayout = sl_true);
	
	sl_real getItemWeight(sl_size index);
	
	void setItemWeight(sl_size index, sl_real weight, sl_bool flagRelayout = sl_true);
	
	sl_real getItemMinimumWeight(sl_size index);
	
	void setItemMinimumWeight(sl_size index, sl_real weight, sl_bool flagRelayout = sl_true);
	
	sl_real getItemMaximumWeight(sl_size index);
	
	void setItemMaximumWeight(sl_size index, sl_real weight, sl_bool flagRelayout = sl_true);
	
	sl_ui_len getItemMinimumSize(sl_size index);
	
	void setItemMinimumSize(sl_size index, sl_ui_len size, sl_bool flagRelayout = sl_true);
	
	sl_ui_len getItemMaximumSize(sl_size index);
	
	void setItemMaximumSize(sl_size index, sl_ui_len size, sl_bool flagRelayout = sl_true);
	
	sl_ui_len getItemDividerWidth(sl_size index);
	
	void setItemDividerWidth(sl_size index, sl_ui_len width, sl_bool flagRelayout = sl_true);
	
	Ref<Drawable> getItemDividerBackground(sl_size index);
	
	void setItemDividerBackground(sl_size index, const Ref<Drawable>& background, sl_bool flagRedraw = sl_true);
	
	Color getItemDividerColor(sl_size index);
	
	void setItemDividerColor(sl_size index, const Color& color, sl_bool flagRedraw = sl_true);

	sl_ui_len getDividerWidth();
	
	void setDividerWidth(sl_ui_len width, sl_bool flagRelayout = sl_true);
	
	Ref<Drawable> getDividerBackground();
	
	void setDividerBackground(const Ref<Drawable>& background, sl_bool flagRedraw = sl_true);
	
	Color getDividerColor();
	
	void setDividerColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getCursorMargin();
	
	void setCursorMargin(sl_ui_len margin);

	void relayout(sl_bool flagRedraw = sl_true);
	
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
	Ref<ViewInstance> createGenericInstance(ViewInstance* parent);
	
	// override
	void dispatchMouseEvent(UIEvent* ev);
	
	// override
	void dispatchSetCursor(UIEvent* ev);
	
protected:
	sl_ui_len _getTotalSize();

	void _refreshItemFrames(sl_bool flagRedraw);
	
	void _resetWeights(sl_bool flagRelayout);
	
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
};

class SLIB_EXPORT HorizontalSplitView : public SplitView
{
public:
	HorizontalSplitView();
};

SLIB_UI_NAMESPACE_END

#endif
