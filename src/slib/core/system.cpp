#include "../../../inc/slib/core/system.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/list.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_NAMESPACE_BEGIN

String System::getApplicationDirectory()
{
	String path = getApplicationPath();
#if !defined(SLIB_PLATFORM_IS_ANDROID)
	path = File::getParentDirectoryPath(path);
#endif
	return path;
}

#if !defined(SLIB_PLATFORM_IS_APPLE)
String System::getApplicationHomeDirectory()
{
	return getApplicationDirectory();
}

String System::getCachesDirectory()
{
	String dir = getApplicationDirectory() + "/caches";
	File::createDirectory(dir);
	return dir;
}
#endif

#if defined(SLIB_PLATFORM_IS_MOBILE)

SLIB_SAFE_STATIC_GETTER(CList<String>, _System_getGlobalUniqueInstances)

struct _SYS_GLOBAL_UNIQUE_INSTANCE
{
	String name;
};

void* System::createGlobalUniqueInstance(const String& _name)
{
	String name = _name;
	if (name.isEmpty()) {
		return sl_null;
	}
	name = File::makeSafeFileName(name);
	CList<String>* lst = _System_getGlobalUniqueInstances();
	if (!lst) {
		return sl_null;
	}
	if (lst->indexOf(name) >= 0) {
		return sl_null;
	}
	_SYS_GLOBAL_UNIQUE_INSTANCE* instance = new _SYS_GLOBAL_UNIQUE_INSTANCE();
	instance->name = name;
	lst->add(name);
	return instance;
}

void System::freeGlobalUniqueInstance(void* instance)
{
	if (instance) {
		_SYS_GLOBAL_UNIQUE_INSTANCE* l = (_SYS_GLOBAL_UNIQUE_INSTANCE*)(instance);
		if (l) {
			CList<String>* lst = _System_getGlobalUniqueInstances();
			if (lst) {
				lst->removeValue(l->name);
			}
			delete l;
		}
	}
}
#endif

#if defined(SLIB_PLATFORM_IS_MOBILE)
sl_bool System::createProcess(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
{
	LogError("System::createProcess", "Not supported");
	return sl_false;
}

void System::exec(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
{
	LogError("System::exec", "Not supported");
}

void System::exit(int code)
{
	LogError("System::exit", "Not supported");
}

void System::setCrashHandler(SIGNAL_HANDLER handler)
{
	LogError("System::setCrashHandler", "Not supported");
}
#endif

void System::yield(sl_uint32 ellapsed)
{
	if (ellapsed < 16) {
		return;
	}
	if (ellapsed < 32) {
		System::yield();
	} else {
		System::sleep(1);
	}
}

void Console::println(const String& s)
{
	SLIB_STATIC_STRING(l, "\n");
	print(s + l);
}

#if defined(SLIB_PLATFORM_IS_MOBILE)
String Console::readLine()
{
	return sl_null;
}
#endif

SLIB_NAMESPACE_END
