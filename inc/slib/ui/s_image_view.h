#ifndef CHECKHEADER_SLIB_UI_S_IMAGE_VIEW
#define CHECKHEADER_SLIB_UI_S_IMAGE_VIEW

#include "definition.h"

#include "s_view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT SImageView : public SView
{
	SLIB_DECLARE_OBJECT
	
public:
	SImageView();
	
public:
	Ref<Drawable> getSource();
	
	virtual void setSource(const Ref<Drawable>& drawable);
	
	
	ScaleMode getScaleMode();
	
	virtual void setScaleMode(ScaleMode mode);
	
	
	Alignments getSourceAlignment();
	
	virtual void setSourceAlignment(Alignments align);
	
public:
	// override
	void onDraw(Canvas* canvas);
	
protected:
	SafeRef<Drawable> m_source;
	ScaleMode m_scaleMode;
	Alignments m_sourceAlignment;
	
};

SLIB_UI_NAMESPACE_END

#endif
