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

#include "slib/device/sensor.h"

#include "slib/core/system.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(SensorParam)

	SensorParam::SensorParam()
	{
		flagUseLocation = sl_true;
		locationProviderType = LocationProviderType::GPS;
		
		flagUseCompass = sl_false;
		flagUseAccelerometor = sl_false;
		
		flagAutoStart = sl_true;
	}

	SLIB_DEFINE_OBJECT(Sensor, Object)

	Sensor::Sensor()
	{	
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
		
		m_flagRunningLocation = sl_false;
		m_flagRunningCompass = sl_false;
		m_flagRunningAccelerometer = sl_false;

		m_flagRunning = sl_false;

		m_lastCompassDeclination = 0;
	}

	Sensor::~Sensor()
	{
	}

	sl_bool Sensor::start()
	{
		ObjectLocker lock(this);
		if (m_flagRunning) {
			return sl_false;
		}
		
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
		
		m_flagRunningLocation = sl_false;
		m_flagRunningCompass = sl_false;
		m_flagRunningAccelerometer = sl_false;
		
		if (_start()) {
			m_flagRunning = sl_true;
			return sl_true;
		}
		
		return sl_false;
	}

	void Sensor::stop()
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		
		_stop();
		
		m_flagRunningLocation = sl_false;
		m_flagRunningCompass = sl_false;
		m_flagRunningAccelerometer = sl_false;
		
		m_flagValidLocation = sl_false;
		m_flagValidCompassDeclination = sl_false;
		m_flagValidAccerometer = sl_false;
		
		m_flagRunning = sl_false;
		
	}

	sl_bool Sensor::isRunning()
	{
		return m_flagRunning;
	}

	sl_bool Sensor::isRunningLocation()
	{
		return m_flagRunningLocation;
	}

	sl_bool Sensor::isRunningCompass()
	{
		return m_flagRunningCompass;
	}

	sl_bool Sensor::isRunningAccelerometer()
	{
		return m_flagRunningAccelerometer;
	}

	sl_bool Sensor::getLastLocation(GeoLocation& location)
	{
		if (m_flagValidLocation) {
			location = m_lastLocation;
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Sensor::getLastCompassDeclination(float& declination)
	{
		if (m_flagValidCompassDeclination) {
			declination = m_lastCompassDeclination;
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Sensor::getLastAccelerometer(float& xAccel, float& yAccel, float& zAccel)
	{
		if (m_flagValidAccerometer) {
			xAccel = m_lastAccelerometer.xAccel;
			yAccel = m_lastAccelerometer.yAccel;
			zAccel = m_lastAccelerometer.zAccel;
			return sl_true;
		}
		return sl_false;
	}

	void Sensor::_init(const SensorParam& param)
	{
		m_onLocationChanged = param.onLocationChanged;
		m_onCompassChanged = param.onCompassChanged;
		m_onAccelerometerChanged = param.onAccelerometerChanged;
	}

	void Sensor::_onLocationChanged(const GeoLocation& location)
	{
		m_lastLocation = location;
		m_flagValidLocation = sl_true;

		m_onLocationChanged(this, location);
	};

	void Sensor::_onCompassChanged(float declination)
	{
		m_lastCompassDeclination = declination;
		m_flagValidCompassDeclination = sl_true;
		
		m_onCompassChanged(this, declination);
	}

	void Sensor::_onAccelerometerChanged(float xAccel, float yAccel, float zAccel)
	{
		m_lastAccelerometer.xAccel = xAccel;
		m_lastAccelerometer.yAccel = yAccel;
		m_lastAccelerometer.zAccel = zAccel;
		m_flagValidAccerometer = sl_true;

		m_onAccelerometerChanged(this, xAccel, yAccel, zAccel);
	}

#if !defined(SLIB_PLATFORM_IS_IOS) && !defined(SLIB_PLATFORM_IS_ANDROID)

	Ref<Sensor> Sensor::create(const SensorParam& param)
	{
		return sl_null;
	}

	sl_bool Sensor::isAvailableLocation()
	{
		return sl_false;
	}

	sl_bool Sensor::isAvailableCompass()
	{
		return sl_false;
	}

	sl_bool Sensor::isAvailableAccelerometer()
	{
		return sl_false;
	}

#endif

}

