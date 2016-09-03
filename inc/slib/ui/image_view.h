#ifndef CHECKHEADER_SLIB_UI_IMAGE_VIEW
#define CHECKHEADER_SLIB_UI_IMAGE_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ImageView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ImageView();
	
public:
	Ref<Drawable> getSource();
	
	virtual void setSource(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	
	ScaleMode getScaleMode();
	
	virtual void setScaleMode(ScaleMode mode, sl_bool flagRedraw = sl_true);
	
	
	Alignment getGravity();
	
	virtual void setGravity(Alignment align, sl_bool flagRedraw = sl_true);
	
public:
	// override
	void onDraw(Canvas* canvas);
	
protected:
	SafeRef<Drawable> m_source;
	ScaleMode m_scaleMode;
	Alignment m_gravity;
	
};

SLIB_UI_NAMESPACE_END

#endif
