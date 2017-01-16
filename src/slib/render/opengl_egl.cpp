#include "../../../inc/slib/render/opengl_egl.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_EGL)

#include "opengl_egl_entries.h"

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/platform_windows.h"

SLIB_RENDER_NAMESPACE_BEGIN

class _EGLRendererImpl : public Renderer
{
public:
	sl_bool m_flagRequestRender;

	EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
	EGLConfig m_config;

	RendererParam m_param;

	AtomicRef<Thread> m_threadRender;

	EGLNativeWindowType m_hWindow;
	EGLNativeDisplayType m_hDisplay;

public:
	_EGLRendererImpl()
	{
		m_context = sl_null;
		m_flagRequestRender = sl_true;
	}

	~_EGLRendererImpl()
	{
		release();
	}

public:
	
#if defined(SLIB_PLATFORM_IS_WIN32)
	static EGLNativeDisplayType createDisplay(EGLNativeWindowType window)
	{
		HWND hWnd = (HWND)window;
		HDC hDC = ::GetDC(hWnd);
		return (EGLNativeDisplayType)hDC;
	}

	static void releaseDisplay(EGLNativeWindowType window, EGLNativeDisplayType display)
	{
		HWND hWnd = (HWND)window;
		HDC hDC = (HDC)display;
		::ReleaseDC(hWnd, hDC);
	}

	static sl_bool isWindowVisible(EGLNativeWindowType window)
	{
		return Windows::isWindowVisible((HWND)window);
	}

	static Sizei getWindowSize(EGLNativeWindowType window)
	{
		RECT rc;
		::GetClientRect((HWND)window, &rc);
		return Sizei((sl_int32)(rc.right), (sl_int32)(rc.bottom));
	}
#endif

	static Ref<_EGLRendererImpl> create(void* _windowHandle, const RendererParam& _param)
	{
		EGLNativeWindowType windowHandle = (EGLNativeWindowType)_windowHandle;
		if (windowHandle == 0) {
			return sl_null;
		}

		RendererParam param = _param;

		PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)(_EGL_ENTRY(eglGetProcAddress)("eglGetPlatformDisplayEXT"));
		if (!eglGetPlatformDisplayEXT) {
			return sl_null;
		}

		EGLint _majorVersion = EGL_DONT_CARE;
		EGLint _minorVersion = EGL_DONT_CARE;
		EGLint _useWarp = EGL_FALSE;
		EGLint _platform = 0;
		EGLint _renderer = 0;
		EGLint _clientVersion = 0;
		
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		_platform = EGL_PLATFORM_ANGLE_ANGLE;
		_renderer = EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE;
		_clientVersion = 2;
#endif
		if (_platform == 0) {
			return sl_null;
		}

		const EGLint displayAttributes[] =
		{
			EGL_PLATFORM_ANGLE_TYPE_ANGLE, _renderer,
			EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, _majorVersion,
			EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, _minorVersion,
			EGL_PLATFORM_ANGLE_USE_WARP_ANGLE, _useWarp,
			EGL_NONE
		};
		EGLNativeDisplayType displayHandle = createDisplay(windowHandle);
		if (displayHandle) {
			EGLDisplay display = eglGetPlatformDisplayEXT(_platform, displayHandle, displayAttributes);
			if (display != EGL_NO_DISPLAY) {
				EGLint majorVersion, minorVersion;
				if (_EGL_ENTRY(eglInitialize)(display, &majorVersion, &minorVersion)) {
					_EGL_ENTRY(eglBindAPI)(EGL_OPENGL_ES_API);
					if (_EGL_ENTRY(eglGetError)() == EGL_SUCCESS) {
						const EGLint configAttributes[] =
						{
							EGL_RED_SIZE, (param.nRedBits >= 0) ? param.nRedBits : EGL_DONT_CARE,
							EGL_GREEN_SIZE, (param.nGreenBits >= 0) ? param.nGreenBits : EGL_DONT_CARE,
							EGL_BLUE_SIZE, (param.nBlueBits >= 0) ? param.nBlueBits : EGL_DONT_CARE,
							EGL_ALPHA_SIZE, (param.nAlphaBits >= 0) ? param.nAlphaBits : EGL_DONT_CARE,
							EGL_DEPTH_SIZE, (param.nDepthBits >= 0) ? param.nDepthBits : EGL_DONT_CARE,
							EGL_STENCIL_SIZE, (param.nStencilBits >= 0) ? param.nStencilBits : EGL_DONT_CARE,
							EGL_SAMPLE_BUFFERS, param.flagMultisample ? 1 : 0,
							EGL_NONE
						};
						EGLint configCount;
						EGLConfig config;
						if (_EGL_ENTRY(eglChooseConfig)(display, configAttributes, &config, 1, &configCount)) {
							if (configCount == 1) {
								EGLint nRedBits;
								_EGL_ENTRY(eglGetConfigAttrib)(display, config, EGL_RED_SIZE, &nRedBits);
								param.nRedBits = nRedBits;
								EGLint nGreenBits;
								_EGL_ENTRY(eglGetConfigAttrib)(display, config, EGL_GREEN_SIZE, &nGreenBits);
								param.nGreenBits = nGreenBits;
								EGLint nBlueBits;
								_EGL_ENTRY(eglGetConfigAttrib)(display, config, EGL_BLUE_SIZE, &nBlueBits);
								param.nBlueBits = nBlueBits;
								EGLint nAlphaBits;
								_EGL_ENTRY(eglGetConfigAttrib)(display, config, EGL_ALPHA_SIZE, &nAlphaBits);
								param.nAlphaBits = nAlphaBits;
								EGLint nDepthBits;
								_EGL_ENTRY(eglGetConfigAttrib)(display, config, EGL_DEPTH_SIZE, &nDepthBits);
								param.nDepthBits = nDepthBits;
								EGLint nStencilBits;
								_EGL_ENTRY(eglGetConfigAttrib)(display, config, EGL_STENCIL_SIZE, &nStencilBits);
								param.nStencilBits = nStencilBits;

								const EGLint surfaceAttributes[] =
								{
									EGL_POST_SUB_BUFFER_SUPPORTED_NV, EGL_TRUE,
									EGL_NONE, EGL_NONE,
								};

								EGLSurface surface = _EGL_ENTRY(eglCreateWindowSurface)(display, config, windowHandle, surfaceAttributes);
								if (surface != EGL_NO_SURFACE) {

									if (_EGL_ENTRY(eglGetError()) == EGL_SUCCESS) {

										EGLint contextAttibutes[] =
										{
											EGL_CONTEXT_CLIENT_VERSION, _clientVersion,
											EGL_NONE
										};

										EGLContext context = _EGL_ENTRY(eglCreateContext)(display, config, NULL, contextAttibutes);
										if (_EGL_ENTRY(eglGetError()) == EGL_SUCCESS) {

											Ref<_EGLRendererImpl> ret = new _EGLRendererImpl();

											if (ret.isNotNull()) {

												ret->m_param = param;

												ret->m_hWindow = windowHandle;
												ret->m_hDisplay = displayHandle;

												ret->m_display = display;
												ret->m_surface = surface;
												ret->m_context = context;
												ret->m_config = config;
												ret->m_callback = param.callback;

												ret->m_threadRender = Thread::start(SLIB_FUNCTION_CLASS(_EGLRendererImpl, run, ret.get()));

												return ret;
											}

											_EGL_ENTRY(eglDestroyContext)(display, context);
										}
									}

									_EGL_ENTRY(eglDestroySurface)(display, surface);
								}
							}
						}
					}
				}
				_EGL_ENTRY(eglTerminate)(display);
			}
			releaseDisplay(windowHandle, displayHandle);
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
			_EGL_ENTRY(eglDestroyContext)(m_display, m_context);
			_EGL_ENTRY(eglDestroySurface)(m_display, m_surface);
			_EGL_ENTRY(eglTerminate)(m_display);

			releaseDisplay(m_hWindow, m_hDisplay);

			m_context = sl_null;
		}
	}

	void run()
	{
		_EGL_ENTRY(eglMakeCurrent)(m_display, m_surface, m_surface, m_context);

		Ref<RenderEngine> engine = GLES::createEngine();
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
		_EGL_ENTRY(eglMakeCurrent)(NULL, NULL, NULL, NULL);
	}

	void runStep(RenderEngine* engine)
	{
		PtrLocker<IRenderCallback> callback(m_callback);
		if (callback.isNotNull()) {
			if (!isWindowVisible(m_hWindow)) {
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
				Sizei size = getWindowSize(m_hWindow);
				if (size.x != 0 && size.y != 0) {
					engine->setViewport(0, 0, size.x, size.y);
					callback->onFrame(engine);
					_EGL_ENTRY(eglSwapInterval)(m_display, 0);
					_EGL_ENTRY(eglSwapBuffers)(m_display, m_surface);
				}
			}
		}
	}

	void requestRender()
	{
		m_flagRequestRender = sl_true;
	}

};

Ref<Renderer> EGL::createRenderer(void* windowHandle, const RendererParam& param)
{
	return _EGLRendererImpl::create(windowHandle, param);
}

SLIB_RENDER_NAMESPACE_END

#if defined (SLIB_PLATFORM_IS_WIN32)
SLIB_RENDER_NAMESPACE_BEGIN
_EGL_EntryPoints _EGL_ENTRIES;

#undef _SLIB_RENDER_EGL_ENTRY
#define _SLIB_RENDER_EGL_ENTRY(TYPE, name, ...) \
	proc = ::GetProcAddress(hDll, #name); \
if (proc == 0) { \
	LogError("EGL", "Failed to get function entry point - " #name); \
	return; \
} \
	*((FARPROC*)(&(_EGL_ENTRIES.name))) = proc;

static sl_bool _g_render_egl_flagLoadedEntryPoints = sl_false;

void EGL::loadEntries(const String& _pathDll, sl_bool flagReload)
{
	String16 pathDll = _pathDll;
	if (pathDll.isEmpty()) {
		return;
	}
	if (!flagReload) {
		if (_g_render_egl_flagLoadedEntryPoints) {
			return;
		}
	}
	HMODULE hDll;
	hDll = ::LoadLibraryW((LPCWSTR)(pathDll.getData()));
	if (!hDll) {
		//LogError("GLES", "Failed to load EGL dll - %s", pathEGL);
		return;
	}
	FARPROC proc;
#define ENTRIES _EGL_ENTRIES
	_SLIB_RENDER_EGL_ENTRIES

		_g_render_egl_flagLoadedEntryPoints = sl_true;
}

void EGL::loadEntries(sl_bool flagReload)
{
	SLIB_STATIC_STRING16(s, "libEGL.dll");
	loadEntries(s, flagReload);
}

sl_bool EGL::isAvailable()
{
	return _g_render_egl_flagLoadedEntryPoints;
}
SLIB_RENDER_NAMESPACE_END

#else

SLIB_RENDER_NAMESPACE_BEGIN
void EGL::loadEntries(const String& pathDll, sl_bool flagReload)
{
}

void EGL::loadEntries(sl_bool flagReload)
{
}

sl_bool EGL::isAvailable()
{
	return sl_true;
}
SLIB_RENDER_NAMESPACE_END
#endif

#else

SLIB_RENDER_NAMESPACE_BEGIN
Ref<Renderer> EGL::createRenderer(void* windowHandle, const RendererParam& param)
{
	return sl_null;
}

void EGL::loadEntries(const String& pathDll, sl_bool flagReload)
{
}

void EGL::loadEntries(sl_bool flagReload)
{
}

sl_bool EGL::isAvailable()
{
	return sl_false;
}
SLIB_RENDER_NAMESPACE_END

#endif
