/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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
