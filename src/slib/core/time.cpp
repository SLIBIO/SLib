#include "../../../inc/slib/core/time.h"

SLIB_NAMESPACE_BEGIN

#define TIME_MILLIS SLIB_INT64(1000)
#define TIME_SECOND SLIB_INT64(1000000)
#define TIME_MINUTE SLIB_INT64(60000000)
#define TIME_HOUR SLIB_INT64(3600000000)
#define TIME_DAY SLIB_INT64(86400000000)

void Time::set(int year, int month, int day) {
	Time old(m_time);
	sl_int64 time = old.m_time % TIME_DAY;
	old.set(year, month, day, 0, 0, 0);
	m_time = old.m_time + time;
}

int Time::getYear() const
{
	DATE date;
	get(&date);
	return date.year;
}

void Time::setYear(int year)
{
	DATE date;
	get(&date);
	set(year, date.month, date.day);
}

void Time::addYear(int year)
{
	DATE date;
	get(&date);
	set(date.year + year, date.month, date.day);
}

int Time::getMonth() const
{
	DATE date;
	get(&date);
	return date.month;
}

void Time::setMonth(int month)
{
	DATE date;
	get(&date);
	int monthNew = (month - 1) % 12;
	int yearAdd = (month - 1) / 12;
	if (monthNew < 0) {
		monthNew += 12;
		yearAdd--;
	}
	set(date.year + yearAdd, monthNew + 1, date.day);
}

void Time::addMonth(int month)
{
	DATE date;
	get(&date);
	month += date.month;
	int monthNew = (month - 1) % 12;
	int yearAdd = (month - 1) / 12;
	if (monthNew < 0) {
		monthNew += 12;
		yearAdd--;
	}
	set(date.year + yearAdd, monthNew + 1, date.day);
}

int Time::getDay() const
{
	DATE date;
	get(&date);
	return date.day;
}

void Time::setDay(int day)
{
	Time old(m_time);
	DATE date;
	old.get(&date);
	m_time = old.m_time + (sl_int64)(day - date.day)*TIME_DAY;
}

void Time::addDay(int day)
{
	m_time += (sl_int64)(day)*TIME_DAY;
}

int Time::getHour() const
{
	return (int)((m_time / TIME_HOUR) % 24);
}

void Time::setHour(int hour)
{
	sl_int64 old = m_time;
	int oldHour = (int)((old / TIME_HOUR) % 24);
	m_time = old + (sl_int64)(hour - oldHour)*TIME_HOUR;
}

void Time::addHour(int hour)
{
	m_time += (sl_int64)(hour)*TIME_HOUR;
}

int Time::getMinute() const
{
	return (int)((m_time / TIME_MINUTE) % 60);
}

void Time::setMinute(int minute)
{
	sl_int64 old = m_time;
	int oldMinute = (int)((old / TIME_MINUTE) % 60);
	m_time = old + (sl_int64)(minute - oldMinute)*TIME_MINUTE;
}

void Time::addMinute(int minute)
{
	m_time += (sl_int64)(minute)*TIME_MINUTE;
}

int Time::getSecond() const
{
	return (int)((m_time / TIME_SECOND) % 60);
}

void Time::setSecond(int second)
{
	sl_int64 old = m_time;
	int oldSecond = (int)((old / TIME_SECOND) % 60);
	m_time = old + (sl_int64)(second - oldSecond)*TIME_SECOND;
}

void Time::addSecond(int second)
{
	m_time += (sl_int64)(second)*TIME_SECOND;
}

int Time::getMillisecond() const
{
	return (int)((m_time / TIME_MILLIS) % 1000);
}

void Time::setMillisecond(int millis)
{
	sl_int64 old = m_time;
	int oldMillis = (int)((old / TIME_MILLIS) % 1000);
	m_time = old + (sl_int64)(millis - oldMillis)*TIME_MILLIS;
}

void Time::addMillisecond(int hour)
{
	m_time += (sl_int64)(hour)*TIME_MILLIS;
}

int Time::getDayOfWeek() const
{
	DATE date;
	get(&date);
	return date.dayOfWeek;
}

void Time::setDayOfWeek(int day)
{
	Time old = m_time;
	DATE date;
	old.get(&date);
	m_time = old.m_time + (sl_int64)(day - date.dayOfWeek)*TIME_DAY;
}

int Time::getDayOfYear() const
{
	Time old(m_time);
	DATE date;
	old.get(&date);
	Time time(date.year, 1, 1);
	return (int)((old.m_time - time.m_time) / TIME_DAY) + 1;
}

void Time::setDayOfYear(int day)
{
	Time time(m_time);
	DATE date;
	time.get(&date);
	time.set(date.year, 1, 1);
	m_time = time.m_time + (sl_int64)(day - 1)*TIME_DAY;
}

sl_int64 Time::getDaysCount() const
{
	return (m_time / TIME_DAY);
}

void Time::setDaysCount(sl_int64 count)
{
	m_time = (sl_int64)(count)*TIME_DAY + (m_time % TIME_DAY);
}

sl_int64 Time::getHoursCount() const
{
	return (m_time / TIME_HOUR);
}

void Time::setHoursCount(sl_int64 count)
{
	m_time = (sl_int64)(count)*TIME_HOUR + (m_time % TIME_HOUR);
}

sl_int64 Time::getMinutesCount() const
{
	return (m_time / TIME_MINUTE);
}

void Time::setMinutesCount(sl_int64 count)
{
	m_time = (sl_int64)(count)*TIME_MINUTE + (m_time % TIME_MINUTE);
}

sl_int64 Time::getSecondsCount() const
{
	return (int)(m_time / TIME_SECOND);
}

void Time::setSecondsCount(sl_int64 count) {
	m_time = (sl_int64)(count)*TIME_SECOND + (m_time % TIME_SECOND);
}

sl_int64 Time::getMillisecondsCount() const
{
	return (m_time / TIME_MILLIS);
}

void Time::setMillisecondsCount(sl_int64 count)
{
	m_time = (sl_int64)(count)*TIME_MILLIS + (m_time % TIME_MILLIS);
}

int Time::getDaysCountInMonth() const
{
	DATE date;
	get(&date);
	Time timeStart(date.year, date.month, 1);
	Time timeEnd = timeStart;
	timeEnd.addMonth(1);
	return (int)((timeEnd.m_time - timeStart.m_time) / TIME_DAY);
}

int Time::getDaysCountInYear() const
{
	DATE date;
	get(&date);
	Time timeStart(date.year, 1, 1);
	Time timeEnd(date.year + 1, 1, 1);
	return (int)((timeEnd.m_time - timeStart.m_time) / TIME_DAY);
}

int Time::getQuarter() const
{
	return ((getMonth() - 1) / 3) + 1;
}

String Time::getWeekday(sl_bool flagShort) const
{
	switch (getDayOfWeek()) {
	case 0:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Sun");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Sunday");
			return s;
		}
	case 1:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Mon");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Monday");
			return s;
		}
	case 2:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Tue");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Tuesday");
			return s;
		}
	case 3:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Wed");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Wednesday");
			return s;
		}
	case 4:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Thu");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Thursday");
			return s;
		}
	case 5:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Fri");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Friday");
			return s;
		}
	case 6:
		if (flagShort) {
			SLIB_STATIC_STRING(s, "Sat");
			return s;
		} else {
			SLIB_STATIC_STRING(s, "Saturday");
			return s;
		}
	}
	return String::null();
}

String Time::format(const String& fmt) const
{
	String str = fmt;
	str = str.replaceAll("%y", String::fromInt32(getYear()));
	str = str.replaceAll("%Y", String::fromInt32(getYear(), 10, 4));
	str = str.replaceAll("%m", String::fromInt32(getMonth()));
	str = str.replaceAll("%M", String::fromInt32(getMonth(), 10, 2));
	str = str.replaceAll("%d", String::fromInt32(getDay()));
	str = str.replaceAll("%D", String::fromInt32(getDay(), 10, 2));
	String w = getWeekday(sl_true);
	str = str.replaceAll("%w", w);
	w = getWeekday(sl_false);
	str = str.replaceAll("%W", w);
	str = str.replaceAll("%h", String::fromInt32(getHour()));
	str = str.replaceAll("%H", String::fromInt32(getHour(), 10, 2));
	str = str.replaceAll("%i", String::fromInt32(getMinute()));
	str = str.replaceAll("%I", String::fromInt32(getMinute(), 10, 2));
	str = str.replaceAll("%s", String::fromInt32(getSecond()));
	str = str.replaceAll("%S", String::fromInt32(getSecond(), 10, 2));
	str = str.replaceAll("%l", String::fromInt32(getMillisecond()));
	return str;
}

void Time::set(const String& str)
{
	int arr[6];
	parseTime(str, arr);
	set(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
}

void Time::parseTime(const String& _date, int* out)
{
	String8 date = _date;
	Base::resetMemory(out, 0, 6 * sizeof(int));

	const sl_char8* ch = date.getBuf();
	sl_uint32 len = date.getLength();

	sl_uint32 index = 0;
	sl_uint32 i = 0;

	while (i < len && index < 6) {
		int value = 0;
		while (i < len) {
			if (ch[i] >= '0' && ch[i] <= '9') {
				value = value * 10 + (ch[i] - '0');
			} else {
				break;
			}
			i++;
		}
		while (i < len && (ch[i] < '0' || ch[i] > '9')) {
			if (index < 3 && ch[i] == ':') {
				index = 3;
			}
			i++;
		}
		out[index] = value;
		index++;
	}
}

String Time::toString() const
{
	SLIB_STATIC_STRING(FMT, "%Y-%M-%D %H:%I:%S");
	return format(FMT);
}
SLIB_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

SLIB_NAMESPACE_BEGIN

void Time::setNow()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sl_int64 n;
	SystemTimeToFileTime(&st, (PFILETIME)&n);
	m_time = n / 10 - SLIB_INT64(11644473600000000); // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
}

void Time::get(DATE* date) const
{
	SYSTEMTIME st;
	sl_int64 n = (m_time + SLIB_INT64(11644473600000000)) * 10;  // Convert 1970 Based (time_t mode) to 1601 Based (FILETIME mode)
	FileTimeToSystemTime((PFILETIME)&n, &st);
	date->year = st.wYear;
	date->month = st.wMonth;
	date->day = st.wDay;
	date->dayOfWeek = st.wDayOfWeek;
}

void Time::set(int year, int month, int day, int hour, int minute, int second)
{
	if (year == 0 && month == 0 && day == 0) {
		m_time = hour * TIME_HOUR + minute * TIME_MINUTE + second * TIME_SECOND;
		return;
	}
	SYSTEMTIME st;
	st.wYear = (WORD)year;
	st.wMonth = (WORD)month;
	st.wDay = (WORD)day;
	st.wDayOfWeek = 0;
	st.wHour = (WORD)hour;
	st.wMinute = (WORD)minute;
	st.wSecond = (WORD)second;
	st.wMilliseconds = 0;
	sl_int64 n;
	SystemTimeToFileTime(&st, (PFILETIME)&n);
	m_time = n / 10 - SLIB_INT64(11644473600000000);  // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
}

SLIB_NAMESPACE_END
#if defined(SLIB_PLATFORM_IS_DESKTOP)
SLIB_NAMESPACE_BEGIN
void Time::setToSystem()
{
	SYSTEMTIME st;
	sl_int64 n = m_time * 10;
	FileTimeToSystemTime((PFILETIME)&n, &st);
	SetLocalTime(&st);
}
SLIB_NAMESPACE_END
#elif defined(SLIB_PLATFORM_IS_MOBILE)
SLIB_NAMESPACE_BEGIN
void Time::setToSystem()
{
}
SLIB_NAMESPACE_END
#endif

#elif defined(SLIB_PLATFORM_IS_UNIX)

#include <sys/time.h>
#include <time.h>

SLIB_NAMESPACE_BEGIN


void Time::setNow()
{
	sl_uint64 t;
	timeval tv;
	if (0 == gettimeofday(&tv, 0)) {
		t = tv.tv_sec;
		t *= 1000000;
		t += tv.tv_usec;
		time_t gt = 360000000;
		tm lt;
		Base::resetMemory(&lt, 0, sizeof(tm));
		if (gmtime_r(&gt, &lt)) {
			sl_int64 o = (int)(mktime(&lt) - gt);
			o *= 1000000;
			t -= o;
		}
	} else {
		t = 0;
	}
	m_time = t;
}

void Time::get(DATE* date) const
{
	time_t t = (time_t)(m_time / 1000000);
	tm v;
	if (gmtime_r(&t, &v)) {
		date->year = v.tm_year + 1900;
		date->month = v.tm_mon + 1;
		date->day = v.tm_mday;
		date->dayOfWeek = v.tm_wday;
	} else {
		date->year = 0;
		date->month = 0;
		date->day = 0;
		date->dayOfWeek = 0;
	}
}

void Time::set(int year, int month, int day, int hour, int minute, int second)
{
	if (year == 0 && month == 0 && day == 0) {
		m_time = hour * TIME_HOUR + minute * TIME_MINUTE + second * TIME_SECOND;
		return;
	}
	tm v;
	Base::resetMemory(&v, 0, sizeof(tm));
	v.tm_year = year - 1900;
	v.tm_mon = month - 1;
	v.tm_mday = day;
	v.tm_hour = hour;
	v.tm_min = minute;
	v.tm_sec = second;
	time_t t = mktime(&v);
	m_time = (sl_int64)(t) * 1000000;
	
	time_t gt = 360000000;
	tm lt;
	Base::resetMemory(&lt, 0, sizeof(tm));
	if (gmtime_r(&gt, &lt)) {
		sl_int64 o = (int)(mktime(&lt) - gt);
		o *= 1000000;
		m_time -= o;
	}
}

void Time::setToSystem()
{
}
SLIB_NAMESPACE_END
#endif

SLIB_NAMESPACE_BEGIN
TimeCounter::TimeCounter()
{
	reset();
}

void TimeCounter::reset()
{
	m_timeStart = Time::now();
	m_relStart = 0;
}

void TimeCounter::update()
{
	Time orig = m_timeStart;
	Time time = Time::now();
	if (time > orig) {
		m_relStart += (time - orig).toInt();
	}
	m_timeStart = time;
}

Time TimeCounter::getRelative() const
{
	Time orig = m_timeStart;
	Time time = Time::now();
	if (time > orig) {
		return m_relStart + (time - orig).toInt();
	} else {
		return m_relStart;
	}
}

sl_uint64 TimeCounter::getEllapsedMilliseconds() const
{
	return getRelative().getMillisecondsCount();
}
SLIB_NAMESPACE_END
