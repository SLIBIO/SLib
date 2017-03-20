/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/device/vibrator.h"

#import <AudioToolbox/AudioServices.h>

namespace slib
{

	sl_bool Vibrator::vibrate(sl_int32 millisec)
	{
		AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
		return sl_true;
	}

}

#endif
