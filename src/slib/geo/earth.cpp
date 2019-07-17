/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/geo/earth.h"

namespace slib
{

	namespace priv
	{
		namespace earth
		{
			
			struct EarthGlobe
			{
				double radiusEquatorial;
				double radiusPolar;
				
				double inverseFlattening;
				double eccentricitySquared;
			};

			static EarthGlobe g_earthGlobe = {SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84, SLIB_GEO_EARTH_RADIUS_POLAR_WGS84, SLIB_GEO_EARTH_INVERSE_FLATTENING_WGS84, SLIB_GEO_EARTH_ECCENTRICITY_SQUARED_WGS84};
			
			static const Globe& g_globe = *((Globe*)((void*)&g_earthGlobe));


			struct EarthSphericalGlobe
			{
				double radius;
			};

			static EarthSphericalGlobe g_earthSphericalGlobe = {SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84};
			
			static const SphericalGlobe& g_sphericalGlobe = *((SphericalGlobe*)((void*)&g_earthSphericalGlobe));

		}
	}

	using namespace priv::earth;

	const Globe& Earth::getGlobe()
	{
		return g_globe;
	}

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

	Vector3lf Earth::getSurfaceNormal(double latitude, double longitude)
	{
		return g_globe.getSurfaceNormal(latitude, longitude);
	}

	Vector3lf Earth::getSurfaceNormal(const LatLon& latlon)
	{
		return g_globe.getSurfaceNormal(latlon.latitude, latlon.longitude);
	}

	Vector3lf Earth::getSurfaceNormal(const GeoLocation& location)
	{
		return g_globe.getSurfaceNormal(location.latitude, location.longitude);
	}

	Vector3lf Earth::getNorthPointingTangent(double latitude, double longitude)
	{
		return g_globe.getNorthPointingTangent(latitude, longitude);
	}

	Vector3lf Earth::getNorthPointingTangent(const LatLon& latlon)
	{
		return g_globe.getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}

	Vector3lf Earth::getNorthPointingTangent(const GeoLocation& location)
	{
		return g_globe.getNorthPointingTangent(location.latitude, location.longitude);
	}

	Vector3lf Earth::getSurfaceNormalAtCartesianPosition(double x, double y, double z)
	{
		return g_globe.getSurfaceNormalAtCartesianPosition(x, y, z);
	}

	Vector3lf Earth::getSurfaceNormalAtCartesianPosition(const Vector3lf& position)
	{
		return g_globe.getSurfaceNormalAtCartesianPosition(position.x, position.y, position.z);
	}

	Vector3lf Earth::getCartesianPosition(double latitude, double longitude, double altitude)
	{
		return g_globe.getCartesianPosition(latitude, longitude, altitude);
	}

	Vector3lf Earth::getCartesianPosition(const LatLon& latlon)
	{
		return g_globe.getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}

	Vector3lf Earth::getCartesianPosition(const GeoLocation& location)
	{
		return g_globe.getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	GeoLocation Earth::getGeoLocation(double x, double y, double z)
	{
		return g_globe.getGeoLocation(x, y, z);
	}

	GeoLocation Earth::getGeoLocation(const Vector3lf& position)
	{
		return g_globe.getGeoLocation(position.x, position.y, position.z);
	}


	const SphericalGlobe& SphericalEarth::getGlobe()
	{
		return g_sphericalGlobe;
	}

	double SphericalEarth::getRadius()
	{
		return SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84;
	}

	Vector3lf SphericalEarth::getSurfaceNormal(double latitude, double longitude)
	{
		return g_sphericalGlobe.getSurfaceNormal(latitude, longitude);
	}

	Vector3lf SphericalEarth::getSurfaceNormal(const LatLon& latlon)
	{
		return g_sphericalGlobe.getSurfaceNormal(latlon.latitude, latlon.longitude);
	}

	Vector3lf SphericalEarth::getSurfaceNormal(const GeoLocation& location)
	{
		return g_sphericalGlobe.getSurfaceNormal(location.latitude, location.longitude);
	}

	Vector3lf SphericalEarth::getNorthPointingTangent(double latitude, double longitude)
	{
		return g_sphericalGlobe.getNorthPointingTangent(latitude, longitude);
	}

	Vector3lf SphericalEarth::getNorthPointingTangent(const LatLon& latlon)
	{
		return g_sphericalGlobe.getNorthPointingTangent(latlon.latitude, latlon.longitude);
	}

	Vector3lf SphericalEarth::getNorthPointingTangent(const GeoLocation& location)
	{
		return g_sphericalGlobe.getNorthPointingTangent(location.latitude, location.longitude);
	}

	Vector3lf SphericalEarth::getCartesianPosition(double latitude, double longitude, double altitude)
	{
		return g_sphericalGlobe.getCartesianPosition(latitude, longitude, altitude);
	}

	Vector3lf SphericalEarth::getCartesianPosition(const LatLon& latlon)
	{
		return g_sphericalGlobe.getCartesianPosition(latlon.latitude, latlon.longitude, 0);
	}

	Vector3lf SphericalEarth::getCartesianPosition(const GeoLocation& location)
	{
		return g_sphericalGlobe.getCartesianPosition(location.latitude, location.longitude, location.altitude);
	}

	GeoLocation SphericalEarth::getGeoLocation(double x, double y, double z)
	{
		return g_sphericalGlobe.getGeoLocation(x, y, z);
	}

	GeoLocation SphericalEarth::getGeoLocation(const Vector3lf& position)
	{
		return g_sphericalGlobe.getGeoLocation(position.x, position.y, position.z);
	}

}
