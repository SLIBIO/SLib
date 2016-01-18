#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/device/sensor.h"

#include "../../../inc/slib/core/platform_android.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class _Android_Sensor;
typedef HashMap<jlong, WeakRef<_Android_Sensor> > _AndroidSensorMap;
SLIB_SAFE_STATIC_GETTER(_AndroidSensorMap, _AndroidSensors_get);

SLIB_JNI_BEGIN_CLASS(_JAndroidSensor, "slib/platform/android/device/Sensor")
	SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;ZIZZ)Lslib/platform/android/device/Sensor;");
	SLIB_JNI_METHOD(setInstance, "setInstance", "(J)V");
	SLIB_JNI_METHOD(start, "start", "()Z");
	SLIB_JNI_METHOD(stop, "stop", "()V");
SLIB_JNI_END_CLASS

class _Android_Sensor : public Sensor
{
public:
	JniGlobal<jobject> m_sensor;
    
    sl_bool m_flagRunning;

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
    
public:
	_Android_Sensor()
	{
        m_flagRunning = sl_false;
        
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
	}

	~_Android_Sensor()
	{
		stop();
		_AndroidSensors_get().remove((jlong)this);
	}

public:
	static Ref<_Android_Sensor> create(const SensorParam& param)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jobject> obj = _JAndroidSensor::create.callObject(sl_null, jactivity,
                                                                        param.flagUseLocation, param.locationProviderType,
                                                                        param.flagUseCompass,
                                                                        param.flagUseAccelerometor);
			if (obj.isNotNull()) {
				JniGlobal<jobject> sensor = obj;
				if (sensor.isNotNull()) {
					Ref<_Android_Sensor> ret = new _Android_Sensor;
					if (ret.isNotNull()) {
						ret->m_sensor = sensor;
						jlong instance = (jlong)(ret.get());
						_JAndroidSensor::setInstance.call(obj, instance);
						_AndroidSensors_get().put(instance, ret);
                        if (param.flagAutoStart) {
                            ret->start();
                        }
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
		return sensor;
	}

	sl_bool start()
	{
        ObjectLocker lock(this);
        if (m_flagRunning) {
            return sl_true;
        }
        
        m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
        
		if (_JAndroidSensor::start.callBoolean(m_sensor)) {
            m_flagRunning = sl_true;
            return sl_true;
        }
        return sl_false;
	}

	void stop()
	{
        ObjectLocker lock(this);
        if (!m_flagRunning) {
            return;
        }
        m_flagRunning = sl_false;
        
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
		_JAndroidSensor::stop.call(m_sensor);
	}
    
    sl_bool isRunning()
    {
        return m_flagRunning;
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

SLIB_JNI_BEGIN_CLASS_SECTION(_JAndroidSensor)
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

Ref<Sensor> Sensor::create(const SensorParam& param)
{
	return _Android_Sensor::create(param);
}
SLIB_DEVICE_NAMESPACE_END

#endif
