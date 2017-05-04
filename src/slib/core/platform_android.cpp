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

#ifdef SLIB_PLATFORM_IS_ANDROID

#include "slib/core/platform_android.h"

#include "slib/core/io.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_Android, "slib/platform/android/Android")
		SLIB_JNI_STATIC_METHOD(finishActivity, "finishActivity", "(Landroid/app/Activity;)V")
		SLIB_JNI_STATIC_METHOD(openAsset, "openAsset", "(Landroid/app/Activity;Ljava/lang/String;)Ljava/io/InputStream;")
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(_Util, "slib/platform/android/ui/Util")
		SLIB_JNI_STATIC_METHOD(showKeyboard, "showKeyboard", "(Landroid/app/Activity;)V")
		SLIB_JNI_STATIC_METHOD(dismissKeyboard, "dismissKeyboard", "(Landroid/app/Activity;)V")
	SLIB_JNI_END_CLASS

	void Android::initialize(JavaVM* jvm)
	{
		Jni::initialize(jvm);
	}

	SLIB_STATIC_ZERO_INITIALIZED(AtomicJniGlobal<jobject>, _g_android_current_activity);

	jobject Android::getCurrentActivity()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_android_current_activity)) {
			return 0;
		}
		return _g_android_current_activity.get();
	}

	void Android::setCurrentActivity(jobject activity)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_android_current_activity)) {
			return;
		}
		_g_android_current_activity = activity;
	}

	void Android::finishActivity()
	{
		Android::finishActivity(Android::getCurrentActivity());
	}

	void Android::finishActivity(jobject jactivity)
	{
		if (jactivity) {
			_Android::finishActivity.call(sl_null, jactivity);
		}
	}

	jobject Android::openAssetFile(const String& path)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jpath = Jni::getJniString(path);
			return _Android::openAsset.callObject(sl_null, jactivity, jpath.value);
		} else {
			return sl_null;
		}
	}

	Memory Android::readAllBytesFromAsset(const String& path)
	{
		JniLocal<jobject> is = Android::openAssetFile(path);
		if (is.isNotNull()) {
			JniLocal<jbyteArray> arr = Jni::newByteArray(512);
			jbyte buf[512];
			if (arr.isNotNull()) {
				MemoryWriter writer;
				while (1) {
					sl_int32 n = Jni::readFromInputStream(is, arr);
					if (n > 0) {
						Jni::getByteArrayRegion(arr, 0, n, buf);
						writer.write(buf, n);
					} else {
						break;
					}
				}
				Jni::closeInputStream(is);
				return writer.getData();
			}
		}
		return sl_null;
	}

	void Android::showKeyboard()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			_Util::showKeyboard.call(sl_null, jactivity);
		}
	}

	void Android::dismissKeyboard()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			_Util::dismissKeyboard.call(sl_null, jactivity);
		}
	}

}

#endif
