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

#include "slib/ui/photo.h"

#include "slib/core/file.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TakePhotoResult)
	
	TakePhotoResult::TakePhotoResult()
	 : flagSuccess(sl_false), flagCancel(sl_false)
	{
	}
	
	String TakePhotoResult::getFilePath()
	{
		return filePath;
	}
	
	Memory TakePhotoResult::getFileContent()
	{
		if (fileContent.isNotNull()) {
			return fileContent;
		}
		if (filePath.isNotEmpty()) {
			return File::readAllBytes(filePath);
		}
		return sl_null;
	}
	
	Ref<Drawable> TakePhotoResult::getDrawable()
	{
		if (drawable.isNotNull()) {
			return drawable;
		}
		return PlatformDrawable::loadFromMemory(getFileContent());
	}
	
	Ref<Bitmap> TakePhotoResult::getBitmap()
	{
		if (drawable.isNotNull()) {
			return drawable->toBitmap();
		}
		return Bitmap::loadFromMemory(getFileContent());
	}

	Ref<Image> TakePhotoResult::getImage()
	{
		if (drawable.isNotNull()) {
			return drawable->toImage();
		}
		return Image::loadFromMemory(getFileContent());
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TakePhoto)
	
	TakePhoto::TakePhoto()
	{
	}
	
	void TakePhoto::takeFromCamera(const Function<void(TakePhotoResult&)>& onComplete)
	{
		TakePhoto takePhoto;
		takePhoto.onComplete = onComplete;
		takePhoto.takeFromCamera();
	}

	void TakePhoto::chooseFromLibrary(const Function<void(TakePhotoResult&)>& onComplete)
	{
		TakePhoto takePhoto;
		takePhoto.onComplete = onComplete;
		takePhoto.chooseFromLibrary();
	}
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	void TakePhoto::takeFromCamera()
	{
	}
	
	void TakePhoto::chooseFromLibrary()
	{
	}
#endif
	
	
	SLIB_DEFINE_MEMBER_CLASS_DEFAULT_MEMBERS(PhotoKit, SaveImageParam)
	
	PhotoKit::SaveImageParam::SaveImageParam()
	{
	}

#if !defined(SLIB_PLATFORM_IS_IOS) && !defined(SLIB_PLATFORM_IS_ANDROID)
	void PhotoKit::saveImage(const PhotoKit::SaveImageParam& param)
	{
		param.onComplete(String::null());
	}
#endif
	
	void PhotoKit::saveImage(const Ref<Drawable>& image, const Function<void(const String& localIdentifier)>& onComplete)
	{
		PhotoKit::SaveImageParam param;
		param.image = image;
		param.onComplete = onComplete;
		saveImage(param);
	}
	
	void PhotoKit::saveImageWithFileContent(const Memory& content, const Function<void(const String& localIdentifier)>& onComplete)
	{
		PhotoKit::SaveImageParam param;
		param.content = content;
		param.onComplete = onComplete;
		saveImage(param);
	}

}
