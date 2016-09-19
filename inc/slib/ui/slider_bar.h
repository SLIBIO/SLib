#ifndef CHECKHEADER_SLIB_UI_SLIDER_BAR
#define CHECKHEADER_SLIB_UI_SLIDER_BAR

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

typedef double sl_slider_bar_value;

class SliderBar;

class SLIB_EXPORT ISliderBarListener
{
public:
	virtual void onValueChanged(SliderBar* slider, sl_slider_bar_value value) = 0;
};

class SLIB_EXPORT SliderBar : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	SliderBar(LayoutOrientation orientation = LayoutOrientation::Horizontal);
	
public:
	LayoutOrientation getOrientation();
	
	void setOrientation(LayoutOrientation orientation, sl_bool flagRedraw = sl_true);
	
	sl_bool isVertical();
	
	void setVertical(sl_bool flagRedraw);
	
	sl_bool isHorizontal();
	
	void setHorizontal(sl_bool flagRedraw);
	
	sl_slider_bar_value getValue();
	
	void setValue(sl_slider_bar_value value, sl_bool flagRedraw = sl_true);
	
	sl_slider_bar_value getMinimumValue();
	
	void setMinimumValue(sl_slider_bar_value value, sl_bool flagRedraw = sl_true);
	
	sl_slider_bar_value getMaximumValue();
	
	void setMaximumValue(sl_slider_bar_value value, sl_bool flagRedraw = sl_true);
	
	sl_slider_bar_value getRange();
	
	void setRange(sl_slider_bar_value range, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getBar();
	
	void setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getClickedBar();
	
	void setClickedBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getHoverBar();
	
	void setHoverBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	sl_real getBarLengthRatio();
	
	void setBarLengthRatio(sl_real ratio, sl_bool flagRedraw = sl_true);
	
	sl_slider_bar_value getLine();
	
	void setLine(sl_slider_bar_value line);
	
	
	sl_slider_bar_value getValueFromBarPosition(sl_ui_pos pos);

	sl_bool isValid();
	
	sl_bool getBarPositionRange(sl_ui_pos& pos_begin, sl_ui_pos& pos_end);
	
	sl_bool getBarRegion(UIRect& region);
	
protected:
	// override
	void onDraw(Canvas* canvas);
	
	// override
	void onMouseEvent(UIEvent* ev);
	
	// override
	void onMouseWheelEvent(UIEvent* ev);
	
	// override
	void onKeyEvent(UIEvent* event);
	
	virtual void onValueChanged(sl_slider_bar_value value);
	
public:
	virtual void dispatchValueChanged(sl_slider_bar_value value);
	
public:
	SLIB_PTR_PROPERTY(ISliderBarListener, Listener);
	
protected:
	LayoutOrientation m_orientation;
	sl_slider_bar_value m_value;
	sl_slider_bar_value m_value_min;
	sl_slider_bar_value m_value_max;
	sl_slider_bar_value m_line;
	
	SafeRef<Drawable> m_bar;
	SafeRef<Drawable> m_clickedBar;
	SafeRef<Drawable> m_hoverBar;
	sl_real m_barLengthRatio;

private:
	sl_bool m_flagHoverBar;
	sl_ui_pos m_posDown;
	sl_scroll_pos m_valueDown;
};


SLIB_UI_NAMESPACE_END

#endif
