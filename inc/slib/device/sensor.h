#ifndef CHECKHEADER_SLIB_DEVICE_SENSOR
#define CHECKHEADER_SLIB_DEVICE_SENSOR

#include "definition.h"

#include "../core/object.h"
#include "../core/pointer.h"
#include "../math/geograph.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class Sensor;
class SLIB_EXPORT ISensorListener
{
public:
	SLIB_INLINE ISensorListener() {}

public:
	virtual void onLocationChanged(Sensor* sensor, const GeoLocation& location) {};

	// degree, 0 - North, 180 - South, 90 - East, 270 - West
	virtual void onCompassChanged(Sensor* sensor, sl_real declination) {};
	
	virtual void onAccelerometerChanged(Sensor* sensor, sl_real xAccel, sl_real yAccel, sl_real zAccel){};
};

class SLIB_EXPORT Sensor : public Object
{
public:
	enum LocationProviderType
	{
		GPS = 0,
		MobileNetwork = 1,
		Passive = 2
	};

	struct Param
	{
		sl_bool flagUseLocation;
		LocationProviderType providerType;
		sl_bool flagUseCompass;
		sl_bool flagUseAccelerometor;
		Param()
		{
			flagUseLocation = sl_true;
			providerType = GPS;
			flagUseCompass = sl_false;
			flagUseAccelerometor = sl_false;
		}
	};

	virtual sl_bool start(const Param& param) = 0;
	virtual void stop() = 0;

	virtual sl_bool getLastLocation(GeoLocation& location) = 0;
	virtual sl_bool getLastCompassDeclination(sl_real& declination) = 0;
	virtual sl_bool getLastAccelerometer(sl_real& xAccel, sl_real& yAccel, sl_real& zAccel) = 0;
public:
	SLIB_PROPERTY_INLINE(Ptr<ISensorListener>, Listener)

protected:
	void _onLocationChanged(const GeoLocation& location);
	void _onCompassChanged(sl_real declination);
	void _onAccelerometerChanged(sl_real xAccel, sl_real yAccel, sl_real zAccel);
public:
	static Ref<Sensor> create();
};

class SLIB_EXPORT SensorLogListener : public Referable, public ISensorListener
{
public:
	SLIB_INLINE SensorLogListener() {}

public:
	void onLocationChanged(Sensor* sensor, const GeoLocation& location);
	void onCompassChanged(Sensor* sensor, sl_real declination);
	void onAccelerometerChanged(Sensor* sensor, sl_real xAccel, sl_real yAccel, sl_real zAccel);
};

SLIB_DEVICE_NAMESPACE_END

#endif
