#ifndef CHECKHEADER_SLIB_UI_SLIDER
#define CHECKHEADER_SLIB_UI_SLIDER

#include "definition.h"

#include "progress_bar.h"

SLIB_UI_NAMESPACE_BEGIN

class Slider;

class SLIB_EXPORT ISliderListener
{
public:
	virtual void onValueChanged(Slider* slider, float value) = 0;
};

class SLIB_EXPORT Slider : public ProgressBar
{
	SLIB_DECLARE_OBJECT
	
public:
	Slider(LayoutOrientation orientation = LayoutOrientation::Horizontal);
	
public:
	Ref<Drawable> getThumbDrawable();
	
	void setThumbDrawable(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setThumbColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getPressedThumbDrawable();
	
	void setPressedThumbDrawable(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setPressedThumbColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getHoverThumbDrawable();
	
	void setHoverThumbDrawable(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setHoverThumbColor(const Color& color, sl_bool flagRedraw = sl_true);


	const UISize& getThumbSize();
	
	virtual void setThumbSize(const UISize& size, sl_bool flagRedraw = sl_true);
	
	void setThumbSize(sl_ui_len width, sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	void setThumbSize(sl_ui_len size, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getThumbWidth();
	
	void setThumbWidth(sl_ui_len width, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getThumbHeight();
	
	void setThumbHeight(sl_ui_len height, sl_bool flagRedraw = sl_true);
		
protected:
	// override
	void onDraw(Canvas* canvas);
	
	// override
	void onMouseEvent(UIEvent* ev);
	
	// override
	void onMouseWheelEvent(UIEvent* ev);
	
	// override
	void onKeyEvent(UIEvent* event);
	
	virtual void onValueChanged(float value);
	
public:
	virtual void dispatchValueChanged(float value);
	
protected:
	void _drawTrack(Canvas* canvas, const Ref<Drawable>& track, const Rectangle& rectDst);
	
	void _drawThumb(Canvas* canvas, const Ref<Drawable>& thumb, const Rectangle& rectDst);
	
	sl_ui_pos _getThumbLength();
	
	sl_ui_pos _getStartPadding();
	
	sl_ui_pos _getEndPadding();
	
	sl_ui_pos _getMinimumPadding();
	
	sl_ui_pos _getPositionFromValue(float value);
	
	float _getValueFromPosition(sl_ui_pos pos);
	
	void _getRegions(UIRect& outTrack, UIRect& outProgress, UIRect& outSecondaryProgress, UIRect& outThumb, UIRect& outSecondaryThumb);
	
	void _setHoverThumb(int index);
		
public:
	SLIB_PTR_PROPERTY(ISliderListener, Listener);
	
protected:
	SafeRef<Drawable> m_thumb;
	SafeRef<Drawable> m_pressedThumb;
	SafeRef<Drawable> m_hoverThumb;

	UISize m_thumbSize;

	int m_indexHoverThumb;
	int m_indexPressedThumb;
	
};

SLIB_UI_NAMESPACE_END

#endif
