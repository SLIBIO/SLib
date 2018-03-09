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

#include "slib/core/preference.h"

#include "slib/core/json.h"
#include "slib/core/platform_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_priv_AndroidPreference, "slib/platform/android/Preference")
		SLIB_JNI_STATIC_METHOD(setValue, "setValue", "(Lslib/platform/android/SlibActivity;Ljava/lang/String;Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(getValue, "getValue", "(Lslib/platform/android/SlibActivity;Ljava/lang/String;)Ljava/lang/String;");
	SLIB_JNI_END_CLASS

	void Preference::setValue(const String& key, const Json& value)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jkey = Jni::getJniString(key);
			String _value = value.toJsonString();
			JniLocal<jstring> jvalue = Jni::getJniString(_value);
			_priv_AndroidPreference::setValue.call(sl_null, jactivity, jkey.get(), jvalue.get());
		}
	}

	Json Preference::getValue(const String& key)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jkey = Jni::getJniString(key);
			String value = _priv_AndroidPreference::getValue.callString(sl_null, jactivity, jkey.get());
			return Json::parseJson(value);
		}
		return sl_null;
	}

}

#endif
