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

#include "slib/core/safe_static.h"

namespace slib
{

	TakePhoto::TakePhoto()
	{
		
	}
	
	TakePhoto::~TakePhoto()
	{
		
	}
    
    Ref<Image> TakePhoto::getImage(sl_uint32 width, sl_uint32 height)
    {
        if (image.isNull() && outputFilePath.isNotEmpty()) {
            image = Image::loadFromFile(outputFilePath);
            sl_uint32 photoW = image->getWidth();
            sl_uint32 photoH = image->getHeight();
            sl_real scaleFactor = 1.0;
            if (width > 0 || height > 0) {
                scaleFactor = Math::min((sl_real)photoW / width, (sl_real)photoH / height);
            }
            
            image = image->scale(photoW / scaleFactor, photoH / scaleFactor);
        }
        return image;
    }
    
    String TakePhoto::getFilePath()
    {
        return outputFilePath;
    }
	
	TakePhotoParam::TakePhotoParam()
	{
		
	}
	
	TakePhotoParam::~TakePhotoParam()
	{
		
	}
    
	void Camera::takePhoto(const String &outputFilePath, const Function<void (TakePhoto*)> &onComplete)
	{
		TakePhotoParam param;
		param.outputFilePath = outputFilePath;
		param.onComplete = onComplete;
		takePhoto(param);
	}
	
	void Camera::takePhoto(const Function<void (TakePhoto*)> &onComplete)
	{
		TakePhotoParam param;
		param.onComplete = onComplete;
		takePhoto(param);
	}
	
}
