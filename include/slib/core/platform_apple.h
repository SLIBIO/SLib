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

#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_APPLE
#define CHECKHEADER_SLIB_CORE_PLATFORM_APPLE

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_APPLE

#include "string.h"
#include "time.h"

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#ifdef SLIB_PLATFORM_IS_MACOS
#import <Cocoa/Cocoa.h>
#endif
#if defined(SLIB_PLATFORM_IS_IOS)
#include <UIKit/UIKit.h>
#endif
#endif

namespace slib
{

	// specific functions for macOS & iOS
	class SLIB_EXPORT Apple
	{
	public:
#ifdef __OBJC__
		static NSString* getNSStringFromString(const String& str, NSString* def = @"");

		static NSString* getNSStringFromString16(const String16& str, NSString* def = @"");
	
		static String getStringFromNSString(NSString* str);

		static String16 getString16FromNSString(NSString* str);

		static Time getTimeFromNSDate(NSDate* date);
		
		static NSDate* getNSDateFromTime(const Time& time);
		
		static String getFilePathFromNSURL(NSURL* url);

		static Memory getMemoryFromNSData(NSData* data);

		static NSData* getNSDataFromMemory(const Memory& memory);
		
#endif

		static String getAssetFilePath(const String& path);

		static String getSystemVersion();

		static sl_uint32 getSystemMajorVersion();

		static sl_uint32 getSystemMinorVersion();

		static String getMainBundlePath();
	
	};

}

#endif // APPLE

#endif
