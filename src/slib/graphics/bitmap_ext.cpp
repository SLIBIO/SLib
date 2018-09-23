/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
