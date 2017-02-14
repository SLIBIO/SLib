#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/device/sensor.h"

#include "../../../inc/slib/core/platform_android.h"
#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

	class _Android_Sensor;
	typedef HashMap<jlong, WeakRef<_Android_Sensor> > _AndroidSensorMap;
	SLIB_SAFE_STATIC_GETTER(_AndroidSensorMap, _AndroidSensors_get)

	SLIB_JNI_BEGIN_CLASS(_JAndroidSensor, "slib/platform/android/device/Sensor")
		SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;ZIZZ)Lslib/platform/android/device/Sensor;");
		SLIB_JNI_STATIC_METHOD(isAvailableLocation, "isAvailableLocation", "(Landroid/app/Activity;)Z");
		SLIB_JNI_STATIC_METHOD(isAvailableCompass, "isAvailableCompass", "(Landroid/app/Activity;)Z");
		SLIB_JNI_STATIC_METHOD(isAvailableAccelerometer, "isAvailableAccelerometer", "(Landroid/app/Activity;)Z");
		SLIB_JNI_METHOD(setInstance, "setInstance", "(J)V");
		SLIB_JNI_METHOD(start, "start", "()Z");
		SLIB_JNI_METHOD(stop, "stop", "()V");
	SLIB_JNI_END_CLASS

	class _Android_Sensor : public Sensor
	{
	public:
		JniGlobal<jobject> m_sensor;
		
	public:
		_Android_Sensor()
		{
		}

		~_Android_Sensor()
		{
			stop();

			_AndroidSensorMap* sensorMap = _AndroidSensors_get();
			if (sensorMap) {
				sensorMap->remove((jlong)this);
			}
		}

	public:
		static sl_bool  _isAvailableLocation()
		{
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				return _JAndroidSensor::isAvailableLocation.callBoolean(sl_null, jactivity);
			}
			return sl_false;
		}

		static sl_bool  _isAvailableCompass()
		{
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				return _JAndroidSensor::isAvailableCompass.callBoolean(sl_null, jactivity);
			}
			return sl_false;
		}

		static sl_bool  _isAvailableAccelerometer()
		{
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				return _JAndroidSensor::isAvailableAccelerometer.callBoolean(sl_null, jactivity);
			}
			return sl_false;
		}

		static Ref<_Android_Sensor> create(const SensorParam& param)
		{
			_AndroidSensorMap* sensorMap = _AndroidSensors_get();
			if (!sensorMap) {
				return sl_null;
			}

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
							ret->_init(param);
							ret->m_sensor = sensor;
							jlong instance = (jlong)(ret.get());
							_JAndroidSensor::setInstance.call(obj, instance);
							sensorMap->put(instance, ret);
							if (param.flagAutoStart) {
								ret->start();
							}
							return ret;
						}
					}
				}
			}
			return sl_null;
		}

		static Ref<_Android_Sensor> get(jlong instance)
		{
			_AndroidSensorMap* sensorMap = _AndroidSensors_get();
			if (!sensorMap) {
				return sl_null;
			}

			WeakRef<_Android_Sensor> sensor;
			sensorMap->get(instance, &sensor);
			return sensor;
		}

		// override
		sl_bool _start()
		{
			if (_JAndroidSensor::start.callBoolean(m_sensor)) {
				return sl_true;
			}
			return sl_false;
		}

		// override
		void _stop()
		{
			_JAndroidSensor::stop.call(m_sensor);
		}
		
		void onChangeLocation(double latitude, double longitude, double altitude)
		{
			_onLocationChanged(GeoLocation(latitude, longitude, altitude));
		}

		void onChangeCompass(float declination)
		{
			_onCompassChanged(declination);
		}

		void onChangeAccelerometer(float xAccel, float yAccel, float zAccel)
		{
			_onAccelerometerChanged(xAccel, yAccel, zAccel);
		}
	};

	SLIB_JNI_BEGIN_CLASS_SECTION(_JAndroidSensor)
		SLIB_JNI_NATIVE_IMPL(nativeOnChangeLocation, "nativeOnChangeLocation", "(JDDD)V", void, jlong instance, double latitude, double longitude, double altitude)
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

	sl_bool  Sensor::isAvailableLocation()
	{
		return _Android_Sensor::_isAvailableLocation();
	}

	sl_bool  Sensor::isAvailableAccelerometer()
	{
		return _Android_Sensor::_isAvailableAccelerometer();
	}

	sl_bool Sensor::isAvailableCompass()
	{
		return _Android_Sensor::_isAvailableCompass();
	}

	Ref<Sensor> Sensor::create(const SensorParam& param)
	{
		return _Android_Sensor::create(param);
	}

}

#endif
