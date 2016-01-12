#ifndef CHECKHEADER_SLIB_CORE_APP
#define CHECKHEADER_SLIB_CORE_APP

#include "definition.h"

#include "object.h"
#include "string.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT Application : public Object
{
	SLIB_DECLARE_OBJECT(Application, Object)
public:
	Application();
	
	enum Type
	{
		typeUI = 0,
		typeService = 1
	};
	virtual Type getAppType() = 0;

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
	SLIB_DECLARE_OBJECT(CLASS, slib::Application) \
public: \
	SLIB_INLINE static void main(const slib::String& param) { \
		slib::Ref<CLASS> app = new CLASS; \
		if (app.isNotNull()) { \
			app->run(param); \
		} \
	} \
	SLIB_INLINE static void main() { CLASS::main(slib::String::null()); } \
	SLIB_INLINE static slib::Ref<CLASS> getApp() { \
		slib::Ref<slib::Application> app = slib::Application::getApp(); \
		if (app.isNotNull() && CLASS::checkInstance(app)) { \
			return slib::Ref<CLASS>::from(app); \
		} \
		return slib::Ref<CLASS>::null(); \
	}

#endif
