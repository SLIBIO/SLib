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
String Resource::getFilePath(const String& _path)
{
	String path = File::makeSafeFilePath(_path);
	if (path.isNotEmpty()) {
		return Application::getAppPath() + "/" + path;
	}
	return String::null();
}
#endif

Memory Resource::readAllBytes(const String& _path)
{
	String path = Resource::getFilePath(_path);
	Memory ret;
	if (path.isNotEmpty()) {
		ret = File::readAllBytes(path);
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

Memory Resource::readAllBytes(const String& _path)
{
	Memory ret = Android::readAllBytesFromAsset(_path);
	return ret;
}
SLIB_NAMESPACE_END

#endif



