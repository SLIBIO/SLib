/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_IMAGE_STB
#define CHECKHEADER_SLIB_GRAPHICS_IMAGE_STB

#include "../../../inc/slib/graphics/image.h"

#ifdef SLIB_GRAPHICS_IMAGE_USE_STB

namespace slib
{

	class Image_STB
	{
	public:
		static Ref<Image> loadImage(const void* content, sl_size size);
	};

}

#endif

#endif
