/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
