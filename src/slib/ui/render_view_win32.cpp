#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_RenderViewInstance : public Win32_ViewInstance, public IRenderCallback
{
public:
	Ref<Renderer> m_renderer;

public:
	_Win32_RenderViewInstance()
	{
	}

	~_Win32_RenderViewInstance()
	{
		if (m_renderer.isNotNull()) {
			m_renderer->release();
		}
	}

public:
	void setRenderer(const Ref<Renderer>& renderer, RedrawMode renderMode)
	{
		m_renderer = renderer;
		if (m_renderer.isNotNull()) {
			m_renderer->setRenderingContinuously(renderMode == redrawMode_Continuously);
		}
	}

	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		if (msg == WM_PAINT) {
			m_renderer->requestRender();
		} else if (msg == WM_ERASEBKGND) {
			result = TRUE;
			return sl_true;
		}
		return Win32_ViewInstance::processWindowMessage(msg, wParam, lParam, result);
	}

	void onFrame(RenderEngine* engine)
	{
		Ref<View> _view = getView();
		if (RenderView::checkInstance(_view)) {
			Ref<RenderView> view = Ref<RenderView>::from(_view);
			if (view.isNotNull()) {
				view->dispatchOnFrame(engine);
			}
		}
	}
};

Ref<ViewInstance> RenderView::createInstance(ViewInstance* parent)
{
	Win32_View_Shared* shared = Win32_View_Shared::get();
	DWORD styleEx = 0;
	DWORD style = 0;
	Ref<_Win32_RenderViewInstance> ret = Win32_ViewInstance::create<_Win32_RenderViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClass)), L"", style, styleEx);
	if (ret.isNotNull()) {
		RenderEngineType engineType = getPreferredEngineType();
		if (engineType == renderEngineType_OpenGL_ES) {
			EGL::loadEntries();
			GLES::loadEntries();
			if (!(EGL::isAvailable() && GLES::isAvailable())) {
				engineType = renderEngineType_OpenGL;
			}
		} else if (engineType == renderEngineType_OpenGL) {
		} else {
			engineType = renderEngineType_OpenGL;
		}
		if (engineType == renderEngineType_OpenGL_ES) {
			RendererParam rp;
			rp.callback = WeakRef<_Win32_RenderViewInstance>(ret);
			Ref<Renderer> renderer = EGL::createRenderer((void*)(ret->getHandle()), rp);
			if (renderer.isNotNull()) {
				ret->setRenderer(renderer, m_redrawMode);
				return ret;
			}
		} else if (engineType == renderEngineType_OpenGL) {
			RendererParam rp;
			rp.callback = WeakRef<_Win32_RenderViewInstance>(ret);
			Ref<Renderer> renderer = WGL::createRenderer((void*)(ret->getHandle()), rp);
			if (renderer.isNotNull()) {
				ret->setRenderer(renderer, m_redrawMode);
				return ret;
			}
		}
		ret.setNull();
	}
	return ret;
}

void RenderView::setRedrawMode(RedrawMode mode)
{
	Ref<_Win32_RenderViewInstance> instance = Ref<_Win32_RenderViewInstance>::from(getViewInstance());
	if (instance.isNotNull()) {
		Ref<Renderer> renderer = instance->m_renderer;
		if (renderer.isNotNull()) {
			renderer->setRenderingContinuously(mode == redrawMode_Continuously);
		}
	}
	m_redrawMode = mode;
}

void RenderView::requestRender()
{
	Ref<_Win32_RenderViewInstance> instance = Ref<_Win32_RenderViewInstance>::from(getViewInstance());
	if (instance.isNotNull()) {
		Ref<Renderer> renderer = instance->m_renderer;
		if (renderer.isNotNull()) {
			renderer->requestRender();
		}
	}
}

SLIB_UI_NAMESPACE_END

#endif
