/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_YUV
#define CHECKHEADER_SLIB_GRAPHICS_YUV

#include "definition.h"

namespace slib
{
	
	class SLIB_EXPORT YUV
	{
	public:
		static void convertRGBToYUV(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8& Y, sl_uint8& U, sl_uint8& V);

		static void convertYUVToRGB(sl_uint8 Y, sl_uint8 U, sl_uint8 V, sl_uint8& R, sl_uint8& G, sl_uint8& B);

	};

}

#endif
