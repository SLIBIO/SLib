#ifndef CHECKHEADER_SLIB_MAP_GEOLOCATION
#define CHECKHEADER_SLIB_MAP_GEOLOCATION

#include "definition.h"

#include "latlon.h"

SLIB_MAP_NAMESPACE_BEGIN

class SLIB_EXPORT GeoLocation
{
public:
	double latitude;
	double longitude;
	double altitude; // Unit: m

public:
	GeoLocation();

	GeoLocation(const GeoLocation& other);
	
	GeoLocation(double latitude, double longitude, double altitude = 0);
	
	GeoLocation(const LatLon& latlon, double altitude = 0);
	
public:
	GeoLocation& operator=(const GeoLocation& other);
	
	GeoLocation& operator=(const LatLon& other);
	
	sl_bool operator==(const GeoLocation& other) const;
	
	sl_bool operator!=(const GeoLocation& other) const;

public:
	LatLon& getLatLon();
	
	const LatLon& getLatLon() const;
	
	void setLatLon(const LatLon& v);
	
	void setLatLon(double latitude, double longitude);

	void normalize();

	GeoLocation lerp(const GeoLocation& target, float factor) const;
	
};


SLIB_INLINE GeoLocation::GeoLocation()
{
}

SLIB_INLINE GeoLocation::GeoLocation(const GeoLocation& other)
: latitude(other.latitude), longitude(other.longitude), altitude(other.altitude)
{
}

SLIB_INLINE GeoLocation::GeoLocation(double _latitude, double _longitude, double _altitude)
: latitude(_latitude), longitude(_longitude), altitude(_altitude)
{
}

SLIB_INLINE GeoLocation::GeoLocation(const LatLon& latlon, double _altitude)
: latitude(latlon.latitude), longitude(latlon.longitude), altitude(_altitude)
{
}

SLIB_INLINE GeoLocation& GeoLocation::operator=(const GeoLocation& other)
{
	latitude = other.latitude;
	longitude = other.longitude;
	altitude = other.altitude;
	return *this;
}

template <>
SLIB_INLINE GeoLocation Interpolation<GeoLocation>::interpolate(const GeoLocation& a, const GeoLocation& b, float factor)
{
	return a.lerp(b, factor);
}

SLIB_MAP_NAMESPACE_END

#endif
