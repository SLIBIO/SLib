/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MAP_GEOLINE
#define CHECKHEADER_SLIB_MAP_GEOLINE

#include "definition.h"

#include "latlon.h"

namespace slib
{
	
	class SLIB_EXPORT GeoLine
	{
	public:
		LatLon point1;
		LatLon point2;

	public:
		GeoLine();

		GeoLine(const GeoLine& other);

		GeoLine(const LatLon& pt1, const LatLon& pt2);

	public:
		GeoLine& operator=(const GeoLine& other);

	public:
		sl_bool intersectGeoLine(const GeoLine& otherLine, LatLon* outIntersectPoint = sl_null) const;

	};

}

#endif
