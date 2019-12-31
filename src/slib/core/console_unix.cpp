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

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "slib/core/console.h"

#include "slib/core/log.h"

#include <stdio.h>
#include <stdlib.h>

#if !defined(SLIB_PLATFORM_IS_MOBILE)
#	include <termios.h>
#endif

namespace slib
{

	void Console::print(const StringParam& s)
	{
#if defined(SLIB_PLATFORM_IS_ANDROID) || defined(SLIB_PLATFORM_IS_TIZEN)
		SLIB_STATIC_STRING(c, "Console");
		Logger::getConsoleLogger()->log(c, s);
#else
		StringCstr _s(s);
		printf("%s", _s.getData());
#endif
	}
	
#if !defined(SLIB_PLATFORM_IS_MOBILE)
	String Console::readLine()
	{
		String ret;
		char* line = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		if (line) {
			ret = String(line, Base::getStringLength(line, (sl_int32)len));
			free(line);
		}
		return ret;
	}
	
	sl_char16 Console::readChar(sl_bool flagPrintEcho)
	{
		termios tOld, tNew;
		tcgetattr(0, &tOld);
		tNew = tOld;
		tNew.c_lflag &= ~ICANON;
		if (flagPrintEcho) {
			tNew.c_lflag |= ECHO;
		} else {
			tNew.c_lflag &= ~ECHO;
		}
		tcsetattr(0, TCSANOW, &tNew);
		sl_char16 ch = (sl_char16)(::getchar());
		tcsetattr(0, TCSANOW, &tOld);
		return ch;
	}
#endif
	
}

#endif
