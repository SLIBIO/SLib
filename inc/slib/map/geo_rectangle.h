/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MAP_GEORECTANGLE
#define CHECKHEADER_SLIB_MAP_GEORECTANGLE

#include "definition.h"

#include "latlon.h"

namespace slib
{
	
	class SLIB_EXPORT GeoRectangle
	{
	public:
		LatLon bottomLeft;
		LatLon topRight;

	public:
		GeoRectangle();

		GeoRectangle(const GeoRectangle& other);

		GeoRectangle(const LatLon& pt1, const LatLon& pt2);

	public:
		GeoRectangle& operator=(const GeoRectangle& other);

	public:
		sl_bool contains(const LatLon& pt) const;
	
		sl_bool isValid() const;

	};

}

#endif
