/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_EGL_ENTRIES
#define CHECKHEADER_SLIB_RENDER_OPENGL_EGL_ENTRIES

#include "slib/render/definition.h"

#include "gl/EGL/egl.h"
#include "gl/EGL/eglext.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#undef PRIV_SLIB_RENDER_EGL_ENTRY
#define PRIV_SLIB_RENDER_EGL_ENTRY(TYPE, name, ...) \
	TYPE(__stdcall* name)(__VA_ARGS__);

#define PRIV_SLIB_RENDER_EGL_ENTRIES \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglChooseConfig, EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglCopyBuffers, EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLContext, eglCreateContext, EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePbufferSurface, EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePixmapSurface, EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreateWindowSurface, EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglDestroyContext, EGLDisplay dpy, EGLContext ctx) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglDestroySurface, EGLDisplay dpy, EGLSurface surface) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglGetConfigAttrib, EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglGetConfigs, EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLDisplay, eglGetCurrentDisplay) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglGetCurrentSurface, EGLint readdraw) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLDisplay, eglGetDisplay, EGLNativeDisplayType display_id) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLint, eglGetError) \
	PRIV_SLIB_RENDER_EGL_ENTRY(__eglMustCastToProperFunctionPointerType, eglGetProcAddress, const char *procname) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglInitialize, EGLDisplay dpy, EGLint *major, EGLint *minor) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglMakeCurrent, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglQueryContext, EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value) \
	PRIV_SLIB_RENDER_EGL_ENTRY(const char *, eglQueryString, EGLDisplay dpy, EGLint name) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglQuerySurface, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglSwapBuffers, EGLDisplay dpy, EGLSurface surface) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglTerminate, EGLDisplay dpy) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitGL) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitNative, EGLint engine) \
	\
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglBindTexImage, EGLDisplay dpy, EGLSurface surface, EGLint buffer) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglReleaseTexImage, EGLDisplay dpy, EGLSurface surface, EGLint buffer) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglSurfaceAttrib, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglSwapInterval, EGLDisplay dpy, EGLint interval) \
	\
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglBindAPI, EGLenum api) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLenum, eglQueryAPI) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePbufferFromClientBuffer, EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglReleaseThread) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitClient) \
	\
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLContext, eglGetCurrentContext) \
/*	\
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSync, eglCreateSync, EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglDestroySync, EGLDisplay dpy, EGLSync sync) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLint, eglClientWaitSync, EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglGetSyncAttrib, EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLDisplay, eglGetPlatformDisplay, EGLenum platform, void *native_display, const EGLAttrib *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePlatformWindowSurface, EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLSurface, eglCreatePlatformPixmapSurface, EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list) \
	PRIV_SLIB_RENDER_EGL_ENTRY(EGLBoolean, eglWaitSync, EGLDisplay dpy, EGLSync sync, EGLint flags) \
*/

namespace slib
{
	struct _priv_EGL_EntryPoints
	{
		PRIV_SLIB_RENDER_EGL_ENTRIES
	};
	extern _priv_EGL_EntryPoints _priv_EGL_entries;
}

#define PRIV_EGL_ENTRY(name) _priv_EGL_entries.name

#else

#define PRIV_EGL_ENTRY(name) name

#endif

#endif
