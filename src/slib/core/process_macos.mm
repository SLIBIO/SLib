/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_MACOS)

#include "slib/core/process.h"

#include "slib/core/app.h"
#include "slib/core/string_buffer.h"
#include "slib/core/platform_apple.h"

namespace slib
{
	namespace priv
	{
		namespace process
		{
			static String FixArgument(const String& arg)
			{
				String s = arg;
				s = s.replaceAll("\\", sl_null);
				s = s.replaceAll("\"", sl_null);
				s = s.replaceAll("'", sl_null);
				if (s.contains(" ") || s.contains("\t") || s.contains("\r") || s.contains("\n")) {
					s = "'" + s + "'";
				}
				if (s.isEmpty()) {
					return "''";
				}
				return s;
			}
			
			static String BuildCommand(const String& pathExecutable, const String* arguments, sl_uint32 nArguments)
			{
				StringBuffer commandLine;
				commandLine.add(FixArgument(pathExecutable));
				if (nArguments > 0) {
					commandLine.addStatic(" ", 1);
				}
				for (sl_size i = 0; i < nArguments; i++) {
					if (i > 0) {
						commandLine.add(" ");
					}
					commandLine.add(FixArgument(arguments[i]));
				}
				return commandLine.merge();

			}
		}
	}

	using namespace priv::process;

	sl_bool Process::run(const String& pathExecutable, const String* strArguments, sl_uint32 nArguments)
	{
		NSMutableArray* arguments = [NSMutableArray array];
		for (sl_uint32 i = 0; i < nArguments; i++) {
			[arguments addObject:(Apple::getNSStringFromString(strArguments[i]))];
		}
		NSTask* task = [NSTask launchedTaskWithLaunchPath:(Apple::getNSStringFromString(pathExecutable)) arguments:arguments];
		if (task != nil) {
			return sl_true;
		}
		return sl_false;
	}

	void Process::runAsAdmin(const String& pathExecutable, const String* strArguments, sl_uint32 nArguments)
	{
		String command = BuildCommand(pathExecutable, strArguments, nArguments);
		String source = "do shell script \"" + command + "\" with administrator privileges";
		NSAppleScript* script = [[NSAppleScript alloc] initWithSource:(Apple::getNSStringFromString(source))];
		[script executeAndReturnError:nil];
	}
	
}

#endif
