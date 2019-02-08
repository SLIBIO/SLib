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

#ifndef CHECKHEADER_SLIB_DEVICE_SENSOR
#define CHECKHEADER_SLIB_DEVICE_SENSOR

#include "definition.h"

#include "../core/object.h"
#include "../core/function.h"
#include "../geo/geo_location.h"

namespace slib
{
	
	class Sensor;
	
	enum class LocationProviderType
	{
		GPS = 0,
		MobileNetwork = 1,
		Passive = 2
	};
	
	class SLIB_EXPORT SensorParam
	{
	public:
		sl_bool flagUseLocation;
		LocationProviderType locationProviderType;

		sl_bool flagUseCompass;
		sl_bool flagUseAccelerometor;

		sl_bool flagAutoStart;

		Function<void(Sensor*, const GeoLocation&)> onLocationChanged;
		Function<void(Sensor*, float declination)> onCompassChanged;
		Function<void(Sensor*, float xAccel, float yAccel, float zAccel)> onAccelerometerChanged;

	public:
		SensorParam();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(SensorParam)

	};
	
	
	class SLIB_EXPORT Sensor : public Object
	{
		SLIB_DECLARE_OBJECT
	
	protected:
		Sensor();

		~Sensor();

	public:
		static Ref<Sensor> create(const SensorParam& param);

		static sl_bool isAvailableLocation();

		static sl_bool isAvailableCompass();

		static sl_bool isAvailableAccelerometer();

	public:
		sl_bool start();

		void stop();

		sl_bool isRunning();
	
		sl_bool isRunningLocation();

		sl_bool isRunningCompass();

		sl_bool isRunningAccelerometer();

		sl_bool getLastLocation(GeoLocation& location);

		sl_bool getLastCompassDeclination(float& declination);

		sl_bool getLastAccelerometer(float& xAccel, float& yAccel, float& zAccel);

	protected:
		void _init(const SensorParam& param);

		virtual sl_bool _start() = 0;

		virtual void _stop() = 0;

		void _onLocationChanged(const GeoLocation& location);

		void _onCompassChanged(float declination);

		void _onAccelerometerChanged(float xAccel, float yAccel, float zAccel);

	protected:
		sl_bool m_flagRunning;

		sl_bool m_flagRunningLocation;
		sl_bool m_flagRunningCompass;
		sl_bool m_flagRunningAccelerometer;
	
		GeoLocation m_lastLocation;
		sl_bool m_flagValidLocation;

		float m_lastCompassDeclination;
		sl_bool m_flagValidCompassDeclination;

		struct Accelerometer {
			float xAccel;
			float yAccel;
			float zAccel;
		} m_lastAccelerometer;
		sl_bool m_flagValidAccerometer;

		Function<void(Sensor*, const GeoLocation&)> m_onLocationChanged;
		Function<void(Sensor*, float declination)> m_onCompassChanged;
		Function<void(Sensor*, float xAccel, float yAccel, float zAccel)> m_onAccelerometerChanged;

	};
	
}

#endif
