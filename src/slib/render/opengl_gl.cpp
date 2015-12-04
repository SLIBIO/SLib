#include "../../../inc/slib/render/opengl.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GL)

#include "opengl_gl.h"

#include "../../../inc/slib/render/program.h"
#include "../../../inc/slib/core/queue.h"

#define _OPENGL_IMPL
#define GL_BASE GL
#define GL_ENGINE _GL_Engine
#define GL_ENTRY(x) _GL_ENTRY(x)
#include "opengl_impl.h"

#if defined (SLIB_PLATFORM_IS_WIN32)

SLIB_RENDER_NAMESPACE_BEGIN

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
		hDll = ::LoadLibraryW((LPCWSTR)(pathDll.getBuf()));
		if (!hDll) {
			//SLIB_LOG_ERROR("GLES", "Failed to load OpenGL dll - " + pathDll);
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
SLIB_RENDER_NAMESPACE_END

#else
SLIB_RENDER_NAMESPACE_BEGIN
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
SLIB_RENDER_NAMESPACE_END
#endif

#else

SLIB_RENDER_NAMESPACE_BEGIN
Ref<RenderEngine> GL::createEngine()
{
	return Ref<RenderEngine>::null();
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
SLIB_RENDER_NAMESPACE_END

#endif


