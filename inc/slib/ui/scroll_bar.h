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
	
	void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isVertical();
	
	void setVertical(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isHorizontal();
	
	void setHorizontal(UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getValue();
	
	void setValue(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setValueOfOutRange(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getPage();
	
	void setPage(sl_scroll_pos page, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getLine();
	
	void setLine(sl_scroll_pos line, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getMinimumValue();
	
	void setMinimumValue(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getMaximumValue();
	
	void setMaximumValue(sl_scroll_pos value, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_scroll_pos getRange();
	
	void setRange(sl_scroll_pos range, UIUpdateMode mode = UIUpdateMode::Redraw);

	
	Ref<Drawable> getThumbDrawable();
	
	void setThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	Ref<Drawable> getPressedThumbDrawable();
	
	void setPressedThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setPressedThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Drawable> getHoverThumbDrawable();
	
	void setHoverThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setHoverThumbColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	
	Ref<Drawable> getTrackDrawable();
	
	void setTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	Ref<Drawable> getPressedTrackDrawable();
	
	void setPressedTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setPressedTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Ref<Drawable> getHoverTrackDrawable();
	
	void setHoverTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void setHoverTrackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

	
	float getMinimumThumbLengthRatio();
	
	void setMinimumThumbLengthRatio(float ratio);
	
	
	sl_bool getThumbPositionRange(sl_ui_pos& begin, sl_ui_pos& end);
	
	sl_bool getThumbRegion(UIRect& _out);
	
	sl_scroll_pos getValueFromThumbPosition(sl_ui_pos pos);
	
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
	void _setHoverThumb(sl_bool flag);
	
protected:
	LayoutOrientation m_orientation;
	sl_scroll_pos m_value;
	sl_scroll_pos m_page;
	sl_scroll_pos m_line;
	sl_scroll_pos m_value_min;
	sl_scroll_pos m_value_max;
	
	SafeRef<Drawable> m_thumb;
	SafeRef<Drawable> m_pressedThumb;
	SafeRef<Drawable> m_hoverThumb;
	
	SafeRef<Drawable> m_track;
	SafeRef<Drawable> m_pressedTrack;
	SafeRef<Drawable> m_hoverTrack;
	
	float m_thumb_len_ratio_min;
	
	sl_ui_pos m_posDown;
	sl_scroll_pos m_valueDown;
	sl_bool m_flagHoverThumb;
	
};

SLIB_UI_NAMESPACE_END

#endif
