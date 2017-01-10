#ifndef CHECKHEADER_SLIB_CORE_SYSTEM
#define CHECKHEADER_SLIB_CORE_SYSTEM

#include "definition.h"

#include "string.h"

SLIB_NAMESPACE_BEGIN

typedef void (*SIGNAL_HANDLER)(int signal);

class SLIB_EXPORT System
{
public:
	// System & Application Information
	static String getApplicationPath();
	
	static String getApplicationDirectory();
	
	static String getApplicationHomeDirectory();
	
	static String getCachesDirectory();
	
	static String getTempDirectory();
	
	static String getCurrentDirectory();
	
	static sl_bool setCurrentDirectory(const String& dir);

	
	// Tick count
	static sl_uint32 getTickCount();

	
	// Process & Thread
	static sl_uint32 getProcessId();
	
	static sl_uint32 getThreadId();
	
	static sl_bool createProcess(const String& pathExecutable, const String* command, sl_uint32 nCommands);
	
	static void exec(const String& pathExecutable, const String* command, sl_uint32 nCommands);
	
	static void exit(int code);
	
	static void sleep(sl_uint32 millis);
	
	static void yield();
	
	static void yield(sl_uint32 ellapsed);

	
	static void abort(const String& msg, const String& file, sl_uint32 line);
	
	static void setCrashHandler(SIGNAL_HANDLER handler);
	
};

class SLIB_EXPORT Console
{
public:
	static void print(const String& s);
	
	static void println(const String& s);

	static String readLine();
};

class SLIB_EXPORT GlobalUniqueInstance : public Referable
{
public:
	static Ref<GlobalUniqueInstance> create(const String& name);
	
	static sl_bool exists(const String& name);
	
};

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_SYSTEM

