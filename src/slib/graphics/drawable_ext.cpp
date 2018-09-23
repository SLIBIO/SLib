/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/graphics/drawable.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"

#include "slib/core/file.h"
#include "slib/core/asset.h"

namespace slib
{
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
