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

#include "slib/graphics/image.h"

#ifdef _MSC_VER
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)
#endif

#define STB_IMAGE_IMPLEMENTATION

#ifdef SLIB_PLATFORM_IS_APPLE
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wcomma"
#	include "stb/stb_image.h"
#	pragma clang diagnostic pop
#else
#	include "stb/stb_image.h"
#endif

namespace slib
{

	Ref<Image> Image::loadSTB(const void* content, sl_size size)
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

