#ifndef CHECKHEADER_SLIB_MATH_GEOGRAPH
#define CHECKHEADER_SLIB_MATH_GEOGRAPH

#include "definition.h"

#include "vector3.h"

#include "../core/list.h"

SLIB_MATH_NAMESPACE_BEGIN

class SLIB_EXPORT LatLon
{
public:
	double latitude;
	double longitude;

public:
	SLIB_INLINE LatLon() {}

	SLIB_INLINE LatLon(const LatLon& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
	}

	SLIB_INLINE LatLon& operator=(const LatLon& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		return *this;
	}

	SLIB_INLINE LatLon(double _latitude, double _longitude)
	{
		latitude = _latitude;
		longitude = _longitude;
	}

	SLIB_INLINE sl_bool operator==(const LatLon& other) const
	{
		return latitude == other.latitude && longitude == other.longitude;
	}

	SLIB_INLINE sl_bool operator!=(const LatLon& other) const
	{
		return !(*this == other);
	}

	static LatLon getCenter(const LatLon* list, sl_size count);
	static LatLon getCenter(const List<LatLon>& list);

	static sl_bool checkIntersectionLineToLine(
		LatLon& pointIntersect
		, const LatLon& line1Start, const LatLon& line1End
		, const LatLon& line2Start, const LatLon& line2End);

	static double normalizeLatitude(double latitude);
	static double normalizeLongitude(double longitude);

	SLIB_INLINE void normalize()
	{
		latitude = normalizeLatitude(latitude);
		longitude = normalizeLongitude(longitude);
	}

	SLIB_INLINE LatLon lerp(const LatLon& target, sl_real factor) const;
};

class SLIB_EXPORT GeoLocation
{
public:
	double latitude;
	double longitude;
	double altitude; // Unit: m

public:
	SLIB_INLINE GeoLocation() {}

	SLIB_INLINE GeoLocation(const GeoLocation& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		altitude = other.altitude;
	}
	SLIB_INLINE GeoLocation& operator=(const GeoLocation& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		altitude = other.altitude;
		return *this;
	}
	SLIB_INLINE GeoLocation& operator=(const LatLon& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		altitude = 0;
		return *this;
	}

	SLIB_INLINE GeoLocation(double _latitude, double _longitude, double _altitude = 0)
	{
		latitude = _latitude;
		longitude = _longitude;
		altitude = _altitude;
	}
	SLIB_INLINE GeoLocation(const LatLon& latlon, double _altitude = 0)
	{
		latitude = latlon.latitude;
		longitude = latlon.longitude;
		altitude = _altitude;
	}

	LatLon getLatLon() const
	{
		return LatLon(latitude, longitude);
	}
	
	void setLatLon(const LatLon& v)
	{
		latitude = v.latitude;
		longitude = v.longitude;
	}
	void setLatLon(double _latitude, double _longitude)
	{
		latitude = _latitude;
		longitude = _longitude;
	}

	SLIB_INLINE sl_bool operator==(const GeoLocation& other) const
	{
		return latitude == other.latitude && longitude == other.longitude && altitude == other.altitude;
	}

	SLIB_INLINE sl_bool operator!=(const GeoLocation& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE void normalize()
	{
		latitude = LatLon::normalizeLatitude(latitude);
		longitude = LatLon::normalizeLongitude(longitude);
	}

	GeoLocation lerp(const GeoLocation& target, sl_real factor) const;
};

class SLIB_EXPORT GeoRectangle
{
public:
	LatLon bottomLeft;
	LatLon topRight;

	SLIB_INLINE GeoRectangle() {}

	SLIB_INLINE GeoRectangle(const GeoRectangle& other)
	{
		bottomLeft = other.bottomLeft;
		topRight = other.topRight;
	}

	SLIB_INLINE GeoRectangle(const LatLon& pt1, const LatLon& pt2)
	{
		bottomLeft.latitude = SLIB_MIN(pt1.latitude, pt2.latitude);
		bottomLeft.longitude = SLIB_MIN(pt1.longitude, pt2.longitude);
		topRight.latitude = SLIB_MAX(pt1.latitude, pt2.latitude);
		topRight.longitude = SLIB_MAX(pt1.longitude, pt2.longitude);
	}

	SLIB_INLINE GeoRectangle& operator=(const GeoRectangle& other)
	{
		bottomLeft = other.bottomLeft;
		topRight = other.topRight;
		return *this;
	}

	SLIB_INLINE sl_bool contains(LatLon& pt) const
	{
		return (bottomLeft.latitude <= pt.latitude && bottomLeft.longitude <= pt.longitude
			&& pt.latitude <= topRight.latitude && pt.longitude <= topRight.longitude);
	}

	SLIB_INLINE sl_bool isValid()
	{
		if (bottomLeft.longitude > topRight.longitude || bottomLeft.latitude > topRight.latitude) {
			return sl_false;
		}
		return sl_true;
	}

	static SLIB_INLINE sl_bool contains(LatLon& pt1, LatLon& pt2, LatLon& position)
	{
		return GeoRectangle(pt1, pt2).contains(position);
	}
};


// ellipsoid globe structure
class SLIB_EXPORT Globe
{
public:
	double radiusEquatorial;
	double radiusPolar;

	double inverseFlattening;
	double eccentricitySquared;

public:
	SLIB_INLINE Globe()
	{
	}

	SLIB_INLINE Globe(double _radiusEquatorial, double _radiusPolar)
	{
		radiusEquatorial = _radiusEquatorial;
		radiusPolar = _radiusPolar;
		_initializeParameters();
	}

	SLIB_INLINE Globe(double _radiusEquatorial, double _radiusPolar, double _inverseFlattening, double _eccentricitySquared)
	{
		radiusEquatorial = _radiusEquatorial;
		radiusPolar = _radiusPolar;

		inverseFlattening = _inverseFlattening;
		eccentricitySquared = _eccentricitySquared;
	}

	SLIB_INLINE Globe(const Globe& other)
	{
		radiusEquatorial = other.radiusEquatorial;
		radiusPolar = other.radiusPolar;

		inverseFlattening = other.inverseFlattening;
		eccentricitySquared = other.eccentricitySquared;
	}

	SLIB_INLINE Globe& operator=(const Globe& other)
	{
		radiusEquatorial = other.radiusEquatorial;
		radiusPolar = other.radiusPolar;

		inverseFlattening = other.inverseFlattening;
		eccentricitySquared = other.eccentricitySquared;

		return *this;
	}

	SLIB_INLINE double getEquatorialRadius() const
	{
		return radiusEquatorial;
	}

	SLIB_INLINE void setEquatorialRadius(double radius)
	{
		radiusEquatorial = radius;
		_initializeParameters();
	}

	SLIB_INLINE double getPolarRadius() const
	{
		return radiusPolar;
	}

	SLIB_INLINE void setPolarRadius(double radius)
	{
		radiusPolar = radius;
		_initializeParameters();
	}

	SLIB_INLINE double getInverseFlattening() const
	{
		return inverseFlattening;
	}

	SLIB_INLINE double getEccentricitySquared() const
	{
		return eccentricitySquared;
	}

	Vector3lf getSurfaceNormal(double latitude, double longitude) const;
	SLIB_INLINE Vector3lf getSurfaceNormal(const LatLon& latlon) const
	{
		return getSurfaceNormal(latlon.latitude, latlon.longitude);
	}
	SLIB_INLINE Vector3lf getSurfaceNormal(const GeoLocation& location) const
	{
		return getSurfaceNormal(location.latitude, location.longitude);
	}

	Vector3lf getNorthPointingTangent(double latitude, double longitude) const;
	SLIB_INLINE Vector3lf getNorthPointingTangent(const LatLon& latlon) const
	{
		return getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}
	SLIB_INLINE Vector3lf getNorthPointingTangent(const GeoLocation& location) const
	{
		return getNorthPointingTangent(location.latitude, location.longitude);
	}

	Vector3lf getSurfaceNormalAtCartesianPosition(double x, double y, double z) const;
	SLIB_INLINE Vector3lf getSurfaceNormalAtCartesianPosition(const Vector3lf& position) const
	{
		return getSurfaceNormalAtCartesianPosition(position.x, position.y, position.z);
	}

	/*
		unit: m
		(0, 0, 0): center
		(0, 1, 0): north pole
		(1, 0, 0): for LatLon(0, 90)
		(0, 0, 1): for LatLon(0, 180)
	*/
	Vector3lf getCartesianPosition(double latitude, double longitude, double altitude) const;
	SLIB_INLINE Vector3lf getCartesianPosition(const LatLon& latlon) const
	{
		return getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}
	SLIB_INLINE Vector3lf getCartesianPosition(const GeoLocation& location) const
	{
		return getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	GeoLocation getGeoLocation(double x, double y, double z) const;
	SLIB_INLINE GeoLocation getGeoLocation(const Vector3lf& position) const
	{
		return getGeoLocation(position.x, position.y, position.z);
	}

protected:
	void _initializeParameters();
};

// spherical globe structure
class SLIB_EXPORT SphericalGlobe
{
public:
	double radius;

public:
	SLIB_INLINE SphericalGlobe()
	{
	}

	SLIB_INLINE SphericalGlobe(double _radius)
	{
		radius = _radius;
	}

	SLIB_INLINE SphericalGlobe(const SphericalGlobe& other)
	{
		radius = other.radius;
	}

	SLIB_INLINE SphericalGlobe& operator=(const SphericalGlobe& other)
	{
		radius = other.radius;
		return *this;
	}

	SLIB_INLINE double getRadius() const
	{
		return radius;
	}

	SLIB_INLINE void setRadius(double _radius)
	{
		radius = _radius;
	}

	Vector3lf getSurfaceNormal(double latitude, double longitude) const;
	SLIB_INLINE Vector3lf getSurfaceNormal(const LatLon& latlon) const
	{
		return getSurfaceNormal(latlon.latitude, latlon.longitude);
	}
	SLIB_INLINE Vector3lf getSurfaceNormal(const GeoLocation& location) const
	{
		return getSurfaceNormal(location.latitude, location.longitude);
	}

	Vector3lf getNorthPointingTangent(double latitude, double longitude) const;
	SLIB_INLINE Vector3lf getNorthPointingTangent(const LatLon& latlon) const
	{
		return getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}
	SLIB_INLINE Vector3lf getNorthPointingTangent(const GeoLocation& location) const
	{
		return getNorthPointingTangent(location.latitude, location.longitude);
	}

	/*
		unit: m
		(0, 0, 0): center
		(0, 1, 0): north pole
		(1, 0, 0): for LatLon(0, 90)
		(0, 0, 1): for LatLon(0, 180)
	*/
	Vector3lf getCartesianPosition(double latitude, double longitude, double altitude) const;
	SLIB_INLINE Vector3lf getCartesianPosition(const LatLon& latlon) const
	{
		return getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}
	SLIB_INLINE Vector3lf getCartesianPosition(const GeoLocation& location) const
	{
		return getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	GeoLocation getGeoLocation(double x, double y, double z) const;
	SLIB_INLINE GeoLocation getGeoLocation(const Vector3lf& position) const
	{
		return getGeoLocation(position.x, position.y, position.z);
	}
};

// Based on WGS-84 World Geodetic System (X,Y,Z axes are custom-defined)
#define SLIB_GEO_EARTH_AVERAGE_RADIUS 6371009.0 // unit - m
#define SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84 6378137.0 // unit - m
#define SLIB_GEO_EARTH_RADIUS_POLAR_WGS84 6356752.314245 // unit - m
// first eccentricity squared = 1 - (short_radius / long_radius)^2
#define SLIB_GEO_EARTH_ECCENTRICITY_SQUARED_WGS84 0.00669437999014
// inverse flattening = 1 / (1 - (short_radius / long_radius))
#define SLIB_GEO_EARTH_INVERSE_FLATTENING_WGS84 298.257223563

class SLIB_EXPORT Earth
{
public:
	SLIB_INLINE static double getAverageRadius()
	{
		return SLIB_GEO_EARTH_AVERAGE_RADIUS;
	}

	SLIB_INLINE static double getEquatorialRadius()
	{
		return SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84;
	}

	SLIB_INLINE static double getPolarRadius()
	{
		return SLIB_GEO_EARTH_RADIUS_POLAR_WGS84;
	}

	SLIB_INLINE static void getGlobe(Globe* out)
	{
		out->radiusEquatorial = getEquatorialRadius();
		out->radiusPolar = getPolarRadius();
		out->inverseFlattening = SLIB_GEO_EARTH_INVERSE_FLATTENING_WGS84;
		out->eccentricitySquared = SLIB_GEO_EARTH_ECCENTRICITY_SQUARED_WGS84;
	}

	SLIB_INLINE static Globe getGlobe()
	{
		Globe ret;
		getGlobe(&ret);
		return ret;
	}

	SLIB_INLINE static Vector3lf getSurfaceNormal(double latitude, double longitude)
	{
		Globe globe;
		getGlobe(&globe);
		return globe.getSurfaceNormal(latitude, longitude);
	}
	SLIB_INLINE static Vector3lf getSurfaceNormal(const LatLon& latlon)
	{
		return getSurfaceNormal(latlon.latitude, latlon.longitude);
	}
	SLIB_INLINE static Vector3lf getSurfaceNormal(const GeoLocation& location)
	{
		return getSurfaceNormal(location.latitude, location.longitude);
	}

	SLIB_INLINE static Vector3lf getNorthPointingTangent(double latitude, double longitude)
	{
		Globe globe;
		getGlobe(&globe);
		return globe.getNorthPointingTangent(latitude, longitude);
	}
	SLIB_INLINE static Vector3lf getNorthPointingTangent(const LatLon& latlon)
	{
		return getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}
	SLIB_INLINE static Vector3lf getNorthPointingTangent(const GeoLocation& location)
	{
		return getNorthPointingTangent(location.latitude, location.longitude);
	}

	SLIB_INLINE static Vector3lf getSurfaceNormalAtCartesianPosition(double x, double y, double z)
	{
		Globe globe;
		getGlobe(&globe);
		return globe.getSurfaceNormalAtCartesianPosition(x, y, z);
	}
	SLIB_INLINE static Vector3lf getSurfaceNormalAtCartesianPosition(const Vector3lf& position)
	{
		return getSurfaceNormalAtCartesianPosition(position.x, position.y, position.z);
	}

	/*
		unit: m
		(0, 0, 0): center
		(0, 1, 0): north pole
		(1, 0, 0): for LatLon(0, 90)
		(0, 0, 1): for LatLon(0, 180)
	*/
	SLIB_INLINE static Vector3lf getCartesianPosition(double latitude, double longitude, double altitude)
	{
		Globe globe;
		getGlobe(&globe);
		return globe.getCartesianPosition(latitude, longitude, altitude);
	}
	SLIB_INLINE static Vector3lf getCartesianPosition(const LatLon& latlon)
	{
		return getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}
	SLIB_INLINE static Vector3lf getCartesianPosition(const GeoLocation& location)
	{
		return getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	SLIB_INLINE static GeoLocation getGeoLocation(double x, double y, double z)
	{
		Globe globe;
		getGlobe(&globe);
		return globe.getGeoLocation(x, y, z);
	}
	SLIB_INLINE static GeoLocation getGeoLocation(const Vector3lf& position)
	{
		return getGeoLocation(position.x, position.y, position.z);
	}

};

SLIB_MATH_NAMESPACE_END

#endif
