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

SLIB_NAMESPACE_END

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
