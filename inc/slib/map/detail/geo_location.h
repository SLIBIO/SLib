#ifndef CHECKHEADER_SLIB_MAP_DETAIL_GEOLOCATION
#define CHECKHEADER_SLIB_MAP_DETAIL_GEOLOCATION

#include "../latlon.h"

namespace slib
{
	
	SLIB_INLINE GeoLocation& GeoLocation::operator=(const GeoLocation& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		altitude = other.altitude;
		return *this;
	}
	
}

#endif
