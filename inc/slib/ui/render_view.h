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
	enum RenderMode
	{
		renderModeContinuously = 0,
		renderModeWhenDirty = 1
	};
	
public:
	RenderView();
	
public:
	virtual void requestRender();
	
	RenderMode getRenderMode();
	virtual void setRenderMode(RenderMode mode);

	// override
	void onAttach();
	
public:
	virtual void onFrame(RenderEngine* engine);
	virtual void dispatchOnFrame(RenderEngine* engine);
	
public:
	SLIB_PROPERTY_INLINE(RenderEngine::EngineType, PreferredEngineType)
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* _parent);
	
protected:
	RenderMode m_renderMode;
	
};
SLIB_UI_NAMESPACE_END

#endif
