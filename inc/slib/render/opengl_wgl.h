/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_WGL
#define CHECKHEADER_SLIB_RENDER_OPENGL_WGL

#include "definition.h"

#include "engine.h"

namespace slib
{

	class SLIB_EXPORT WGL
	{
	public:
		static Ref<Renderer> createRenderer(void* windowHandle, const RendererParam& param);
		
	};

}

#endif
