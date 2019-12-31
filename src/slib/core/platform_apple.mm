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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "slib/core/platform_apple.h"

#include "slib/core/file.h"
#include "slib/core/variant.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
#if defined(SLIB_PLATFORM_IS_IOS)
	namespace priv
	{
		namespace platform
		{
			UIInterfaceOrientation g_screenOrientation = UIInterfaceOrientationPortrait;
		}
	}
#endif

	NSString* Apple::getNSStringFromString(const StringParam& str, NSString* def)
	{
		if (str.isNull()) {
			return def;
		}
		if (str.is8()) {
			StringData s(str);
			NSString* ret = [[NSString alloc] initWithBytes:s.getData() length:s.getLength() encoding:NSUTF8StringEncoding];
			if (ret != nil) {
				return ret;
			}
		} else {
			StringData16 s(str);
			NSString* ret = [[NSString alloc] initWithCharacters:(unichar*)s.getData() length:s.getLength()];
			if (ret != nil) {
				return ret;
			}
		}
		return def;
	}

	String Apple::getStringFromNSString(NSString* str)
	{
		if (str == nil) {
			return sl_null;
		}
		const char* buf = [str UTF8String];
		return String::fromUtf8(buf);
	}

	String16 Apple::getString16FromNSString(NSString* str)
	{
		if (str == nil) {
			return sl_null;
		}
		sl_size len = (sl_size)([str length]);
		String16 ret = String16::allocate(len);
		if (ret.isNull()) {
			return ret;
		}
		NSRange range;
		range.length = len;
		range.location = 0;
		[str getCharacters:((unichar*)(ret.getData())) range:range];
		return ret;
	}

	Time Apple::getTimeFromNSDate(NSDate* date)
	{
		if (date == nil) {
			return Time::zero();
		}
		Time time;
		time.setSecondsCountf([date timeIntervalSince1970]);
		return time;
	}
	
	NSDate* Apple::getNSDateFromTime(const Time& time)
	{
		return [NSDate dateWithTimeIntervalSince1970:(time.getSecondsCountf())];
	}
	
	String Apple::getFilePathFromNSURL(NSURL* url)
	{
		if (url != nil) {
			String path = Apple::getStringFromNSString([url path]);
			if (path.startsWith("file://")) {
				path = path.substring(7);
			}
			return path;
		}
		return sl_null;
	}

	Memory Apple::getMemoryFromNSData(NSData* data)
	{
		if (data != nil) {
			sl_size n = (sl_size)([data length]);
			if (n > 0) {
				Memory mem = Memory::create(n);
				if (mem.isNotNull()) {
					char* p = (char*)(mem.getData());
					[data enumerateByteRangesUsingBlock:^(const void* bytes, NSRange byteRange, BOOL* stop) {
						Base::copyMemory(p + byteRange.location, bytes, byteRange.length);
					}];
					return mem;
				}
			}
		}
		return sl_null;
	}

	NSData* Apple::getNSDataFromMemory(const Memory& memory)
	{
		if(memory.isNotNull()) {
			return [NSData dataWithBytes:memory.getData() length:memory.getSize()];
		}
		return nil;
	}
	
	NSString* Apple::getSystemLocalizedNSString(NSString* str)
	{
#if defined(SLIB_PLATFORM_IS_IOS)
		NSBundle* bundle = [NSBundle bundleForClass:[UIApplication class]];
		if (bundle != nil) {
			return [bundle localizedStringForKey:str value:str table:nil];
		}
#endif
		return str;
	}

	String Apple::getAssetFilePath(const String &path)
	{
		String fileExt = File::getFileExtension(path);
		String fileName = File::getFileNameOnly(path);
		String dirPath = File::getParentDirectoryPath(path);
		
		NSString* strFileName = getNSStringFromString(fileName);
		NSString* strFolderPath = getNSStringFromString(dirPath);
		NSString* strFileExtension = getNSStringFromString(fileExt);
		
		NSString *filePath = [[NSBundle mainBundle] pathForResource:strFileName ofType:strFileExtension inDirectory:strFolderPath];
		return Apple::getStringFromNSString(filePath);
	}
	
	namespace priv
	{
		namespace platform
		{
			
			SLIB_STATIC_ZERO_INITIALIZED(AtomicString, g_systemVersion);
			sl_uint32 g_systemVersionMajor = 0;
			sl_uint32 g_systemVersionMinor = 0;
			sl_bool g_flagInitSystemVersion = sl_true;
			
			void initSystemVersion()
			{
				if (g_flagInitSystemVersion) {
#if defined(SLIB_PLATFORM_IS_MACOS)
					double v = NSAppKitVersionNumber;
					if (v >= NSAppKitVersionNumber10_10) {
						NSOperatingSystemVersion version = [[NSProcessInfo processInfo] operatingSystemVersion];
						g_systemVersionMajor = (sl_uint32)(version.majorVersion);
						g_systemVersionMinor = (sl_uint32)(version.minorVersion);
						g_systemVersion = String::format("%d.%d", g_systemVersionMajor, g_systemVersionMinor);
					} else if (v >= NSAppKitVersionNumber10_9) {
						g_systemVersion = "10.9";
						g_systemVersionMajor = 10;
						g_systemVersionMinor = 9;
					} else if (v >= NSAppKitVersionNumber10_8) {
						g_systemVersion = "10.8";
						g_systemVersionMajor = 10;
						g_systemVersionMinor = 8;
					} else if (v >= NSAppKitVersionNumber10_7) {
						g_systemVersion = "10.7";
						g_systemVersionMajor = 10;
						g_systemVersionMinor = 7;
					} else if (v >= NSAppKitVersionNumber10_6) {
						g_systemVersion = "10.6";
						g_systemVersionMajor = 10;
						g_systemVersionMinor = 6;
					}
#elif defined(SLIB_PLATFORM_IS_IOS)
					NSString* _version = [[UIDevice currentDevice] systemVersion];
					String version = Apple::getStringFromNSString(_version);
					if (version.isNotEmpty()) {
						ListLocker<String> list(version.split("."));
						if (list.count > 0) {
							g_systemVersionMajor = list[0].parseUint32();
							if (list.count > 1) {
								g_systemVersionMinor = list[1].parseUint32();
							}
						}
					}
					g_systemVersion = version;
#endif
					g_flagInitSystemVersion = sl_false;
				}
			}
		}
	}


	String Apple::getSystemVersion()
	{
		priv::platform::initSystemVersion();
		return priv::platform::g_systemVersion;
	}

	sl_uint32 Apple::getSystemMajorVersion()
	{
		priv::platform::initSystemVersion();
		return priv::platform::g_systemVersionMajor;
	}

	sl_uint32 Apple::getSystemMinorVersion()
	{
		priv::platform::initSystemVersion();
		return priv::platform::g_systemVersionMinor;
	}

	String Apple::getMainBundlePath()
	{
		NSString* path = [[NSBundle mainBundle] bundlePath];
		return getStringFromNSString(path);
	}

	String Apple::getSystemLocalizedString(const String& key)
	{
		return getStringFromNSString(getSystemLocalizedNSString(getNSStringFromString(key)));
	}
	
#if defined(SLIB_PLATFORM_IS_MACOS)
	void Apple::setBundleLoginItemEnabled(const String& path, sl_bool flagEnabled)
	{
		if (path.isEmpty()) {
			return;
		}
		
		NSURL *itemURL = [NSURL fileURLWithPath:(Apple::getNSStringFromString(path))];
		LSSharedFileListItemRef existingItem = NULL;
		
		LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
		
		if(loginItems) {
			UInt32 seed = 0U;
			NSArray *currentLoginItems = CFBridgingRelease(LSSharedFileListCopySnapshot(loginItems, &seed));
			for (id itemObject in currentLoginItems) {
				LSSharedFileListItemRef item = (__bridge LSSharedFileListItemRef)itemObject;
				UInt32 resolutionFlags = kLSSharedFileListNoUserInteraction | kLSSharedFileListDoNotMountVolumes;
				CFURLRef URL = NULL;
				OSStatus err = LSSharedFileListItemResolve(item, resolutionFlags, &URL, NULL);
				if (err == noErr) {
					Boolean foundIt = CFEqual(URL, (__bridge CFTypeRef)(itemURL));
					CFRelease(URL);
					if (foundIt) {
						existingItem = item;
						break;
					}
				}
			}
			if (flagEnabled) {
				if (existingItem == NULL) {
					LSSharedFileListInsertItemURL(loginItems, kLSSharedFileListItemBeforeFirst, NULL, NULL, (__bridge CFURLRef)itemURL, NULL, NULL);
				}
			} else {
				if (existingItem != NULL) {
					LSSharedFileListItemRemove(loginItems, existingItem);
				}
			}
			CFRelease(loginItems);
		}
	}
#endif

}

#endif
