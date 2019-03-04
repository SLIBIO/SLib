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

#include "slib/graphics/drawable.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"

#include "slib/core/file.h"
#include "slib/core/asset.h"

namespace slib
{
	
	Ref<Bitmap> Drawable::toBitmap()
	{
		if (isBitmap()) {
			return (Bitmap*)this;
		}
		sl_int32 width = (sl_int32)(getDrawableWidth());
		sl_int32 height = (sl_int32)(getDrawableHeight());
		if (width > 0 && height > 0) {
			Ref<Bitmap> bitmap = Bitmap::create(width, height);
			if (bitmap.isNotNull()) {
				bitmap->resetPixels(Color::None);
				Ref<Canvas> canvas = bitmap->getCanvas();
				if (canvas.isNotNull()) {
					canvas->draw(Rectangle(0, 0, (sl_real)width, (sl_real)height), this);
				}
				return bitmap;
			}
		}
		return sl_null;
	}
	
	Ref<Image> Drawable::toImage()
	{
		if (isImage()) {
			return (Image*)this;
		}
		Ref<Bitmap> bitmap = toBitmap();
		if (bitmap.isNotNull()) {
			return Image::create(bitmap);
		}
		return sl_null;
	}

	Ref<Drawable> PlatformDrawable::create(const Ref<Image>& image)
	{
		if (image.isNotNull()) {
			ImageDesc desc;
			image->getDesc(desc);
			return PlatformDrawable::create(desc);
		}
		return sl_null;
	}
	
	Ref<Drawable> PlatformDrawable::loadFromMemory(const Memory& mem)
	{
		if (mem.isNotNull()) {
			return PlatformDrawable::loadFromMemory(mem.getData(), mem.getSize());
		}
		return sl_null;
	}
	
	
	Ref<Drawable> PlatformDrawable::loadFromFile(const String& filePath)
	{
		Memory mem = File::readAllBytes(filePath);
		if (mem.isNotNull()) {
			return PlatformDrawable::loadFromMemory(mem);
		}
		return sl_null;
	}
	
	Ref<Drawable> PlatformDrawable::loadFromAsset(const String& path)
	{
		Memory mem = Assets::readAllBytes(path);
		if (mem.isNotNull()) {
			return PlatformDrawable::loadFromMemory(mem);
		}
#if defined(SLIB_PLATFORM_IS_APPLE)
		CGImageRef image = GraphicsPlatform::loadCGImageFromApp(path);
		if (image) {
			Ref<Drawable> ret = GraphicsPlatform::createImageDrawable(image);
			CGImageRelease(image);
			return ret;
		}
#endif
		return sl_null;
	}

}
