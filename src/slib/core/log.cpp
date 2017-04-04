/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/log.h"

#include "slib/core/file.h"
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

	void Console::println(const String& s)
	{
		SLIB_STATIC_STRING(l, "\n");
		print(s + l);
	}

#if defined(SLIB_PLATFORM_IS_MOBILE)
	String Console::readLine()
	{
		return sl_null;
	}
#endif


	SLIB_DEFINE_OBJECT(Logger, Object)

	Logger::Logger()
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::logError(const String& tag, const String& content)
	{
		log(tag, content);
	}

	static String _Log_getLineString(const String& tag, const String& content)
	{
		return String::format("%s [%s] %s", Time::now(), tag, content);
	}

	FileLogger::FileLogger()
	{
	}

	FileLogger::FileLogger(const String& fileName)
	{
		setFileName(fileName);
	}

	FileLogger::~FileLogger()
	{
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
			File::appendAllTextUTF8(fileName, s);
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
			__android_log_print(ANDROID_LOG_INFO, tag.getData(), "%s", content.getData());
#elif defined(SLIB_PLATFORM_IS_TIZEN)
			ObjectLocker lock(this);
			if (content.isNotEmpty()) {
				::dlog_print(DLOG_INFO, tag.getData(), "%s", content.getData());
			} else {
				::dlog_print(DLOG_INFO, tag.getData(), " ");
			}
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
			__android_log_print(ANDROID_LOG_ERROR, tag.getData(), "%s", content.getData());
#elif defined(SLIB_PLATFORM_IS_TIZEN)
			ObjectLocker lock(this);
			if (content.isNotEmpty()) {
				::dlog_print(DLOG_ERROR, tag.getData(), "%s", content.getData());
			} else {
				::dlog_print(DLOG_ERROR, tag.getData(), " ");
			}
#else
			log(tag, content);
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
		m_listLoggers.removeValue(logger);
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
		m_listErrorLoggers.removeValue(logger);
	}


	void LoggerSet::log(const String& tag, const String& content)
	{
		ListLocker< Ref<Logger> > list(m_listLoggers);
		for (sl_size i = 0; i < list.count; i++) {
			list[i]->log(tag, content);
		}
	}

	void LoggerSet::logError(const String& tag, const String& content)
	{
		ListLocker< Ref<Logger> > list(m_listLoggers);
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

	Ref<Logger> Logger::createFileLogger(const String& fileName)
	{
		return new FileLogger(fileName);
	}

	void Logger::logGlobal(const String& tag, const String& content)
	{
		Ref<LoggerSet> log = global();
		if (log.isNotNull()) {
			log->log(tag, content);
		}
	}

	void Logger::logGlobalError(const String& tag, const String& content)
	{
		Ref<LoggerSet> log = global();
		if (log.isNotNull()) {
			log->logError(tag, content);
		}
	}

}

