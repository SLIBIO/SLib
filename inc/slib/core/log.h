#ifndef CHECKHEADER_SLIB_CORE_LOG
#define CHECKHEADER_SLIB_CORE_LOG

#include "definition.h"

#include "string.h"
#include "object.h"
#include "list.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Logger : public Object
{
public:
	virtual void log(const String& tag, const String& content) = 0;
	
	virtual void logError(const String& tag, const String& content);
	
};

class SLIB_EXPORT FileLogger : public Logger
{
public:
	SLIB_INLINE FileLogger() {}
	
	SLIB_INLINE FileLogger(const String& fileName)
	{
		setFileName(fileName);
	}

public:
	void log(const String& tag, const String& content);
	
protected:
	SLIB_STRING_PROPERTY_INLINE(FileName)

};

class SLIB_EXPORT Log : public Object
{
public:
	SLIB_INLINE Log() {}
	
	Log(const Ref<Logger>& logger, const Ref<Logger>& errorLogger);
	
public:
	static Ref<Log> global();
	
	static Ref<Logger> getConsoleLogger();
	
	static Ref<Logger> createFileLogger(const String& fileName);
	
public:
	void clearDefaultLogger();
	
	void addDefaultLogger(const Ref<Logger>& logger);
	
	void removeDefaultLogger(const Ref<Logger>& logger);

	
	void clearErrorLogger();
	
	void addErrorLogger(const Ref<Logger>& logger);
	
	void removeErrorLogger(const Ref<Logger>& logger);

	
	void log(const String& tag, const String& content);
	
	void logError(const String& tag, const String& content);

	
	static void logGlobal(const String& tag, const String& content);
	
	static void logGlobalError(const String& tag, const String& content);

private:
	CList< Ref<Logger> > m_listLoggers;
	CList< Ref<Logger> > m_listErrorLoggers;
	
};

SLIB_NAMESPACE_END

#define SLIB_LOG(tag, content) slib::Log::logGlobal((tag), (content));
#define SLIB_LOG_ERROR(tag, content) slib::Log::logGlobalError((tag), (content));

#endif
