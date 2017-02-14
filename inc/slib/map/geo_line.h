#ifndef CHECKHEADER_SLIB_MAP_GEOLINE
#define CHECKHEADER_SLIB_MAP_GEOLINE

#include "definition.h"

#include "latlon.h"

namespace slib
{
	
	class SLIB_EXPORT GeoLine
	{
	public:
		LatLon point1;
		LatLon point2;

	public:
		GeoLine();

		GeoLine(const GeoLine& other);

		GeoLine(const LatLon& pt1, const LatLon& pt2);

	public:
		GeoLine& operator=(const GeoLine& other);

	public:
		sl_bool intersectGeoLine(const GeoLine& otherLine, LatLon* outIntersectPoint = sl_null) const;

	};

}

#endif
