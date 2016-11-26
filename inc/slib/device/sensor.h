#ifndef CHECKHEADER_SLIB_DEVICE_SENSOR
#define CHECKHEADER_SLIB_DEVICE_SENSOR

#include "definition.h"

#include "../core/object.h"
#include "../core/pointer.h"
#include "../map/geo_location.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class Sensor;

class SLIB_EXPORT ISensorListener
{
public:
	virtual void onLocationChanged(Sensor* sensor, const GeoLocation& location);

	// degree, 0 - North, 180 - South, 90 - East, 270 - West
	virtual void onCompassChanged(Sensor* sensor, sl_real declination);
	
	virtual void onAccelerometerChanged(Sensor* sensor, sl_real xAccel, sl_real yAccel, sl_real zAccel);
	
};

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
	
	Ptr<ISensorListener> listener;
	
public:
	SensorParam();
	
};


class SLIB_EXPORT Sensor : public Object
{
	SLIB_DECLARE_OBJECT

public:
	static Ref<Sensor> create(const SensorParam& param);

public:
	virtual sl_bool start() = 0;
	
	virtual void stop() = 0;
	
	virtual sl_bool isRunning() = 0;

	virtual sl_bool getLastLocation(GeoLocation& location) = 0;
	
	virtual sl_bool getLastCompassDeclination(sl_real& declination) = 0;
	
	virtual sl_bool getLastAccelerometer(sl_real& xAccel, sl_real& yAccel, sl_real& zAccel) = 0;
	
protected:
	void _onLocationChanged(const GeoLocation& location);
	void _onCompassChanged(sl_real declination);
	void _onAccelerometerChanged(sl_real xAccel, sl_real yAccel, sl_real zAccel);
	
protected:
	Ptr<ISensorListener> m_listener;
};

class SLIB_EXPORT SensorLogListener : public Referable, public ISensorListener
{
public:
	// override
	void onLocationChanged(Sensor* sensor, const GeoLocation& location);

	// override
	void onCompassChanged(Sensor* sensor, sl_real declination);
	
	// override
	void onAccelerometerChanged(Sensor* sensor, sl_real xAccel, sl_real yAccel, sl_real zAccel);
};

SLIB_DEVICE_NAMESPACE_END

#endif
