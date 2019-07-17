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

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GLES)

#include "opengl_gles.h"

#include "slib/render/program.h"
#include "slib/core/queue.h"

#define PRIV_OPENGL_ES_IMPL
#define GL_BASE GLES
#define GL_ENGINE GLES_Engine
#define GL_ENTRY(x) PRIV_GLES_ENTRY(x)
#include "opengl_impl.h"

#	if defined (SLIB_PLATFORM_IS_WIN32)

#include <Windows.h>

namespace slib
{

	namespace priv
	{
		namespace gles
		{

			EntryPoints g_entries;
	
			static sl_bool g_flagLoadedEntryPoints = sl_false;

		}
	}

	using namespace priv::gles;
	
#undef PRIV_SLIB_RENDER_GLES_ENTRY
#define PRIV_SLIB_RENDER_GLES_ENTRY(TYPE, name, ...) \
	proc = ::GetProcAddress(hDll, #name); \
	*((FARPROC*)(&(g_entries.name))) = proc;
	

	void GLES::loadEntries(const String& _pathDll, sl_bool flagReload)
	{
		String16 pathDll = _pathDll;
		if (pathDll.isEmpty()) {
			return;
		}
		if (!flagReload) {
			if (g_flagLoadedEntryPoints) {
				return;
			}
		}
		HMODULE hDll = ::LoadLibraryW((LPCWSTR)(pathDll.getData()));
		if (!hDll) {
			//LogError("GLES", "Failed to load GLES dll - %s", pathDll);
			return;
		}
		FARPROC proc;
		PRIV_SLIB_RENDER_GLES_ENTRIES
		g_flagLoadedEntryPoints = sl_true;
	}
	
	void GLES::loadEntries(sl_bool flagReload)
	{
		SLIB_STATIC_STRING16(s, "libGLESv2.dll");
		loadEntries(s, flagReload);
	}
	
	sl_bool GLES::isAvailable()
	{
		return g_flagLoadedEntryPoints;
	}
}

#	else

namespace slib
{
	void GLES::loadEntries(const String& pathDll, sl_bool flagReload)
	{
	}
	
	void GLES::loadEntries(sl_bool flagReload)
	{
	}
	
	sl_bool GLES::isAvailable()
	{
		return sl_true;
	}
}

#	endif

#else

namespace slib
{
	Ref<GLRenderEngine> GLES::createEngine()
	{
		return sl_null;
	}
	
	void GLES::loadEntries(const String& pathDll, sl_bool flagReload)
	{
	}
	
	void GLES::loadEntries(sl_bool flagReload)
	{
	}
	
	sl_bool GLES::isAvailable()
	{
		return sl_false;
	}
}

#endif


