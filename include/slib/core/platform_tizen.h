/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_TIZEN
#define CHECKHEADER_SLIB_CORE_PLATFORM_TIZEN

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_TIZEN

#include "string.h"

namespace slib
{

	// specific functions for Tizen
	class SLIB_EXPORT Tizen
	{
	public:
		static String getAssetFilePath(const String& path);

		static List<int> getAvailableScreenOrientations();

		static void setAvailableScreenOrientations(const List<int>& orientations);

	};

}

#endif

#endif
