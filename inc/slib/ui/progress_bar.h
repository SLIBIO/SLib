#ifndef CHECKHEADER_SLIB_UI_PROGRESS_BAR
#define CHECKHEADER_SLIB_UI_PROGRESS_BAR

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ProgressBar : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ProgressBar(LayoutOrientation orientation = LayoutOrientation::Horizontal);
	
public:
	LayoutOrientation getOrientation();
	
	void setOrientation(LayoutOrientation orientation, sl_bool flagRedraw = sl_true);
	
	sl_bool isVertical();
	
	void setVertical(sl_bool flagRedraw = sl_true);
	
	sl_bool isHorizontal();
	
	void setHorizontal(sl_bool flagRedraw = sl_true);
	
	float getMinimumValue();
	
	void setMinimumValue(float value, sl_bool flagRedraw = sl_true);
	
	float getMaximumValue();
	
	void setMaximumValue(float value, sl_bool flagRedraw = sl_true);
	
	float getRange();
	
	void setRange(float range, sl_bool flagRedraw = sl_true);
	
	float getValue();
	
	void setValue(float value, sl_bool flagRedraw = sl_true);
	
	float getSecondaryValue();
	
	void setSecondaryValue(float value, sl_bool flagRedraw = sl_true);
	
	sl_bool isDualValues();
	
	void setDualValues(sl_bool flagDualValues, sl_bool flagRedraw = sl_true);

	sl_bool isDiscrete();
	
	void setDiscrete(sl_bool flagDiscrete);
	
	float getStep();
	
	void setStep(float step);
	
	sl_bool isReversed();
	
	void setReversed(sl_bool flagReversed, sl_bool flagRedraw = sl_true);
	
	
	Ref<Drawable> getTrackDrawable();
	
	void setTrackDrawable(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setTrackColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getProgressDrawable();
	
	void setProgressDrawable(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setProgressColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getSecondaryProgressDrawable();
	
	void setSecondaryProgressDrawable(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setSecondaryProgressColor(const Color& color, sl_bool flagRedraw = sl_true);

protected:
	// override
	void onDraw(Canvas* canvas);
	
protected:
	float _refineValue(float value);
	
	float _refineStep();
	
	sl_ui_pos _getPositionFromValue(float value);
	
	void _getProgressRegions(UIRect& outProgress, UIRect& outSecondaryProgress);

protected:
	LayoutOrientation m_orientation;
	float m_value_min;
	float m_value_max;
	float m_value;
	float m_value2;
	sl_bool m_flagDualValues;
	sl_bool m_flagDiscrete;
	float m_step;
	sl_bool m_flagReversed;
	
	SafeRef<Drawable> m_track;
	SafeRef<Drawable> m_progress;
	SafeRef<Drawable> m_progress2;
	
};

SLIB_UI_NAMESPACE_END

#endif
