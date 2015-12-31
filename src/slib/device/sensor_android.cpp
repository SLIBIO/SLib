#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/device/sensor.h"

#include "../../../inc/slib/core/platform_android.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class _Android_Sensor;
typedef Map<jlong, WeakRef<_Android_Sensor> > _AndroidSensorMap;
SLIB_SAFE_STATIC_GETTER(_AndroidSensorMap, _AndroidSensors_get);

SLIB_JNI_BEGIN_CLASS(_JAndroidSensors, "slib/platform/android/device/Sensors")
	SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;)Lslib/platform/android/device/Sensors;");
	SLIB_JNI_METHOD(setInstance, "setInstance", "(J)V");
	SLIB_JNI_METHOD(start, "start", "(ZIZZ)Z");
	SLIB_JNI_METHOD(stop, "stop", "()V");
SLIB_JNI_END_CLASS

class _Android_Sensor : public Sensor
{
public:
	_Android_Sensor()
	{
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
	}

	JniGlobal<jobject> m_sensors;

	GeoLocation m_lastLocation;
	sl_bool m_flagValidLocation;

	sl_real m_lastCompassDeclination;
	sl_bool m_flagValidCompassDeclination;

	struct Accelerometer {
		sl_real xAccel;
		sl_real yAccel;
		sl_real zAccel;
	} m_lastAccelerometer;
	sl_bool m_flagValidAccerometer;

	~_Android_Sensor()
	{
		stop();
		_AndroidSensors_get().remove((jlong)this);
	}

	static Ref<_Android_Sensor> create()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jobject> obj = _JAndroidSensors::create.callObject(sl_null, jactivity);
			if (obj.isNotNull()) {
				JniGlobal<jobject> sensors = obj;
				if (sensors.isNotNull()) {
					Ref<_Android_Sensor> ret = new _Android_Sensor;
					if (ret.isNotNull()) {
						ret->m_sensors = sensors;
						jlong instance = (jlong)(ret.get());
						_JAndroidSensors::setInstance.call(obj, instance);
						_AndroidSensors_get().put(instance, ret);
						return ret;
					}
				}
			}
		}
		return Ref<_Android_Sensor>::null();
	}

	static Ref<_Android_Sensor> get(jlong instance)
	{
		WeakRef<_Android_Sensor> sensor;
		_AndroidSensors_get().get(instance, &sensor);
		return sensor.lock();
	}

	sl_bool start(const Param& param)
	{
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
		return _JAndroidSensors::start.callBoolean(m_sensors
				, param.flagUseLocation, param.providerType
				, param.flagUseCompass
				, param.flagUseAccelerometor
				) != 0;
	}

	void stop()
	{
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
		_JAndroidSensors::stop.call(m_sensors);
	}

	sl_bool getLastLocation(GeoLocation& location)
	{
		if (m_flagValidLocation) {
			location = m_lastLocation;
			return sl_true;
		}
		return sl_false;
	}
	sl_bool getLastCompassDeclination(sl_real& declination)
	{
		if (m_flagValidCompassDeclination) {
			declination = m_lastCompassDeclination;
			return sl_true;
		}
		return sl_false;
	}
	sl_bool getLastAccelerometer(sl_real& xAccel, sl_real& yAccel, sl_real& zAccel)
	{
		if (m_flagValidAccerometer) {
			xAccel = m_lastAccelerometer.xAccel;
			yAccel = m_lastAccelerometer.yAccel;
			zAccel = m_lastAccelerometer.zAccel;
			return sl_true;
		}
		return sl_false;
	}

	void onChangeLocation(float latitude, float longitude, float altitude)
	{
		m_lastLocation.latitude = latitude;
		m_lastLocation.longitude = longitude;
		m_lastLocation.altitude = altitude;
		m_flagValidLocation = sl_true;
		_onLocationChanged(m_lastLocation);
	}

	void onChangeCompass(float declination)
	{
		m_lastCompassDeclination = declination;
		m_flagValidCompassDeclination = sl_true;
		_onCompassChanged(m_lastCompassDeclination);
	}

	void onChangeAccelerometer(float xAccel, float yAccel, float zAccel)
	{
		m_lastAccelerometer.xAccel = xAccel;
		m_lastAccelerometer.yAccel = yAccel;
		m_lastAccelerometer.zAccel = zAccel;
		m_flagValidAccerometer = sl_true;
		_onAccelerometerChanged(xAccel, yAccel, zAccel);
	}
};

SLIB_JNI_BEGIN_CLASS_SECTION(_JAndroidSensors)
	SLIB_JNI_NATIVE_IMPL(nativeOnChangeLocation, "nativeOnChangeLocation", "(JFFF)V", void, jlong instance, float latitude, float longitude, float altitude)
	{
		Ref<_Android_Sensor> sensor = _Android_Sensor::get(instance);
		if (sensor.isNotNull()) {
			sensor->onChangeLocation(latitude, longitude, altitude);
		}
	}
	SLIB_JNI_NATIVE_IMPL(nativeOnChangeCompass, "nativeOnChangeCompass", "(JF)V", void, jlong instance, float declination)
	{
		Ref<_Android_Sensor> sensor = _Android_Sensor::get(instance);
		if (sensor.isNotNull()) {
			sensor->onChangeCompass(declination);
		}
	}
	SLIB_JNI_NATIVE_IMPL(nativeOnChangeAccelerometer, "nativeOnChangeAccelerometer", "(JFFF)V", void, jlong instance, float xAccel, float yAccel, float zAccel)
	{
		Ref<_Android_Sensor> sensor = _Android_Sensor::get(instance);
		if (sensor.isNotNull()) {
			sensor->onChangeAccelerometer(xAccel, yAccel, zAccel);
		}
	}
SLIB_JNI_END_CLASS_SECTION

Ref<Sensor> Sensor::create()
{
	return Ref<Sensor>::from(_Android_Sensor::create());
}
SLIB_DEVICE_NAMESPACE_END

#endif
