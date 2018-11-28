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

#include "slib/device/sensor.h"

#include "slib/core/hash_map.h"
#include "slib/core/platform_android.h"
#include "slib/core/safe_static.h"

namespace slib
{

	class _priv_Android_Sensor;
	typedef CHashMap<jlong, WeakRef<_priv_Android_Sensor> > _priv_AndroidSensorMap;
	SLIB_SAFE_STATIC_GETTER(_priv_AndroidSensorMap, _priv_AndroidSensors_get)

	SLIB_JNI_BEGIN_CLASS(JAndroidSensor, "slib/platform/android/device/Sensor")
		SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;ZIZZ)Lslib/platform/android/device/Sensor;");
		SLIB_JNI_STATIC_METHOD(isAvailableLocation, "isAvailableLocation", "(Landroid/app/Activity;)Z");
		SLIB_JNI_STATIC_METHOD(isAvailableCompass, "isAvailableCompass", "(Landroid/app/Activity;)Z");
		SLIB_JNI_STATIC_METHOD(isAvailableAccelerometer, "isAvailableAccelerometer", "(Landroid/app/Activity;)Z");
		SLIB_JNI_METHOD(setInstance, "setInstance", "(J)V");
		SLIB_JNI_METHOD(start, "start", "()Z");
		SLIB_JNI_METHOD(stop, "stop", "()V");
	SLIB_JNI_END_CLASS

	class _priv_Android_Sensor : public Sensor
	{
	public:
		JniGlobal<jobject> m_sensor;
		
	public:
		_priv_Android_Sensor()
		{
		}

		~_priv_Android_Sensor()
		{
			stop();

			_priv_AndroidSensorMap* sensorMap = _priv_AndroidSensors_get();
			if (sensorMap) {
				sensorMap->remove((jlong)this);
			}
		}

	public:
		static sl_bool  _isAvailableLocation()
		{
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				return JAndroidSensor::isAvailableLocation.callBoolean(sl_null, jactivity);
			}
			return sl_false;
		}

		static sl_bool  _isAvailableCompass()
		{
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				return JAndroidSensor::isAvailableCompass.callBoolean(sl_null, jactivity);
			}
			return sl_false;
		}

		static sl_bool  _isAvailableAccelerometer()
		{
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				return JAndroidSensor::isAvailableAccelerometer.callBoolean(sl_null, jactivity);
			}
			return sl_false;
		}

		static Ref<_priv_Android_Sensor> create(const SensorParam& param)
		{
			_priv_AndroidSensorMap* sensorMap = _priv_AndroidSensors_get();
			if (!sensorMap) {
				return sl_null;
			}

			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				JniLocal<jobject> obj = JAndroidSensor::create.callObject(sl_null, jactivity,
																			param.flagUseLocation, param.locationProviderType,
																			param.flagUseCompass,
																			param.flagUseAccelerometor);
				if (obj.isNotNull()) {
					JniGlobal<jobject> sensor = obj;
					if (sensor.isNotNull()) {
						Ref<_priv_Android_Sensor> ret = new _priv_Android_Sensor;
						if (ret.isNotNull()) {
							ret->_init(param);
							ret->m_sensor = sensor;
							jlong instance = (jlong)(ret.get());
							JAndroidSensor::setInstance.call(obj, instance);
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

		static Ref<_priv_Android_Sensor> get(jlong instance)
		{
			_priv_AndroidSensorMap* sensorMap = _priv_AndroidSensors_get();
			if (!sensorMap) {
				return sl_null;
			}

			WeakRef<_priv_Android_Sensor> sensor;
			sensorMap->get(instance, &sensor);
			return sensor;
		}

		sl_bool _start() override
		{
			if (JAndroidSensor::start.callBoolean(m_sensor)) {
				return sl_true;
			}
			return sl_false;
		}

		void _stop() override
		{
			JAndroidSensor::stop.call(m_sensor);
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

	SLIB_JNI_BEGIN_CLASS_SECTION(JAndroidSensor)
		SLIB_JNI_NATIVE_IMPL(nativeOnChangeLocation, "nativeOnChangeLocation", "(JDDD)V", void, jlong instance, double latitude, double longitude, double altitude)
		{
			Ref<_priv_Android_Sensor> sensor = _priv_Android_Sensor::get(instance);
			if (sensor.isNotNull()) {
				sensor->onChangeLocation(latitude, longitude, altitude);
			}
		}
		SLIB_JNI_NATIVE_IMPL(nativeOnChangeCompass, "nativeOnChangeCompass", "(JF)V", void, jlong instance, float declination)
		{
			Ref<_priv_Android_Sensor> sensor = _priv_Android_Sensor::get(instance);
			if (sensor.isNotNull()) {
				sensor->onChangeCompass(declination);
			}
		}
		SLIB_JNI_NATIVE_IMPL(nativeOnChangeAccelerometer, "nativeOnChangeAccelerometer", "(JFFF)V", void, jlong instance, float xAccel, float yAccel, float zAccel)
		{
			Ref<_priv_Android_Sensor> sensor = _priv_Android_Sensor::get(instance);
			if (sensor.isNotNull()) {
				sensor->onChangeAccelerometer(xAccel, yAccel, zAccel);
			}
		}
	SLIB_JNI_END_CLASS_SECTION

	sl_bool  Sensor::isAvailableLocation()
	{
		return _priv_Android_Sensor::_isAvailableLocation();
	}

	sl_bool  Sensor::isAvailableAccelerometer()
	{
		return _priv_Android_Sensor::_isAvailableAccelerometer();
	}

	sl_bool Sensor::isAvailableCompass()
	{
		return _priv_Android_Sensor::_isAvailableCompass();
	}

	Ref<Sensor> Sensor::create(const SensorParam& param)
	{
		return _priv_Android_Sensor::create(param);
	}

}

#endif
