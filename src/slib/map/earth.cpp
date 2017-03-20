/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/map/earth.h"

namespace slib
{

	struct _EarthGlobe
	{
		double radiusEquatorial;
		double radiusPolar;
		
		double inverseFlattening;
		double eccentricitySquared;
	};
	static _EarthGlobe __earth_globe = {SLIB_MAP_EARTH_RADIUS_EQUATORIAL_WGS84, SLIB_MAP_EARTH_RADIUS_POLAR_WGS84, SLIB_MAP_EARTH_INVERSE_FLATTENING_WGS84, SLIB_MAP_EARTH_ECCENTRICITY_SQUARED_WGS84};
	static const Globe& _earth_globe = *((Globe*)((void*)&__earth_globe));

	const Globe& Earth::getGlobe()
	{
		return _earth_globe;
	}

	double Earth::getAverageRadius()
	{
		return SLIB_MAP_EARTH_AVERAGE_RADIUS;
	}

	double Earth::getEquatorialRadius()
	{
		return SLIB_MAP_EARTH_RADIUS_EQUATORIAL_WGS84;
	}

	double Earth::getPolarRadius()
	{
		return SLIB_MAP_EARTH_RADIUS_POLAR_WGS84;
	}

	Vector3lf Earth::getSurfaceNormal(double latitude, double longitude)
	{
		return _earth_globe.getSurfaceNormal(latitude, longitude);
	}

	Vector3lf Earth::getSurfaceNormal(const LatLon& latlon)
	{
		return _earth_globe.getSurfaceNormal(latlon.latitude, latlon.longitude);
	}

	Vector3lf Earth::getSurfaceNormal(const GeoLocation& location)
	{
		return _earth_globe.getSurfaceNormal(location.latitude, location.longitude);
	}

	Vector3lf Earth::getNorthPointingTangent(double latitude, double longitude)
	{
		return _earth_globe.getNorthPointingTangent(latitude, longitude);
	}

	Vector3lf Earth::getNorthPointingTangent(const LatLon& latlon)
	{
		return _earth_globe.getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}

	Vector3lf Earth::getNorthPointingTangent(const GeoLocation& location)
	{
		return _earth_globe.getNorthPointingTangent(location.latitude, location.longitude);
	}

	Vector3lf Earth::getSurfaceNormalAtCartesianPosition(double x, double y, double z)
	{
		return _earth_globe.getSurfaceNormalAtCartesianPosition(x, y, z);
	}

	Vector3lf Earth::getSurfaceNormalAtCartesianPosition(const Vector3lf& position)
	{
		return _earth_globe.getSurfaceNormalAtCartesianPosition(position.x, position.y, position.z);
	}

	Vector3lf Earth::getCartesianPosition(double latitude, double longitude, double altitude)
	{
		return _earth_globe.getCartesianPosition(latitude, longitude, altitude);
	}

	Vector3lf Earth::getCartesianPosition(const LatLon& latlon)
	{
		return _earth_globe.getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}

	Vector3lf Earth::getCartesianPosition(const GeoLocation& location)
	{
		return _earth_globe.getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	GeoLocation Earth::getGeoLocation(double x, double y, double z)
	{
		return _earth_globe.getGeoLocation(x, y, z);
	}

	GeoLocation Earth::getGeoLocation(const Vector3lf& position)
	{
		return _earth_globe.getGeoLocation(position.x, position.y, position.z);
	}


	struct _EarthSphericalGlobe
	{
		double radius;
	};
	static _EarthSphericalGlobe __earth_spherical_globe = {SLIB_MAP_EARTH_RADIUS_EQUATORIAL_WGS84};
	static const SphericalGlobe& _earth_spherical_globe = *((SphericalGlobe*)((void*)&__earth_spherical_globe));

	const SphericalGlobe& SphericalEarth::getGlobe()
	{
		return _earth_spherical_globe;
	}

	double SphericalEarth::getRadius()
	{
		return SLIB_MAP_EARTH_RADIUS_EQUATORIAL_WGS84;
	}

	Vector3lf SphericalEarth::getSurfaceNormal(double latitude, double longitude)
	{
		return _earth_spherical_globe.getSurfaceNormal(latitude, longitude);
	}

	Vector3lf SphericalEarth::getSurfaceNormal(const LatLon& latlon)
	{
		return _earth_spherical_globe.getSurfaceNormal(latlon.latitude, latlon.longitude);
	}

	Vector3lf SphericalEarth::getSurfaceNormal(const GeoLocation& location)
	{
		return _earth_spherical_globe.getSurfaceNormal(location.latitude, location.longitude);
	}

	Vector3lf SphericalEarth::getNorthPointingTangent(double latitude, double longitude)
	{
		return _earth_spherical_globe.getNorthPointingTangent(latitude, longitude);
	}

	Vector3lf SphericalEarth::getNorthPointingTangent(const LatLon& latlon)
	{
		return _earth_spherical_globe.getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}

	Vector3lf SphericalEarth::getNorthPointingTangent(const GeoLocation& location)
	{
		return _earth_spherical_globe.getNorthPointingTangent(location.latitude, location.longitude);
	}

	Vector3lf SphericalEarth::getCartesianPosition(double latitude, double longitude, double altitude)
	{
		return _earth_spherical_globe.getCartesianPosition(latitude, longitude, altitude);
	}

	Vector3lf SphericalEarth::getCartesianPosition(const LatLon& latlon)
	{
		return _earth_spherical_globe.getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}

	Vector3lf SphericalEarth::getCartesianPosition(const GeoLocation& location)
	{
		return _earth_spherical_globe.getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	GeoLocation SphericalEarth::getGeoLocation(double x, double y, double z)
	{
		return _earth_spherical_globe.getGeoLocation(x, y, z);
	}

	GeoLocation SphericalEarth::getGeoLocation(const Vector3lf& position)
	{
		return _earth_spherical_globe.getGeoLocation(position.x, position.y, position.z);
	}

}
