#include "../../../inc/slib/device/sensor.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/system.h"

SLIB_DEVICE_NAMESPACE_BEGIN

void Sensor::_onLocationChanged(const GeoLocation& location)
{
	PtrLocker<ISensorListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onLocationChanged(this, location);
	}
};

void Sensor::_onCompassChanged(sl_real declination)
{
	PtrLocker<ISensorListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onCompassChanged(this, declination);
	}
}

void Sensor::_onAccelerometerChanged(sl_real xAccel, sl_real yAccel, sl_real zAccel)
{
	PtrLocker<ISensorListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onAccelerometerChanged(this, xAccel, yAccel, zAccel);
	}
}

void SensorLogListener::onLocationChanged(Sensor* sensor, const GeoLocation& location)
{
	String str = "Location(" + String::fromDouble(location.latitude) + "," + location.longitude + "," + location.altitude + ")";
	SLIB_LOG("Sensor", str);
}

void SensorLogListener::onCompassChanged(Sensor* sensor, sl_real declination)
{
	String str = String("Compass(") + declination + ")";
	SLIB_LOG("Sensor", str);
}

void SensorLogListener::onAccelerometerChanged(Sensor* sensor, sl_real xAccel, sl_real yAccel, sl_real zAccel)
{
	String str = String("Accelerometer(") + xAccel + "," + yAccel + "," + zAccel + ")";
	SLIB_LOG("Sensor", str);
}

SLIB_DEVICE_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_DESKTOP)

SLIB_DEVICE_NAMESPACE_BEGIN
Ref<Sensor> Sensor::create()
{
	return Ref<Sensor>::null();
}
SLIB_DEVICE_NAMESPACE_END

#endif
