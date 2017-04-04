/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_CAMERA
#define CHECKHEADER_SLIB_MEDIA_CAMERA

#include "definition.h"

#include "video_capture.h"

namespace slib
{
	class SLIB_EXPORT CameraInfo
	{
	public:
		String id;
		String name;
		String description;
		
	public:
		CameraInfo();
		
		~CameraInfo();
		
	};
	
	class SLIB_EXPORT CameraParam : public VideoCaptureParam
	{
	public:
		String deviceId;
		
		sl_uint32 preferedFrameWidth;
		sl_uint32 preferedFrameHeight;
		BitmapFormat preferedFrameFormat;
		
	public:
		CameraParam();
		
		~CameraParam();
		
	public:
		void setFrontCamera();
		
		sl_bool isFrontCamera();
		
		
		void setBackCamera();
		
		sl_bool isBackCamera();
		
	};
	
	class SLIB_EXPORT Camera : public VideoCapture
	{
		SLIB_DECLARE_OBJECT
		
	public:
		Camera();
		
		~Camera();
		
	public:
		static Ref<Camera> create(const CameraParam& param);
		
		static List<CameraInfo> getCamerasList();
		
	};

}

#endif
