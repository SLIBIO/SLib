#ifndef CHECKHEADER_SLIB_CORE_DETAIL_LOG
#define CHECKHEADER_SLIB_CORE_DETAIL_LOG

#include "../log.h"

namespace slib
{

	template <class... ARGS>
	void Console::print(const String& format, ARGS&&... args)
	{
		String content = String::format(format, args...);
		print(content);
	}

	template <class... ARGS>
	void Console::println(const String& format, ARGS&&... args)
	{
		String content = String::format(format, args...);
		println(content);
	}

	template <class... ARGS>
	void Log(const String& tag, const String& format, ARGS&&... args)
	{
		String content = String::format(format, args...);
		Logger::logGlobal(tag, content);
	}
	
	template <class... ARGS>
	void LogError(const String& tag, const String& format, ARGS&&... args)
	{
		String content = String::format(format, args...);
		Logger::logGlobalError(tag, content);
	}
	
}

#endif
