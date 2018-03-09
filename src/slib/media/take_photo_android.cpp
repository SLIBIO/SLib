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

#include "slib/media/camera.h"

#include "slib/graphics/image.h"
#include "slib/core/platform_android.h"
#include "slib/core/safe_static.h"

namespace slib
{
	class Android_TakePhoto;
	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Android_TakePhoto>, _g_take_photo)

	void Android_TakePhoto_onSuccess(JNIEnv* env, jobject _this, jstring path);
	void Android_TakePhoto_onFailure(JNIEnv* env, jobject _this);
	void Android_TakePhoto_onCanceled(JNIEnv* env, jobject _this);

	SLIB_JNI_BEGIN_CLASS(JAndroidTakePhoto, "slib/platform/android/camera/TakePhoto")
		SLIB_JNI_STATIC_METHOD(takePhoto, "takePhoto", "(Landroid/app/Activity;Ljava/lang/String;)V");
		SLIB_JNI_NATIVE(onSuccess, "onSuccess", "(Ljava/lang/String;)V", Android_TakePhoto_onSuccess);
		SLIB_JNI_NATIVE(onFailure, "onFailure", "()V", Android_TakePhoto_onFailure);
		SLIB_JNI_NATIVE(onCanceled, "onCanceled", "()V", Android_TakePhoto_onCanceled);
	SLIB_JNI_END_CLASS

	class Android_TakePhoto : public Referable
	{
	public:
		Android_TakePhoto()
		{

		}

		~Android_TakePhoto()
		{

		}

	public:
		static Ref<Android_TakePhoto> create(const TakePhotoParam& param)
		{
			Ref<Android_TakePhoto> ret = new Android_TakePhoto();
			if (ret.isNotNull()) {
				ret->param = param;
			}
			return ret;
		}

	public:
		TakePhotoParam param;
	};

	void Camera::takePhoto(const TakePhotoParam &param)
	{
		Ref<Android_TakePhoto> ret =  Android_TakePhoto::create(param);
		if (ret.isNull()) {
			if (param.onComplete.isNotNull()) {
				TakePhoto tp;
				tp.result = TakePhotoResult::Failed;
				param.onComplete(&tp);
			}
			return;
		}
		_g_take_photo = ret;
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jpath = Jni::getJniString(param.outputFilePath);
			JAndroidTakePhoto::takePhoto.call(sl_null, jactivity, jpath.get());
		}
	}

    void Android_TakePhoto_onSuccess(JNIEnv* env, jobject _this, jstring path)
    {
        Ref<Android_TakePhoto> takePhoto = _g_take_photo;
        if (takePhoto.isNotNull()) {
            String strPath = Jni::getString(path);
			TakePhoto tp;
			tp.result = TakePhotoResult::Successed;
			tp.outputFilePath = strPath;
			takePhoto->param.onComplete(&tp);
        }
		_g_take_photo.setNull();
    }

    void Android_TakePhoto_onFailure(JNIEnv* env, jobject _this)
    {
        Ref<Android_TakePhoto> takePhoto = _g_take_photo;
        if (takePhoto.isNotNull()) {
			TakePhoto tp;
			tp.result = TakePhotoResult::Failed;
			takePhoto->param.onComplete(&tp);
        }
		_g_take_photo.setNull();
    }

    void Android_TakePhoto_onCanceled(JNIEnv* env, jobject _this)
    {
        Ref<Android_TakePhoto> takePhoto = _g_take_photo;
        if (takePhoto.isNotNull()) {
			TakePhoto tp;
			tp.result = TakePhotoResult::Canceled;
			takePhoto->param.onComplete(&tp);
        }
		_g_take_photo.setNull();
    }
}

#endif
