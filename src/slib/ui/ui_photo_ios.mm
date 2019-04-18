/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/ui/photo.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"

#include <Photos/Photos.h>

@interface _priv_SLib_TakePictureController : UIImagePickerController<UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
	@public slib::TakePhoto takePhoto;
}
@end

namespace slib
{
	
	class _priv_TakePhotoResult : public TakePhotoResult
	{
	public:
		void setResult(UIImage* image)
		{
			if (image != nil) {
				drawable = GraphicsPlatform::createImageDrawable(image.CGImage);
				if (drawable.isNotNull()) {
					UIImageOrientation orientation = image.imageOrientation;
					if (orientation != UIImageOrientationUp) {
						switch (orientation) {
							case UIImageOrientationDown:
								drawable = drawable->rotate(RotationMode::Rotate180);
								break;
							case UIImageOrientationLeft:
								drawable = drawable->rotate(RotationMode::Rotate270);
								break;
							case UIImageOrientationRight:
								drawable = drawable->rotate(RotationMode::Rotate90);
								break;
							case UIImageOrientationUpMirrored:
								drawable = drawable->flip(FlipMode::Horizontal);
								break;
							case UIImageOrientationDownMirrored:
								drawable = drawable->flip(FlipMode::Vertical);
								break;
							case UIImageOrientationLeftMirrored:
								drawable = drawable->rotate(RotationMode::Rotate270, FlipMode::Horizontal);
								break;
							case UIImageOrientationRightMirrored:
								drawable = drawable->rotate(RotationMode::Rotate90, FlipMode::Horizontal);
								break;
							default:
								break;
						}
						if (drawable.isNull()) {
							return;
						}
					}
					flagSuccess = sl_true;
				}
			}
		}		
	};
	
	static void _priv_TakePhoto_run(TakePhoto& takePhoto, _priv_SLib_TakePictureController* controller)
	{
		controller->takePhoto = takePhoto;
		[controller setDelegate:controller];
		
		UIViewController* rootController = UIPlatform::getCurrentViewController(takePhoto.parent);
		if (rootController != nil) {
			if (![rootController isBeingPresented]) {
				[rootController presentViewController:controller animated:YES completion:nil];
				return;
			}
		}
		TakePhotoResult result;
		takePhoto.onComplete(result);
	}
	
	void TakePhoto::takeFromCamera()
	{
		_priv_SLib_TakePictureController* controller = [[_priv_SLib_TakePictureController alloc] init];
		if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
			[controller setSourceType:UIImagePickerControllerSourceTypeCamera];
			_priv_TakePhoto_run(*this, controller);
		} else {
			TakePhotoResult result;
			onComplete(result);
		}
	}
	
	void TakePhoto::chooseFromLibrary()
	{
		_priv_SLib_TakePictureController* controller = [[_priv_SLib_TakePictureController alloc] init];
		[controller setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
		_priv_TakePhoto_run(*this, controller);
	}

}

@implementation _priv_SLib_TakePictureController

- (void)imagePickerController:(UIImagePickerController*)picker didFinishPickingMediaWithInfo:(NSDictionary*)info
{
	[picker dismissViewControllerAnimated:YES completion:nil];
	UIImage *image = [info objectForKey:UIImagePickerControllerEditedImage];
	if (image == nil) {
		image = [info objectForKey:UIImagePickerControllerOriginalImage];
	}
	slib::_priv_TakePhotoResult result;
	result.setResult(image);
	takePhoto.onComplete(result);
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController*)picker
{
	[picker dismissViewControllerAnimated:YES completion:nil];
	slib::TakePhotoResult result;
	result.flagCancel = sl_true;
	takePhoto.onComplete(result);
}

@end

namespace slib
{
	
	void PhotoKit::saveImage(const PhotoKit::SaveImageParam& param)
	{
		Function<void(const String&)> onComplete = param.onComplete;
		UIImage* image = nil;
		
		if (param.image.isNotNull()) {
			CGImageRef cgImage = GraphicsPlatform::getImageDrawableHandle(param.image.get());
			if (cgImage) {
				image = [UIImage imageWithCGImage:cgImage];
			} else {
				Ref<Image> data = param.image->toImage();
				if (data.isNotNull()) {
					Ref<Drawable> drawable = PlatformDrawable::create(data);
					if (drawable.isNotNull()) {
						cgImage = GraphicsPlatform::getImageDrawableHandle(drawable.get());
						if (cgImage) {
							image = [UIImage imageWithCGImage:cgImage];
						}
					}
				}
			}
		} else if (param.content.isNotNull()) {
			NSData* data = Apple::getNSDataFromMemory(param.content);
			image = [UIImage imageWithData:data];
		}
		
		if (image == nil) {
			onComplete(String::null());
			return;
		}
		
		__block PHAssetChangeRequest* changeRequest = nil;
		__block PHObjectPlaceholder* placeholder;
		[[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{
			changeRequest = [PHAssetChangeRequest creationRequestForAssetFromImage:image];
			placeholder = changeRequest.placeholderForCreatedAsset;
		} completionHandler:^(BOOL success, NSError *error) {
			dispatch_async(dispatch_get_main_queue(), ^{
				if (success) {
					onComplete(Apple::getStringFromNSString([placeholder localIdentifier]));
				} else {
					onComplete(String::null());
				}
			});
		}];
	}
	
}

#endif
