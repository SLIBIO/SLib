#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/platform_apple.h"

#include <mach-o/dyld.h>

SLIB_NAMESPACE_BEGIN
String System::getApplicationPath()
{
	uint32_t size = 2048;
	char* path = (char*)(Base::createMemory(size + 1));
	if (!path) {
		return String::null();
	}
	char* pathResolved = (char*)(Base::createMemory(size + 1));
	if (!pathResolved) {
		Base::freeMemory(path);
		return String::null();
	}
	pathResolved[0] = 0;
	if (_NSGetExecutablePath(path, &size) == 0) {
		realpath(path, pathResolved);
	}
	String ret = String::fromUtf8(pathResolved);
	Base::freeMemory(path);
	Base::freeMemory(pathResolved);
	return ret;
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
