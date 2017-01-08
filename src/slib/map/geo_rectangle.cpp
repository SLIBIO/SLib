#include "../../../inc/slib/map/geo_rectangle.h"

#include "../../../inc/slib/core/macro.h"

SLIB_MAP_NAMESPACE_BEGIN

GeoRectangle::GeoRectangle() = default;

GeoRectangle::GeoRectangle(const GeoRectangle& other) = default;

GeoRectangle::GeoRectangle(const LatLon& pt1, const LatLon& pt2)
{
	bottomLeft.latitude = SLIB_MIN(pt1.latitude, pt2.latitude);
	bottomLeft.longitude = SLIB_MIN(pt1.longitude, pt2.longitude);
	topRight.latitude = SLIB_MAX(pt1.latitude, pt2.latitude);
	topRight.longitude = SLIB_MAX(pt1.longitude, pt2.longitude);
}

GeoRectangle& GeoRectangle::operator=(const GeoRectangle& other) = default;

sl_bool GeoRectangle::contains(const LatLon& pt) const
{
	return (bottomLeft.latitude <= pt.latitude && bottomLeft.longitude <= pt.longitude
			&& pt.latitude <= topRight.latitude && pt.longitude <= topRight.longitude);
}

sl_bool GeoRectangle::isValid() const
{
	if (bottomLeft.longitude > topRight.longitude || bottomLeft.latitude > topRight.latitude) {
		return sl_false;
	}
	return sl_true;
}

SLIB_MAP_NAMESPACE_END
