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

#include "slib/geo/geo_rectangle.h"

#include "slib/core/macro.h"

namespace slib
{

	GeoRectangle::GeoRectangle() = default;

	GeoRectangle::GeoRectangle(const GeoRectangle& other) = default;

	GeoRectangle::GeoRectangle(const LatLon& pt1, const LatLon& pt2)
	{
		bottomLeft.latitude = SLIB_MIN(pt1.latitude, pt2.latitude);
		bottomLeft.longitude = SLIB_MIN(pt1.longitude, pt2.longitude);
		topRight.latitude = SLIB_MAX(pt1.latitude, pt2.latitude);
		topRight.longitude = SLIB_MAX(pt1.longitude, pt2.longitude);
	}

	GeoRectangle& GeoRectangle::operator=(const GeoRectangle& other) = default;

	sl_bool GeoRectangle::contains(const LatLon& pt) const
	{
		return (bottomLeft.latitude <= pt.latitude && bottomLeft.longitude <= pt.longitude
				&& pt.latitude <= topRight.latitude && pt.longitude <= topRight.longitude);
	}

	sl_bool GeoRectangle::isValid() const
	{
		if (bottomLeft.longitude > topRight.longitude || bottomLeft.latitude > topRight.latitude) {
			return sl_false;
		}
		return sl_true;
	}

}
