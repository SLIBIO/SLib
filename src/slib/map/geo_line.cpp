/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/map/geo_line.h"

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
