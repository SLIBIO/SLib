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

void RenderView::onAttach()
{
	requestRender();
}

void RenderView::onFrame(RenderEngine* engine)
{
}

void RenderView::dispatchOnFrame(RenderEngine* engine)
{
	if (engine) {
		engine->beginScene();
	}
	onFrame(engine);
	if (engine) {
		engine->endScene();
	}
}
SLIB_UI_NAMESPACE_END
