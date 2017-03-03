#include "../../../inc/slib/core/asset.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/app.h"

#include "../../../inc/slib/core/platform_apple.h"
#include "../../../inc/slib/core/platform_android.h"
#include "../../../inc/slib/core/platform_tizen.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP) || defined(SLIB_PLATFORM_IS_APPLE) || defined(SLIB_PLATFORM_IS_TIZEN)

namespace slib
{

	sl_bool Assets::isBasedOnFileSystem()
	{
		return sl_true;
	}

#if defined(SLIB_PLATFORM_IS_APPLE)
	String Assets::getFilePath(const String& path)
	{
		return Apple::getAssetFilePath(path);
	}
#elif defined(SLIB_PLATFORM_IS_TIZEN)
	String Assets::getFilePath(const String& path)
	{
		return Tizen::getAssetFilePath(path);
	}
#else
	String Assets::getFilePath(const String& path)
	{
		String s = File::makeSafeFilePath(path);
		if (s.isNotEmpty()) {
			return Application::getApplicationDirectory() + "/" + s;
		}
		return sl_null;
	}
#endif

	Memory Assets::readAllBytes(const String& path)
	{
		Memory ret;
		String s = Assets::getFilePath(path);
		if (s.isNotEmpty()) {
			ret = File::readAllBytes(s);
		}
		return ret;
	}
}

#endif


#if defined(SLIB_PLATFORM_IS_ANDROID)

namespace slib
{
	sl_bool Assets::isBasedOnFileSystem()
	{
		return sl_false;
	}

	String Assets::getFilePath(const String& path)
	{
		return sl_null;
	}

	Memory Assets::readAllBytes(const String& path)
	{
		return Android::readAllBytesFromAsset(path);
	}

}

#endif



