/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_DEVICE_INFORMATION
#define CHECKHEADER_SLIB_DEVICE_INFORMATION

#include "definition.h"

#include "../core/string.h"
#include "../math/size.h"

namespace slib
{

	class SLIB_EXPORT DeviceInformation
	{
	public:
		static String getDeviceId();

		static String getDeviceName();

		static String getSystemVersion();

		static String getSystemName();
	
		static sl_uint32 getDevicePPI();

		static Size getScreenSize();

		static sl_uint32 getScreenWidth();

		static sl_uint32 getScreenHeight();

	};

}

#endif
