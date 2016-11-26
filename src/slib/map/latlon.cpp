#include "../../../inc/slib/map/latlon.h"

#include "../../../inc/slib/core/math.h"

SLIB_MAP_NAMESPACE_BEGIN

sl_bool LatLon::operator==(const LatLon& other) const
{
	return latitude == other.latitude && longitude == other.longitude;
}

sl_bool LatLon::operator!=(const LatLon& other) const
{
	return !(*this == other);
}

LatLon LatLon::getCenter(const LatLon* list, sl_size count)
{
	LatLon ret;
	ret.latitude = 0;
	ret.longitude = 0;

	double signedArea = 0;
	double lat0 = 0;
	double lon0 = 0;
	double lat1 = 0;
	double lon1 = 0;
	double a = 0;

	sl_size i;
	for (i = 0; i < count - 1; i++) {
		lat0 = list[i].latitude;
		lon0 = list[i].longitude;
		lat1 = list[i + 1].latitude;
		lon1 = list[i + 1].longitude;
		a = lat0 * lon1 - lat1 * lon0;
		signedArea += a;
		ret.latitude += (lat0 + lat1) * a;
		ret.longitude += (lon0 + lon1) * a;
	}

	lat0 = list[i].latitude;
	lon0 = list[i].longitude;
	lat1 = list[0].latitude;
	lon1 = list[0].longitude;
	a = lat0 * lon1 - lat1 * lon0;
	signedArea += a;
	if (signedArea == 0 || (ret.latitude == 0 && ret.longitude == 0)) {
		ret.latitude = lat1;
		ret.longitude = lon1;
	}
	else{
		ret.latitude += (lat0 + lat1) * a;
		ret.longitude += (lon0 + lon1) * a;

		signedArea /= 2;
		ret.latitude /= (6 * signedArea);
		ret.longitude /= (6 * signedArea);
	}
	

	return ret;
}

double LatLon::normalizeLatitude(double latitude)
{
	if (latitude < -90) {
		latitude = -90;
	}
	if (latitude > 90) {
		latitude = 90;
	}
	return latitude;
}

double LatLon::normalizeLongitude(double longitude)
{
	longitude = Math::normalizeDegree(longitude + 180) - 180;
	return longitude;
}

void LatLon::normalize()
{
	latitude = normalizeLatitude(latitude);
	longitude = normalizeLongitude(longitude);
}

LatLon LatLon::lerp(const LatLon& target, float factor) const
{
	double tlon = target.longitude;
	double dlon = tlon - longitude;
	if (dlon > 180) {
		tlon -= 360;
	} else if (dlon < -180) {
		tlon += 360;
	}
	LatLon r;
	r.longitude = (tlon - longitude) * factor + longitude;
	r.latitude = (target.latitude - latitude) * factor + latitude;
	return r;
}

SLIB_MAP_NAMESPACE_END
