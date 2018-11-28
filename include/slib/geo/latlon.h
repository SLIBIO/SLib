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

#ifndef CHECKHEADER_SLIB_GEO_LATLON
#define CHECKHEADER_SLIB_GEO_LATLON

#include "definition.h"

#include "../core/interpolation.h"

namespace slib
{
	
	class SLIB_EXPORT LatLon
	{
	public:
		double latitude;
		double longitude;

	public:
		constexpr LatLon(): latitude(0), longitude(0) {}

		constexpr LatLon(const LatLon& other): latitude(other.latitude), longitude(other.longitude) {}

		LatLon(double _latitude, double _longitude): latitude(_latitude), longitude(_longitude) {}

	public:
		LatLon& operator=(const LatLon& other);

		sl_bool operator==(const LatLon& other) const;

		sl_bool operator!=(const LatLon& other) const;

	public:
		static LatLon getCenter(const LatLon* list, sl_size count);

		static double normalizeLatitude(double latitude);

		static double normalizeLongitude(double longitude);

		void normalize();

		LatLon lerp(const LatLon& target, float factor) const;

	};
	
	template <>
	SLIB_INLINE LatLon Interpolation<LatLon>::interpolate(const LatLon& a, const LatLon& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#include "detail/latlon.inc"

#endif
