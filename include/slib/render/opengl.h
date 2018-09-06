/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
