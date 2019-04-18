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

#include "slib/social/instagram.h"

#include "slib/ui/photo.h"
#include "slib/ui/core.h"
#include "slib/core/platform_apple.h"

namespace slib
{
	
	void Instagram::openInstagramAppSharingFile(const Memory& mem)
	{
		PhotoKit::saveImageWithFileContent(mem, [](const String& localIdentifier) {
			if (localIdentifier.isNotNull()) {
				String url = "instagram://library?LocalIdentifier=" + localIdentifier;
				UI::runOnUiThread([url]() {
					NSURL* instagramURL = [NSURL URLWithString:Apple::getNSStringFromString(url)];
					if ([[UIApplication sharedApplication] canOpenURL:instagramURL]) {
						[[UIApplication sharedApplication] openURL:instagramURL options:@{} completionHandler:NULL];
					}
				});
			}
		});
	}
	
}

#endif
