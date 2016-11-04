#ifndef CHECKHEADER_SLIB_RENDER_OPENGL
#define CHECKHEADER_SLIB_RENDER_OPENGL

#include "definition.h"

#include "opengl_engine.h"
#include "opengl_wgl.h"
#include "opengl_egl.h"

#define _SLIB_RENDER_ENTRIES_CLASS GLES
#define _SLIB_RENDER_ENTRIES_CLASS_GLES
#include "opengl_entries.h"
#undef _SLIB_RENDER_ENTRIES_CLASS
#undef _SLIB_RENDER_ENTRIES_CLASS_GLES

#define _SLIB_RENDER_ENTRIES_CLASS GL
#define _SLIB_RENDER_ENTRIES_CLASS_GL
#include "opengl_entries.h"
#undef _SLIB_RENDER_ENTRIES_CLASS
#undef _SLIB_RENDER_ENTRIES_CLASS_GL

#endif
