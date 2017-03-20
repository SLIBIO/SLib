/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MAP_GEOLOCATION
#define CHECKHEADER_SLIB_MAP_GEOLOCATION

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

#include "detail/geo_location.h"

#endif
