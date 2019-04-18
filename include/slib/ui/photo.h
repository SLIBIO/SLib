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

#ifndef CHECKHEADER_SLIB_UI_PHOTO
#define CHECKHEADER_SLIB_UI_PHOTO

#include "definition.h"

#include "constants.h"
#include "window.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/function.h"
#include "../graphics/image.h"

namespace slib
{

	class SLIB_EXPORT TakePhotoResult
	{
	public:
		sl_bool flagSuccess;
		sl_bool flagCancel;

	public:
		TakePhotoResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TakePhotoResult)
		
	public:
		String getFilePath();
		
		Memory getFileContent();
		
		Ref<Drawable> getDrawable();
		
		Ref<Bitmap> getBitmap();
		
		Ref<Image> getImage();
		
	protected:
		String filePath;
		Memory fileContent;
		Ref<Drawable> drawable;
		
	};
	
	class SLIB_EXPORT TakePhoto
	{
	public:
		Ref<Window> parent;
		String outputFilePath;
		Function<void(TakePhotoResult&)> onComplete;
		
	public:
		TakePhoto();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TakePhoto)
		
	public:
		void takeFromCamera();
		
		static void takeFromCamera(const Function<void(TakePhotoResult&)>& onComplete);
		
		void chooseFromLibrary();

		static void chooseFromLibrary(const Function<void(TakePhotoResult&)>& onComplete);
		
	};
	
	class SLIB_EXPORT PhotoKit
	{
	public:
		class SaveImageParam
		{
		public:
			Memory content;
			Ref<Drawable> image;
			
			Function<void(const String& localIdentifier)> onComplete;
			
		public:
			SaveImageParam();
			
			SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(SaveImageParam)
			
		};
		
		static void saveImage(const SaveImageParam& param);

		static void saveImage(const Ref<Drawable>& image, const Function<void(const String& localIdentifier)>& onComplete);
		
		static void saveImageWithFileContent(const Memory& content, const Function<void(const String& localIdentifier)>& onComplete);
		
	};
	
}

#endif
