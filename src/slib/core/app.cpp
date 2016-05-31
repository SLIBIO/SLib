#include "../../../inc/slib/core/app.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Application, Object)

SLIB_SAFE_STATIC_REF(SafeWeakRef<Application>, _g_app);

Ref<Application> Application::getApp()
{
	return _g_app;
}

void Application::setApp(Application* app)
{
	_g_app = app;
}

void Application::run(const String& param)
{
	setApp(this);
}

void Application::run()
{
	run(String::null());
}

static Map<String, String>& _Application_getEnvList()
{
	typedef Map<String, String> _T;
	SLIB_SAFE_STATIC(_T, ret, _T::createHash());
	return ret;
}

void Application::setEnvironmentPath(const String& key, const String& path)
{
	_Application_getEnvList().put(key, path);
}

String Application::getEnvironmentPath(const String& key)
{
	return _Application_getEnvList().getValue(key, String::null());
}

String Application::parseEnvironmentPath(const String& _path)
{
	String path = _path;
	if (path.startsWith('<')) {
		sl_reg index = path.indexOf('>', 1);
		if (index > 2) {
			String key = path.substring(1, index);
			String v;
			if (key == "APP") {
				v = getAppPath();
			} else {
				v = getEnvironmentPath(key);
			}
			path = v + path.substring(index + 1);
		}
	}
	return path;
}

static String& _Application_getAppPath()
{
	SLIB_SAFE_STATIC(String, ret, System::getApplicationDirectory());
	return ret;
}

String Application::getAppPath()
{
	return _Application_getAppPath();
}

void Application::setAppPath(const String& path)
{
	_Application_getAppPath() = path;
}

String Application::findFileAndSetAppPath(const String& filePath, sl_uint32 nDeep)
{
	String appPath = getAppPath();
	String path = File::findParentPathContainingFile(appPath, filePath);
	if (path.isNotNull()) {
		appPath = path;
		setAppPath(path);
	}
	return appPath;
}

SLIB_NAMESPACE_END
