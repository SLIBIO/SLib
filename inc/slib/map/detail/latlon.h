#ifndef CHECKHEADER_SLIB_MAP_DETAIL_LATLON
#define CHECKHEADER_SLIB_MAP_DETAIL_LATLON

#include "../latlon.h"

namespace slib
{

	SLIB_INLINE LatLon& LatLon::operator=(const LatLon& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		return *this;
	}

}

#endif
