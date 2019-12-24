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
	
	class SLIB_EXPORT Logger : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Logger();

		~Logger();

	public:
		virtual void log(const StringParam& tag, const StringParam& content) = 0;

		virtual void logError(const StringParam& tag, const StringParam& content);
	
	public:
		static Ref<LoggerSet> global();

		static Ref<Logger> getConsoleLogger();

		static Ref<Logger> createFileLogger(const String& fileNameFormat);

		static void logGlobal(const StringParam& tag, const StringParam& content);

		static void logGlobalError(const StringParam& tag, const StringParam& content);
	
	};
	
	class SLIB_EXPORT FileLogger : public Logger
	{
	public:
		FileLogger();

		FileLogger(const String& fileNameFormat);

		~FileLogger();
	
	public:
		void log(const StringParam& tag, const StringParam& content) override;
	
		virtual String getFileName();
		
	protected:
		String m_fileNameFormat;
		
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

		void setDefaultLogger(const Ref<Logger>& logger);
		

		void clearErrorLogger();

		void addErrorLogger(const Ref<Logger>& logger);

		void removeErrorLogger(const Ref<Logger>& logger);

		void setErrorLogger(const Ref<Logger>& logger);
		
	public:
		void log(const StringParam& tag, const StringParam& content) override;

		void logError(const StringParam& tag, const StringParam& content) override;

	protected:
		CList< Ref<Logger> > m_listLoggers;
		CList< Ref<Logger> > m_listErrorLoggers;
	
	};

	template <class... ARGS>
	void Log(const StringParam& tag, const StringParam& format, ARGS&&... args);
	
	template <class... ARGS>
	void LogError(const StringParam& tag, const StringParam& format, ARGS&&... args);

	
}

#include "detail/log.inc"

#endif
