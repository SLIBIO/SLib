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

#include "slib/geo/geo_location.h"

#include "slib/core/math.h"

namespace slib
{

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
