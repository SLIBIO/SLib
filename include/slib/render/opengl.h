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

#ifndef CHECKHEADER_SLIB_RENDER_OPENGL
#define CHECKHEADER_SLIB_RENDER_OPENGL

#include "definition.h"

#include "opengl_engine.h"
#include "opengl_wgl.h"
#include "opengl_egl.h"
#include "opengl_glx.h"

#define PRIV_SLIB_RENDER_ENTRIES_CLASS GLES
#define PRIV_SLIB_RENDER_ENTRIES_CLASS_GLES
#include "opengl_entries.h"
#undef PRIV_SLIB_RENDER_ENTRIES_CLASS
#undef PRIV_SLIB_RENDER_ENTRIES_CLASS_GLES

#define PRIV_SLIB_RENDER_ENTRIES_CLASS GL
#define PRIV_SLIB_RENDER_ENTRIES_CLASS_GL
#include "opengl_entries.h"
#undef PRIV_SLIB_RENDER_ENTRIES_CLASS
#undef PRIV_SLIB_RENDER_ENTRIES_CLASS_GL

#endif
