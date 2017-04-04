/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/render/opengl.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(GLRenderEngine, RenderEngine)
	
	GLRenderEngine::GLRenderEngine()
	{
	}
	
	GLRenderEngine::~GLRenderEngine()
	{
	}
	
	sl_int32 GLRenderEngine::getAttributeLocation(sl_uint32 program, const String& _name)
	{
		String name(_name);
		return getAttributeLocation(program, name.getData());
	}
	
	sl_int32 GLRenderEngine::getUniformLocation(sl_uint32 program, const String& _name)
	{
		String name(_name);
		return getUniformLocation(program, name.getData());
	}

}

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GL)

#include "opengl_gl.h"

#include "slib/render/program.h"
#include "slib/core/queue.h"

#define _OPENGL_IMPL
#define GL_BASE GL
#define GL_ENGINE _GL_Engine
#define GL_ENTRY(x) _GL_ENTRY(x)
#include "opengl_impl.h"

#	if defined (SLIB_PLATFORM_IS_WIN32)

namespace slib
{
	_GL_ENTRY_POINTS _GL_ENTRIES;
	
#undef _SLIB_RENDER_GL_ENTRY
#define _SLIB_RENDER_GL_ENTRY(TYPE, name, ...) \
	if (hDll) { \
		proc = ::GetProcAddress(hDll, #name); \
	} else { \
		proc = wglGetProcAddress(#name); \
		if (proc == 0) { \
			proc = ::GetProcAddress(hDllCommon, #name); \
		} \
	} \
	if (proc == 0) { \
		flagSupport = sl_false; \
	} \
	*((FARPROC*)(&(_GL_ENTRIES.name))) = proc;
	
#undef _SLIB_RENDER_GL_SUPPORT
#define _SLIB_RENDER_GL_SUPPORT(name) \
	_GL_ENTRIES.flagSupports##name = flagSupport; \
	flagSupport = sl_true;
	
	static sl_bool _g_render_GL_flagLoadedEntryPoints = sl_false;
	
	void GL::loadEntries(const String& _pathDll, sl_bool flagReload)
	{
		String16 pathDll = _pathDll;
		if (!flagReload) {
			if (_g_render_GL_flagLoadedEntryPoints) {
				return;
			}
		}
		HMODULE hDll;
		if (pathDll.isEmpty()) {
			hDll = NULL;
		} else {
			hDll = ::LoadLibraryW((LPCWSTR)(pathDll.getData()));
			if (!hDll) {
				//LogError("GLES", "Failed to load OpenGL dll - %s", pathDll);
				return;
			}
		}
		HMODULE hDllCommon = ::LoadLibraryW(L"opengl32.dll");
		if (hDllCommon == NULL) {
			return;
		}
		FARPROC proc;
		sl_bool flagSupport = sl_true;
		_SLIB_RENDER_GL_ENTRIES
		_g_render_GL_flagLoadedEntryPoints = sl_true;
	}
	
	void GL::loadEntries(sl_bool flagReload)
	{
		loadEntries(String::null(), flagReload);
	}
	
	sl_bool GL::isAvailable()
	{
		return _g_render_GL_flagLoadedEntryPoints;
	}
}

#	else

namespace slib
{
	void GL::loadEntries(const String& pathDll, sl_bool flagReload)
	{
	}
	
	void GL::loadEntries(sl_bool flagReload)
	{
	}
	
	sl_bool GL::isAvailable()
	{
		return sl_true;
	}
}

#	endif

#else

namespace slib
{
	Ref<GLRenderEngine> GL::createEngine()
	{
		return sl_null;
	}
	
	void GL::loadEntries(const String& pathDll, sl_bool flagReload)
	{
	}
	
	void GL::loadEntries(sl_bool flagReload)
	{
	}
	
	sl_bool GL::isAvailable()
	{
		return sl_false;
	}
}

#endif


