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

#ifndef CHECKHEADER_SLIB_GEO_EARTH
#define CHECKHEADER_SLIB_GEO_EARTH

#include "definition.h"

#include "globe.h"

// Based on WGS-84 World Geodetic System (X,Y,Z axes are custom-defined)
#define SLIB_GEO_EARTH_AVERAGE_RADIUS				6371009.0 // unit - m
#define SLIB_GEO_EARTH_RADIUS_EQUATORIAL_WGS84		6378137.0 // unit - m
#define SLIB_GEO_EARTH_RADIUS_POLAR_WGS84			6356752.314245 // unit - m
// first eccentricity squared = 1 - (short_radius / long_radius)^2
#define SLIB_GEO_EARTH_ECCENTRICITY_SQUARED_WGS84	0.00669437999014
// inverse flattening = 1 / (1 - (short_radius / long_radius))
#define SLIB_GEO_EARTH_INVERSE_FLATTENING_WGS84		298.257223563

namespace slib
{
	
	class SLIB_EXPORT Earth
	{
	public:
		static const Globe& getGlobe();

		static double getAverageRadius();
	
		static double getEquatorialRadius();
	
		static double getPolarRadius();
	
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
			(0, 1, 0): direction to north pole
			(1, 0, 0): direction to LatLon(0, 90)
			(0, 0, 1): direction to LatLon(0, 180)
		*/
		static Vector3lf getCartesianPosition(double latitude, double longitude, double altitude);

		static Vector3lf getCartesianPosition(const LatLon& latlon);

		static Vector3lf getCartesianPosition(const GeoLocation& location);

		static GeoLocation getGeoLocation(double x, double y, double z);

		static GeoLocation getGeoLocation(const Vector3lf& position);
	
	};
	
	// spherical earth
	class SLIB_EXPORT SphericalEarth
	{
	public:
		static const SphericalGlobe& getGlobe();
	
		static double getRadius();

		static Vector3lf getSurfaceNormal(double latitude, double longitude);

		static Vector3lf getSurfaceNormal(const LatLon& latlon);

		static Vector3lf getSurfaceNormal(const GeoLocation& location);

		static Vector3lf getNorthPointingTangent(double latitude, double longitude);

		static Vector3lf getNorthPointingTangent(const LatLon& latlon);

		static Vector3lf getNorthPointingTangent(const GeoLocation& location);

		/*
			unit: m
			(0, 0, 0): center
			(0, 1, 0): direction to north pole
			(1, 0, 0): direction to LatLon(0, 90)
			(0, 0, 1): direction to LatLon(0, 180)
		 */
		static Vector3lf getCartesianPosition(double latitude, double longitude, double altitude);

		static Vector3lf getCartesianPosition(const LatLon& latlon);

		static Vector3lf getCartesianPosition(const GeoLocation& location);

		static GeoLocation getGeoLocation(double x, double y, double z);

		static GeoLocation getGeoLocation(const Vector3lf& position);

	};
	
	typedef SphericalEarth RenderEarth;

}

#endif
