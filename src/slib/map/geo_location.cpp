/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/map/geo_location.h"

#include "../../../inc/slib/core/math.h"

namespace slib
{

	GeoLocation& GeoLocation::operator=(const LatLon& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		altitude = 0;
		return *this;
	}

	sl_bool GeoLocation::operator==(const GeoLocation& other) const
	{
		return Math::isAlmostZero(latitude - other.latitude) && Math::isAlmostZero(longitude - other.longitude) && Math::isAlmostZero(altitude - other.altitude);
	}

	sl_bool GeoLocation::operator!=(const GeoLocation& other) const
	{
		return !(Math::isAlmostZero(latitude - other.latitude)) || !(Math::isAlmostZero(longitude - other.longitude)) || !(Math::isAlmostZero(altitude - other.altitude));
	}

	LatLon& GeoLocation::getLatLon()
	{
		return *((LatLon*)((void*)this));
	}

	const LatLon& GeoLocation::getLatLon() const
	{
		return *((LatLon*)((void*)this));
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

	GeoLocation GeoLocation::lerp(const GeoLocation& target, float factor) const
	{	
		return GeoLocation(LatLon(latitude, longitude).lerp(LatLon(target.latitude, target.longitude), factor),
						SLIB_LERP(altitude, target.altitude, factor));
	}

}
