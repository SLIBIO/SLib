#include "../../../inc/slib/ui/render_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(RenderView, GenericViewWithEvent)

RenderView::RenderView()
{
	setPreferredEngineType(RenderEngineType::OpenGL_ES);
	m_redrawMode = RedrawMode::Continuously;
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
