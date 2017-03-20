/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_CONSTANTS
#define CHECKHEADER_SLIB_CORE_CONSTANTS

#include "definition.h"

namespace slib
{

	enum class MapPutMode
	{
		AddOrReplace = 0,
		ReplaceExisting = 1,
		AddNew = 2,
		AddAlways = 3,
		Default = AddOrReplace
	};

}

#endif
