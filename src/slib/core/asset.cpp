/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/asset.h"

#include "slib/core/file.h"
#include "slib/core/system.h"
#include "slib/core/app.h"

#include "slib/core/platform_apple.h"
#include "slib/core/platform_android.h"
#include "slib/core/platform_tizen.h"


#if defined(SLIB_PLATFORM_IS_ANDROID)

namespace slib
{
	sl_bool Assets::isBasedOnFileSystem()
	{
		return sl_false;
	}

	String Assets::getFilePath(const String& path)
	{
		return sl_null;
	}

	Memory Assets::readAllBytes(const String& path)
	{
		return Android::readAllBytesFromAsset(path);
	}

}

#else

namespace slib
{

	sl_bool Assets::isBasedOnFileSystem()
	{
		return sl_true;
	}

#	if defined(SLIB_PLATFORM_IS_APPLE)

	String Assets::getFilePath(const String& path)
	{
		return Apple::getAssetFilePath(path);
	}

#	elif defined(SLIB_PLATFORM_IS_TIZEN)

	String Assets::getFilePath(const String& path)
	{
		return Tizen::getAssetFilePath(path);
	}

#	else

	String Assets::getFilePath(const String& path)
	{
		String s = File::makeSafeFilePath(path);
		if (s.isNotEmpty()) {
			return Application::getApplicationDirectory() + "/" + s;
		}
		return sl_null;
	}

#	endif

	Memory Assets::readAllBytes(const String& path)
	{
		Memory ret;
		String s = Assets::getFilePath(path);
		if (s.isNotEmpty()) {
			ret = File::readAllBytes(s);
		}
		return ret;
	}
}

#endif
