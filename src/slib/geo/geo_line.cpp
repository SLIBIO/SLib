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

#include "slib/geo/geo_line.h"

namespace slib
{

	GeoLine::GeoLine()
	{
	}

	GeoLine::GeoLine(const GeoLine& other)
	: point1(other.point1), point2(other.point2)
	{
	}

	GeoLine::GeoLine(const LatLon& _point1, const LatLon& _point2)
	: point1(_point1), point2(_point2)
	{
	}

	GeoLine& GeoLine::operator=(const GeoLine& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}

	sl_bool GeoLine::intersectGeoLine(const GeoLine& line2, LatLon* outIntersectPoint) const
	{
		double p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
		const GeoLine& line1 = *this;
		p0_x = line1.point1.longitude;
		p0_y = line1.point1.latitude;
		p1_x = line1.point2.longitude;
		p1_y = line1.point2.latitude;
		p2_x = line2.point1.longitude;
		p2_y = line2.point1.latitude;
		p3_x = line2.point2.longitude;
		p3_y = line2.point2.latitude;

		double s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;
		s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;
		s2_y = p3_y - p2_y;

		double s, t;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
			if (outIntersectPoint) {
				outIntersectPoint->longitude = p0_x + (t * s1_x);
				outIntersectPoint->latitude = p0_y + (t * s1_y);
			}
			return sl_true;
		}
		return sl_false;
	}

}
