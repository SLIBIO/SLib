#ifndef CHECKHEADER_SLIB_UI_SCROLL_BAR
#define CHECKHEADER_SLIB_UI_SCROLL_BAR

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class ScrollBar;

class SLIB_EXPORT IScrollBarListener
{
public:
	virtual void onScroll(ScrollBar* scrollBar, sl_real value) = 0;
};

class SLIB_EXPORT ScrollBar : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ScrollBar();
	
public:
	sl_bool isVertical();
	
	void setVertical(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	sl_bool isHorizontal();
	
	void setHorizontal(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	sl_real getValue();
	
	void setValue(sl_real value, sl_bool flagRedraw = sl_true);
	
	void setValueOfOutRange(sl_real value, sl_bool flagRedraw = sl_true);
	
	sl_real getPage();
	
	void setPage(sl_real page, sl_bool flagRedraw = sl_true);
	
	sl_real getLine();
	
	void setLine(sl_real line, sl_bool flagRedraw = sl_true);
	
	sl_real getMinimumValue();
	
	void setMinimumValue(sl_real value, sl_bool flagRedraw = sl_true);
	
	sl_real getMaximumValue();
	
	void setMaximumValue(sl_real value, sl_bool flagRedraw = sl_true);
	
	sl_real getRange();

	
	Ref<Drawable> getBar();
	
	void setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getClickedBar();
	
	void setClickedBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getHoverBackground();
	
	void setHoverBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	sl_real getMinimumBarLengthRatio();
	
	void setMinimumBarLengthRatio(sl_real ratio);
	
	
	sl_bool getBarPositionRange(sl_real& begin, sl_real& end);
	
	sl_bool getBarRegion(Rectangle& _out);
	
	sl_real getValueFromBarPosition(sl_real pos);
	
	sl_bool isValid();
	
protected:
	// override
	void onDrawBackground(Canvas* canvas);
	
	// override
	void onDraw(Canvas* canvas);

	// override
	void onMouseEvent(UIEvent* ev);
	
	// override
	void onMouseWheelEvent(UIEvent* ev);
	
	virtual void onScroll(sl_real value);
	
public:
	void dispatchScroll(sl_real value);
	
public:
	SLIB_PTR_PROPERTY(IScrollBarListener, Listener);

protected:
	sl_bool m_flagVertical;
	sl_real m_value;
	sl_real m_page;
	sl_real m_line;
	sl_real m_value_min;
	sl_real m_value_max;
	
	SafeRef<Drawable> m_bar;
	SafeRef<Drawable> m_clickedBar;
	SafeRef<Drawable> m_hoverBackground;
	sl_real m_bar_len_ratio_min;
	
private:
	sl_real m_posDown;
	sl_real m_valueDown;
	
};

SLIB_UI_NAMESPACE_END

#endif
