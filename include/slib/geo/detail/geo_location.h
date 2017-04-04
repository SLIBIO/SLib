/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GEO_DETAIL_GEOLOCATION
#define CHECKHEADER_SLIB_GEO_DETAIL_GEOLOCATION

#include "../latlon.h"

namespace slib
{
	
	SLIB_INLINE GeoLocation& GeoLocation::operator=(const GeoLocation& other)
	{
		latitude = other.latitude;
		longitude = other.longitude;
		altitude = other.altitude;
		return *this;
	}
	
}

#endif
