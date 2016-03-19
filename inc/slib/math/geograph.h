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
	LatLon() = default;

	LatLon(const LatLon& other) = default;
	
	LatLon(double latitude, double longitude);

public:
	static LatLon getCenter(const LatLon* list, sl_size count);
	
	static LatLon getCenter(const List<LatLon>& list);

	static sl_bool checkIntersectionLineToLine(
		LatLon& pointIntersect,
		const LatLon& line1Start, const LatLon& line1End,
		const LatLon& line2Start, const LatLon& line2End);

	
	static double normalizeLatitude(double latitude);
	
	static double normalizeLongitude(double longitude);

	void normalize();

	LatLon lerp(const LatLon& target, sl_real factor) const;
	
	
public:
	LatLon& operator=(const LatLon& other) = default;
	
	sl_bool operator==(const LatLon& other) const;
	
	sl_bool operator!=(const LatLon& other) const;

};

class SLIB_EXPORT GeoLocation
{
public:
	double latitude;
	double longitude;
	double altitude; // Unit: m

public:
	GeoLocation() = default;

	GeoLocation(const GeoLocation& other) = default;
	
	GeoLocation(double latitude, double longitude, double altitude = 0);
	
	GeoLocation(const LatLon& latlon, double altitude = 0);
	
public:
	LatLon getLatLon() const;
	
	void setLatLon(const LatLon& v);
	
	void setLatLon(double latitude, double longitude);

	void normalize();

	GeoLocation lerp(const GeoLocation& target, sl_real factor) const;
	
	
public:
	GeoLocation& operator=(const GeoLocation& other) = default;
	
	GeoLocation& operator=(const LatLon& other);
	
	sl_bool operator==(const GeoLocation& other) const;
	
	sl_bool operator!=(const GeoLocation& other) const;
	
};

class SLIB_EXPORT GeoRectangle
{
public:
	LatLon bottomLeft;
	LatLon topRight;

public:
	GeoRectangle() = default;

	GeoRectangle(const GeoRectangle& other) = default;

	GeoRectangle(const LatLon& pt1, const LatLon& pt2);

public:
	sl_bool contains(LatLon& pt) const;

	sl_bool isValid() const;

public:
	GeoRectangle& operator=(const GeoRectangle& other) = default;
	
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
	Globe() = default;
	
	Globe(const Globe& other) = default;
	
	Globe(double radiusEquatorial, double radiusPolar);

	Globe(double radiusEquatorial, double radiusPolar, double inverseFlattening, double eccentricitySquared);

public:
	double getEquatorialRadius() const;
	
	void setEquatorialRadius(double radius);

	double getPolarRadius() const;

	void setPolarRadius(double radius);

	double getInverseFlattening() const;

	double getEccentricitySquared() const;

	
	Vector3lf getSurfaceNormal(double latitude, double longitude) const;
	
	Vector3lf getSurfaceNormal(const LatLon& latlon) const;
	
	Vector3lf getSurfaceNormal(const GeoLocation& location) const;

	Vector3lf getNorthPointingTangent(double latitude, double longitude) const;
	
	Vector3lf getNorthPointingTangent(const LatLon& latlon) const;
	
	Vector3lf getNorthPointingTangent(const GeoLocation& location) const;
	
	Vector3lf getSurfaceNormalAtCartesianPosition(double x, double y, double z) const;
	
	Vector3lf getSurfaceNormalAtCartesianPosition(const Vector3lf& position) const;
	
	/*
		unit: m
		(0, 0, 0): center
		(0, 1, 0): north pole
		(1, 0, 0): for LatLon(0, 90)
		(0, 0, 1): for LatLon(0, 180)
	*/
	Vector3lf getCartesianPosition(double latitude, double longitude, double altitude) const;
	
	Vector3lf getCartesianPosition(const LatLon& latlon) const;
	
	Vector3lf getCartesianPosition(const GeoLocation& location) const;

	GeoLocation getGeoLocation(double x, double y, double z) const;
	
	GeoLocation getGeoLocation(const Vector3lf& position) const;
	
public:
	Globe& operator=(const Globe& other);

protected:
	void _initializeParameters();
	
};

// spherical globe structure
class SLIB_EXPORT SphericalGlobe
{
public:
	double radius;

public:
	SphericalGlobe() = default;
	
	SphericalGlobe(const SphericalGlobe& other) = default;

	SphericalGlobe(double radius);

public:
	double getRadius() const;

	void setRadius(double radius);

	Vector3lf getSurfaceNormal(double latitude, double longitude) const;
	
	Vector3lf getSurfaceNormal(const LatLon& latlon) const;
	
	Vector3lf getSurfaceNormal(const GeoLocation& location) const;

	Vector3lf getNorthPointingTangent(double latitude, double longitude) const;
	
	Vector3lf getNorthPointingTangent(const LatLon& latlon) const;
	
	Vector3lf getNorthPointingTangent(const GeoLocation& location) const;

	/*
		unit: m
		(0, 0, 0): center
		(0, 1, 0): north pole
		(1, 0, 0): for LatLon(0, 90)
		(0, 0, 1): for LatLon(0, 180)
	*/
	Vector3lf getCartesianPosition(double latitude, double longitude, double altitude) const;
	
	Vector3lf getCartesianPosition(const LatLon& latlon) const;
	
	Vector3lf getCartesianPosition(const GeoLocation& location) const;

	GeoLocation getGeoLocation(double x, double y, double z) const;
	
	GeoLocation getGeoLocation(const Vector3lf& position) const;
	
public:
	SphericalGlobe& operator=(const SphericalGlobe& other) = default;

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
	static double getAverageRadius();

	static double getEquatorialRadius();

	static double getPolarRadius();

	static void getGlobe(Globe* _out);

	static Globe getGlobe();

	static Vector3lf getSurfaceNormal(double latitude, double longitude);
	
	static Vector3lf getSurfaceNormal(const LatLon& latlon);
	
	static Vector3lf getSurfaceNormal(const GeoLocation& location);
	
	static Vector3lf getNorthPointingTangent(double latitude, double longitude);
	
	static Vector3lf getNorthPointingTangent(const LatLon& latlon);
	
	static Vector3lf getNorthPointingTangent(const GeoLocation& location);
	
	static Vector3lf getSurfaceNormalAtCartesianPosition(double x, double y, double z);
	
	static Vector3lf getSurfaceNormalAtCartesianPosition(const Vector3lf& position);

	/*
		unit: m
		(0, 0, 0): center
		(0, 1, 0): north pole
		(1, 0, 0): for LatLon(0, 90)
		(0, 0, 1): for LatLon(0, 180)
	*/
	static Vector3lf getCartesianPosition(double latitude, double longitude, double altitude);
	
	static Vector3lf getCartesianPosition(const LatLon& latlon);
	
	static Vector3lf getCartesianPosition(const GeoLocation& location);
	
	static GeoLocation getGeoLocation(double x, double y, double z);
	
	static GeoLocation getGeoLocation(const Vector3lf& position);

};

SLIB_MATH_NAMESPACE_END

#endif
