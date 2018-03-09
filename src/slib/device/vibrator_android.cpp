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

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/device/vibrator.h"

#include "slib/core/platform_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidVibrator, "slib/platform/android/device/Vibrator")
		SLIB_JNI_STATIC_METHOD(vibrate, "vibrate", "(Landroid/app/Activity;I)Z");
	SLIB_JNI_END_CLASS

	sl_bool Vibrator::vibrate(sl_int32 millisec)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			sl_bool ret = JAndroidVibrator::vibrate.callBoolean(sl_null, jactivity, millisec);
			return ret;
		}
		return sl_false;
	}

}

#endif
