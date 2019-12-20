/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/app.h"

#include "slib/core/hash_map.h"
#include "slib/core/system.h"
#include "slib/core/process.h"
#include "slib/core/file.h"
#include "slib/core/safe_static.h"
#include "slib/core/string_buffer.h"
#include "slib/core/log.h"

#ifdef SLIB_PLATFORM_IS_WIN32
#include <windows.h>
#endif

namespace slib
{

	namespace priv
	{
		namespace app
		{
			
			SLIB_STATIC_ZERO_INITIALIZED(AtomicWeakRef<Application>, g_app)

			typedef HashMap<String, String> EnvironmentList;
			
			SLIB_SAFE_STATIC_GETTER(EnvironmentList, getEnvironmentList, EnvironmentList::create())

			SLIB_SAFE_STATIC_GETTER(String, getAppPath, System::getApplicationPath())
		
			SLIB_SAFE_STATIC_GETTER(String, getAppDir, System::getApplicationDirectory())
			
#if !defined(SLIB_PLATFORM_IS_MOBILE)
			static void CrashHandler(int)
			{
				Ref<Application> app = Application::getApp();
				if (app.isNotNull()) {
					List<String> args = app->getArguments();
					String* s = args.getData();
					sl_uint32 n = (sl_uint32)(args.getCount());
					Process::exec(app->getExecutablePath(), s, n);
				}
			}
#endif
			
		}
	}

	using namespace priv::app;

	SLIB_DEFINE_OBJECT(Application, Object)

	Application::Application()
	{
		m_flagCrashRecoverySupport = sl_false;
	}

	Application::~Application()
	{
	}

	Ref<Application> Application::getApp()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_app)) {
			return sl_null;
		}
		return g_app;
	}

	void Application::setApp(Application* app)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_app)) {
			return;
		}
		g_app = app;
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
		return m_arguments.getValueAt(index);
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
#ifdef SLIB_PLATFORM_IS_WIN32
		String commandLine = GetCommandLineW();
		m_commandLine = commandLine;
		m_arguments = breakCommandLine(commandLine);
#endif
		doRun();
	}

	void Application::doRun()
	{
		Application::setApp(this);
		
		m_executablePath = Application::getApplicationPath();
		
#if !defined(SLIB_PLATFORM_IS_MOBILE)
		String instanceId = getUniqueInstanceId();
		if (instanceId.isNotEmpty()) {
			m_uniqueInstance = GlobalUniqueInstance::create(instanceId);
			if (m_uniqueInstance.isNull()) {
				onExistingInstance();
				return;
			}
		}
		
		if (isCrashRecoverySupport()) {
			System::setCrashHandler(CrashHandler);
		}
		
#endif
		
		onRunApp();
		
		dispatchQuitApp();
		
	}

	void Application::dispatchQuitApp()
	{
		onQuitApp();
#if !defined(SLIB_PLATFORM_IS_MOBILE)
		m_uniqueInstance.setNull();
#endif
	}

	void Application::onQuitApp()
	{
	}

	void Application::onExistingInstance()
	{
		LogError("APP", "%s is ALREADY RUNNING", getUniqueInstanceId());
	}

	sl_bool Application::isUniqueInstanceRunning()
	{
#if !defined(SLIB_PLATFORM_IS_MOBILE)
		String instanceId = getUniqueInstanceId();
		if (instanceId.isNotEmpty()) {
			return GlobalUniqueInstance::exists(instanceId);
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
	
	void Application::setEnvironmentPath(const String& key, const String& path)
	{
		EnvironmentList* envMap = getEnvironmentList();
		if (envMap) {
			envMap->put(key, path);
		}
	}

	String Application::getEnvironmentPath(const String& key)
	{
		EnvironmentList* envMap = getEnvironmentList();
		if (envMap) {
			return envMap->getValue(key, String::null());
		}
		return sl_null;
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

	String Application::getApplicationPath()
	{
		String* s = getAppPath();
		if (!s) {
			return sl_null;
		}
		return *s;
	}

	String Application::getApplicationDirectory()
	{
		String* s = getAppDir();
		if (!s) {
			return sl_null;
		}
		return *s;
	}

	void Application::setApplicationDirectory(const String& path)
	{
		String* s = getAppDir();
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
		for (sl_size i = 0; i < argc; i++) {
			String s = argv[i];
			if (i > 0) {
				commandLine.add(" ");
			}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
			if (s.contains(" ") || s.contains("\t") || s.contains("\r") || s.contains("\n") || s.contains("\"")) {
				commandLine.addStatic("\"");
				ListElements<String> items(s.split("\""));
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
						commandLine.addStatic("\\\"");
					}
				}
				commandLine.addStatic("\"");
			} else {
				if (s.isEmpty()) {
					commandLine.addStatic("\"\"");
				} else {
					commandLine.add(s);
				}
			}
#else
			if (s.contains(" ") || s.contains("\t") || s.contains("\r") || s.contains("\n") || s.contains("\"") || s.contains("\\")) {
				s = s.replaceAll("\\", "\\\\");
				s = s.replaceAll("\"", "\\\"");
				commandLine.addStatic("\"");
				commandLine.add(s);
				commandLine.addStatic("\"");
			} else {
				if (s.isEmpty()) {
					commandLine.addStatic("\"\"");
				} else {
					commandLine.add(s);
				}
			}
#endif
		}
		return commandLine.merge();
	}


#if !defined(SLIB_UI_IS_ANDROID)
	sl_bool Application::checkPermissions(const AppPermissions& permissions)
	{
		return sl_true;
	}
	
	void Application::grantPermissions(const AppPermissions& permissions, const Function<void()>& callback)
	{
		callback();
	}

	sl_bool Application::isRoleHeld(AppRole role)
	{
		return sl_true;
	}

	void Application::requestRole(AppRole role, const Function<void()>& callback)
	{
		callback();
	}

	void Application::openDefaultAppsSetting()
	{
	}

	sl_bool Application::isSupportedDefaultCallingApp()
	{
		return sl_false;
	}

	sl_bool Application::isDefaultCallingApp()
	{
		return sl_false;
	}

	void Application::setDefaultCallingApp(const Function<void()>& callback)
	{
	}

	sl_bool Application::isSystemOverlayEnabled()
	{
		return sl_false;
	}

	void Application::openSystemOverlaySetting()
	{
	}
#endif

}
