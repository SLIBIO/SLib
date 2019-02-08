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

#ifndef CHECKHEADER_SLIB_GEO_GLOBE
#define CHECKHEADER_SLIB_GEO_GLOBE

#include "definition.h"

#include "geo_location.h"

#include "../math/vector3.h"

namespace slib
{
	
	// ellipsoid globe structure
	class SLIB_EXPORT Globe
	{
	public:
		double radiusEquatorial;
		double radiusPolar;
	
		double inverseFlattening;
		double eccentricitySquared;
	
	public:
		Globe();

		Globe(double radiusEquatorial, double radiusPolar);
	
		Globe(double radiusEquatorial, double radiusPolar, double inverseFlattening, double eccentricitySquared);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(Globe)
		
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
			(0, 1, 0): direction to north pole
			(1, 0, 0): direction to LatLon(0, 90)
			(0, 0, 1): direction to LatLon(0, 180)
		*/
		Vector3lf getCartesianPosition(double latitude, double longitude, double altitude) const;

		Vector3lf getCartesianPosition(const LatLon& latlon) const;

		Vector3lf getCartesianPosition(const GeoLocation& location) const;
	
		GeoLocation getGeoLocation(double x, double y, double z) const;

		GeoLocation getGeoLocation(const Vector3lf& position) const;

	protected:
		void _initializeParameters();

	};
	
	// spherical globe structure
	class SLIB_EXPORT SphericalGlobe
	{
	public:
		double radius;
	
	public:
		SphericalGlobe();

		SphericalGlobe(const SphericalGlobe& other);
	
		SphericalGlobe(double radius);

	public:
		SphericalGlobe& operator=(const SphericalGlobe& other);
	
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
			(0, 1, 0): direction to north pole
			(1, 0, 0): direction to LatLon(0, 90)
			(0, 0, 1): direction to LatLon(0, 180)
		*/
		Vector3lf getCartesianPosition(double latitude, double longitude, double altitude) const;

		Vector3lf getCartesianPosition(const LatLon& latlon) const;

		Vector3lf getCartesianPosition(const GeoLocation& location) const;
	
		GeoLocation getGeoLocation(double x, double y, double z) const;

		GeoLocation getGeoLocation(const Vector3lf& position) const;

	};

}

#endif
