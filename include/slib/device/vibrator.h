/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_DEVICE_VIBRATOR
#define CHECKHEADER_SLIB_DEVICE_VIBRATOR

#include "definition.h"

#include "../core/object.h"

namespace slib
{
	
	class SLIB_EXPORT Vibrator
	{
	public:
		static sl_bool vibrate(sl_int32 millisec = 500);

	};

}

#endif
