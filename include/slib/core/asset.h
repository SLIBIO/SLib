/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ASSET
#define CHECKHEADER_SLIB_CORE_ASSET

#include "definition.h"

#include "string.h"
#include "memory.h"

namespace slib
{

	class SLIB_EXPORT Assets
	{
	public:
		static sl_bool isBasedOnFileSystem();

		static String getFilePath(const String& path);
	

		static Memory readAllBytes(const String& path);
	
	};

}

#endif
