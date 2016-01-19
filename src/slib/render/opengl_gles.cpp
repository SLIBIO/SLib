#include "../../../inc/slib/render/opengl.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GLES)

#include "opengl_gles.h"

#include "../../../inc/slib/render/program.h"
#include "../../../inc/slib/core/queue.h"

#define _OPENGL_ES_IMPL
#define GL_BASE GLES
#define GL_ENGINE _GLES_Engine
#define GL_ENTRY(x) _GLES_ENTRY(x)
#include "opengl_impl.h"

#if defined (SLIB_PLATFORM_IS_WIN32)

#include <Windows.h>

SLIB_RENDER_NAMESPACE_BEGIN

_GLES_ENTRY_POINTS _GLES_ENTRIES;

#undef _SLIB_RENDER_GLES_ENTRY
#define _SLIB_RENDER_GLES_ENTRY(TYPE, name, ...) \
	proc = ::GetProcAddress(hDll, #name); \
	*((FARPROC*)(&(_GLES_ENTRIES.name))) = proc;

static sl_bool _g_render_GLES_flagLoadedEntryPoints = sl_false;

void GLES::loadEntries(const String& _pathDll, sl_bool flagReload)
{
	String16 pathDll = _pathDll;
	if (pathDll.isEmpty()) {
		return;
	}
	if (!flagReload) {
		if (_g_render_GLES_flagLoadedEntryPoints) {
			return;
		}
	}
	HMODULE hDll = ::LoadLibraryW((LPCWSTR)(pathDll.getBuf()));
	if (!hDll) {
		//SLIB_LOG_ERROR("GLES", "Failed to load GLES dll - " + pathDll);
		return;
	}
	FARPROC proc;
	_SLIB_RENDER_GLES_ENTRIES
	_g_render_GLES_flagLoadedEntryPoints = sl_true;
}

void GLES::loadEntries(sl_bool flagReload)
{
	SLIB_STATIC_STRING16(s, "libGLESv2.dll");
	loadEntries(s, flagReload);
}

sl_bool GLES::isAvailable()
{
	return _g_render_GLES_flagLoadedEntryPoints;
}

SLIB_RENDER_NAMESPACE_END

#else

SLIB_RENDER_NAMESPACE_BEGIN

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

SLIB_RENDER_NAMESPACE_END

#endif

#else

SLIB_RENDER_NAMESPACE_BEGIN

Ref<RenderEngine> GLES::createEngine()
{
	return Ref<RenderEngine>::null();
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

SLIB_RENDER_NAMESPACE_END

#endif


