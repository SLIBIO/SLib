#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_APPLE
#define CHECKHEADER_SLIB_CORE_PLATFORM_APPLE

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_APPLE

#include "string.h"

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#ifdef SLIB_PLATFORM_IS_OSX
#import <Cocoa/Cocoa.h>
#endif
#if defined(SLIB_PLATFORM_IS_IOS)
#include <UIKit/UIKit.h>
#endif
#endif

SLIB_NAMESPACE_BEGIN

// specific functions for OSX & iOS
class SLIB_EXPORT Apple
{
public:
#ifdef __OBJC__
	static NSString* getNSStringFromString(const String& str, NSString* def = @"");
	
	static NSString* getNSStringFromString16(const String16& str, NSString* def = @"");

	static String8 getStringFromNSString(NSString* str);
	
	static String16 getString16FromNSString(NSString* str);
	
	static String getFilePathFromNSURL(NSURL* url);
	
	static Memory getMemoryFromNSData(NSData* data);
	
	static NSData* getNSDataFromMemory(const Memory& memory);
	
#endif
	
	static String getAssetFilePath(const String& path);
	
	static String getSystemVersion();
	
	static sl_uint32 getSystemMajorVersion();
	
	static sl_uint32 getSystemMinorVersion();

};

SLIB_NAMESPACE_END

#endif // APPLE

#endif
