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

#include "slib/core/hash_map.h"
#include "slib/core/platform_android.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidCameraInfo, "slib/platform/android/camera/SCameraInfo")
		SLIB_JNI_STRING_FIELD(id);
		SLIB_JNI_STRING_FIELD(name);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JAndroidCamera, "slib/platform/android/camera/SCamera")

		SLIB_JNI_STATIC_METHOD(getCamerasList, "getCamerasList", "()[Lslib/platform/android/camera/SCameraInfo;");
		SLIB_JNI_STATIC_METHOD(create, "create", "(Ljava/lang/String;J)Lslib/platform/android/camera/SCamera;");

		SLIB_JNI_METHOD(setPreferedFrameSettings, "setPreferedFrameSettings", "(II)V");
		SLIB_JNI_METHOD(release, "release", "()V");
		SLIB_JNI_METHOD(start, "start", "()V");
		SLIB_JNI_METHOD(stop, "stop", "()V");
		SLIB_JNI_METHOD(isRunning, "isRunning", "()Z");

	SLIB_JNI_END_CLASS

	class _priv_Android_Camera;
	typedef CHashMap<jlong, WeakRef<_priv_Android_Camera> > _priv_AndroidCameraMap;
	SLIB_SAFE_STATIC_GETTER(_priv_AndroidCameraMap, _priv_AndroidCameras_get)

	class _priv_Android_Camera : public Camera
	{
	public:
		JniGlobal<jobject> m_camera;

	public:
		_priv_Android_Camera()
		{
		}

		~_priv_Android_Camera()
		{
			release();
		}

	public:
		static Ref<_priv_Android_Camera> _create(const CameraParam& param)
		{
			_priv_AndroidCameraMap* cameraMap = _priv_AndroidCameras_get();
			if (!cameraMap) {
				return sl_null;
			}
			Ref<_priv_Android_Camera> ret = new _priv_Android_Camera();
			if (ret.isNotNull()) {
				jlong instance = (jlong)(ret.get());
				cameraMap->put(instance, ret);
				JniLocal<jstring> jid = Jni::getJniString(param.deviceId);
				JniLocal<jobject> jcamera = JAndroidCamera::create.callObject(sl_null, jid.get(), instance);
				if (jcamera.isNotNull()) {
					ret->m_camera = jcamera;
					ret->_init(param);
					JAndroidCamera::setPreferedFrameSettings.call(jcamera,
							param.preferedFrameWidth,
							param.preferedFrameHeight);
					if (param.flagAutoStart) {
						ret->start();
					}
					return ret;
				}
			}
			return sl_null;
		}

		static Ref<_priv_Android_Camera> get(jlong instance)
		{
			_priv_AndroidCameraMap* cameraMap = _priv_AndroidCameras_get();
			if (!cameraMap) {
				return sl_null;
			}
			WeakRef<_priv_Android_Camera> camera;
			cameraMap->get(instance, &camera);
			return camera;
		}

		void release()
		{
			ObjectLocker lock(this);

			jobject jcamera = m_camera.get();
			if (!jcamera) {
				return;
			}

			JAndroidCamera::release.call(jcamera);
			m_camera.setNull();

			_priv_AndroidCameraMap* cameraMap = _priv_AndroidCameras_get();
			if (cameraMap) {
				cameraMap->remove((jlong)this);
			}
		}

		sl_bool isOpened()
		{
			return m_camera.isNotNull();
		}

		void start()
		{
			jobject jcamera = m_camera.get();
			if (jcamera) {
				JAndroidCamera::start.call(jcamera);
			}
		}

		void stop()
		{
			jobject jcamera = m_camera.get();
			if (jcamera) {
				JAndroidCamera::stop.call(jcamera);
			}
		}

		sl_bool isRunning()
		{
			jobject jcamera = m_camera.get();
			if (jcamera) {
				return JAndroidCamera::isRunning.callBoolean(jcamera) != 0;
			}
			return sl_false;
		}

		Memory m_memFrame;
		void _onFrame(jbyteArray jdata, jint width, jint height) {
			if (width & 1) {
				return;
			}
			if (height & 1) {
				return;
			}
			sl_uint32 size = Jni::getArrayLength(jdata);
			Memory mem = m_memFrame;
			if (mem.isNull()) {
				mem = Memory::create(size);
				if (mem.isNull()) {
					return;
				}
				m_memFrame = mem;
			}
			Jni::getByteArrayRegion(jdata, 0, size, (jbyte*)(mem.getData()));
			VideoCaptureFrame frame;
			frame.image.width = (sl_uint32)(width);
			frame.image.height = (sl_uint32)(height);
			frame.image.format = BitmapFormat::YUV_NV21;
			frame.image.data = mem.getData();
			frame.image.pitch = 0;
			frame.image.ref = mem.ref;
			frame.rotation = RotationMode::Rotate90;
			_onCaptureVideoFrame(&frame);
		}

	};

	SLIB_JNI_BEGIN_CLASS_SECTION(JAndroidCamera)
		SLIB_JNI_NATIVE_IMPL(nativeOnFrame, "nativeOnFrame", "(J[BII)V", void, jlong instance, jbyteArray jdata, jint jwidth, jint jheight)
		{
			Ref<_priv_Android_Camera> camera = _priv_Android_Camera::get(instance);
			if (camera.isNotNull()) {
				camera->_onFrame(jdata, jwidth, jheight);
			}
		}
	SLIB_JNI_END_CLASS_SECTION

	Ref<Camera> Camera::create(const CameraParam& param)
	{
		return _priv_Android_Camera::_create(param);
	}

	List<CameraInfo> Camera::getCamerasList()
	{
		List<CameraInfo> ret;
		JniLocal<jobjectArray> arr = (jobjectArray)(JAndroidCamera::getCamerasList.callObject(sl_null));
		sl_uint32 len = Jni::getArrayLength(arr);
		for (sl_uint32 i = 0; i < len; i++) {
			JniLocal<jobject> jinfo = Jni::getObjectArrayElement(arr, i);
			if (jinfo.isNotNull()) {
				CameraInfo info;
				info.id = JAndroidCameraInfo::id.get(jinfo);
				info.name = JAndroidCameraInfo::name.get(jinfo);
				ret.add_NoLock(info);
			}
		}
		return ret;
	}

}

#endif
