#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN
static String _Log_getLineString(const String& tag, const String& content)
{
	return Time::now().toString() + " " + tag + " - " + content;
}

void FileLogger::log(const String& tag, const String& content)
{
	if (m_fileName.isEmpty()) {
		return;
	}
	String s = _Log_getLineString(tag, content) + "\r\n";
	if (s.getLength() > 0) {
		ObjectLocker lock(this);
		File::appendUtf8Text(m_fileName, s);
	}
}
SLIB_NAMESPACE_END

SLIB_NAMESPACE_BEGIN
class ConsoleLogger : public Logger
{
public:
	ConsoleLogger() {}
	void log(const String& type, const String& content);
	void logError(const String& type, const String& content);
};
SLIB_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_ANDROID)
#include <android/log.h>
SLIB_NAMESPACE_BEGIN
void ConsoleLogger::log(const String& tag, const String& content)
{
	ObjectLocker lock(this);
	__android_log_print(ANDROID_LOG_INFO
			, tag.getBuf()
			, "%s"
			, content.getBuf());
}
void ConsoleLogger::logError(const String& tag, const String& content)
{
	ObjectLocker lock(this);
	__android_log_print(ANDROID_LOG_ERROR
			, tag.getBuf()
			, "%s"
			, content.getBuf());
}
SLIB_NAMESPACE_END
#else
SLIB_NAMESPACE_BEGIN
void ConsoleLogger::log(const String& tag, const String& content)
{
	String s = _Log_getLineString(tag, content);
	Console::println(s);
}
void ConsoleLogger::logError(const String& tag, const String& content)
{
    log(tag, content);
}
SLIB_NAMESPACE_END
#endif

SLIB_NAMESPACE_BEGIN
Ref<Log> Log::global()
{
	Ref<Logger> console = getConsoleLogger();
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

Log::Log(Ref<Logger> logger, Ref<Logger> errorLogger)
{
	addDefaultLogger(logger);
	addErrorLogger(errorLogger);
}

void Log::clearDefaultLogger()
{
	m_listLoggers.removeAll();
}
void Log::addDefaultLogger(Ref<Logger> logger)
{
	m_listLoggers.add(logger);
}
void Log::removeDefaultLogger(Ref<Logger> logger)
{
	m_listLoggers.removeValue(logger);
}

void Log::clearErrorLogger()
{
	m_listErrorLoggers.removeAll();
}
void Log::addErrorLogger(Ref<Logger> logger)
{
	m_listErrorLoggers.add(logger);
}
void Log::removeErrorLogger(Ref<Logger> logger)
{
	m_listErrorLoggers.removeValue(logger);
}

void Log::log(const String& tag, const String& content)
{
	ListLocker< Ref<Logger> > list(m_listLoggers);
	for (sl_size i = 0; i < list.getCount(); i++) {
		list[i]->log(tag, content);
	}
}

void Log::logError(const String& tag, const String& content)
{
	ListLocker< Ref<Logger> > list(m_listLoggers);
	for (sl_size i = 0; i < list.getCount(); i++) {
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

