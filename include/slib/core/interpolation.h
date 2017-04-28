/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_INTERPOLATION
#define CHECKHEADER_SLIB_CORE_INTERPOLATION

#include "definition.h"

namespace slib
{
	
	template <class TYPE>
	class SLIB_EXPORT Interpolation
	{
	public:
		static TYPE interpolate(const TYPE& a, const TYPE& b, float factor);

	};

}

#define SLIB_LERP(a, b, factor) (((a) * (1 - factor)) + ((b) * factor))

#include "detail/interpolation.inc"

#endif
