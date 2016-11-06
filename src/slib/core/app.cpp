#include "../../../inc/slib/core/app.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"

SLIB_NAMESPACE_BEGIN

SLIB_STATIC_ZERO_INITIALIZED(SafeWeakRef<Application>, _g_app)

SLIB_DEFINE_OBJECT(Application, Object)

Application::Application()
{
	m_uniqueInstanceHandle = sl_null;
	m_flagCrashRecoverySupport = sl_false;
}

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

String Application::getExecutablePath()
{
	return m_executablePath;
}

String Application::getCommandLine()
{
	return m_commandLine;
}

List<String> Application::getArguments()
{
	return m_arguments;
}

String Application::getCommand(sl_uint32 index)
{
	return m_arguments.getItemValue(index, String::null());
}

void Application::run(const String& commandLine)
{
	m_commandLine = commandLine;
	m_arguments = breakCommandLine(commandLine);
	doRun();
}

void Application::run(int argc, const char* argv[])
{
	List<String> list;
	for (int i = 0; i < argc; i++) {
		list.add(argv[i]);
	}
	m_arguments = list;
	m_commandLine = buildCommandLine(list.getData(), list.getCount());
	doRun();
}

void Application::run()
{
	doRun();
}

void _app_crash_handler(int n)
{
	Ref<Application> app = Application::getApp();
	if (app.isNotNull()) {
		List<String> args = app->getArguments();
		String* s = args.getData();
		sl_uint32 n = (sl_uint32)(args.getCount());
		if (n > 0) {
			System::exec(app->getExecutablePath(), s + 1, n - 1);
		} else {
			System::exec(app->getExecutablePath(), sl_null, 0);
		}
	}
}

void Application::doRun()
{
	Application::setApp(this);
	
	m_executablePath = Application::getApplicationPath();
	
#if !defined(SLIB_PLATFORM_IS_MOBILE)
	void* instance = sl_null;
	String instanceId = getUniqueInstanceId();
	if (instanceId.isNotEmpty()) {
		for (int i = 0; i < 30; i++) {
			m_uniqueInstanceHandle = System::createGlobalUniqueInstance(instanceId);
			if (m_uniqueInstanceHandle) {
				break;
			}
			System::sleep(100);
		}
		if (!m_uniqueInstanceHandle) {
			SLIB_LOG_ERROR("APP", instanceId + " is ALREADY RUNNING");
			return;
		}
	}
	
	if (isCrashRecoverySupport()) {
		System::setCrashHandler(_app_crash_handler);
	}
	
#endif
	
	onRunApp();
	
	dispatchQuitApp();
	
}

void Application::dispatchQuitApp()
{
	onQuitApp();
#if !defined(SLIB_PLATFORM_IS_MOBILE)
	if (m_uniqueInstanceHandle) {
		System::freeGlobalUniqueInstance(m_uniqueInstanceHandle);
	}
#endif
}

void Application::onQuitApp()
{
}

sl_bool Application::isUniqueInstanceRunning()
{
#if !defined(SLIB_PLATFORM_IS_MOBILE)
	String instanceId = getUniqueInstanceId();
	if (instanceId.isNotEmpty()) {
		void* instance = System::createGlobalUniqueInstance(instanceId);
		if (instance) {
			System::freeGlobalUniqueInstance(instance);
			return sl_false;
		} else {
			return sl_true;
		}
	}
#endif
	return sl_false;
}

String Application::getUniqueInstanceId()
{
	return m_uniqueInstanceId;
}

void Application::setUniqueInstanceId(const String& _id)
{
	m_uniqueInstanceId = _id;
}

sl_bool Application::isCrashRecoverySupport()
{
	return m_flagCrashRecoverySupport;
}

void Application::setCrashRecoverySupport(sl_bool flagSupport)
{
	m_flagCrashRecoverySupport = flagSupport;
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
				v = getApplicationDirectory();
			} else {
				v = getEnvironmentPath(key);
			}
			path = v + path.substring(index + 1);
		}
	}
	return path;
}

SLIB_SAFE_STATIC_GETTER(String, _Application_getAppPath, System::getApplicationPath())

String Application::getApplicationPath()
{
	String* s = _Application_getAppPath();
	if (!s) {
		return String::null();
	}
	return *s;
}

SLIB_SAFE_STATIC_GETTER(String, _Application_getAppDir, System::getApplicationDirectory())

String Application::getApplicationDirectory()
{
	String* s = _Application_getAppDir();
	if (!s) {
		return String::null();
	}
	return *s;
}

void Application::setApplicationDirectory(const String& path)
{
	String* s = _Application_getAppDir();
	if (s) {
		*s = path;
	}
}

String Application::findFileAndSetAppPath(const String& filePath, sl_uint32 nDeep)
{
	String appPath = getApplicationDirectory();
	String path = File::findParentPathContainingFile(appPath, filePath);
	if (path.isNotNull()) {
		appPath = path;
		setApplicationDirectory(path);
	}
	return appPath;
}


/**************************************************************************************
 For breaking command line in Win32 platform,
	refered to https://msdn.microsoft.com/en-us/library/windows/desktop/17w5ykft(v=vs.85).aspx

 Microsoft Specific
	Microsoft C/C++ startup code uses the following rules when interpreting arguments given on the operating system command line:
	* Arguments are delimited by white space, which is either a space or a tab.
	* The caret character (^) is not recognized as an escape character or delimiter. The character is handled completely by the command-line parser in the operating system before being passed to the argv array in the program.
	* A string surrounded by double quotation marks ("string") is interpreted as a single argument, regardless of white space contained within. A quoted string can be embedded in an argument.
	* A double quotation mark preceded by a backslash (\") is interpreted as a literal double quotation mark character (").
	* Backslashes are interpreted literally, unless they immediately precede a double quotation mark.
	* If an even number of backslashes is followed by a double quotation mark, one backslash is placed in the argv array for every pair of backslashes, and the double quotation mark is interpreted as a string delimiter.
	* If an odd number of backslashes is followed by a double quotation mark, one backslash is placed in the argv array for every pair of backslashes, and the double quotation mark is "escaped" by the remaining backslash, causing a literal double quotation mark (") to be placed in argv.
***************************************************************************************/

List<String> Application::breakCommandLine(const String& commandLine)
{
	List<String> ret;
	sl_char8* sz = commandLine.getData();
	sl_size len = commandLine.getLength();
	StringBuffer sb;
	sl_size start = 0;
	sl_size pos = 0;
	sl_bool flagQuote = sl_false;
	while (pos < len) {
		sl_char8 ch = sz[pos];
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		if (ch == '\"') {
			sl_size k = pos - 1;
			while (k > 0) {
				if (sz[k] != '\\') {
					break;
				}
				k--;
			}
			sl_size n = pos - 1 - k;
			sl_size m = n / 2;
			sb.addStatic(sz + start, k + 1 + m - start);
			if (n % 2) {
				start = pos;
				pos++;
				continue;
			} else {
				start = pos + 1;
			}
		}
#else
		if (ch == '\\') {
			if (pos > start) {
				sb.addStatic(sz + start, pos - start);
			}
			start = pos + 1;
			pos++;
			if (pos < len) {
				pos++;
				continue;
			} else {
				break;
			}
		}
#endif

		if (flagQuote) {
			if (ch == '\"') {
				flagQuote = sl_false;
				if (pos > start) {
					sb.addStatic(sz + start, pos - start);
				}
				start = pos + 1;
			}
		} else {
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
				if (pos > start) {
					sb.addStatic(sz + start, pos - start);
				}
				start = pos + 1;
				String s = sb.merge();
				if (s.isNotEmpty()) {
					ret.add(s);
				}
				sb.clear();
			} else if (ch == '\"') {
				flagQuote = sl_true;
				if (pos > start) {
					sb.addStatic(sz + start, pos - start);
				}
				start = pos + 1;
			}
		}
		pos++;
	}
	if (!flagQuote) {
		if (pos > start) {
			sb.addStatic(sz + start, pos - start);
		}
		String s = sb.merge();
		if (s.isNotEmpty()) {
			ret.add(s);
		}
	}
	return ret;
}

String Application::buildCommandLine(const String* argv, sl_size argc)
{
	StringBuffer commandLine;
	for (int i = 0; i < argc; i++) {
		String s = argv[i];
		if (i > 0) {
			commandLine.add(" ");
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		if (s.contains(" ") || s.contains("\t") || s.contains("\r") || s.contains("\n") || s.contains("\"")) {
			commandLine.addStatic("\"", 1);
			ListItems<String> items(s.split("\""));
			for (sl_size k = 0; k < items.count; k++) {
				String t = items[k];
				commandLine.add(t);
				sl_char8* sz = t.getData();
				sl_size len = t.getLength();
				sl_size p = 0;
				for (; p < len; p++) {
					if (sz[len-1-p] != '\\') {
						break;
					}
				}
				commandLine.add(String('\\', p));
				if (k < items.count - 1) {
					commandLine.addStatic("\\\"", 2);
				}
			}
			commandLine.addStatic("\"", 1);
		} else {
			commandLine.add(s);
		}
#else
		if (s.contains(" ") || s.contains("\t") || s.contains("\r") || s.contains("\n") || s.contains("\"") || s.contains("\\")) {
			s = s.replaceAll("\\", "\\\\");
			s = s.replaceAll("\"", "\\\"");
			commandLine.addStatic("\"", 1);
			commandLine.add(s);
			commandLine.addStatic("\"", 1);
		} else {
			commandLine.add(s);
		}
#endif
	}
	return commandLine.merge();
}

SLIB_NAMESPACE_END
