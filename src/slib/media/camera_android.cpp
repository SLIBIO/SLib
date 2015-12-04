#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/media/camera.h"

#include "../../../inc/slib/core/platform_android.h"

SLIB_MEDIA_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidCameraInfo, "slib/platform/android/camera/SCameraInfo")
	SLIB_JNI_STRING_FIELD(id);
	SLIB_JNI_STRING_FIELD(name);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidCamera, "slib/platform/android/camera/SCamera")

	SLIB_JNI_STATIC_METHOD(getCamerasList, "getCamerasList", "()[Lslib/platform/android/camera/SCameraInfo;");
	SLIB_JNI_STATIC_METHOD(create, "create", "(Ljava/lang/String;J)Lslib/platform/android/camera/SCamera;");

	SLIB_JNI_METHOD(setPreferedFrameSize, "setPreferedFrameSize", "(II)V");
	SLIB_JNI_METHOD(release, "release", "()V");
	SLIB_JNI_METHOD(start, "start", "()V");
	SLIB_JNI_METHOD(stop, "stop", "()V");
	SLIB_JNI_METHOD(isRunning, "isRunning", "()Z");

SLIB_JNI_END_CLASS

class _Android_Camera;
typedef Map<jlong, WeakRef<_Android_Camera> > _AndroidCameraMap;
SLIB_SAFE_STATIC_GETTER(_AndroidCameraMap, _AndroidCameras_get);

class _Android_Camera : public Camera
{
public:
	JniGlobal<jobject> m_camera;

	_Android_Camera()
	{
	}

	~_Android_Camera()
	{
		release();
		_AndroidCameras_get().remove((jlong)this);
	}

	static Ref<_Android_Camera> _create(const CameraParam& param)
	{
		Ref<_Android_Camera> ret = new _Android_Camera();
		if (ret.isNotNull()) {
			jlong instance = (jlong)(ret.getObject());
			_AndroidCameras_get().put(instance, ret);
			JniLocal<jstring> jid = Jni::getJniString(param.deviceId);
			JniLocal<jobject> jcamera = _JAndroidCamera::create.callObject(sl_null, jid.get(), instance);
			if (jcamera.isNotNull()) {
				ret->m_camera = jcamera;
				ret->setListener(param.listener);
				_JAndroidCamera::setPreferedFrameSize.call(jcamera, param.preferedFrameWidth, param.preferedFrameHeight);
				if (param.flagAutoStart) {
					ret->start();
				}
				return ret;
			}
		}
		return Ref<_Android_Camera>::null();
	}

	static Ref<_Android_Camera> get(jlong instance)
	{
		WeakRef<_Android_Camera> camera;
		_AndroidCameras_get().get(instance, &camera);
		return camera.lock();
	}

	void release()
	{
		MutexLocker lock(getLocker());
		jobject jcamera = m_camera.get();
		if (!jcamera) {
			return;
		}
		_JAndroidCamera::release.call(jcamera);
		m_camera.setNull();
	}

	sl_bool isOpened()
	{
		return m_camera.isNotNull();
	}

	void start()
	{
		jobject jcamera = m_camera.get();
		if (jcamera) {
			_JAndroidCamera::start.call(jcamera);
		}
	}

	void stop()
	{
		jobject jcamera = m_camera.get();
		if (jcamera) {
			_JAndroidCamera::stop.call(jcamera);
		}
	}

	sl_bool isRunning()
	{
		jobject jcamera = m_camera.get();
		if (jcamera) {
			return _JAndroidCamera::isRunning.callBoolean(jcamera) != 0;
		}
		return sl_false;
	}

	void _onFrame(jbyteArray jdata, jint width, jint height) {
		if (width & 1) {
			return;
		}
		if (height & 1) {
			return;
		}
		sl_uint32 size = Jni::getArrayLength(jdata);
		Memory mem = _getMemProcess(size);
		if (mem.isNull()) {
			return;
		}
		Jni::getByteArrayRegion(jdata, 0, size, (jbyte*)(mem.getBuf()));
		VideoCaptureFrame frame;
		frame.width = (sl_uint32)(width);
		frame.height = (sl_uint32)(height);
		frame.colorModel = Color::YUV_NV21;
		frame.data = mem.getBuf();
		frame.pitch = 0;
		frame.ref = mem.getReference();
		onCaptureVideoFrame(&frame);
	}
};

SLIB_JNI_BEGIN_CLASS_SECTION(_JAndroidCameraInfo)
	SLIB_JNI_NATIVE_IMPL(nativeOnFrame, "nativeOnFrame", "(J[BII)V", void, jlong instance, jbyteArray jdata, jint jwidth, jint jheight)
	{
		Ref<_Android_Camera> camera = _Android_Camera::get(instance);
		if (camera.isNotNull()) {
			camera->_onFrame(jdata, jwidth, jheight);
		}
	}
SLIB_JNI_END_CLASS_SECTION

Ref<Camera> Camera::create(const CameraParam& param)
{
	Ref<_Android_Camera> ret = _Android_Camera::_create(param);
	return Ref<Camera>::from(ret);
}

List<CameraInfo> Camera::getCamerasList()
{
	List<CameraInfo> ret;
	JniLocal<jobjectArray> arr = (jobjectArray)(_JAndroidCamera::getCamerasList.callObject(sl_null));
	sl_uint32 len = Jni::getArrayLength(arr);
	for (sl_uint32 i = 0; i < len; i++) {
		JniLocal<jobject> jinfo = Jni::getObjectArrayElement(arr, i);
		if (jinfo.isNotNull()) {
			CameraInfo info;
			info.id = _JAndroidCameraInfo::id.get(jinfo);
			info.name = _JAndroidCameraInfo::name.get(jinfo);
			ret.add(info);
		}
	}
	return ret;
}

SLIB_MEDIA_NAMESPACE_END

#endif
