#include "../../../inc/slib/ui/render_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(RenderView, View)

RenderView::RenderView()
{
	setCreatingNativeWidget(sl_true);
	
	setPreferredEngineType(RenderEngineType::OpenGL_ES);
	m_redrawMode = RedrawMode::Continuously;
}

RedrawMode RenderView::getRedrawMode()
{
	return m_redrawMode;
}

void RenderView::setRedrawMode(RedrawMode mode)
{
	m_redrawMode = mode;
	if (isNativeWidget()) {
		_setRedrawMode_NW(mode);
	}
}

void RenderView::requestRender()
{
	if (isNativeWidget()) {
		_requestRender_NW();
	}
}

void RenderView::invalidate()
{
	requestRender();
}

void RenderView::invalidate(const UIRect& rect)
{
	requestRender();
}

void RenderView::onFrame(RenderEngine* engine)
{
}

void RenderView::onAttach()
{
	requestRender();
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


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void RenderView::_setRedrawMode_NW(RedrawMode mode)
{
}

void RenderView::_requestRender_NW()
{
}

#endif

SLIB_UI_NAMESPACE_END
