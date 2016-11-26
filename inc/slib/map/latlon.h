#ifndef CHECKHEADER_SLIB_MAP_LATLON
#define CHECKHEADER_SLIB_MAP_LATLON

#include "definition.h"

#include "../core/interpolation.h"

SLIB_MAP_NAMESPACE_BEGIN

class SLIB_EXPORT LatLon
{
public:
	double latitude;
	double longitude;

public:
	LatLon();

	LatLon(const LatLon& other);
	
	LatLon(double latitude, double longitude);
	
public:
	LatLon& operator=(const LatLon& other);
	
	sl_bool operator==(const LatLon& other) const;
	
	sl_bool operator!=(const LatLon& other) const;

public:
	static LatLon getCenter(const LatLon* list, sl_size count);
	
	static double normalizeLatitude(double latitude);
	
	static double normalizeLongitude(double longitude);

	void normalize();

	LatLon lerp(const LatLon& target, float factor) const;
	
};


SLIB_INLINE LatLon::LatLon()
{
}

SLIB_INLINE LatLon::LatLon(const LatLon& other)
: latitude(other.latitude), longitude(other.longitude)
{
}

SLIB_INLINE LatLon::LatLon(double _latitude, double _longitude)
: latitude(_latitude), longitude(_longitude)
{
}

SLIB_INLINE LatLon& LatLon::operator=(const LatLon& other)
{
	latitude = other.latitude;
	longitude = other.longitude;
	return *this;
}

template <>
SLIB_INLINE LatLon Interpolation<LatLon>::interpolate(const LatLon& a, const LatLon& b, float factor)
{
	return a.lerp(b, factor);
}

SLIB_MAP_NAMESPACE_END

#endif
