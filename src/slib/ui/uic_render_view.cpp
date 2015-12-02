#include "../../../inc/slib/ui/render_view.h"

SLIB_UI_NAMESPACE_BEGIN
RenderView::RenderView()
{
	setPreferredEngineType(RenderEngine::OPENGL_ES);
	m_renderMode = renderModeContinuously;
}

RenderView::RenderMode RenderView::getRenderMode()
{
	return m_renderMode;
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
