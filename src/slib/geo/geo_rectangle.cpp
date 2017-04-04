/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
