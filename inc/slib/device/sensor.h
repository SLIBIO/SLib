#ifndef CHECKHEADER_SLIB_DEVICE_SENSOR
#define CHECKHEADER_SLIB_DEVICE_SENSOR

#include "definition.h"

#include "../core/object.h"
#include "../core/ptr.h"
#include "../core/function.h"
#include "../map/geo_location.h"

namespace slib
{
	
	class Sensor;
	
	class SLIB_EXPORT ISensorListener
	{
	public:
		ISensorListener();

		virtual ~ISensorListener();

	public:
		virtual void onLocationChanged(Sensor* sensor, const GeoLocation& location);
	
		// degree, 0 - North, 180 - South, 90 - East, 270 - West
		virtual void onCompassChanged(Sensor* sensor, float declination);

		virtual void onAccelerometerChanged(Sensor* sensor, float xAccel, float yAccel, float zAccel);

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
		Function<void(const GeoLocation& location)> onLocationChanged;
		Function<void(float declination)> onCompassChanged;
		Function<void(float xAccel, float yAccel, float zAccel)> onAccelerometerChanged;

	public:
		SensorParam();

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

		Ptr<ISensorListener> m_listener;
		Function<void(const GeoLocation& location)> m_onLocationChanged;
		Function<void(float declination)> m_onCompassChanged;
		Function<void(float xAccel, float yAccel, float zAccel)> m_onAccelerometerChanged;

	};
	
	class SLIB_EXPORT SensorLogListener : public Referable, public ISensorListener
	{
	public:
		SensorLogListener();

		~SensorLogListener();

	public:
		// override
		void onLocationChanged(Sensor* sensor, const GeoLocation& location);
	
		// override
		void onCompassChanged(Sensor* sensor, float declination);

		// override
		void onAccelerometerChanged(Sensor* sensor, float xAccel, float yAccel, float zAccel);
	};

}

#endif
