/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_APPLE
#define CHECKHEADER_SLIB_CORE_PLATFORM_APPLE

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_APPLE

#include "string.h"

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
