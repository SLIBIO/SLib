#include "../../../inc/slib/core/app.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Application, Object)

SLIB_STATIC_ZERO_INITIALIZED(SafeWeakRef<Application>, _g_app)

Ref<Application> Application::getApp()
{
	if (SLIB_SAFE_STATIC_CHECK_FREED(_g_app)) {
		return Ref<Application>::null();
	}
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

typedef Map<String, String> _Application_EnvList;
SLIB_SAFE_STATIC_GETTER(_Application_EnvList, _Application_getEnvList, _Application_EnvList::createHash())

void Application::setEnvironmentPath(const String& key, const String& path)
{
	Map<String, String>* envMap = _Application_getEnvList();
	if (envMap) {
		envMap->put(key, path);
	}
}

String Application::getEnvironmentPath(const String& key)
{
	Map<String, String>* envMap = _Application_getEnvList();
	if (envMap) {
		return envMap->getValue(key, String::null());
	}
	return String::null();
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

SLIB_SAFE_STATIC_GETTER(String, _Application_getAppPath, System::getApplicationDirectory())

String Application::getAppPath()
{
	String* s = _Application_getAppPath();
	if (!s) {
		return String::null();
	}
	return *s;
}

void Application::setAppPath(const String& path)
{
	String* s = _Application_getAppPath();
	if (s) {
		*s = path;
	}
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
