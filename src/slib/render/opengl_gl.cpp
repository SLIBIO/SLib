/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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

#define PRIV_OPENGL_IMPL
#define GL_BASE GL
#define GL_ENGINE _priv_GL_Engine
#define GL_ENTRY(x) PRIV_GL_ENTRY(x)
#include "opengl_impl.h"

#	if defined (SLIB_PLATFORM_IS_WIN32)

namespace slib
{
	_priv_GL_EntryPoints _priv_GL_entries;
	
#undef PRIV_SLIB_RENDER_GL_ENTRY
#define PRIV_SLIB_RENDER_GL_ENTRY(TYPE, name, ...) \
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
	*((FARPROC*)(&(_priv_GL_entries.name))) = proc;
	
#undef PRIV_SLIB_RENDER_GL_SUPPORT
#define PRIV_SLIB_RENDER_GL_SUPPORT(name) \
	_priv_GL_entries.flagSupports##name = flagSupport; \
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
		PRIV_SLIB_RENDER_GL_ENTRIES
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


