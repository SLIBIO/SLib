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
#include "../graphics/image.h"

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
	
    enum class TakePhotoResult
    {
        Successed = 0,
        Failed = 1,
        Canceled = 2
    };
    
    class SLIB_EXPORT TakePhoto
	{
	public:
		String outputFilePath;
		TakePhotoResult result;

	public:
		TakePhoto();
		
		~TakePhoto();
		
	public:
		String getFilePath();
		
        Ref<Image> getImage(sl_uint32 width = 0, sl_uint32 height = 0);

	private:
		Ref<Image> image;
	};
	
	class SLIB_EXPORT TakePhotoParam
	{
	public:
		String outputFilePath;
		Function<void(TakePhoto*)> onComplete;
		
	public:
		TakePhotoParam();
		
		~TakePhotoParam();
		
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
		
		static void takePhoto(const TakePhotoParam& param);
		
		static void takePhoto(const String& outputFilePath, const Function<void(TakePhoto*)>& onComplete);
		
		static void takePhoto(const Function<void(TakePhoto*)>& callback);
		
	};

}

#endif
