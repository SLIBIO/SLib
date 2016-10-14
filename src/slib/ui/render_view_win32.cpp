#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN
class _Win32_RenderViewInstance : public Win32_ViewInstance, public IRenderCallback
{
public:
	SafeRef<Renderer> m_renderer;

public:
	_Win32_RenderViewInstance()
	{
	}

	~_Win32_RenderViewInstance()
	{
        Ref<Renderer> renderer = m_renderer;
		if (renderer.isNotNull()) {
			renderer->release();
		}
	}

public:
	void setRenderer(const Ref<Renderer>& renderer, RedrawMode redrawMode)
	{
		m_renderer = renderer;
		if (renderer.isNotNull()) {
			renderer->setRenderingContinuously(redrawMode == RedrawMode::Continuously);
		}
	}

    // override
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		if (msg == WM_PAINT) {
			PAINTSTRUCT ps;
			::BeginPaint(m_handle, &ps);
			::EndPaint(m_handle, &ps);
			Ref<Renderer> renderer = m_renderer;
			if (renderer.isNotNull()) {
				renderer->requestRender();
			}
			return sl_true;
		} else if (msg == WM_ERASEBKGND) {
			result = TRUE;
			return sl_true;
		}
		return Win32_ViewInstance::processWindowMessage(msg, wParam, lParam, result);
	}

    // override
	void onFrame(RenderEngine* engine)
	{
		Ref<View> _view = getView();
		if (RenderView::checkInstance(_view.ptr)) {
			Ref<RenderView> view = Ref<RenderView>::from(_view);
			if (view.isNotNull()) {
				view->dispatchFrame(engine);
			}
		}
	}
};

Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
{
	Win32_UI_Shared* shared = Win32_UI_Shared::get();
	if (!shared) {
		return Ref<ViewInstance>::null();
	}

	DWORD styleEx = 0;
	DWORD style = 0;
	Ref<_Win32_RenderViewInstance> ret = Win32_ViewInstance::create<_Win32_RenderViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
	if (ret.isNotNull()) {
		RenderEngineType engineType = getPreferredEngineType();
		if (engineType == RenderEngineType::OpenGL_ES) {
			EGL::loadEntries();
			GLES::loadEntries();
			if (!(EGL::isAvailable() && GLES::isAvailable())) {
				engineType = RenderEngineType::OpenGL;
			}
		} else if (engineType == RenderEngineType::OpenGL) {
		} else {
			engineType = RenderEngineType::OpenGL;
		}
		if (engineType == RenderEngineType::OpenGL_ES) {
			RendererParam rp;
			rp.callback = WeakRef<_Win32_RenderViewInstance>(ret);
			Ref<Renderer> renderer = EGL::createRenderer((void*)(ret->getHandle()), rp);
			if (renderer.isNotNull()) {
				ret->setRenderer(renderer, m_redrawMode);
				return ret;
			}
		} else if (engineType == RenderEngineType::OpenGL) {
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

void RenderView::_setRedrawMode_NW(RedrawMode mode)
{
	Ref<_Win32_RenderViewInstance> instance = Ref<_Win32_RenderViewInstance>::from(getViewInstance());
	if (instance.isNotNull()) {
		Ref<Renderer> renderer = instance->m_renderer;
		if (renderer.isNotNull()) {
			renderer->setRenderingContinuously(mode == RedrawMode::Continuously);
		}
	}
}

void RenderView::_requestRender_NW()
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
