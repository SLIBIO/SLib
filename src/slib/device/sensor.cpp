#include "../../../inc/slib/device/sensor.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/system.h"

SLIB_DEVICE_NAMESPACE_BEGIN

void ISensorListener::onLocationChanged(Sensor* sensor, const GeoLocation& location)
{
}

void ISensorListener::onCompassChanged(Sensor* sensor, float declination)
{
}

void ISensorListener::onAccelerometerChanged(Sensor* sensor, float xAccel, float yAccel, float zAccel)
{
}


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
	m_listener = param.listener;
	m_onLocationChanged = param.onLocationChanged;
	m_onCompassChanged = param.onCompassChanged;
	m_onAccelerometerChanged = param.onAccelerometerChanged;
}

void Sensor::_onLocationChanged(const GeoLocation& location)
{
	m_lastLocation = location;
	m_flagValidLocation = sl_true;

    m_onLocationChanged(location);
	PtrLocker<ISensorListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onLocationChanged(this, location);
	}
};

void Sensor::_onCompassChanged(float declination)
{
	m_lastCompassDeclination = declination;
	m_flagValidCompassDeclination = sl_true;
	
	m_onCompassChanged(declination);
	PtrLocker<ISensorListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onCompassChanged(this, declination);
	}
}

void Sensor::_onAccelerometerChanged(float xAccel, float yAccel, float zAccel)
{
	m_lastAccelerometer.xAccel = xAccel;
	m_lastAccelerometer.yAccel = yAccel;
	m_lastAccelerometer.zAccel = zAccel;
	m_flagValidAccerometer = sl_true;

	m_onAccelerometerChanged(xAccel, yAccel, zAccel);
	PtrLocker<ISensorListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onAccelerometerChanged(this, xAccel, yAccel, zAccel);
	}
}


void SensorLogListener::onLocationChanged(Sensor* sensor, const GeoLocation& location)
{
	String str = "Location(" + String::fromDouble(location.latitude) + "," + location.longitude + "," + location.altitude + ")";
	SLIB_LOG("Sensor", str);
}

void SensorLogListener::onCompassChanged(Sensor* sensor, float declination)
{
	String str = String("Compass(") + declination + ")";
	SLIB_LOG("Sensor", str);
}

void SensorLogListener::onAccelerometerChanged(Sensor* sensor, float xAccel, float yAccel, float zAccel)
{
	String str = String("Accelerometer(") + xAccel + "," + yAccel + "," + zAccel + ")";
	SLIB_LOG("Sensor", str);
}

SLIB_DEVICE_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_DESKTOP)

SLIB_DEVICE_NAMESPACE_BEGIN

Ref<Sensor> Sensor::create(const SensorParam& param)
{
	return Ref<Sensor>::null();
}

sl_bool Sensor::isAvailableLocation()
{
	return sl_true;
}

sl_bool Sensor::isAvailableCompass()
{
	return sl_false;
}

sl_bool Sensor::isAvailableAccelerometer()
{
	return sl_false;
}

SLIB_DEVICE_NAMESPACE_END

#endif
