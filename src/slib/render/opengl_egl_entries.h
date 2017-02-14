#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_EGL_ENTRIES
#define CHECKHEADER_SLIB_RENDER_OPENGL_EGL_ENTRIES

#include "../../../inc/slib/render/definition.h"

#include "../../../inc/thirdparty/gl/EGL/egl.h"
#include "../../../inc/thirdparty/gl/EGL/eglext.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#undef _SLIB_RENDER_EGL_ENTRY
#define _SLIB_RENDER_EGL_ENTRY(TYPE, name, ...) \
	TYPE(__stdcall* name)(__VA_ARGS__);

#define _SLIB_RENDER_EGL_ENTRIES \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglChooseConfig, EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglCopyBuffers, EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target) \
	_SLIB_RENDER_EGL_ENTRY(EGLContext, eglCreateContext, EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePbufferSurface, EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePixmapSurface, EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreateWindowSurface, EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglDestroyContext, EGLDisplay dpy, EGLContext ctx) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglDestroySurface, EGLDisplay dpy, EGLSurface surface) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglGetConfigAttrib, EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglGetConfigs, EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config) \
	_SLIB_RENDER_EGL_ENTRY(EGLDisplay, eglGetCurrentDisplay) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglGetCurrentSurface, EGLint readdraw) \
	_SLIB_RENDER_EGL_ENTRY(EGLDisplay, eglGetDisplay, EGLNativeDisplayType display_id) \
	_SLIB_RENDER_EGL_ENTRY(EGLint, eglGetError) \
	_SLIB_RENDER_EGL_ENTRY(__eglMustCastToProperFunctionPointerType, eglGetProcAddress, const char *procname) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglInitialize, EGLDisplay dpy, EGLint *major, EGLint *minor) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglMakeCurrent, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglQueryContext, EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value) \
	_SLIB_RENDER_EGL_ENTRY(const char *, eglQueryString, EGLDisplay dpy, EGLint name) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglQuerySurface, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglSwapBuffers, EGLDisplay dpy, EGLSurface surface) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglTerminate, EGLDisplay dpy) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitGL) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitNative, EGLint engine) \
	\
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglBindTexImage, EGLDisplay dpy, EGLSurface surface, EGLint buffer) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglReleaseTexImage, EGLDisplay dpy, EGLSurface surface, EGLint buffer) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglSurfaceAttrib, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglSwapInterval, EGLDisplay dpy, EGLint interval) \
	\
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglBindAPI, EGLenum api) \
	_SLIB_RENDER_EGL_ENTRY(EGLenum, eglQueryAPI) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePbufferFromClientBuffer, EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglReleaseThread) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitClient) \
	\
	_SLIB_RENDER_EGL_ENTRY(EGLContext, eglGetCurrentContext) \
/*	\
	_SLIB_RENDER_EGL_ENTRY(EGLSync, eglCreateSync, EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglDestroySync, EGLDisplay dpy, EGLSync sync) \
	_SLIB_RENDER_EGL_ENTRY(EGLint, eglClientWaitSync, EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglGetSyncAttrib, EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value) \
	_SLIB_RENDER_EGL_ENTRY(EGLDisplay, eglGetPlatformDisplay, EGLenum platform, void *native_display, const EGLAttrib *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePlatformWindowSurface, EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePlatformPixmapSurface, EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list) \
	_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitSync, EGLDisplay dpy, EGLSync sync, EGLint flags) \
*/

namespace slib
{
	struct _EGL_EntryPoints
	{
		_SLIB_RENDER_EGL_ENTRIES
	};
	extern _EGL_EntryPoints _EGL_ENTRIES;
}

#define _EGL_ENTRY(name) _EGL_ENTRIES.name

#else

#define _EGL_ENTRY(name) name

#endif

#endif
