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
		SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;Ljava/lang/String;J)Lslib/platform/android/camera/SCamera;");

		SLIB_JNI_METHOD(setSettings, "setSettings", "(II)V");
		SLIB_JNI_METHOD(release, "release", "()V");
		SLIB_JNI_METHOD(start, "start", "()V");
		SLIB_JNI_METHOD(stop, "stop", "()V");
		SLIB_JNI_METHOD(isRunning, "isRunning", "()Z");
		SLIB_JNI_METHOD(takePicture, "takePicture", "(I)V");

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
			jobject jactivity = Android::getCurrentActivity();
			if (!jactivity) {
				return sl_null;
			}
			_priv_AndroidCameraMap* cameraMap = _priv_AndroidCameras_get();
			if (!cameraMap) {
				return sl_null;
			}
			Ref<_priv_Android_Camera> ret = new _priv_Android_Camera();
			if (ret.isNotNull()) {
				jlong instance = (jlong)(ret.get());
				cameraMap->put(instance, ret);
				JniLocal<jstring> jid = Jni::getJniString(param.deviceId);
				JniLocal<jobject> jcamera = JAndroidCamera::create.callObject(sl_null, jactivity, jid.get(), instance);
				if (jcamera.isNotNull()) {
					ret->m_camera = jcamera;
					ret->_init(param);
					JAndroidCamera::setSettings.call(jcamera,
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

		void release() override
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

		sl_bool isOpened() override
		{
			return m_camera.isNotNull();
		}

		void start() override
		{
			jobject jcamera = m_camera.get();
			if (jcamera) {
				JAndroidCamera::start.call(jcamera);
			}
		}

		void stop() override
		{
			jobject jcamera = m_camera.get();
			if (jcamera) {
				JAndroidCamera::stop.call(jcamera);
			}
		}

		sl_bool isRunning() override
		{
			jobject jcamera = m_camera.get();
			if (jcamera) {
				return JAndroidCamera::isRunning.callBoolean(jcamera) != 0;
			}
			return sl_false;
		}

		static RotationMode _getRotation(jint rotation)
		{
			switch (rotation) {
				case 90:
					return RotationMode::Rotate90;
				case 180:
					return RotationMode::Rotate180;
				case 270:
					return RotationMode::Rotate270;
				default:
					return RotationMode::Rotate0;
			}
		}

		static FlipMode _getFlip(jint flip)
		{
			switch (flip) {
				case 1:
					return FlipMode::Horizontal;
				case 2:
					return FlipMode::Vertical;
				case 3:
					return FlipMode::Both;
				default:
					return FlipMode::None;
			}
		}

		Memory m_memFrame;
		void _onFrame(jbyteArray jdata, jint width, jint height, jint orientation, jint flip) {
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
			frame.rotation = _getRotation(orientation);
			frame.flip = _getFlip(flip);
			onCaptureVideoFrame(frame);
		}

		void takePicture(const CameraTakePictureParam& param) override
		{
			ObjectLocker lock(this);
			jobject jcamera = m_camera.get();
			if (!jcamera || !isRunning()) {
				CameraTakePictureResult result;
				param.onComplete(result);
				return;
			}			
			m_queueTakePictureRequests.push(param);
			JAndroidCamera::takePicture.call(jcamera, (int)(param.flashMode));
		}

		void _onPicture(jbyteArray jdata, jint orientation, jint flip) {
			CameraTakePictureParam param;
			if (m_queueTakePictureRequests.pop(&param)) {
				CameraTakePictureResult result;
				if (jdata) {
					sl_uint32 size = Jni::getArrayLength(jdata);
					Memory mem = Memory::create(size);
					if (mem.isNotNull()) {
						Jni::getByteArrayRegion(jdata, 0, size, (jbyte*)(mem.getData()));
						result.flagSuccess = sl_true;
						result.setJpeg(mem);
						result.rotation = _getRotation(orientation);
						result.flip = _getFlip(flip);
					}
				}
				param.onComplete(result);
			}
		}

		void onCaptureVideoFrame(VideoCaptureFrame& frame) override
		{
			VideoCapture::onCaptureVideoFrame(frame);
		}

	};


	SLIB_JNI_BEGIN_CLASS_SECTION(JAndroidCamera)
		SLIB_JNI_NATIVE_IMPL(nativeOnFrame, "nativeOnFrame", "(J[BIIII)V", void, jlong instance, jbyteArray data, jint width, jint height, jint orientation, jint flip)
		{
			Ref<_priv_Android_Camera> camera = _priv_Android_Camera::get(instance);
			if (camera.isNotNull()) {
				camera->_onFrame(data, width, height, orientation, flip);
			}
		}

		SLIB_JNI_NATIVE_IMPL(nativeOnPicture, "nativeOnPicture", "(J[BII)V", void, jlong instance, jbyteArray data, jint orientation, jint flip)
		{
			Ref<_priv_Android_Camera> camera = _priv_Android_Camera::get(instance);
			if (camera.isNotNull()) {
				camera->_onPicture(data, orientation, flip);
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
