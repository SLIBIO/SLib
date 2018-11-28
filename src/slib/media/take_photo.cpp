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
