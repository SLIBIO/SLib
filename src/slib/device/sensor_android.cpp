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

	namespace priv
	{
		namespace sensor
		{

			class SensorImpl;

			typedef CHashMap<jlong, WeakRef<SensorImpl> > SensorMap;
			
			SLIB_SAFE_STATIC_GETTER(SensorMap, GetSensorMap)

			SLIB_JNI_BEGIN_CLASS(JSensor, "slib/platform/android/device/Sensor")
				SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;ZIZZ)Lslib/platform/android/device/Sensor;");
				SLIB_JNI_STATIC_METHOD(isAvailableLocation, "isAvailableLocation", "(Landroid/app/Activity;)Z");
				SLIB_JNI_STATIC_METHOD(isAvailableCompass, "isAvailableCompass", "(Landroid/app/Activity;)Z");
				SLIB_JNI_STATIC_METHOD(isAvailableAccelerometer, "isAvailableAccelerometer", "(Landroid/app/Activity;)Z");
				SLIB_JNI_METHOD(setInstance, "setInstance", "(J)V");
				SLIB_JNI_METHOD(start, "start", "()Z");
				SLIB_JNI_METHOD(stop, "stop", "()V");
			SLIB_JNI_END_CLASS

			class SensorImpl : public Sensor
			{
			public:
				JniGlobal<jobject> m_sensor;
				
			public:
				SensorImpl()
				{
				}

				~SensorImpl()
				{
					stop();

					SensorMap* sensorMap = GetSensorMap();
					if (sensorMap) {
						sensorMap->remove((jlong)this);
					}
				}

			public:
				static sl_bool  _isAvailableLocation()
				{
					jobject jactivity = Android::getCurrentActivity();
					if (jactivity) {
						return JSensor::isAvailableLocation.callBoolean(sl_null, jactivity);
					}
					return sl_false;
				}

				static sl_bool  _isAvailableCompass()
				{
					jobject jactivity = Android::getCurrentActivity();
					if (jactivity) {
						return JSensor::isAvailableCompass.callBoolean(sl_null, jactivity);
					}
					return sl_false;
				}

				static sl_bool  _isAvailableAccelerometer()
				{
					jobject jactivity = Android::getCurrentActivity();
					if (jactivity) {
						return JSensor::isAvailableAccelerometer.callBoolean(sl_null, jactivity);
					}
					return sl_false;
				}

				static Ref<SensorImpl> create(const SensorParam& param)
				{
					SensorMap* sensorMap = GetSensorMap();
					if (!sensorMap) {
						return sl_null;
					}

					jobject jactivity = Android::getCurrentActivity();
					if (jactivity) {
						JniLocal<jobject> obj = JSensor::create.callObject(sl_null, jactivity,
																					param.flagUseLocation, param.locationProviderType,
																					param.flagUseCompass,
																					param.flagUseAccelerometor);
						if (obj.isNotNull()) {
							JniGlobal<jobject> sensor = obj;
							if (sensor.isNotNull()) {
								Ref<SensorImpl> ret = new SensorImpl;
								if (ret.isNotNull()) {
									ret->_init(param);
									ret->m_sensor = sensor;
									jlong instance = (jlong)(ret.get());
									JSensor::setInstance.call(obj, instance);
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

				static Ref<SensorImpl> get(jlong instance)
				{
					SensorMap* sensorMap = GetSensorMap();
					if (!sensorMap) {
						return sl_null;
					}

					WeakRef<SensorImpl> sensor;
					sensorMap->get(instance, &sensor);
					return sensor;
				}

				sl_bool _start() override
				{
					if (JSensor::start.callBoolean(m_sensor)) {
						return sl_true;
					}
					return sl_false;
				}

				void _stop() override
				{
					JSensor::stop.call(m_sensor);
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

			SLIB_JNI_BEGIN_CLASS_SECTION(JSensor)
				SLIB_JNI_NATIVE_IMPL(nativeOnChangeLocation, "nativeOnChangeLocation", "(JDDD)V", void, jlong instance, double latitude, double longitude, double altitude)
				{
					Ref<SensorImpl> sensor = SensorImpl::get(instance);
					if (sensor.isNotNull()) {
						sensor->onChangeLocation(latitude, longitude, altitude);
					}
				}
				SLIB_JNI_NATIVE_IMPL(nativeOnChangeCompass, "nativeOnChangeCompass", "(JF)V", void, jlong instance, float declination)
				{
					Ref<SensorImpl> sensor = SensorImpl::get(instance);
					if (sensor.isNotNull()) {
						sensor->onChangeCompass(declination);
					}
				}
				SLIB_JNI_NATIVE_IMPL(nativeOnChangeAccelerometer, "nativeOnChangeAccelerometer", "(JFFF)V", void, jlong instance, float xAccel, float yAccel, float zAccel)
				{
					Ref<SensorImpl> sensor = SensorImpl::get(instance);
					if (sensor.isNotNull()) {
						sensor->onChangeAccelerometer(xAccel, yAccel, zAccel);
					}
				}
			SLIB_JNI_END_CLASS_SECTION

		}
	}

	using namespace priv::sensor;

	sl_bool  Sensor::isAvailableLocation()
	{
		return SensorImpl::_isAvailableLocation();
	}

	sl_bool  Sensor::isAvailableAccelerometer()
	{
		return SensorImpl::_isAvailableAccelerometer();
	}

	sl_bool Sensor::isAvailableCompass()
	{
		return SensorImpl::_isAvailableCompass();
	}

	Ref<Sensor> Sensor::create(const SensorParam& param)
	{
		return SensorImpl::create(param);
	}

}

#endif
