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

#ifndef CHECKHEADER_SLIB_CORE_APP
#define CHECKHEADER_SLIB_CORE_APP

#include "definition.h"

#include "object.h"
#include "string.h"

namespace slib
{

	enum class AppType
	{
		UI = 0,
		Service = 1
	};
	
	class GlobalUniqueInstance;
	
	class SLIB_EXPORT Application : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Application();

		~Application();

	public:
		virtual AppType getAppType() = 0;

		String getExecutablePath();
	
		String getCommandLine();

		List<String> getArguments();
	
		String getCommand(sl_uint32 index = 0);

		void run(const String& commandLine);

		void run(int argc, const char * argv[]);
	
		void run();
	
		void dispatchQuitApp();

		sl_bool isUniqueInstanceRunning();

		virtual String getUniqueInstanceId();

		void setUniqueInstanceId(const String& _id);

		virtual sl_bool isCrashRecoverySupport();

		void setCrashRecoverySupport(sl_bool flagSupport);
	
	protected:
		virtual void doRun();
	
		virtual void onRunApp() = 0;

		virtual void onQuitApp();
	
	public:
		static Ref<Application> getApp();

		static void setApp(Application* app);


		static void setEnvironmentPath(const String& key, const String& path);

		static String getEnvironmentPath(const String& key);

		static String parseEnvironmentPath(const String& path);
	

		static String getApplicationPath();

		static String getApplicationDirectory();

		static void setApplicationDirectory(const String& path);

		static String findFileAndSetAppPath(const String& filePath, sl_uint32 nDeep = SLIB_UINT32_MAX);


		static List<String> breakCommandLine(const String& commandLine);

		static String buildCommandLine(const String* argv, sl_size argc);

	protected:
		String m_executablePath;
		String m_commandLine;
		List<String> m_arguments;

		AtomicString m_uniqueInstanceId;
		Ref<GlobalUniqueInstance> m_uniqueInstance;

		sl_bool m_flagCrashRecoverySupport;

	};
	
}


#define SLIB_DECLARE_APPLICATION(CLASS) \
	SLIB_DECLARE_OBJECT \
public: \
	static void main(const slib::String& commandLine); \
	static void main(int argc, const char * argv[]); \
	static void main(int argc, char** argv); \
	static void main(); \
	static slib::Ref<CLASS> getApp();


#define SLIB_DEFINE_APPLICATION(CLASS, BASE) \
	SLIB_DEFINE_OBJECT(CLASS, BASE) \
	void CLASS::main(const slib::String& commandLine) { \
		slib::Ref<CLASS> app = new CLASS; \
		if (app.isNotNull()) { \
			app->run(commandLine); \
		} \
	} \
	void CLASS::main(int argc, const char * argv[]) { \
		slib::Ref<CLASS> app = new CLASS; \
		if (app.isNotNull()) { \
			app->run(argc, argv); \
		} \
	} \
	void CLASS::main(int argc, char** argv) { \
		slib::Ref<CLASS> app = new CLASS; \
		if (app.isNotNull()) { \
			app->run(argc, (const char**)argv); \
		} \
	} \
	void CLASS::main() { \
		slib::Ref<CLASS> app = new CLASS; \
		if (app.isNotNull()) { \
			app->run(); \
		} \
	} \
	slib::Ref<CLASS> CLASS::getApp() { \
		return slib::CastRef<CLASS>(slib::Application::getApp()); \
	}

#endif
