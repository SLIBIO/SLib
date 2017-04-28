/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_LOG
#define CHECKHEADER_SLIB_CORE_LOG

#include "definition.h"

#include "string.h"
#include "object.h"
#include "list.h"
#include "variant.h"

namespace slib
{

	class LoggerSet;
	
	class SLIB_EXPORT Console
	{
	public:
		static void print(const String& s);

		template <class... ARGS>
		static void print(const String& format, ARGS&&... args);
	
		static void println(const String& s);

		template <class... ARGS>
		static void println(const String& format, ARGS&&... args);
	
		static String readLine();

	};
	
	class SLIB_EXPORT Logger : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Logger();

		~Logger();

	public:
		virtual void log(const String& tag, const String& content) = 0;

		virtual void logError(const String& tag, const String& content);
	
	public:
		static Ref<LoggerSet> global();

		static Ref<Logger> getConsoleLogger();

		static Ref<Logger> createFileLogger(const String& fileName);

		static void logGlobal(const String& tag, const String& content);

		static void logGlobalError(const String& tag, const String& content);
	
	};
	
	class SLIB_EXPORT FileLogger : public Logger
	{
	public:
		FileLogger();

		FileLogger(const String& fileName);

		~FileLogger();
	
	public:
		// override
		void log(const String& tag, const String& content);
	
	public:
		SLIB_PROPERTY(AtomicString, FileName)
	
	};
	
	class SLIB_EXPORT LoggerSet : public Logger
	{
	public:
		LoggerSet();

		LoggerSet(const Ref<Logger>& logger, const Ref<Logger>& errorLogger);

		~LoggerSet();

	public:
		void clearDefaultLogger();

		void addDefaultLogger(const Ref<Logger>& logger);

		void removeDefaultLogger(const Ref<Logger>& logger);


		void clearErrorLogger();

		void addErrorLogger(const Ref<Logger>& logger);

		void removeErrorLogger(const Ref<Logger>& logger);

	public:
		// override
		void log(const String& tag, const String& content);

		// override
		void logError(const String& tag, const String& content);

	protected:
		CList< Ref<Logger> > m_listLoggers;
		CList< Ref<Logger> > m_listErrorLoggers;
	
	};
	
}

#include "detail/log.inc"

#endif
