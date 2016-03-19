#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/system.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)
#include <android/log.h>
#endif

SLIB_NAMESPACE_BEGIN

void Logger::logError(const String& tag, const String& content)
{
	log(tag, content);
}

static String _Log_getLineString(const String& tag, const String& content)
{
	return Time::now().toString() + " " + tag + " - " + content;
}

FileLogger::FileLogger()
{
}

FileLogger::FileLogger(const String& fileName)
{
	setFileName(fileName);
}

void FileLogger::log(const String& tag, const String& content)
{
	String fileName = getFileName();
	if (fileName.isEmpty()) {
		return;
	}
	String s = _Log_getLineString(tag, content) + "\r\n";
	if (s.getLength() > 0) {
		ObjectLocker lock(this);
		File::appendUtf8Text(fileName, s);
	}
}

class ConsoleLogger : public Logger
{
public:
	SLIB_INLINE ConsoleLogger() {}
	
public:
	// override
	void log(const String& tag, const String& content)
	{
#if defined(SLIB_PLATFORM_IS_ANDROID)
		ObjectLocker lock(this);
		__android_log_print(ANDROID_LOG_INFO
							, tag.getData()
							, "%s"
							, content.getData());
#else
		String s = _Log_getLineString(tag, content);
		Console::println(s);
#endif
	}
	
	// override
	void logError(const String& tag, const String& content)
	{
#if defined(SLIB_PLATFORM_IS_ANDROID)
		ObjectLocker lock(this);
		__android_log_print(ANDROID_LOG_ERROR
							, tag.getData()
							, "%s"
							, content.getData());
#else
		log(tag, content);
#endif
	}
};


Log::Log()
{
}

Log::Log(const Ref<Logger>& logger, const Ref<Logger>& errorLogger)
{
	addDefaultLogger(logger);
	addErrorLogger(errorLogger);
}

Ref<Log> Log::global()
{
	Ref<Logger> console(getConsoleLogger());
	SLIB_SAFE_STATIC(Ref<Log>, log, new Log(console, console));
	return log;
}

Ref<Logger> Log::getConsoleLogger()
{
	SLIB_SAFE_STATIC(Ref<Logger>, logger, new ConsoleLogger);
	return logger;
}

Ref<Logger> Log::createFileLogger(const String& fileName)
{
	return new FileLogger(fileName);
}

void Log::clearDefaultLogger()
{
	m_listLoggers.removeAll();
}

void Log::addDefaultLogger(const Ref<Logger>& logger)
{
	m_listLoggers.add(logger);
}

void Log::removeDefaultLogger(const Ref<Logger>& logger)
{
	m_listLoggers.removeValue(logger);
}


void Log::clearErrorLogger()
{
	m_listErrorLoggers.removeAll();
}

void Log::addErrorLogger(const Ref<Logger>& logger)
{
	m_listErrorLoggers.add(logger);
}

void Log::removeErrorLogger(const Ref<Logger>& logger)
{
	m_listErrorLoggers.removeValue(logger);
}


void Log::log(const String& tag, const String& content)
{
	ListLocker< Ref<Logger> > list(m_listLoggers);
	for (sl_size i = 0; i < list.count; i++) {
		list[i]->log(tag, content);
	}
}

void Log::logError(const String& tag, const String& content)
{
	ListLocker< Ref<Logger> > list(m_listLoggers);
	for (sl_size i = 0; i < list.count; i++) {
		list[i]->logError(tag, content);
	}
}

void Log::logGlobal(const String& tag, const String& content)
{
	Ref<Log> log = global();
	if (log.isNotNull()) {
		log->log(tag, content);
	}
}

void Log::logGlobalError(const String& tag, const String& content)
{
	Ref<Log> log = global();
	if (log.isNotNull()) {
		log->logError(tag, content);
	}
}

SLIB_NAMESPACE_END

void sl_log(const char* tag, const char* msg)
{
    SLIB_LOG(tag, msg);
}

void sl_log_error(const char* tag, const char* msg)
{
    SLIB_LOG_ERROR(tag, msg);
}

