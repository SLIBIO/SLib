/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_RENDER_DEFINITION
#define CHECKHEADER_SLIB_RENDER_DEFINITION

#include "../core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)
#	define SLIB_RENDER_SUPPORT_OPENGL_GL
#	define SLIB_RENDER_SUPPORT_OPENGL_WGL
#	define SLIB_RENDER_SUPPORT_OPENGL_GLES
#	define SLIB_RENDER_SUPPORT_OPENGL_EGL
#elif defined(SLIB_PLATFORM_IS_MACOS)
#	define SLIB_RENDER_SUPPORT_OPENGL_GL
#elif defined(SLIB_PLATFORM_IS_IOS)
#	define SLIB_RENDER_SUPPORT_OPENGL_GLES
#elif defined(SLIB_PLATFORM_IS_ANDROID)
#	define SLIB_RENDER_SUPPORT_OPENGL_GLES
#elif defined(SLIB_PLATFORM_IS_TIZEN)
#	define SLIB_RENDER_SUPPORT_OPENGL_GLES
#endif

#endif
