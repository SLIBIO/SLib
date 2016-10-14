#ifndef CHECKHEADER_SLIB_UI_RENDER_VIEW
#define CHECKHEADER_SLIB_UI_RENDER_VIEW

#include "definition.h"

#include "view.h"

#include "../render/engine.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT RenderView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	RenderView();
	
public:
	RedrawMode getRedrawMode();
	
	void setRedrawMode(RedrawMode mode);
	
	void requestRender();
	
	// override
	void invalidate();
	
	// override
	void invalidate(const UIRect& rect);
	
public:
	SLIB_PROPERTY(RenderEngineType, PreferredEngineType)

protected:
	virtual void onFrame(RenderEngine* engine);
	
	// override
	void onAttach();
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* _parent);
	
	virtual void dispatchFrame(RenderEngine* engine);
	
private:
	void _setRedrawMode_NW(RedrawMode mode);

	void _requestRender_NW();
	
protected:
	RedrawMode m_redrawMode;
	
};

SLIB_UI_NAMESPACE_END

#endif
