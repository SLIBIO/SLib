/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/core/platform_tizen.h"

#include "slib/core/variant.h"

#include <app_common.h>
#include <stdlib.h>

namespace slib
{

	String Tizen::getAssetFilePath(const String& path)
	{
		char* root = ::app_get_resource_path();
		String s = String::format("%s/%s", root, path);
		::free(root);
		return s;
	}

	SLIB_STATIC_ZERO_INITIALIZED(AtomicList<int>, _g_tizen_available_screen_orientations);

	List<int> Tizen::getAvailableScreenOrientations()
	{
		return _g_tizen_available_screen_orientations;
	}

	void Tizen::setAvailableScreenOrientations(const List<int>& orientations)
	{
		_g_tizen_available_screen_orientations = orientations;
	}

}

#endif
