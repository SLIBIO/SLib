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
