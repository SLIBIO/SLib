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
	virtual void logError(const String& tag, const String& content)
	{
		log(tag, content);
	}
};

class SLIB_EXPORT FileLogger : public Logger
{
protected:
	String m_fileName;

public:
	FileLogger() {}
	FileLogger(const String& fileName)
	{
		m_fileName = fileName;
	}

	void setFileName(const String& fileName)
	{
		m_fileName = fileName;
	}
	void log(const String& tag, const String& content);
};

class SLIB_EXPORT Log : public Object
{
private:
	List< Ref<Logger> > m_listLoggers;
	List< Ref<Logger> > m_listErrorLoggers;

public:
	Log() {}
	Log(Ref<Logger> logger, Ref<Logger> errorLogger);

	void clearDefaultLogger();
	void addDefaultLogger(Ref<Logger> logger);
	void removeDefaultLogger(Ref<Logger> logger);

	void clearErrorLogger();
	void addErrorLogger(Ref<Logger> logger);
	void removeErrorLogger(Ref<Logger> logger);

	void log(const String& tag, const String& content);
	void logError(const String& tag, const String& content);

	static void logGlobal(const String& tag, const String& content);
	static void logGlobalError(const String& tag, const String& content);

public:
	static Ref<Log> global();
	static Ref<Logger> getConsoleLogger();
	static Ref<Logger> createFileLogger(const String& fileName);
};

SLIB_NAMESPACE_END

#define SLIB_LOG(tag, content) slib::Log::logGlobal((tag), (content));
#define SLIB_LOG_ERROR(tag, content) slib::Log::logGlobalError((tag), (content));

#endif
