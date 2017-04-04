/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_FRAME
#define CHECKHEADER_SLIB_MEDIA_VIDEO_FRAME

#include "definition.h"

#include "../graphics/color.h"
#include "../graphics/bitmap.h"

namespace slib
{
	class SLIB_EXPORT VideoFrame
	{
	public:
		BitmapData image;
		RotationMode rotation;
		FlipMode flip;
		
	public:
		VideoFrame();
		
		~VideoFrame();
		
	};	
}

#endif
