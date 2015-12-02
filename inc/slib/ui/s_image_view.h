#ifndef CHECKHEADER_SLIB_UI_S_IMAGE_VIEW
#define CHECKHEADER_SLIB_UI_S_IMAGE_VIEW

#include "definition.h"
#include "s_view.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT SImageView : public SView
{
	SLIB_DECLARE_OBJECT(SImageView, SView)
public:
	SImageView();
	
public:
	Ref<Drawable> getSource();
	virtual void setSource(const Ref<Drawable>& drawable);
	
	ScaleType getScaleType();
	virtual void setScaleType(ScaleType type);
	
	Alignment getSourceAlignment();
	virtual void setSourceAlignment(Alignment align);
	
public:
	// override
	void onDraw(Canvas* canvas);
	
protected:
	Ref<Drawable> m_source;
	ScaleType m_scaleType;
	Alignment m_sourceAlignment;
};
SLIB_UI_NAMESPACE_END

#endif
