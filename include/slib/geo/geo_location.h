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

#ifndef CHECKHEADER_SLIB_GEO_GEOLOCATION
#define CHECKHEADER_SLIB_GEO_GEOLOCATION

#include "definition.h"

#include "latlon.h"

namespace slib
{
	
	class SLIB_EXPORT GeoLocation
	{
	public:
		double latitude;
		double longitude;
		double altitude; // Unit: m
	
	public:
		constexpr GeoLocation()
		 : latitude(0), longitude(0), altitude(0)
		{}

		constexpr GeoLocation(const GeoLocation& other)
		: latitude(other.latitude), longitude(other.longitude), altitude(other.altitude)
		{}

		constexpr GeoLocation(double _latitude, double _longitude, double _altitude)
		: latitude(_latitude), longitude(_longitude), altitude(_altitude)
		{}

		constexpr GeoLocation(const LatLon& latlon, double _altitude)
		: latitude(latlon.latitude), longitude(latlon.longitude), altitude(_altitude)
		{}

	public:
		GeoLocation& operator=(const GeoLocation& other);

		GeoLocation& operator=(const LatLon& other);

		sl_bool operator==(const GeoLocation& other) const;

		sl_bool operator!=(const GeoLocation& other) const;
	
	public:
		LatLon& getLatLon();

		const LatLon& getLatLon() const;

		void setLatLon(const LatLon& v);

		void setLatLon(double latitude, double longitude);
	
		void normalize();
	
		GeoLocation lerp(const GeoLocation& target, float factor) const;

	};
	
	template <>
	SLIB_INLINE GeoLocation Interpolation<GeoLocation>::interpolate(const GeoLocation& a, const GeoLocation& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#include "detail/geo_location.inc"

#endif
