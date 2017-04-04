/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_EGL
#define CHECKHEADER_SLIB_RENDER_OPENGL_EGL

#include "definition.h"

#include "engine.h"

namespace slib
{

	class SLIB_EXPORT EGL
	{
	public:
		static Ref<Renderer> createRenderer(void* windowHandle, const RendererParam& param);
		
	public:
		static sl_bool isAvailable();
		
		static void loadEntries(const String& pathDll, sl_bool flagReload = sl_true);
		
		static void loadEntries(sl_bool flagReload = sl_false);
		
	};

}

#endif
