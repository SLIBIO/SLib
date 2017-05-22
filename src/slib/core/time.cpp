/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/time.h"

#include "slib/core/variant.h"
#include "slib/core/string_buffer.h"

#define TIME_MILLIS SLIB_INT64(1000)
#define TIME_MILLISF 1000.0
#define TIME_SECOND SLIB_INT64(1000000)
#define TIME_SECONDF 1000000.0
#define TIME_MINUTE SLIB_INT64(60000000)
#define TIME_MINUTEF 60000000.0
#define TIME_HOUR SLIB_INT64(3600000000)
#define TIME_HOURF 3600000000.0
#define TIME_DAY SLIB_INT64(86400000000)
#define TIME_DAYF 86400000000.0

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#include <windows.h>
#elif defined(SLIB_PLATFORM_IS_UNIX)
#include <sys/time.h>
#include <time.h>
#endif

namespace slib
{

	Time::Time(int year, int month, int date) noexcept
	{
		setElements(year, month, date, 0, 0, 0);
	}

	Time::Time(int year, int month, int date, int hour, int minute, int second) noexcept
	{
		setElements(year, month, date, hour, minute, second);
	}

	Time::Time(const String& str) noexcept
	{
		setString(str);
	}

	Time::Time(const String16& str) noexcept
	{
		setString(str);
	}

	Time::Time(const AtomicString& str) noexcept
	{
		setString(str);
	}

	Time::Time(const AtomicString16& str) noexcept
	{
		setString(str);
	}

	Time::Time(const sl_char8* str) noexcept
	{
		setString(str);
	}

	Time::Time(const sl_char16* str) noexcept
	{
		setString(str);
	}

	Time Time::now() noexcept
	{
		Time ret;
		ret.setNow();
		return ret;
	}

	void Time::setInt(sl_int64 time) noexcept
	{
		m_time = time;
	}

	void Time::add(sl_int64 time) noexcept
	{
		m_time += time;
	}

	void Time::add(const Time& other) noexcept
	{
		m_time += other.m_time;
	}

	void Time::setElements(int year, int month, int day, int hour, int minute, int second) noexcept
	{
		if (year == 0 && month == 0 && day == 0) {
			m_time = hour * TIME_HOUR + minute * TIME_MINUTE + second * TIME_SECOND;
			return;
		}
		if (month <= 0) {
			month = 1;
		}
		if (day <= 0) {
			day = 1;
		}
		_setElements(year, month, day, hour, minute, second);
	}

	void Time::setNow() noexcept
	{
		_setNow();
	}

	void Time::setToSystem() noexcept
	{
		_setToSystem();
	}

	void Time::getDate(DATE* date) const noexcept
	{
		if (!(_getDate(date))) {
			date->year = 0;
			date->month = 0;
			date->day = 0;
			date->dayOfWeek = 0;
		}
	}

	void Time::setDate(int year, int month, int day) noexcept
	{
		Time old(m_time);
		sl_int64 time = old.m_time % TIME_DAY;
		old.setElements(year, month, day, 0, 0, 0);
		m_time = old.m_time + time;
	}

	int Time::getYear() const noexcept
	{
		DATE date;
		getDate(&date);
		return date.year;
	}

	void Time::setYear(int year) noexcept
	{
		DATE date;
		getDate(&date);
		setDate(year, date.month, date.day);
	}

	void Time::addYears(int years) noexcept
	{
		DATE date;
		getDate(&date);
		setDate(date.year + years, date.month, date.day);
	}

	int Time::getMonth() const noexcept
	{
		DATE date;
		getDate(&date);
		return date.month;
	}

	void Time::setMonth(int month) noexcept
	{
		DATE date;
		getDate(&date);
		int monthNew = (month - 1) % 12;
		int yearAdd = (month - 1) / 12;
		if (monthNew < 0) {
			monthNew += 12;
			yearAdd--;
		}
		setDate(date.year + yearAdd, monthNew + 1, date.day);
	}

	void Time::addMonths(int months) noexcept
	{
		DATE date;
		getDate(&date);
		months += date.month;
		int monthNew = (months - 1) % 12;
		int yearAdd = (months - 1) / 12;
		if (monthNew < 0) {
			monthNew += 12;
			yearAdd--;
		}
		setDate(date.year + yearAdd, monthNew + 1, date.day);
	}

	int Time::getDay() const noexcept
	{
		DATE date;
		getDate(&date);
		return date.day;
	}

	void Time::setDay(int day) noexcept
	{
		Time old(m_time);
		DATE date;
		old.getDate(&date);
		m_time = old.m_time + (sl_int64)(day - date.day)*TIME_DAY;
	}

	void Time::addDays(sl_int64 days) noexcept
	{
		m_time += days*TIME_DAY;
	}

	double Time::getDayf() const noexcept
	{
		DATE date;
		getDate(&date);
		return date.day + (m_time % TIME_DAY) / TIME_DAYF;
	}

	void Time::setDayf(double day) noexcept
	{
		Time t(getYear(), getMonth(), 1);
		m_time = t.m_time + (sl_int64)((day - 1)*TIME_DAYF);
	}

	void Time::addDaysf(double days) noexcept
	{
		m_time += (sl_int64)(days*TIME_DAYF);
	}

	int Time::getHour() const noexcept
	{
		return (int)((m_time / TIME_HOUR) % 24);
	}

	void Time::setHour(int hour) noexcept
	{
		sl_int64 old = m_time;
		int oldHour = (int)((old / TIME_HOUR) % 24);
		m_time = old + (sl_int64)(hour - oldHour)*TIME_HOUR;
	}

	void Time::addHours(sl_int64 hours) noexcept
	{
		m_time += hours*TIME_HOUR;
	}

	double Time::getHourf() const noexcept
	{
		return (m_time % TIME_DAY) / TIME_HOURF;
	}

	void Time::setHourf(double hour) noexcept
	{
		sl_int64 old = m_time;
		m_time = old - (old % TIME_DAY) + (sl_int64)(hour * TIME_HOURF);
	}

	void Time::addHoursf(double hours) noexcept
	{
		m_time += (sl_int64)(hours*TIME_HOURF);
	}

	int Time::getMinute() const noexcept
	{
		return (int)((m_time / TIME_MINUTE) % 60);
	}

	void Time::setMinute(int minute) noexcept
	{
		sl_int64 old = m_time;
		int oldMinute = (int)((old / TIME_MINUTE) % 60);
		m_time = old + (sl_int64)(minute - oldMinute)*TIME_MINUTE;
	}

	void Time::addMinutes(sl_int64 minutes) noexcept
	{
		m_time += minutes*TIME_MINUTE;
	}

	double Time::getMinutef() const noexcept
	{
		return (m_time % TIME_HOUR) / TIME_MINUTEF;
	}

	void Time::setMinutef(double minute) noexcept
	{
		sl_int64 old = m_time;
		m_time = old - (old % TIME_HOUR) + (sl_int64)(minute*TIME_MINUTEF);
	}

	void Time::addMinutesf(double minutes) noexcept
	{
		m_time += (sl_int64)(minutes*TIME_MINUTEF);
	}

	int Time::getSecond() const noexcept
	{
		return (int)((m_time / TIME_SECOND) % 60);
	}

	void Time::setSecond(int second) noexcept
	{
		sl_int64 old = m_time;
		int oldSecond = (int)((old / TIME_SECOND) % 60);
		m_time = old + (sl_int64)(second - oldSecond)*TIME_SECOND;
	}

	void Time::addSeconds(sl_int64 seconds) noexcept
	{
		m_time += seconds*TIME_SECOND;
	}

	double Time::getSecondf() const noexcept
	{
		return (m_time % TIME_MINUTE) / TIME_SECONDF;
	}

	void Time::setSecondf(double second) noexcept
	{
		sl_int64 old = m_time;
		m_time = old - (old % TIME_MINUTE) + (sl_int64)(second*TIME_SECOND);
	}

	void Time::addSecondsf(double seconds) noexcept
	{
		m_time += (sl_int64)(seconds*TIME_SECONDF);
	}

	int Time::getMillisecond() const noexcept
	{
		return (int)((m_time / TIME_MILLIS) % 1000);
	}

	void Time::setMillisecond(int millis) noexcept
	{
		sl_int64 old = m_time;
		int oldMillis = (int)((old / TIME_MILLIS) % 1000);
		m_time = old + (sl_int64)(millis - oldMillis)*TIME_MILLIS;
	}

	void Time::addMilliseconds(sl_int64 milis) noexcept
	{
		m_time += milis*TIME_MILLIS;
	}

	double Time::getMillisecondf() const noexcept
	{
		return (m_time % TIME_SECOND) / TIME_MILLISF;
	}

	void Time::setMillisecondf(double millis) noexcept
	{
		sl_int64 old = m_time;
		m_time = old - (old % TIME_SECOND) + (sl_int64)(millis*TIME_MILLIS);
	}

	void Time::addMillisecondsf(double milis) noexcept
	{
		m_time += (sl_int64)(milis*TIME_MILLISF);
	}

	int Time::getMicrosecond() const noexcept
	{
		return (int)(m_time % 1000);
	}

	void Time::setMicrosecond(int micros) noexcept
	{
		sl_int64 old = m_time;
		int oldMicros = (int)(old % 1000);
		m_time = old + (micros - oldMicros);
	}

	void Time::addMicroseconds(sl_int64 micros) noexcept
	{
		m_time += micros;
	}

	double Time::getMicrosecondf() const noexcept
	{
		return (double)(m_time % 1000);
	}

	void Time::setMicrosecondf(double micros) noexcept
	{
		sl_int64 old = m_time;
		m_time = old - (old % TIME_MILLIS) + (sl_int64)(micros);
	}

	void Time::addMicrosecondsf(double micros) noexcept
	{
		m_time += (sl_int64)micros;
	}

	int Time::getDayOfWeek() const noexcept
	{
		DATE date;
		getDate(&date);
		return date.dayOfWeek;
	}

	void Time::setDayOfWeek(int day) noexcept
	{
		Time old = m_time;
		DATE date;
		old.getDate(&date);
		m_time = old.m_time + (sl_int64)(day - date.dayOfWeek)*TIME_DAY;
	}

	int Time::getDayOfYear() const noexcept
	{
		Time old(m_time);
		DATE date;
		old.getDate(&date);
		Time time(date.year, 1, 1);
		return (int)((old.m_time - time.m_time) / TIME_DAY) + 1;
	}

	void Time::setDayOfYear(int day) noexcept
	{
		Time time(m_time);
		DATE date;
		time.getDate(&date);
		time.setDate(date.year, 1, 1);
		m_time = time.m_time + (sl_int64)(day - 1)*TIME_DAY;
	}

	sl_int64 Time::getDaysCount() const noexcept
	{
		return (m_time / TIME_DAY);
	}

	void Time::setDaysCount(sl_int64 days) noexcept
	{
		m_time = days*TIME_DAY + (m_time % TIME_DAY);
	}

	double Time::getDaysCountf() const noexcept
	{
		return (m_time / TIME_DAYF);
	}

	void Time::setDaysCountf(double days) noexcept
	{
		m_time = (sl_int64)(days*TIME_DAYF);
	}

	sl_int64 Time::getHoursCount() const noexcept
	{
		return (m_time / TIME_HOUR);
	}

	void Time::setHoursCount(sl_int64 hours) noexcept
	{
		m_time = hours*TIME_HOUR + (m_time % TIME_HOUR);
	}

	double Time::getHoursCountf() const noexcept
	{
		return (m_time / TIME_HOURF);
	}

	void Time::setHoursCountf(double hours) noexcept
	{
		m_time = (sl_int64)(hours*TIME_HOURF);
	}

	sl_int64 Time::getMinutesCount() const noexcept
	{
		return (m_time / TIME_MINUTE);
	}

	void Time::setMinutesCount(sl_int64 minutes) noexcept
	{
		m_time = minutes*TIME_MINUTE + (m_time % TIME_MINUTE);
	}

	double Time::getMinutesCountf() const noexcept
	{
		return (m_time / TIME_MINUTEF);
	}

	void Time::setMinutesCountf(double minutes) noexcept
	{
		m_time = (sl_int64)(minutes*TIME_MINUTEF);
	}

	sl_int64 Time::getSecondsCount() const noexcept
	{
		return (m_time / TIME_SECOND);
	}

	void Time::setSecondsCount(sl_int64 seconds) noexcept
	{
		m_time = seconds*TIME_SECOND + (m_time % TIME_SECOND);
	}

	double Time::getSecondsCountf() const noexcept
	{
		return (m_time / TIME_SECONDF);
	}

	void Time::setSecondsCountf(double seconds) noexcept
	{
		m_time = (sl_int64)(seconds*TIME_SECONDF);
	}

	sl_int64 Time::getMillisecondsCount() const noexcept
	{
		return (m_time / TIME_MILLIS);
	}

	void Time::setMillisecondsCount(sl_int64 millis) noexcept
	{
		m_time = millis*TIME_MILLIS + (m_time % TIME_MILLIS);
	}

	double Time::getMillisecondsCountf() const noexcept
	{
		return (m_time / TIME_MILLISF);
	}

	void Time::setMillisecondsCountf(double millis) noexcept
	{
		m_time = (sl_int64)(millis*TIME_MILLISF);
	}

	sl_int64 Time::getMicrosecondsCount() const noexcept
	{
		return m_time;
	}

	void Time::setMicrosecondsCount(sl_int64 micros) noexcept
	{
		m_time = micros;
	}

	double Time::getMicrosecondsCountf() const noexcept
	{
		return (double)m_time;
	}

	void Time::setMicrosecondsCountf(double micros) noexcept
	{
		m_time = (sl_int64)(micros);
	}

	int Time::getDaysCountInMonth() const noexcept
	{
		DATE date;
		getDate(&date);
		Time timeStart(date.year, date.month, 1);
		Time timeEnd = timeStart;
		timeEnd.addMonths(1);
		return (int)((timeEnd.m_time - timeStart.m_time) / TIME_DAY);
	}

	int Time::getDaysCountInYear() const noexcept
	{
		DATE date;
		getDate(&date);
		Time timeStart(date.year, 1, 1);
		Time timeEnd(date.year + 1, 1, 1);
		return (int)((timeEnd.m_time - timeStart.m_time) / TIME_DAY);
	}

	int Time::getQuarter() const noexcept
	{
		return ((getMonth() - 1) / 3) + 1;
	}

	String Time::getWeekday(sl_bool flagShort) const noexcept
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
		return sl_null;
	}

	Time Time::getTimeOnly() const noexcept
	{
		return m_time % TIME_DAY;
	}

	String Time::toString() const noexcept
	{
		DATE d;
		getDate(&d);
		StringBuffer sb;
		sb.add(String::fromInt32(d.year, 10, 4));
		sb.addStatic("-", 1);
		sb.add(String::fromInt32(d.month, 10, 2));
		sb.addStatic("-", 1);
		sb.add(String::fromInt32(d.day, 10, 2));
		sb.addStatic(" ", 1);
		sb.add(String::fromInt32(getHour(), 10, 2));
		sb.addStatic(":", 1);
		sb.add(String::fromInt32(getMinute(), 10, 2));
		sb.addStatic(":", 1);
		sb.add(String::fromInt32(getSecond(), 10, 2));
		return sb.merge();
	}

	String16 Time::toString16() const noexcept
	{
		DATE d;
		getDate(&d);
		StringBuffer16 sb;
		sb.add(String16::fromInt32(d.year, 10, 4));
		{
			const sl_char16 ch = '-';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(d.month, 10, 2));
		{
			const sl_char16 ch = '-';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(d.day, 10, 2));
		{
			const sl_char16 ch = ' ';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(getHour(), 10, 2));
		{
			const sl_char16 ch = ':';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(getMinute(), 10, 2));
		{
			const sl_char16 ch = ':';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(getSecond(), 10, 2));
		return sb.merge();
	}

	String Time::getDateString() const noexcept
	{
		DATE d;
		getDate(&d);
		StringBuffer sb;
		sb.add(String::fromInt32(d.year, 10, 4));
		sb.addStatic("-", 1);
		sb.add(String::fromInt32(d.month, 10, 2));
		sb.addStatic("-", 1);
		sb.add(String::fromInt32(d.day, 10, 2));
		return sb.merge();
	}

	String16 Time::getDateString16() const noexcept
	{
		DATE d;
		getDate(&d);
		StringBuffer16 sb;
		sb.add(String16::fromInt32(d.year, 10, 4));
		{
			const sl_char16 ch = '-';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(d.month, 10, 2));
		{
			const sl_char16 ch = '-';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(d.day, 10, 2));
		return sb.merge();
	}

	String Time::getTimeString() const noexcept
	{
		StringBuffer sb;
		sb.add(String::fromInt32(getHour(), 10, 2));
		sb.addStatic(":", 1);
		sb.add(String::fromInt32(getMinute(), 10, 2));
		sb.addStatic(":", 1);
		sb.add(String::fromInt32(getSecond(), 10, 2));
		return sb.merge();
	}

	String16 Time::getTimeString16() const noexcept
	{
		StringBuffer16 sb;
		sb.add(String16::fromInt32(getHour(), 10, 2));
		{
			const sl_char16 ch = ':';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(getMinute(), 10, 2));
		{
			const sl_char16 ch = ':';
			sb.addStatic(&ch, 1);
		}
		sb.add(String16::fromInt32(getSecond(), 10, 2));
		return sb.merge();
	}

	sl_bool Time::setString(const String& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	sl_bool Time::setString(const String16& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	sl_bool Time::setString(const AtomicString& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	sl_bool Time::setString(const AtomicString16& str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	sl_bool Time::setString(const sl_char8* str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	sl_bool Time::setString(const sl_char16* str) noexcept
	{
		if (parse(str)) {
			return sl_true;
		} else {
			setZero();
			return sl_false;
		}
	}

	String Time::format(const String& fmt) const noexcept
	{
		return String::format(fmt, *this);
	}

	String Time::format(const AtomicString& fmt) const noexcept
	{
		return String::format(fmt, *this);
	}

	String Time::format(const String16& fmt) const noexcept
	{
		return String16::format(fmt, *this);
	}

	String Time::format(const AtomicString16& fmt) const noexcept
	{
		return String16::format(fmt, *this);
	}

	String Time::format(const sl_char8* fmt) const noexcept
	{
		return String::format(fmt, *this);
	}

	String Time::format(const sl_char16* fmt) const noexcept
	{
		return String16::format(fmt, *this);
	}


	template <class CT>
	static sl_reg _priv_Time_parseElements(sl_int32* outYMDHMS, const CT* sz, sl_size i, sl_size n) noexcept
	{
		if (i >= n) {
			return SLIB_PARSE_ERROR;
		}
		sl_int32 YMDHMS[6];
		Base::resetMemory4(YMDHMS, 0, 6);
		sl_size index = 0;
		sl_size posParsed = i;
		while (i < n && index < 6) {
			if (sz[i] == 0) {
				break;
			}
			do {
				CT ch = sz[i];
				if (SLIB_CHAR_IS_SPACE_TAB(ch)) {
					i++;
				} else {
					break;
				}
			} while (i < n);
			if (i >= n) {
				break;
			}
			int value = 0;
			sl_bool flagNumber = sl_false;
			do {
				CT ch = sz[i];
				if (SLIB_CHAR_IS_DIGIT(ch)) {
					value = value * 10 + (ch - '0');
					flagNumber = sl_true;
					i++;
				} else {
					break;
				}
			} while (i < n);
			if (!flagNumber) {
				break;
			}
			posParsed = i;
			
			YMDHMS[index] = value;
			index++;
			
			if (i >= n) {
				break;
			}
			do {
				CT ch = sz[i];
				if (SLIB_CHAR_IS_SPACE_TAB(ch)) {
					i++;
				} else {
					break;
				}
			} while (i < n);
			if (i < n) {
				CT ch = sz[i];
				if (!SLIB_CHAR_IS_DIGIT(ch)) {
					if (ch != '/' && ch != '-' && ch != ':') {
						break;
					}
					if (ch == ':') {
						if (index < 3) {
							index = 3;
						}
					}
					i++;
				}
			} else {
				break;
			}
		}
		if (index > 0) {
			if (outYMDHMS) {
				Base::copyMemory(outYMDHMS, YMDHMS, 24);
			}
			return posParsed;
		}
		return SLIB_PARSE_ERROR;
	}

	sl_reg Time::parseElements(sl_int32* outArrayYMDHMS, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Time_parseElements(outArrayYMDHMS, sz, 0, posEnd);
	}

	sl_reg Time::parseElements(sl_int32* outArrayYMDHMS, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Time_parseElements(outArrayYMDHMS, sz, 0, posEnd);
	}

	sl_bool Time::parseElements(const String& time, sl_int32* outArrayYMDHMS) noexcept
	{
		sl_size n = time.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Time_parseElements(outArrayYMDHMS, time.getData(), 0, time.getLength()) == (sl_reg)n;
	}

	sl_bool Time::parseElements(const String16& time, sl_int32* outArrayYMDHMS) noexcept
	{
		sl_size n = time.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Time_parseElements(outArrayYMDHMS, time.getData(), 0, time.getLength()) == (sl_reg)n;
	}

	sl_bool Time::parseElements(const AtomicString& _time, sl_int32* outArrayYMDHMS) noexcept
	{
		String time = _time;
		sl_size n = time.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Time_parseElements(outArrayYMDHMS, time.getData(), 0, time.getLength()) == (sl_reg)n;
	}

	sl_bool Time::parseElements(const AtomicString16& _time, sl_int32* outArrayYMDHMS) noexcept
	{
		String time = _time;
		sl_size n = time.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Time_parseElements(outArrayYMDHMS, time.getData(), 0, time.getLength()) == (sl_reg)n;
	}

	sl_bool Time::parseElements(const sl_char8* time, sl_int32* outArrayYMDHMS) noexcept
	{
		sl_reg ret = _priv_Time_parseElements(outArrayYMDHMS, time, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && time[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Time::parseElements(const sl_char16* time, sl_int32* outArrayYMDHMS) noexcept
	{
		sl_reg ret = _priv_Time_parseElements(outArrayYMDHMS, time, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && time[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}

	template <class CT>
	SLIB_INLINE static sl_reg _priv_Time_parse(Time* _out, const CT* sz, sl_size i, sl_size n) noexcept
	{
		sl_int32 t[6];
		sl_reg ret = _priv_Time_parseElements(t, sz, i, n);
		if (ret != SLIB_PARSE_ERROR) {
			if (_out) {
				_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
			}
		}
		return ret;
	}

	template <>
	sl_reg Parser<Time, sl_char8>::parse(Time* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Time_parse(_out, sz, posBegin, posEnd);
	}

	template <>
	sl_reg Parser<Time, sl_char16>::parse(Time* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Time_parse(_out, sz, posBegin, posEnd);
	}

	Time& Time::operator=(const String& time) noexcept
	{
		setString(time);
		return *this;
	}

	Time& Time::operator=(const String16& time) noexcept
	{
		setString(time);
		return *this;
	}

	Time& Time::operator=(const AtomicString& time) noexcept
	{
		setString(time);
		return *this;
	}

	Time& Time::operator=(const AtomicString16& time) noexcept
	{
		setString(time);
		return *this;
	}

	Time& Time::operator=(const sl_char8* time) noexcept
	{
		setString(time);
		return *this;
	}

	Time& Time::operator=(const sl_char16* time) noexcept
	{
		setString(time);
		return *this;
	}

#if defined(SLIB_PLATFORM_IS_WINDOWS)

	sl_bool Time::_getDate(DATE* date) const noexcept
	{
		SYSTEMTIME st;
		sl_int64 n = (m_time + SLIB_INT64(11644473600000000)) * 10;  // Convert 1970 Based (time_t mode) to 1601 Based (FILETIME mode)
		FileTimeToSystemTime((PFILETIME)&n, &st);
		date->year = st.wYear;
		date->month = st.wMonth;
		date->day = st.wDay;
		date->dayOfWeek = st.wDayOfWeek;
		return sl_true;
	}

	void Time::_setElements(int year, int month, int day, int hour, int minute, int second) noexcept
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
		sl_int64 n;
		SystemTimeToFileTime(&st, (PFILETIME)&n);
		m_time = n / 10 - SLIB_INT64(11644473600000000);  // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
	}

	void Time::_setNow() noexcept
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		sl_int64 n;
		SystemTimeToFileTime(&st, (PFILETIME)&n);
		m_time = n / 10 - SLIB_INT64(11644473600000000); // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
	}

#	if defined(SLIB_PLATFORM_IS_DESKTOP)

	void Time::_setToSystem() noexcept
	{
		SYSTEMTIME st;
		sl_int64 n = m_time * 10;
		FileTimeToSystemTime((PFILETIME)&n, &st);
		SetLocalTime(&st);
	}

#	elif defined(SLIB_PLATFORM_IS_MOBILE)

	void Time::_setToSystem() noexcept
	{
	}

#	endif

#elif defined(SLIB_PLATFORM_IS_UNIX)

	sl_bool Time::_getDate(DATE* date) const noexcept
	{
		time_t t = (time_t)(m_time / 1000000);
		tm v;
		if (gmtime_r(&t, &v)) {
			date->year = v.tm_year + 1900;
			date->month = v.tm_mon + 1;
			date->day = v.tm_mday;
			date->dayOfWeek = v.tm_wday;
			return sl_true;
		}
		return sl_false;
	}

	void Time::_setElements(int year, int month, int day, int hour, int minute, int second) noexcept
	{
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

	void Time::_setNow() noexcept
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

	void Time::_setToSystem() noexcept
	{
	}

#endif

	TimeCounter::TimeCounter() noexcept
	{
		reset();
	}

	TimeCounter::~TimeCounter() noexcept
	{
	}

	Time TimeCounter::getTime() const noexcept
	{
		return getTime(Time::now());
	}

	Time TimeCounter::getTime(const Time& current) const noexcept
	{
		Time last = m_timeLast;
		if (current > last) {
			return current - last + m_timeElapsed;
		} else {
			return m_timeElapsed;
		}
	}

	sl_uint64 TimeCounter::getElapsedMilliseconds() const noexcept
	{
		return getTime().getMillisecondsCount();
	}

	sl_uint64 TimeCounter::getElapsedMilliseconds(const Time& current) const noexcept
	{
		return getTime(current).getMillisecondsCount();
	}

	void TimeCounter::reset() noexcept
	{
		reset(Time::now());
	}

	void TimeCounter::reset(const Time& current) noexcept
	{
		m_timeLast = current;
		m_timeElapsed.setZero();
	}

	void TimeCounter::update() noexcept
	{
		update(Time::now());
	}

	void TimeCounter::update(const Time& current) noexcept
	{
		Time last = m_timeLast;
		if (current > last) {
			m_timeElapsed += (current - last);
		}
		m_timeLast = current;
	}


	TimeKeeper::TimeKeeper() noexcept
	 : m_flagStarted(sl_false), m_flagRunning(sl_false), m_timeLast(0), m_timeElapsed(0)
	{
	}

	TimeKeeper::~TimeKeeper() noexcept
	{
	}

	void TimeKeeper::start() noexcept
	{
		startAndSetTime(Time::zero(), Time::now());
	}

	void TimeKeeper::start(const Time& current) noexcept
	{
		startAndSetTime(Time::zero(), current);
	}

	void TimeKeeper::startAndSetTime(const Time& init) noexcept
	{
		startAndSetTime(init, Time::now());
	}

	void TimeKeeper::startAndSetTime(const Time& init, const Time& current) noexcept
	{
		SpinLocker lock(&m_lock);
		if (m_flagStarted) {
			return;
		}
		m_timeLast = current;
		m_timeElapsed = init;
		m_flagStarted = sl_true;
		m_flagRunning = sl_true;
	}

	void TimeKeeper::restart() noexcept
	{
		restartAndSetTime(Time::zero(), Time::now());
	}

	void TimeKeeper::restart(const Time& current) noexcept
	{
		restartAndSetTime(Time::zero(), current);
	}

	void TimeKeeper::restartAndSetTime(const Time& init) noexcept
	{
		restartAndSetTime(init, Time::now());
	}

	void TimeKeeper::restartAndSetTime(const Time& init, const Time& current) noexcept
	{
		SpinLocker lock(&m_lock);
		m_timeLast = current;
		m_timeElapsed = init;
		m_flagStarted = sl_true;
		m_flagRunning = sl_true;
	}

	void TimeKeeper::stop() noexcept
	{
		m_flagStarted = sl_false;
	}

	void TimeKeeper::resume() noexcept
	{
		resume(Time::now());
	}

	void TimeKeeper::resume(const Time& current) noexcept
	{
		SpinLocker lock(&m_lock);
		if (m_flagStarted) {
			if (!m_flagRunning) {
				m_timeLast = current;
				m_flagRunning = sl_true;
			}
		}
	}

	void TimeKeeper::pause() noexcept
	{
		pause(Time::now());
	}

	void TimeKeeper::pause(const Time& current) noexcept
	{
		SpinLocker lock(&m_lock);
		if (m_flagStarted) {
			if (m_flagRunning) {
				if (current > m_timeLast) {
					m_timeElapsed += (current - m_timeLast);
				}
				m_flagRunning = sl_false;
			}
		}
	}

	Time TimeKeeper::getTime() const noexcept
	{
		return getTime(Time::now());
	}

	Time TimeKeeper::getTime(const Time& current) const noexcept
	{
		SpinLocker lock(&m_lock);
		if (!m_flagStarted) {
			return Time::zero();
		}
		if (!m_flagRunning) {
			return m_timeElapsed;
		}
		return m_timeElapsed + (current - m_timeLast);
	}

	void TimeKeeper::setTime(const Time& time) noexcept
	{
		setTime(time, Time::now());
	}

	void TimeKeeper::setTime(const Time& time, const Time& current) noexcept
	{
		SpinLocker lock(&m_lock);
		if (m_flagStarted) {
			m_timeElapsed = time;
			m_timeLast = current;
		}
	}

	void TimeKeeper::update() noexcept
	{
		update(Time::now());
	}

	void TimeKeeper::update(const Time& current) noexcept
	{
		SpinLocker lock(&m_lock);
		if (m_flagStarted) {
			if (m_flagRunning) {
				sl_int64 add = (current - m_timeLast).toInt();
				if (add > 0) {
					m_timeElapsed += add;
				}
				m_timeLast = current;
			}
		}
	}

	sl_bool TimeKeeper::isStarted() const noexcept
	{
		return m_flagStarted;
	}

	sl_bool TimeKeeper::isStopped() const noexcept
	{
		return !m_flagStarted;
	}

	sl_bool TimeKeeper::isRunning() const noexcept
	{
		return m_flagStarted && m_flagRunning;
	}

	sl_bool TimeKeeper::isNotRunning() const noexcept
	{
		return !(m_flagStarted && m_flagRunning);
	}

	sl_bool TimeKeeper::isPaused() const noexcept
	{
		return m_flagStarted && !m_flagRunning;
	}

}
