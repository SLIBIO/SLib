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

#include "slib/core/log.h"

#include "slib/core/file.h"
#include "slib/core/console.h"
#include "slib/core/variant.h"
#include "slib/core/safe_static.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)
#include <android/log.h>
#endif

#if defined(SLIB_PLATFORM_IS_TIZEN)
#include <dlog.h>
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(Logger, Object)

	Logger::Logger()
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::logError(const StringParam& tag, const StringParam& content)
	{
		log(tag, content);
	}
	
	namespace priv
	{
		namespace log
		{
			static String getLineString(const StringParam& tag, const StringParam& content)
			{
				return String::format("%s [%s] %s", Time::now(), tag, content);
			}
		}
	}

	FileLogger::FileLogger()
	{
	}

	FileLogger::FileLogger(const String& fileNameFormat)
	{
		m_fileNameFormat = fileNameFormat;
	}

	FileLogger::~FileLogger()
	{
	}

	void FileLogger::log(const StringParam& tag, const StringParam& content)
	{
		String fileName = getFileName();
		if (fileName.isEmpty()) {
			return;
		}
		String s = priv::log::getLineString(tag, content) + "\r\n";
		if (s.getLength() > 0) {
			ObjectLocker lock(this);
			File::appendAllTextUTF8(fileName, s);
		}
	}
	
	String FileLogger::getFileName()
	{
		return String::format(m_fileNameFormat, Time::now());
	}
	
	class ConsoleLogger : public Logger
	{
	public:
		SLIB_INLINE ConsoleLogger() {}

	public:
		void log(const StringParam& _tag, const StringParam& _content) override
		{
#if defined(SLIB_PLATFORM_IS_ANDROID)
			StringCstr tag(_tag);
			StringCstr content(_content);
			ObjectLocker lock(this);
			__android_log_print(ANDROID_LOG_INFO, tag.getData(), "%s", content.getData());
#elif defined(SLIB_PLATFORM_IS_TIZEN)
			StringCstr tag(_tag);
			StringCstr content(_content);
			ObjectLocker lock(this);
			if (content.isNotEmpty()) {
				::dlog_print(DLOG_INFO, tag.getData(), "%s", content.getData());
			} else {
				::dlog_print(DLOG_INFO, tag.getData(), " ");
			}
#else
			String s = priv::log::getLineString(_tag, _content);
			Console::println(s);
#endif
		}

		void logError(const StringParam& _tag, const StringParam& _content) override
		{
#if defined(SLIB_PLATFORM_IS_ANDROID)
			StringCstr tag(_tag);
			StringCstr content(_content);
			ObjectLocker lock(this);
			__android_log_print(ANDROID_LOG_ERROR, tag.getData(), "%s", content.getData());
#elif defined(SLIB_PLATFORM_IS_TIZEN)
			StringCstr tag(_tag);
			StringCstr content(_content);
			ObjectLocker lock(this);
			if (content.isNotEmpty()) {
				::dlog_print(DLOG_ERROR, tag.getData(), "%s", content.getData());
			} else {
				::dlog_print(DLOG_ERROR, tag.getData(), " ");
			}
#else
			log(_tag, _content);
#endif
		}
	};


	LoggerSet::LoggerSet()
	{
	}

	LoggerSet::LoggerSet(const Ref<Logger>& logger, const Ref<Logger>& errorLogger)
	{
		addDefaultLogger(logger);
		addErrorLogger(errorLogger);
	}

	LoggerSet::~LoggerSet()
	{
	}

	void LoggerSet::clearDefaultLogger()
	{
		m_listLoggers.removeAll();
	}

	void LoggerSet::addDefaultLogger(const Ref<Logger>& logger)
	{
		m_listLoggers.add(logger);
	}

	void LoggerSet::removeDefaultLogger(const Ref<Logger>& logger)
	{
		m_listLoggers.remove(logger);
	}

	void LoggerSet::setDefaultLogger(const Ref<Logger>& logger)
	{
		m_listLoggers.removeAll();
		m_listLoggers.add(logger);
	}


	void LoggerSet::clearErrorLogger()
	{
		m_listErrorLoggers.removeAll();
	}

	void LoggerSet::addErrorLogger(const Ref<Logger>& logger)
	{
		m_listErrorLoggers.add(logger);
	}

	void LoggerSet::removeErrorLogger(const Ref<Logger>& logger)
	{
		m_listErrorLoggers.remove(logger);
	}

	void LoggerSet::setErrorLogger(const Ref<Logger>& logger)
	{
		m_listErrorLoggers.removeAll();
		m_listErrorLoggers.add(logger);
	}


	void LoggerSet::log(const StringParam& tag, const StringParam& content)
	{
		ListLocker< Ref<Logger> > list(m_listLoggers);
		for (sl_size i = 0; i < list.count; i++) {
			list[i]->log(tag, content);
		}
	}

	void LoggerSet::logError(const StringParam& tag, const StringParam& content)
	{
		ListLocker< Ref<Logger> > list(m_listErrorLoggers);
		for (sl_size i = 0; i < list.count; i++) {
			list[i]->logError(tag, content);
		}
	}

	Ref<LoggerSet> Logger::global()
	{
		Ref<Logger> console(getConsoleLogger());
		SLIB_SAFE_STATIC(Ref<LoggerSet>, log, new LoggerSet(console, console))
		if (SLIB_SAFE_STATIC_CHECK_FREED(log)) {
			return sl_null;
		}
		return log;
	}

	Ref<Logger> Logger::getConsoleLogger()
	{
		SLIB_SAFE_STATIC(Ref<Logger>, logger, new ConsoleLogger)
		if (SLIB_SAFE_STATIC_CHECK_FREED(logger)) {
			return sl_null;
		}
		return logger;
	}

	Ref<Logger> Logger::createFileLogger(const String& fileNameFormat)
	{
		return new FileLogger(fileNameFormat);
	}

	void Logger::logGlobal(const StringParam& tag, const StringParam& content)
	{
		Ref<LoggerSet> log = global();
		if (log.isNotNull()) {
			log->log(tag, content);
		}
	}

	void Logger::logGlobalError(const StringParam& tag, const StringParam& content)
	{
		Ref<LoggerSet> log = global();
		if (log.isNotNull()) {
			log->logError(tag, content);
		}
	}

}

