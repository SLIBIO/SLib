#ifndef CHECKHEADER_SLIB_CORE_APP
#define CHECKHEADER_SLIB_CORE_APP

#include "definition.h"

#include "object.h"
#include "string.h"

SLIB_NAMESPACE_BEGIN

enum class AppType
{
	UI = 0,
	Service = 1
};

class SLIB_EXPORT Application : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	virtual AppType getAppType() = 0;

	virtual void run(const String& param);
	
	void run();

public:
	static Ref<Application> getApp();
	
	static void setApp(Application* app);
	
	static void clearApp();

	
	static void setEnvironmentPath(const String& key, const String& path);
	
	static String getEnvironmentPath(const String& key);
	
	static String parseEnvironmentPath(const String& path);

	
	static String getAppPath();
	
	static void setAppPath(const String& path);
	
	static String findFileAndSetAppPath(const String& filePath, sl_uint32 nDeep = SLIB_UINT32_MAX);
	
};
SLIB_NAMESPACE_END

#define SLIB_DECLARE_APPLICATION(CLASS) \
	SLIB_DECLARE_OBJECT \
public: \
	static void main(const slib::String& param); \
	static void main(); \
	static slib::Ref<CLASS> getApp();


#define SLIB_DEFINE_APPLICATION(CLASS) \
	SLIB_DEFINE_OBJECT(CLASS, slib::Application) \
	void CLASS::main(const slib::String& param) { \
		slib::Ref<CLASS> app = new CLASS; \
		if (app.isNotNull()) { \
			app->run(param); \
		} \
	} \
	void CLASS::main() { \
		CLASS::main(slib::String::null()); \
	} \
	slib::Ref<CLASS> CLASS::getApp() { \
		slib::Ref<slib::Application> app = slib::Application::getApp(); \
		if (app.isNotNull() && CLASS::checkInstance(app.ptr)) { \
			return slib::Ref<CLASS>::from(app); \
		} \
		return slib::Ref<CLASS>::null(); \
	}


#endif
