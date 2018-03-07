/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "image_stb.h"

#ifdef _MSC_VER
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)
#endif

#define STB_IMAGE_IMPLEMENTATION

#ifdef SLIB_PLATFORM_IS_APPLE
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wcomma"
#	include "thirdparty/stb/stbi_ext/stb_image.h"
#	pragma clang diagnostic pop
#else
#	include "thirdparty/stb/stbi_ext/stb_image.h"
#endif

#include "thirdparty/stb/stbi_ext/etc1_utils.c"

namespace slib
{

	Ref<Image> Image_STB::loadImage(const void* content, sl_size size)
	{
		Ref<Image> ret;
		int width;
		int height;
		int channels;
		unsigned char *map = stbi_load_from_memory((stbi_uc*)content, (int)size, &width, &height, &channels, 4);
		if (map) {
			ret = Image::create(width, height);
			if (ret.isNotNull()) {
				Base::copyMemory(ret->getColors(), map, width * height * 4);
			}
			stbi_image_free(map);
		}
		return ret;
	}

}

