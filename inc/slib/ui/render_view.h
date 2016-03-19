#ifndef CHECKHEADER_SLIB_UI_RENDER_VIEW
#define CHECKHEADER_SLIB_UI_RENDER_VIEW

#include "definition.h"

#include "generic_view.h"

#include "../render/engine.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT RenderView : public GenericViewWithEvent
{
	SLIB_DECLARE_OBJECT
	
public:
	RenderView();
	
public:
	virtual void requestRender();
	
	RedrawMode getRedrawMode();
	
	virtual void setRedrawMode(RedrawMode mode);
	
public:
	SLIB_PROPERTY(RenderEngineType, PreferredEngineType)

protected:
	virtual void onFrame(RenderEngine* engine);
	
public:
	virtual void dispatchFrame(RenderEngine* engine);
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* _parent);
	
	// override
	void onAttach();
	
protected:
	RedrawMode m_redrawMode;
	
};

SLIB_UI_NAMESPACE_END

#endif
