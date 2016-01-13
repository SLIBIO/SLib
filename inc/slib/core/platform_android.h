#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_ANDROID
#define CHECKHEADER_SLIB_CORE_PLATFORM_ANDROID

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_ANDROID

#include "java.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT Android
{
public:
	static void initialize(JavaVM* jvm);

	
	static jobject getCurrentActivity();
	
	static void setCurrentActivity(jobject activity);

	
	static void finishActivity();
	
	static void finishActivity(jobject activity);

	
	static jobject openAssetFile(const String& path);
	
	static Memory readAllBytesFromAsset(const String& path);
	
};
SLIB_NAMESPACE_END

#endif

#endif
