/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_DSHOW
#define CHECKHEADER_SLIB_MEDIA_DSHOW

#include "definition.h"

#include "camera.h"

namespace slib
{
	class SLIB_EXPORT DirectShow
	{
	public:
		static Ref<Camera> createCamera(const CameraParam& param);
		
		static List<CameraInfo> getCamerasList();
		
	};	
}

#endif
