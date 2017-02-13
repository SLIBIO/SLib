#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/platform_apple.h"

#include <mach-o/dyld.h>

SLIB_NAMESPACE_BEGIN

#define _PATH_MAX 1024

String System::getApplicationPath()
{
	char path[_PATH_MAX] = {0};
	char pathResolved[_PATH_MAX] = {0};
	uint32_t size = _PATH_MAX - 1;
	if (_NSGetExecutablePath(path, &size) == 0) {
		realpath(path, pathResolved);
	}
	return pathResolved;
}

String System::getApplicationHomeDirectory()
{
	NSString* path = NSHomeDirectory();
	return Apple::getStringFromNSString(path);
}

String System::getCachesDirectory()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
	if ([paths count] > 0) {
		NSString* path = [paths objectAtIndex:0];
		return Apple::getStringFromNSString(path);
	}
	String dir = getApplicationHomeDirectory() + "/caches";
	File::createDirectory(dir);
	return dir;
}

String System::getTempDirectory()
{
	NSString* path = NSTemporaryDirectory();
	return Apple::getStringFromNSString(path);
}

void _System_setBundleLoginItemEnabled(const String& path, sl_bool flagEnabled)
{
#if defined(SLIB_PLATFORM_IS_MACOS)
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
	
#endif
}

void System::registerApplicationRunAtStartup(const String& path)
{
	_System_setBundleLoginItemEnabled(path, sl_true);
}

void System::registerApplicationRunAtStartup()
{
	_System_setBundleLoginItemEnabled(Apple::getMainBundlePath(), sl_true);
}

void System::unregisterApplicationRunAtStartup(const String& path)
{
	_System_setBundleLoginItemEnabled(path, sl_false);
}

void System::unregisterApplicationRunAtStartup()
{
	_System_setBundleLoginItemEnabled(Apple::getMainBundlePath(), sl_false);
}

SLIB_NAMESPACE_END

#endif
