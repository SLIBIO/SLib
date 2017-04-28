/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GEO_LATLON
#define CHECKHEADER_SLIB_GEO_LATLON

#include "definition.h"

#include "../core/interpolation.h"

namespace slib
{
	
	class SLIB_EXPORT LatLon
	{
	public:
		double latitude;
		double longitude;

	public:
		constexpr LatLon(): latitude(0), longitude(0) {}

		constexpr LatLon(const LatLon& other): latitude(other.latitude), longitude(other.longitude) {}

		LatLon(double _latitude, double _longitude): latitude(_latitude), longitude(_longitude) {}

	public:
		LatLon& operator=(const LatLon& other);

		sl_bool operator==(const LatLon& other) const;

		sl_bool operator!=(const LatLon& other) const;

	public:
		static LatLon getCenter(const LatLon* list, sl_size count);

		static double normalizeLatitude(double latitude);

		static double normalizeLongitude(double longitude);

		void normalize();

		LatLon lerp(const LatLon& target, float factor) const;

	};
	
	template <>
	SLIB_INLINE LatLon Interpolation<LatLon>::interpolate(const LatLon& a, const LatLon& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#include "detail/latlon.inc"

#endif
