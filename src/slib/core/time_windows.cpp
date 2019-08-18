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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "slib/core/time.h"

#include "slib/core/platform_windows.h"

namespace slib
{
	
	sl_bool Time::_get(TimeComponents& output, sl_bool flagUTC) const noexcept
	{
		SYSTEMTIME st;
		if (!(Windows::getSYSTEMTIME(*this, flagUTC, &st))) {
			return sl_false;
		}
		output.year = st.wYear;
		output.month = (sl_uint8)(st.wMonth);
		output.day = (sl_uint8)(st.wDay);
		output.dayOfWeek = (sl_uint8)(st.wDayOfWeek);
		output.hour = (sl_uint8)(st.wHour);
		output.minute = (sl_uint8)(st.wMinute);
		output.second = (sl_uint8)(st.wSecond);
		return sl_true;
	}

	sl_int64 Time::_set(sl_int32 year, sl_int32 month, sl_int32 day, sl_int32 hour, sl_int32 minute, sl_int32 second, sl_bool flagUTC) noexcept
	{
		SYSTEMTIME st;
		st.wYear = (WORD)year;
		st.wMonth = (WORD)month;
		st.wDay = (WORD)day;
		st.wDayOfWeek = 0;
		st.wHour = (WORD)hour;
		st.wMinute = (WORD)minute;
		st.wSecond = (WORD)second;
		st.wMilliseconds = 0;
		return Windows::getTime(&st, flagUTC).toInt();
	}

	void Time::_setNow() noexcept
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		sl_int64 n;
		SystemTimeToFileTime(&st, (PFILETIME)&n);
		m_time = n / 10 - SLIB_INT64(11644473600000000); // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
	}
	
	sl_bool Time::_setToSystem() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		SYSTEMTIME st;
		sl_int64 n = (m_time + SLIB_INT64(11644473600000000)) * 10;  // Convert 1970 Based (time_t mode) to 1601 Based (FILETIME mode)
		if (!(FileTimeToSystemTime((PFILETIME)&n, &st))) {
			return sl_false;
		}
		return 0 != SetSystemTime(&st);
#else
		return sl_false;
#endif
	}

}

#endif
