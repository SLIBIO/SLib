#ifndef CHECKHEADER_SLIB_UI_PROGRESS_BAR
#define CHECKHEADER_SLIB_UI_PROGRESS_BAR

#include "definition.h"

#include "view.h"

typedef double sl_progress_bar_pos;

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ProgressBar : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ProgressBar();
	
public:
	LayoutOrientation getOrientation();
	
	void setOrientation(LayoutOrientation orientation, sl_bool flagRedraw = sl_true);
	
	Alignment getGravity();
	
	void setGravity(Alignment gravity, sl_bool flagRedraw = sl_true);
	
	sl_progress_bar_pos getValue();
	
	void setValue(sl_progress_bar_pos value, sl_bool flagRedraw = sl_true);
	
	sl_progress_bar_pos getMinimumValue();
	
	void setMinimumValue(sl_progress_bar_pos value, sl_bool flagRedraw = sl_true);
	
	sl_progress_bar_pos getMaximumValue();
	
	void setMaximumValue(sl_progress_bar_pos value, sl_bool flagRedraw = sl_true);
	
	sl_progress_bar_pos getRange();
	
	void setRange(sl_progress_bar_pos range, sl_bool flagRedraw = sl_true);

	
	Ref<Drawable> getBar();
	
	void setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);

	UIRect getBarRegion();
	
protected:
	// override
	void onDraw(Canvas* canvas);

protected:
	LayoutOrientation m_orientation;
	Alignment m_gravity;
	sl_scroll_pos m_value;
	sl_scroll_pos m_value_min;
	sl_scroll_pos m_value_max;
	
	SafeRef<Drawable> m_bar;
	
};

SLIB_UI_NAMESPACE_END

#endif
