#ifndef CHECKHEADER_SLIB_UI_SCROLL_BAR
#define CHECKHEADER_SLIB_UI_SCROLL_BAR

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class ScrollBar;

class SLIB_EXPORT IScrollBarListener
{
public:
	virtual void onScroll(ScrollBar* scrollBar, sl_scroll_pos value) = 0;
};

class SLIB_EXPORT ScrollBar : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ScrollBar(LayoutOrientation orientation = LayoutOrientation::Horizontal);
	
public:
	LayoutOrientation getOrientation();
	
	void setOrientation(LayoutOrientation orientation, sl_bool flagRedraw = sl_true);
	
	sl_bool isVertical();
	
	void setVertical(sl_bool flagRedraw = sl_true);
	
	sl_bool isHorizontal();
	
	void setHorizontal(sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getValue();
	
	void setValue(sl_scroll_pos value, sl_bool flagRedraw = sl_true);
	
	void setValueOfOutRange(sl_scroll_pos value, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getPage();
	
	void setPage(sl_scroll_pos page, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getLine();
	
	void setLine(sl_scroll_pos line, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getMinimumValue();
	
	void setMinimumValue(sl_scroll_pos value, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getMaximumValue();
	
	void setMaximumValue(sl_scroll_pos value, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getRange();
	
	void setRange(sl_scroll_pos range, sl_bool flagRedraw = sl_true);

	
	Ref<Drawable> getBar();
	
	void setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getClickedBar();
	
	void setClickedBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getHoverBackground();
	
	void setHoverBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	sl_scroll_pos getMinimumBarLengthRatio();
	
	void setMinimumBarLengthRatio(sl_scroll_pos ratio);
	
	
	sl_bool getBarPositionRange(sl_ui_pos& begin, sl_ui_pos& end);
	
	sl_bool getBarRegion(UIRect& _out);
	
	sl_scroll_pos getValueFromBarPosition(sl_ui_pos pos);
	
	sl_bool isValid();
	
protected:
	// override
	void onDraw(Canvas* canvas);

	// override
	void onMouseEvent(UIEvent* ev);
	
	// override
	void onMouseWheelEvent(UIEvent* ev);
	
	virtual void onScroll(sl_scroll_pos value);
	
public:
	void dispatchScroll(sl_scroll_pos value);
	
public:
	SLIB_PTR_PROPERTY(IScrollBarListener, Listener);

protected:
	LayoutOrientation m_orientation;
	sl_scroll_pos m_value;
	sl_scroll_pos m_page;
	sl_scroll_pos m_line;
	sl_scroll_pos m_value_min;
	sl_scroll_pos m_value_max;
	
	SafeRef<Drawable> m_bar;
	SafeRef<Drawable> m_clickedBar;
	SafeRef<Drawable> m_hoverBackground;
	sl_scroll_pos m_bar_len_ratio_min;
	
private:
	sl_ui_pos m_posDown;
	sl_scroll_pos m_valueDown;
	
};

SLIB_UI_NAMESPACE_END

#endif
