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

#include "slib/social/instagram.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/core/platform_apple.h"

#include <Photos/Photos.h>

namespace slib
{
	
	void Instagram::openInstagramAppSharingFile(const Memory& mem)
	{
		if (mem.isNull()) {
			return;
		}
		NSData* data = Apple::getNSDataFromMemory(mem);
		NSString *writePath = [NSTemporaryDirectory() stringByAppendingPathComponent:@"instagram.ig"];
		if (![data writeToFile:writePath atomically:YES]) {
			return;
		}
		NSURL* url = [NSURL fileURLWithPath:writePath];
		__block PHAssetChangeRequest* changeRequest = nil;
		__block PHObjectPlaceholder* placeholder;
		[[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{
			NSData* data = [NSData dataWithContentsOfURL:url];
			UIImage *image = [UIImage imageWithData:data];
			changeRequest = [PHAssetChangeRequest creationRequestForAssetFromImage:image];
			placeholder = changeRequest.placeholderForCreatedAsset;
		} completionHandler:^(BOOL success, NSError *error) {
			dispatch_async(dispatch_get_main_queue(), ^{
				if (success) {
					NSURL* instagramURL = [NSURL URLWithString:[NSString stringWithFormat:@"instagram://library?LocalIdentifier=%@", [placeholder localIdentifier]]];
					if ([[UIApplication sharedApplication] canOpenURL:instagramURL]) {
						[[UIApplication sharedApplication] openURL:instagramURL options:@{} completionHandler:NULL];
					}
				}
			});
		}];
	}
	
}

#endif
