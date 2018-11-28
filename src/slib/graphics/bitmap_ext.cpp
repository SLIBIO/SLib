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

#include "slib/graphics/bitmap.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"

#include "slib/core/file.h"
#include "slib/core/asset.h"

namespace slib
{
	
	Ref<Bitmap> Bitmap::create(const ImageDesc& desc)
	{
		Ref<Bitmap> ret = Bitmap::create(desc.width, desc.height);
		if (ret.isNotNull()) {
			ret->writePixels(0, 0, desc.width, desc.height, desc.colors, desc.stride);
			return ret;
		}
		return sl_null;
	}
	
	Ref<Bitmap> Bitmap::create(const Ref<Image>& image)
	{
		if (image.isNotNull()) {
			ImageDesc desc;
			image->getDesc(desc);
			return Bitmap::create(desc);
		}
		return sl_null;
	}
	
	Ref<Bitmap> Bitmap::loadFromMemory(const Memory& mem)
	{
		if (mem.isNotNull()) {
			return Bitmap::loadFromMemory(mem.getData(), mem.getSize());
		}
		return sl_null;
	}
	
	Ref<Bitmap> Bitmap::loadFromFile(const String& filePath)
	{
		Memory mem = File::readAllBytes(filePath);
		if (mem.isNotNull()) {
			return Bitmap::loadFromMemory(mem);
		}
		return sl_null;
	}
	
	Ref<Bitmap> Bitmap::loadFromAsset(const String& path)
	{
		Memory mem = Assets::readAllBytes(path);
		if (mem.isNotNull()) {
			return Bitmap::loadFromMemory(mem);
		}
#if defined(SLIB_PLATFORM_IS_APPLE)
		CGImageRef image = GraphicsPlatform::loadCGImageFromApp(path);
		if (image) {
			Ref<Bitmap> ret = GraphicsPlatform::createImageBitmap(image);
			CGImageRelease(image);
			return ret;
		}
#endif
		return sl_null;
	}

}
