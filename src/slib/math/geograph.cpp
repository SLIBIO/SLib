#include "../../../inc/slib/math/geograph.h"

#include "../../../inc/slib/core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

/*****************************************
				LatLon
******************************************/

LatLon::LatLon(double _latitude, double _longitude) : latitude(_latitude), longitude(_longitude)
{
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

LatLon LatLon::getCenter(const List<LatLon>& _list)
{
	ListLocker<LatLon> list(_list);
	return getCenter(list.data, list.count);
}

sl_bool LatLon::checkIntersectionLineToLine(
	LatLon& pointIntersect,
	const LatLon& line1Start, const LatLon& line1End,
	const LatLon& line2Start, const LatLon& line2End)
{
	double p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
	p0_x = line1Start.longitude;
	p0_y = line1Start.latitude;
	p1_x = line1End.longitude;
	p1_y = line1End.latitude;
	p2_x = line2Start.longitude;
	p2_y = line2Start.latitude;
	p3_x = line2End.longitude;
	p3_y = line2End.latitude;

	double s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;
	s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;
	s2_y = p3_y - p2_y;

	double s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		pointIntersect.longitude = p0_x + (t * s1_x);
		pointIntersect.latitude = p0_y + (t * s1_y);
		return sl_true;
	}
	return sl_false;
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

LatLon LatLon::lerp(const LatLon& target, sl_real factor) const
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

sl_bool LatLon::operator==(const LatLon& other) const
{
	return latitude == other.latitude && longitude == other.longitude;
}

sl_bool LatLon::operator!=(const LatLon& other) const
{
	return !(*this == other);
}


/**************************************************
					GeoLocation
***************************************************/

GeoLocation::GeoLocation(double _latitude, double _longitude, double _altitude)
: latitude(_latitude), longitude(_longitude), altitude(_altitude)
{
}

GeoLocation::GeoLocation(const LatLon& latlon, double _altitude)
: latitude(latlon.latitude), longitude(latlon.longitude), altitude(_altitude)
{
}

LatLon GeoLocation::getLatLon() const
{
	return {latitude, longitude};
}

void GeoLocation::setLatLon(const LatLon& v)
{
	latitude = v.latitude;
	longitude = v.longitude;
}

void GeoLocation::setLatLon(double _latitude, double _longitude)
{
	latitude = _latitude;
	longitude = _longitude;
}

void GeoLocation::normalize()
{
	latitude = LatLon::normalizeLatitude(latitude);
	longitude = LatLon::normalizeLongitude(longitude);
}

GeoLocation GeoLocation::lerp(const GeoLocation& target, sl_real factor) const
{
	LatLon l(latitude, longitude);
	double a = (target.altitude - altitude) * factor + altitude;
	GeoLocation r(l.lerp(LatLon(target.latitude, target.longitude), factor), a);
	return r;
}

GeoLocation& GeoLocation::operator=(const LatLon& other)
{
	latitude = other.latitude;
	longitude = other.longitude;
	altitude = 0;
	return *this;
}

sl_bool GeoLocation::operator==(const GeoLocation& other) const
{
	return latitude == other.latitude && longitude == other.longitude && altitude == other.altitude;
}

sl_bool GeoLocation::operator!=(const GeoLocation& other) const
{
	return latitude != other.latitude || longitude != other.longitude || altitude != other.altitude;
}


/**************************************************
				GeoRectangle
***************************************************/

GeoRectangle::GeoRectangle(const LatLon& pt1, const LatLon& pt2)
{
	bottomLeft.latitude = SLIB_MIN(pt1.latitude, pt2.latitude);
	bottomLeft.longitude = SLIB_MIN(pt1.longitude, pt2.longitude);
	topRight.latitude = SLIB_MAX(pt1.latitude, pt2.latitude);
	topRight.longitude = SLIB_MAX(pt1.longitude, pt2.longitude);
}

sl_bool GeoRectangle::contains(LatLon& pt) const
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


/**************************************************
					Globe
***************************************************/

void Globe::_initializeParameters()
{
	double f = radiusPolar / radiusEquatorial;
	inverseFlattening = 1 / (1 - f);
	eccentricitySquared = 1 - f * f;
}

Globe::Globe(double _radiusEquatorial, double _radiusPolar)
{
	radiusEquatorial = _radiusEquatorial;
	radiusPolar = _radiusPolar;
	_initializeParameters();
}

Globe::Globe(double _radiusEquatorial, double _radiusPolar, double _inverseFlattening, double _eccentricitySquared)
{
	radiusEquatorial = _radiusEquatorial;
	radiusPolar = _radiusPolar;
	
	inverseFlattening = _inverseFlattening;
	eccentricitySquared = _eccentricitySquared;
}

double Globe::getEquatorialRadius() const
{
	return radiusEquatorial;
}

void Globe::setEquatorialRadius(double radius)
{
	radiusEquatorial = radius;
	_initializeParameters();
}

double Globe::getPolarRadius() const
{
	return radiusPolar;
}

void Globe::setPolarRadius(double radius)
{
	radiusPolar = radius;
	_initializeParameters();
}

double Globe::getInverseFlattening() const
{
	return inverseFlattening;
}

double Globe::getEccentricitySquared() const
{
	return eccentricitySquared;
}

Vector3lf Globe::getSurfaceNormal(double latitude, double longitude) const
{
	double _latitude = Math::getRadianFromDegrees(latitude);
	double _longitude = Math::getRadianFromDegrees(longitude);
	Vector3lf R;
	R.y = (1 - eccentricitySquared) * Math::sin(_latitude) / radiusPolar / radiusPolar;
	double xz = Math::cos(_latitude) / radiusEquatorial / radiusEquatorial;
	R.x = xz * Math::sin(_longitude);
	R.z = -xz * Math::cos(_longitude);
	R.normalize();
	return R;
}

Vector3lf Globe::getSurfaceNormal(const LatLon& latlon) const
{
	return getSurfaceNormal(latlon.latitude, latlon.longitude);
}

Vector3lf Globe::getSurfaceNormal(const GeoLocation& location) const
{
	return getSurfaceNormal(location.latitude, location.longitude);
}

Vector3lf Globe::getNorthPointingTangent(double latitude, double longitude) const
{
	double _latitude = Math::getRadianFromDegrees(latitude);
	double _longitude = Math::getRadianFromDegrees(longitude);
	
	_latitude = -_latitude;
	Vector3lf R;
	R.y = Math::cos(_latitude);
	double xz = Math::sin(_latitude);
	R.x = xz * Math::sin(_longitude);
	R.z = -xz * Math::cos(_longitude);
	R.normalize();
	return R;
}

Vector3lf Globe::getNorthPointingTangent(const LatLon& latlon) const
{
	return getNorthPointingTangent(latlon.latitude, latlon.longitude);
}

Vector3lf Globe::getNorthPointingTangent(const GeoLocation& location) const
{
	return getNorthPointingTangent(location.latitude, location.longitude);
}

Vector3lf Globe::getSurfaceNormalAtCartesianPosition(double x, double y, double z) const
{
	double a2 = radiusEquatorial * radiusEquatorial;
	double b2 = radiusPolar * radiusPolar;
	Vector3lf R;
	R.x = x / a2;
	R.y = y / b2;
	R.z = z / a2;
	R.normalize();
	return R;
}

Vector3lf Globe::getSurfaceNormalAtCartesianPosition(const Vector3lf& position) const
{
	return getSurfaceNormalAtCartesianPosition(position.x, position.y, position.z);
}

Vector3lf Globe::getCartesianPosition(double latitude, double longitude, double altitude) const
{
	double _latitude = Math::getRadianFromDegrees(latitude);
	double _longitude = Math::getRadianFromDegrees(longitude);
	
	double sinLat = Math::sin(_latitude);
	// get radius of vertical in prime meridian
	double rv = radiusEquatorial / Math::sqrt(1 - eccentricitySquared * sinLat * sinLat);
	Vector3lf B;
	B.y = sinLat * (rv * (1 - eccentricitySquared) + altitude);
	double rxz = Math::cos(_latitude) * (rv + altitude);
	B.x = rxz * Math::sin(_longitude);
	B.z = -rxz * Math::cos(_longitude);
	return B;
}

Vector3lf Globe::getCartesianPosition(const LatLon& latlon) const
{
	return getCartesianPosition(latlon.latitude, latlon.longitude, 0);
}

Vector3lf Globe::getCartesianPosition(const GeoLocation& location) const
{
	return getCartesianPosition(location.latitude, location.longitude, location.altitude);
}

/*
	According to H. Vermeille
 "An analytical method to transform geocentric into geodetic coordinates"
 http://www.springerlink.com/content/3t6837t27t351227/fulltext.pdf
 
	Codes are referenced from NASA WorldWind Java version
 */
GeoLocation Globe::getGeoLocation(double x, double y, double z) const
{
	double X = -z;
	double Y = x;
	double Z = y;
	double XXpYY = X * X + Y * Y;
	double sqrtXXpYY = Math::sqrt(XXpYY);
	
	double a = radiusEquatorial;
	double ra2 = 1 / (a * a);
	double e2 = eccentricitySquared;
	double e4 = e2 * e2;
	
	// Step 1
	double p = XXpYY * ra2;
	double q = Z * Z * (1 - e2) * ra2;
	double r = (p + q - e4) / 6;
	
	double h;
	double phi;
	
	double evoluteBorderTest = 8 * r * r * r + e4 * p * q;
	if (evoluteBorderTest > 0 || q != 0) {
		
		double u;
		
		if (evoluteBorderTest > 0) {
			// Step 2: general case
			double rad1 = Math::sqrt(evoluteBorderTest);
			double rad2 = Math::sqrt(e4 * p * q);
			
			// 10*e2 is my arbitrary decision of what Vermeille means by "near... the cusps of the evolute".
			if (evoluteBorderTest > 10 * e2)
			{
				double rad3 = Math::cbrt((rad1 + rad2) * (rad1 + rad2));
				u = r + 0.5 * rad3 + 2 * r * r / rad3;
			} else {
				u = r + 0.5 * Math::cbrt((rad1 + rad2) * (rad1 + rad2)) + 0.5 * Math::cbrt((rad1 - rad2) * (rad1 - rad2));
			}
		} else {
			// Step 3: near evolute
			double rad1 = Math::sqrt(-evoluteBorderTest);
			double rad2 = Math::sqrt(-8 * r * r * r);
			double rad3 = Math::sqrt(e4 * p * q);
			double atan = 2 * Math::arctan2(rad3, rad1 + rad2) / 3;
			
			u = -4 * r * Math::sin(atan) * Math::cos(SLIB_PI_LONG / 6 + atan);
		}
		
		double v = Math::sqrt(u * u + e4 * q);
		double w = e2 * (u + v - q) / (2 * v);
		double k = (u + v) / (Math::sqrt(w * w + u + v) + w);
		double D = k * sqrtXXpYY / (k + e2);
		double sqrtDDpZZ = Math::sqrt(D * D + Z * Z);
		
		h = (k + e2 - 1) * sqrtDDpZZ / k;
		phi = 2 * Math::arctan2(Z, sqrtDDpZZ + D);
		
	} else {
		// Step 4: singular disk
		double rad1 = Math::sqrt(1 - e2);
		double rad2 = Math::sqrt(e2 - p);
		double e = Math::sqrt(e2);
		
		h = -a * rad1 * rad2 / e;
		phi = rad2 / (e * rad2 + rad1 * Math::sqrt(p));
	}
	
	// Compute lambda
	double lambda;
	double s2 = 1.4142135623730950488016887242097; // Math::sqrt(2.0);
	if ((s2 - 1) * Y < sqrtXXpYY + X) {
		// case 1 - -135deg < lambda < 135deg
		lambda = 2 * Math::arctan2(Y, sqrtXXpYY + X);
	} else if (sqrtXXpYY + Y < (s2 + 1) * X) {
		// case 2 - -225deg < lambda < 45deg
		lambda = -SLIB_PI_LONG * 0.5 + 2 * Math::arctan2(X, sqrtXXpYY - Y);
	} else {
		// if (sqrtXXpYY-Y<(s2=1)*X) {  // is the test, if needed, but it's not
		// case 3: - -45deg < lambda < 225deg
		lambda = SLIB_PI_LONG * 0.5 - 2 * Math::arctan2(X, sqrtXXpYY + Y);
	}
	
	return GeoLocation(Math::getDegreesFromRadian(phi), Math::getDegreesFromRadian(lambda), h);
}

GeoLocation Globe::getGeoLocation(const Vector3lf& position) const
{
	return getGeoLocation(position.x, position.y, position.z);
}


/**************************************************
				SphericalGlobe
***************************************************/

SphericalGlobe::SphericalGlobe(double _radius) : radius(_radius)
{
}

double SphericalGlobe::getRadius() const
{
	return radius;
}

void SphericalGlobe::setRadius(double _radius)
{
	radius = _radius;
}

Vector3lf SphericalGlobe::getSurfaceNormal(double latitude, double longitude) const
{
	double _latitude = Math::getRadianFromDegrees(latitude);
	double _longitude = Math::getRadianFromDegrees(longitude);
	Vector3lf R;
	R.y = Math::sin(_latitude);
	double xz = Math::cos(_latitude);
	R.x = xz * Math::sin(_longitude);
	R.z = -xz * Math::cos(_longitude);
	R.normalize();
	return R;
}

Vector3lf SphericalGlobe::getSurfaceNormal(const LatLon& latlon) const
{
	return getSurfaceNormal(latlon.latitude, latlon.longitude);
}

Vector3lf SphericalGlobe::getSurfaceNormal(const GeoLocation& location) const
{
	return getSurfaceNormal(location.latitude, location.longitude);
}

Vector3lf SphericalGlobe::getNorthPointingTangent(double latitude, double longitude) const
{
	double _latitude = Math::getRadianFromDegrees(latitude);
	double _longitude = Math::getRadianFromDegrees(longitude);

	_latitude = -_latitude;
	Vector3lf R;
	R.y = Math::cos(_latitude);
	double xz = Math::sin(_latitude);
	R.x = xz * Math::sin(_longitude);
	R.z = -xz * Math::cos(_longitude);
	R.normalize();
	return R;
}

Vector3lf SphericalGlobe::getNorthPointingTangent(const LatLon& latlon) const
{
	return getNorthPointingTangent(latlon.latitude, latlon.longitude);
}

Vector3lf SphericalGlobe::getNorthPointingTangent(const GeoLocation& location) const
{
	return getNorthPointingTangent(location.latitude, location.longitude);
}

Vector3lf SphericalGlobe::getCartesianPosition(double latitude, double longitude, double altitude) const
{
	double _latitude = Math::getRadianFromDegrees(latitude);
	double _longitude = Math::getRadianFromDegrees(longitude);

	Vector3lf B;
	double R = radius + altitude;
	B.y = Math::sin(_latitude) * R;
	double rxz = Math::cos(_latitude) * R;
	B.x = rxz * Math::sin(_longitude);
	B.z = -rxz * Math::cos(_longitude);
	return B;
}

Vector3lf SphericalGlobe::getCartesianPosition(const LatLon& latlon) const
{
	return getCartesianPosition(latlon.latitude, latlon.longitude, 0);
}

Vector3lf SphericalGlobe::getCartesianPosition(const GeoLocation& location) const
{
	return getCartesianPosition(location.latitude, location.longitude, location.altitude);
}

GeoLocation SphericalGlobe::getGeoLocation(double x, double y, double z) const
{
	double len = Math::sqrt(x * x + y * y + z * z);
	GeoLocation R;
	R.altitude = len - radius;
	if (Math::isLessThanEpsilon(len)) {
		R.longitude = 0;
		R.latitude = 0;
	} else {
		R.latitude = Math::arcsin(y / len) * 180 / SLIB_PI_LONG;
		double rxz = Math::sqrt(x * x + z * z);
		double _v = Math::arcsin(x / rxz) * 180 / SLIB_PI_LONG;
		if (z > 0) {
			_v = 180 - _v;
		}
		R.longitude = _v;
	}
	return R;
}

GeoLocation SphericalGlobe::getGeoLocation(const Vector3lf& position) const
{
	return getGeoLocation(position.x, position.y, position.z);
}


/**************************************************
					Earth
***************************************************/

double Earth::getAverageRadius()
{
	return SLIB_GEO_EARTH_AVERAGE_RADIUS;
}

double Earth::getEquatorialRadius()
{
	return SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84;
}

double Earth::getPolarRadius()
{
	return SLIB_GEO_EARTH_RADIUS_POLAR_WGS84;
}

void Earth::getGlobe(Globe* out)
{
	out->radiusEquatorial = getEquatorialRadius();
	out->radiusPolar = getPolarRadius();
	out->inverseFlattening = SLIB_GEO_EARTH_INVERSE_FLATTENING_WGS84;
	out->eccentricitySquared = SLIB_GEO_EARTH_ECCENTRICITY_SQUARED_WGS84;
}

Globe Earth::getGlobe()
{
	Globe ret;
	getGlobe(&ret);
	return ret;
}

Vector3lf Earth::getSurfaceNormal(double latitude, double longitude)
{
	Globe globe;
	getGlobe(&globe);
	return globe.getSurfaceNormal(latitude, longitude);
}

Vector3lf Earth::getSurfaceNormal(const LatLon& latlon)
{
	return getSurfaceNormal(latlon.latitude, latlon.longitude);
}

Vector3lf Earth::getSurfaceNormal(const GeoLocation& location)
{
	return getSurfaceNormal(location.latitude, location.longitude);
}

Vector3lf Earth::getNorthPointingTangent(double latitude, double longitude)
{
	Globe globe;
	getGlobe(&globe);
	return globe.getNorthPointingTangent(latitude, longitude);
}

Vector3lf Earth::getNorthPointingTangent(const LatLon& latlon)
{
	return getNorthPointingTangent(latlon.latitude, latlon.longitude);
}

Vector3lf Earth::getNorthPointingTangent(const GeoLocation& location)
{
	return getNorthPointingTangent(location.latitude, location.longitude);
}

Vector3lf Earth::getSurfaceNormalAtCartesianPosition(double x, double y, double z)
{
	Globe globe;
	getGlobe(&globe);
	return globe.getSurfaceNormalAtCartesianPosition(x, y, z);
}

Vector3lf Earth::getSurfaceNormalAtCartesianPosition(const Vector3lf& position)
{
	return getSurfaceNormalAtCartesianPosition(position.x, position.y, position.z);
}

Vector3lf Earth::getCartesianPosition(double latitude, double longitude, double altitude)
{
	Globe globe;
	getGlobe(&globe);
	return globe.getCartesianPosition(latitude, longitude, altitude);
}

Vector3lf Earth::getCartesianPosition(const LatLon& latlon)
{
	return getCartesianPosition(latlon.latitude, latlon.longitude, 0);
}

Vector3lf Earth::getCartesianPosition(const GeoLocation& location)
{
	return getCartesianPosition(location.latitude, location.longitude, location.altitude);
}

GeoLocation Earth::getGeoLocation(double x, double y, double z)
{
	Globe globe;
	getGlobe(&globe);
	return globe.getGeoLocation(x, y, z);
}

GeoLocation Earth::getGeoLocation(const Vector3lf& position)
{
	return getGeoLocation(position.x, position.y, position.z);
}

SLIB_MATH_NAMESPACE_END
