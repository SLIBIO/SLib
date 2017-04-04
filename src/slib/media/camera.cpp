/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/camera.h"

namespace slib
{

	CameraInfo::CameraInfo()
	{
	}

	CameraInfo::~CameraInfo()
	{
	}


	CameraParam::CameraParam()
	{
		preferedFrameWidth = 0;
		preferedFrameHeight = 0;
		preferedFrameFormat = BitmapFormat::YUV_NV21;
	}

	CameraParam::~CameraParam()
	{
	}

	void CameraParam::setFrontCamera()
	{
		deviceId = "FRONT";
	}

	sl_bool CameraParam::isFrontCamera()
	{
		return deviceId == "FRONT";
	}

	void CameraParam::setBackCamera()
	{
		deviceId = "BACK";
	}

	sl_bool CameraParam::isBackCamera()
	{
		return deviceId == "BACK";
	}


	SLIB_DEFINE_OBJECT(Camera, VideoCapture)

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

}
