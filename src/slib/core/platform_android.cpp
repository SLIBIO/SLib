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

#ifdef SLIB_PLATFORM_IS_ANDROID

#include "slib/core/platform_android.h"

#include "slib/core/io.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroid, "slib/platform/android/Android")
		SLIB_JNI_STATIC_METHOD(finishActivity, "finishActivity", "(Landroid/app/Activity;)V")
		SLIB_JNI_STATIC_METHOD(openAsset, "openAsset", "(Landroid/app/Activity;Ljava/lang/String;)Ljava/io/InputStream;")
		SLIB_JNI_STATIC_METHOD(showKeyboard, "showKeyboard", "(Landroid/app/Activity;)V")
		SLIB_JNI_STATIC_METHOD(dismissKeyboard, "dismissKeyboard", "(Landroid/app/Activity;)V")
		SLIB_JNI_STATIC_METHOD(sendFile, "sendFile", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
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
			JAndroid::finishActivity.call(sl_null, jactivity);
		}
	}

	jobject Android::openAssetFile(const String& path)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jpath = Jni::getJniString(path);
			return JAndroid::openAsset.callObject(sl_null, jactivity, jpath.value);
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
			JAndroid::showKeyboard.call(sl_null, jactivity);
		}
	}

	void Android::dismissKeyboard()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JAndroid::dismissKeyboard.call(sl_null, jactivity);
		}
	}

	void Android::sendFile(const String& filePath, const String& mimeType, const String& chooserTitle)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jfilePath = Jni::getJniString(filePath);
			JniLocal<jstring> jmimeType = Jni::getJniString(mimeType);
			JniLocal<jstring> jchooserTitle = Jni::getJniString(chooserTitle);
			return JAndroid::sendFile.call(sl_null, jactivity, jfilePath.value, jmimeType.value, jchooserTitle.value);
		}
	}

}

#endif
