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
		if (!content || !size) {
			return sl_null;
		}
		Ref<Image> ret;
		int width = 0;
		int height = 0;
		int nComponents = 0;
		unsigned char * colors = stbi_load_from_memory((stbi_uc*)content, (int)size, &width, &height, &nComponents, 4);
		if (colors) {
			if (width > 0 && height > 0) {
				ret = Image::create(width, height);
				if (ret.isNotNull()) {
					Base::copyMemory(ret->getColors(), colors, width * height * 4);
				}
			}
			stbi_image_free(colors);
		}
		return ret;
	}

	Ref<AnimationDrawable> Image::loadSTB_GIF(const void* content, sl_size size)
	{
		if (!content || !size) {
			return sl_null;
		}
		Ref<AnimationDrawable> ret;
		int* delays = sl_null;
		int width = 0;
		int height = 0;
		int nComponents = 0;
		int nLayers = 0;
		unsigned char* colors = stbi_load_gif_from_memory((stbi_uc*)content, (int)size, &delays, &width, &height, &nLayers, &nComponents, 4);
		if (colors) {
			if (width > 0 && height > 0 && nLayers > 0) {
				ret = new AnimationDrawable;
				if (ret.isNotNull()) {
					int duration = 0;
					int size = width * height * 4;
					for (int i = 0; i < nLayers; i++) {
						Ref<Image> image = Image::create(width, height);
						if (image.isNotNull()) {
							Base::copyMemory(image->getColors(), colors + size * i, size);
						}
						ret->addDrawable(image);
						if (delays[i]) {
							duration += delays[i];
						} else {
							duration += 100;
						}
					}
					ret->setAnimationDuration((float)duration / 1000.0f);
				}
			}
			stbi_image_free(colors);
		}
		if (delays) {
			stbi_image_free(delays);
		}
		return ret;
	}
	
}
