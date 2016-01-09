#include "../../../inc/slib/core/resource.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/app.h"

#include "../../../inc/slib/core/platform_apple.h"
#include "../../../inc/slib/core/platform_android.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP) || defined(SLIB_PLATFORM_IS_APPLE)

SLIB_NAMESPACE_BEGIN
sl_bool Resource::isBasedOnFileSystem()
{
	return sl_true;
}

#if defined(SLIB_PLATFORM_IS_APPLE)
String Resource::getFilePath(const String& path)
{
	return Apple::getResourceFilePath(path);
}
#else
String Resource::getFilePath(const String& path)
{
	String s = File::makeSafeFilePath(path);
	if (s.isNotEmpty()) {
		return Application::getAppPath() + "/" + s;
	}
	return String::null();
}
#endif

Memory Resource::readAllBytes(const String& path)
{
	Memory ret;
	String s = Resource::getFilePath(path);
	if (s.isNotEmpty()) {
		ret = File::readAllBytes(s);
	}
	return ret;
}
SLIB_NAMESPACE_END

#elif defined(SLIB_PLATFORM_IS_ANDROID)

SLIB_NAMESPACE_BEGIN
sl_bool Resource::isBasedOnFileSystem()
{
	return sl_false;
}

String Resource::getFilePath(const String& path)
{
	return String::null();
}

Memory Resource::readAllBytes(const String& path)
{
	return Android::readAllBytesFromAsset(path);
}
SLIB_NAMESPACE_END

#endif



