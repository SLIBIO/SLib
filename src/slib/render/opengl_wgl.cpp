#include "../../../inc/slib/render/opengl_wgl.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_WGL) && defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/platform_windows.h"

#pragma comment (lib, "opengl32.lib")

namespace slib
{

	class _WGLRendererImpl : public Renderer
	{
	public:
		sl_bool m_flagRequestRender;

		HGLRC m_context;

		HWND m_hWindow;
		HDC m_hDC;

		AtomicRef<Thread> m_threadRender;

	public:
		_WGLRendererImpl()
		{
			m_context = sl_null;
			m_flagRequestRender = sl_true;
		}

		~_WGLRendererImpl()
		{
			release();
		}

	public:
		static Ref<_WGLRendererImpl> create(void* _windowHandle, const RendererParam& _param)
		{
			HWND hWnd = (HWND)_windowHandle;
			if (hWnd == 0) {
				return sl_null;
			}

			HDC hDC = ::GetDC(hWnd);
			if (hDC) {

				RendererParam param = _param;

				PIXELFORMATDESCRIPTOR pfd;
				Base::zeroMemory(&pfd, sizeof(pfd));
				pfd.nSize = sizeof(pfd);
				pfd.nVersion = 1;
				pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
				pfd.iPixelType = PFD_TYPE_RGBA;
				pfd.cColorBits = param.nRedBits + param.nGreenBits + param.nBlueBits;
				pfd.cAlphaBits = param.nAlphaBits;
				pfd.cAccumBits = param.nAccumBits;
				pfd.cDepthBits = param.nDepthBits;
				pfd.cStencilBits = param.nStencilBits;
				pfd.iLayerType = PFD_MAIN_PLANE;

				int iPixelFormat = ::ChoosePixelFormat(hDC, &pfd);
				if (iPixelFormat) {

					if (::SetPixelFormat(hDC, iPixelFormat, &pfd)) {
						
						HGLRC context = wglCreateContext(hDC);
						if (context) {

							Ref<_WGLRendererImpl> ret = new _WGLRendererImpl();

							if (ret.isNotNull()) {
								ret->m_hWindow = hWnd;
								ret->m_hDC = hDC;
								ret->m_context = context;

								ret->m_threadRender = Thread::start(SLIB_FUNCTION_CLASS(_WGLRendererImpl, run, ret.get()));

								ret->m_callback = param.callback;

								return ret;
							}

							wglDeleteContext(context);
						}
					}
				}
				::ReleaseDC(hWnd, hDC);
			}
			return sl_null;
		}

		void release()
		{
			ObjectLocker lock(this);
			
			Ref<Thread> thread = m_threadRender;
			if (thread.isNotNull()) {
				thread->finishAndWait();
				m_threadRender.setNull();
			}

			if (m_context) {
				wglDeleteContext(m_context);
				::ReleaseDC(m_hWindow, m_hDC);
				m_context = sl_null;
			}
		}

		void run()
		{
			wglMakeCurrent(m_hDC, m_context);

			GL::loadEntries();

			Ref<RenderEngine> engine = GL::createEngine();
			if (engine.isNull()) {
				return;
			}

			TimeCounter timer;
			while (Thread::isNotStoppingCurrent()) {
				runStep(engine.get());
				if (Thread::isNotStoppingCurrent()) {
					sl_uint64 t = timer.getElapsedMilliseconds();
					if (t < 10) {
						Thread::sleep(10 - (sl_uint32)(t));
					}
					timer.reset();
				} else {
					break;
				}
			}
			wglMakeCurrent(NULL, NULL);
		}

		void runStep(RenderEngine* engine)
		{
			PtrLocker<IRenderCallback> callback(m_callback);
			if (callback.isNotNull()) {
				if (!(Windows::isWindowVisible(m_hWindow))) {
					return;
				}
				sl_bool flagUpdate = sl_false;
				if (isRenderingContinuously()) {
					flagUpdate = sl_true;
				} else {
					if (m_flagRequestRender) {
						flagUpdate = sl_true;
					}
				}
				m_flagRequestRender = sl_false;
				if (flagUpdate) {
					RECT rect;
					::GetClientRect(m_hWindow, &rect);
					if (rect.right != 0 && rect.bottom != 0) {
						engine->setViewport(0, 0, rect.right, rect.bottom);
						callback->onFrame(engine);
						::SwapBuffers(m_hDC);
					}
				}
			}
		}

		void requestRender()
		{
			m_flagRequestRender = sl_true;
		}

	};

	Ref<Renderer> WGL::createRenderer(void* windowHandle, const RendererParam& param)
	{
		return _WGLRendererImpl::create(windowHandle, param);
	}

}

#else

namespace slib
{

	Ref<Renderer> WGL::createRenderer(void* windowHandle, const RendererParam& param)
	{
		return sl_null;
	}

}

#endif
