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

SLIB_NAMESPACE_END

#endif
