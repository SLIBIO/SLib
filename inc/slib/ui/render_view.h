#ifndef CHECKHEADER_SLIB_UI_RENDER_VIEW
#define CHECKHEADER_SLIB_UI_RENDER_VIEW

#include "definition.h"

#include "generic_view.h"

#include "../render/engine.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT RenderView : public GenericViewWithEvent
{
	SLIB_DECLARE_OBJECT(RenderView, GenericViewWithEvent)
public:
	RenderView();
	
public:
	virtual void requestRender();
	
	RedrawMode getRedrawMode();
	virtual void setRedrawMode(RedrawMode mode);

	// override
	void onAttach();
	
public:
	virtual void onFrame(RenderEngine* engine);
	virtual void dispatchOnFrame(RenderEngine* engine);
	
public:
	SLIB_PROPERTY_INLINE(RenderEngineType, PreferredEngineType)
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* _parent);
	
protected:
	RedrawMode m_redrawMode;
	
};
SLIB_UI_NAMESPACE_END

#endif
