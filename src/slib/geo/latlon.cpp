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

#include "slib/geo/latlon.h"

#include "slib/core/math.h"

namespace slib
{
	
	sl_bool LatLon::operator==(const LatLon& other) const
	{
		return Math::isAlmostZero(latitude - other.latitude) && Math::isAlmostZero(longitude - other.longitude);
	}
	
	sl_bool LatLon::operator!=(const LatLon& other) const
	{
		return !(Math::isAlmostZero(latitude - other.latitude)) || !(Math::isAlmostZero(longitude - other.longitude));
	}
	
	LatLon LatLon::getCenter(const LatLon* list, sl_size count)
	{
		LatLon ret;
		ret.latitude = 0;
		ret.longitude = 0;

		double signedArea = 0;
		double lat0 = 0;
		double lon0 = 0;
		double lat1 = 0;
		double lon1 = 0;
		double a = 0;

		sl_size i;
		for (i = 0; i < count - 1; i++) {
			lat0 = list[i].latitude;
			lon0 = list[i].longitude;
			lat1 = list[i + 1].latitude;
			lon1 = list[i + 1].longitude;
			a = lat0 * lon1 - lat1 * lon0;
			signedArea += a;
			ret.latitude += (lat0 + lat1) * a;
			ret.longitude += (lon0 + lon1) * a;
		}

		lat0 = list[i].latitude;
		lon0 = list[i].longitude;
		lat1 = list[0].latitude;
		lon1 = list[0].longitude;
		a = lat0 * lon1 - lat1 * lon0;
		signedArea += a;
		if (signedArea == 0 || (ret.latitude == 0 && ret.longitude == 0)) {
			ret.latitude = lat1;
			ret.longitude = lon1;
		}
		else{
			ret.latitude += (lat0 + lat1) * a;
			ret.longitude += (lon0 + lon1) * a;

			signedArea /= 2;
			ret.latitude /= (6 * signedArea);
			ret.longitude /= (6 * signedArea);
		}
		

		return ret;
	}

	double LatLon::normalizeLatitude(double latitude)
	{
		if (latitude < -90) {
			latitude = -90;
		}
		if (latitude > 90) {
			latitude = 90;
		}
		return latitude;
	}

	double LatLon::normalizeLongitude(double longitude)
	{
		longitude = Math::normalizeDegree(longitude + 180) - 180;
		return longitude;
	}

	void LatLon::normalize()
	{
		latitude = normalizeLatitude(latitude);
		longitude = normalizeLongitude(longitude);
	}

	LatLon LatLon::lerp(const LatLon& target, float factor) const
	{
		double tlon = target.longitude;
		double dlon = tlon - longitude;
		if (dlon > 180) {
			tlon -= 360;
		} else if (dlon < -180) {
			tlon += 360;
		}
		LatLon r;
		r.longitude = (tlon - longitude) * factor + longitude;
		r.latitude = (target.latitude - latitude) * factor + latitude;
		return r;
	}

}
