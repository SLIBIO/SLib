#ifndef CHECKHEADER_SLIB_CORE_SYSTEM
#define CHECKHEADER_SLIB_CORE_SYSTEM

#include "definition.h"

#include "string.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT System
{
public:
	// Generic Information
	static sl_bool isMobilePlatform();

	// System & Application Information
	static String getApplicationPath();
	static String getApplicationDirectory();
	static String getApplicationHomeDirectory();
	static String getCachesDirectory();
	static String getTempDirectory();

	// Tick count
	static sl_uint32 getTickCount();

	// Inter-process Synchronization
	static void* createGlobalUniqueInstance(const String& name);
	static void freeGlobalUniqueInstance(void* instance);

	// Process & Thread
	static sl_uint32 getProcessId();
	static sl_uint32 getThreadId();
	static sl_bool createProcess(const String& pathExecutable, const String* commandLines, sl_uint32 nCommandLines);
	static void sleep(sl_uint32 millis);
	static void yield();
	static void yield(sl_uint32 ellapsed);

	static void abort(const String& msg, const String& file, sl_uint32 line);
};

class SLIB_EXPORT Console
{
public:
	static void print(const String& s);
	static void println(const String& s);

	static String readLine();
};

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_SYSTEM

