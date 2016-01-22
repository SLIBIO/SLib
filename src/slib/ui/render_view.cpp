#include "../../../inc/slib/ui/render_view.h"

SLIB_UI_NAMESPACE_BEGIN

RenderView::RenderView()
{
	setPreferredEngineType(renderEngineType_OpenGL_ES);
	m_redrawMode = redrawMode_Continuously;
}

RedrawMode RenderView::getRedrawMode()
{
	return m_redrawMode;
}

void RenderView::onFrame(RenderEngine* engine)
{
}

void RenderView::dispatchFrame(RenderEngine* engine)
{
	if (engine) {
		engine->beginScene();
	}
	onFrame(engine);
	if (engine) {
		engine->endScene();
	}
}

void RenderView::onAttach()
{
	requestRender();
}

SLIB_UI_NAMESPACE_END
