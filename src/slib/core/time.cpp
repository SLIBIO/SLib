#include "../../../inc/slib/core/time.h"
#include "../../../inc/slib/core/variant.h"

SLIB_NAMESPACE_BEGIN

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

Time::Time(int year, int month, int date)
{
	setElements(year, month, date, 0, 0, 0);
}

Time::Time(int year, int month, int date, int hour, int minute, int second)
{
	setElements(year, month, date, hour, minute, second);
}

Time::Time(const String8& str)
{
	setString(str);
}

Time::Time(const String16& str)
{
	setString(str);
}

Time::Time(const SafeString8& str)
{
	setString(str);
}

Time::Time(const SafeString16& str)
{
	setString(str);
}

Time::Time(const sl_char8* str)
{
	setString(str);
}

Time::Time(const sl_char16* str)
{
	setString(str);
}

void Time::setInt(sl_int64 time)
{
	m_time = time;
}

void Time::add(sl_int64 time)
{
	m_time += time;
}

void Time::add(const Time& other)
{
	m_time += other.m_time;
}

void Time::setDate(int year, int month, int day) {
	Time old(m_time);
	sl_int64 time = old.m_time % TIME_DAY;
	old.setElements(year, month, day, 0, 0, 0);
	m_time = old.m_time + time;
}

int Time::getYear() const
{
	DATE date;
	getDate(&date);
	return date.year;
}

void Time::setYear(int year)
{
	DATE date;
	getDate(&date);
	setDate(year, date.month, date.day);
}

void Time::addYears(int years)
{
	DATE date;
	getDate(&date);
	setDate(date.year + years, date.month, date.day);
}

int Time::getMonth() const
{
	DATE date;
	getDate(&date);
	return date.month;
}

void Time::setMonth(int month)
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

void Time::addMonths(int months)
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

int Time::getDay() const
{
	DATE date;
	getDate(&date);
	return date.day;
}

void Time::setDay(int day)
{
	Time old(m_time);
	DATE date;
	old.getDate(&date);
	m_time = old.m_time + (sl_int64)(day - date.day)*TIME_DAY;
}

void Time::addDays(sl_int64 days)
{
	m_time += days*TIME_DAY;
}

double Time::getDayf() const
{
	DATE date;
	getDate(&date);
	return date.day + (m_time % TIME_DAY) / TIME_DAYF;
}

void Time::setDayf(double day)
{
	Time t(getYear(), getMonth(), 1);
	m_time = t.m_time + (sl_int64)((day - 1)*TIME_DAYF);
}

void Time::addDaysf(double days)
{
	m_time += (sl_int64)(days*TIME_DAYF);
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

void Time::addHours(sl_int64 hours)
{
	m_time += hours*TIME_HOUR;
}

double Time::getHourf() const
{
	return (m_time % TIME_DAY) / TIME_HOURF;
}

void Time::setHourf(double hour)
{
	sl_int64 old = m_time;
	m_time = old - (old % TIME_DAY) + (sl_int64)(hour * TIME_HOURF);
}

void Time::addHoursf(double hours)
{
	m_time += (sl_int64)(hours*TIME_HOURF);
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

void Time::addMinutes(sl_int64 minutes)
{
	m_time += minutes*TIME_MINUTE;
}

double Time::getMinutef() const
{
	return (m_time % TIME_HOUR) / TIME_MINUTEF;
}

void Time::setMinutef(double minute)
{
	sl_int64 old = m_time;
	m_time = old - (old % TIME_HOUR) + (sl_int64)(minute*TIME_MINUTEF);
}

void Time::addMinutesf(double minutes)
{
	m_time += (sl_int64)(minutes*TIME_MINUTEF);
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

void Time::addSeconds(sl_int64 seconds)
{
	m_time += seconds*TIME_SECOND;
}

double Time::getSecondf() const
{
	return (m_time % TIME_MINUTE) / TIME_SECONDF;
}

void Time::setSecondf(double second)
{
	sl_int64 old = m_time;
	m_time = old - (old % TIME_MINUTE) + (sl_int64)(second*TIME_SECOND);
}

void Time::addSecondsf(double seconds)
{
	m_time += (sl_int64)(seconds*TIME_SECONDF);
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

void Time::addMilliseconds(sl_int64 milis)
{
	m_time += milis*TIME_MILLIS;
}

double Time::getMillisecondf() const
{
	return (m_time % TIME_SECOND) / TIME_MILLISF;
}

void Time::setMillisecondf(double millis)
{
	sl_int64 old = m_time;
	m_time = old - (old % TIME_SECOND) + (sl_int64)(millis*TIME_MILLIS);
}

void Time::addMillisecondsf(double milis)
{
	m_time += (sl_int64)(milis*TIME_MILLISF);
}

int Time::getMicrosecond() const
{
	return (int)(m_time % 1000);
}

void Time::setMicrosecond(int micros)
{
	sl_int64 old = m_time;
	int oldMicros = (int)(old % 1000);
	m_time = old + (micros - oldMicros);
}

void Time::addMicroseconds(sl_int64 micros)
{
	m_time += micros;
}

double Time::getMicrosecondf() const
{
	return (double)(m_time % 1000);
}

void Time::setMicrosecondf(double micros)
{
	sl_int64 old = m_time;
	m_time = old - (old % TIME_MILLIS) + (sl_int64)(micros);
}

void Time::addMicrosecondsf(double micros)
{
	m_time += (sl_int64)micros;
}

int Time::getDayOfWeek() const
{
	DATE date;
	getDate(&date);
	return date.dayOfWeek;
}

void Time::setDayOfWeek(int day)
{
	Time old = m_time;
	DATE date;
	old.getDate(&date);
	m_time = old.m_time + (sl_int64)(day - date.dayOfWeek)*TIME_DAY;
}

int Time::getDayOfYear() const
{
	Time old(m_time);
	DATE date;
	old.getDate(&date);
	Time time(date.year, 1, 1);
	return (int)((old.m_time - time.m_time) / TIME_DAY) + 1;
}

void Time::setDayOfYear(int day)
{
	Time time(m_time);
	DATE date;
	time.getDate(&date);
	time.setDate(date.year, 1, 1);
	m_time = time.m_time + (sl_int64)(day - 1)*TIME_DAY;
}

sl_int64 Time::getDaysCount() const
{
	return (m_time / TIME_DAY);
}

void Time::setDaysCount(sl_int64 days)
{
	m_time = days*TIME_DAY + (m_time % TIME_DAY);
}

double Time::getDaysCountf() const
{
	return (m_time / TIME_DAYF);
}

void Time::setDaysCountf(double days)
{
	m_time = (sl_int64)(days*TIME_DAYF);
}

sl_int64 Time::getHoursCount() const
{
	return (m_time / TIME_HOUR);
}

void Time::setHoursCount(sl_int64 hours)
{
	m_time = hours*TIME_HOUR + (m_time % TIME_HOUR);
}

double Time::getHoursCountf() const
{
	return (m_time / TIME_HOURF);
}

void Time::setHoursCountf(double hours)
{
	m_time = (sl_int64)(hours*TIME_HOURF);
}

sl_int64 Time::getMinutesCount() const
{
	return (m_time / TIME_MINUTE);
}

void Time::setMinutesCount(sl_int64 minutes)
{
	m_time = minutes*TIME_MINUTE + (m_time % TIME_MINUTE);
}

double Time::getMinutesCountf() const
{
	return (m_time / TIME_MINUTEF);
}

void Time::setMinutesCountf(double minutes)
{
	m_time = (sl_int64)(minutes*TIME_MINUTEF);
}

sl_int64 Time::getSecondsCount() const
{
	return (m_time / TIME_SECOND);
}

void Time::setSecondsCount(sl_int64 seconds) {
	m_time = seconds*TIME_SECOND + (m_time % TIME_SECOND);
}

double Time::getSecondsCountf() const
{
	return (m_time / TIME_SECONDF);
}

void Time::setSecondsCountf(double seconds) {
	m_time = (sl_int64)(seconds*TIME_SECONDF);
}

sl_int64 Time::getMillisecondsCount() const
{
	return (m_time / TIME_MILLIS);
}

void Time::setMillisecondsCount(sl_int64 millis)
{
	m_time = millis*TIME_MILLIS + (m_time % TIME_MILLIS);
}

double Time::getMillisecondsCountf() const
{
	return (m_time / TIME_MILLISF);
}

void Time::setMillisecondsCountf(double millis)
{
	m_time = (sl_int64)(millis*TIME_MILLISF);
}

sl_int64 Time::getMicrosecondsCount() const
{
	return m_time;
}

void Time::setMicrosecondsCount(sl_int64 micros)
{
	m_time = micros;
}

double Time::getMicrosecondsCountf() const
{
	return (double)m_time;
}

void Time::setMicrosecondsCountf(double micros)
{
	m_time = (sl_int64)(micros);
}

int Time::getDaysCountInMonth() const
{
	DATE date;
	getDate(&date);
	Time timeStart(date.year, date.month, 1);
	Time timeEnd = timeStart;
	timeEnd.addMonths(1);
	return (int)((timeEnd.m_time - timeStart.m_time) / TIME_DAY);
}

int Time::getDaysCountInYear() const
{
	DATE date;
	getDate(&date);
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

Time Time::getTimeOnly() const
{
	return m_time % TIME_DAY;
}

String8 Time::toString() const
{
	DATE d;
	getDate(&d);
	StringBuffer8 sb;
	sb.add(String8::fromInt32(d.year, 10, 4));
	sb.addStatic("-", 1);
	sb.add(String8::fromInt32(d.month, 10, 2));
	sb.addStatic("-", 1);
	sb.add(String8::fromInt32(d.day, 10, 2));
	sb.addStatic(" ", 1);
	sb.add(String8::fromInt32(getHour(), 10, 2));
	sb.addStatic(":", 1);
	sb.add(String8::fromInt32(getMinute(), 10, 2));
	sb.addStatic(":", 1);
	sb.add(String8::fromInt32(getSecond(), 10, 2));
	return sb.merge();
}

String16 Time::toString16() const
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

String8 Time::getDateString() const
{
	DATE d;
	getDate(&d);
	StringBuffer8 sb;
	sb.add(String8::fromInt32(d.year, 10, 4));
	sb.addStatic("-", 1);
	sb.add(String8::fromInt32(d.month, 10, 2));
	sb.addStatic("-", 1);
	sb.add(String8::fromInt32(d.day, 10, 2));
	return sb.merge();
}

String16 Time::getDateString16() const
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

String8 Time::getTimeString() const
{
	StringBuffer8 sb;
	sb.add(String8::fromInt32(getHour(), 10, 2));
	sb.addStatic(":", 1);
	sb.add(String8::fromInt32(getMinute(), 10, 2));
	sb.addStatic(":", 1);
	sb.add(String8::fromInt32(getSecond(), 10, 2));
	return sb.merge();
}

String16 Time::getTimeString16() const
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

sl_bool Time::setString(const String8& str)
{
	if (parse(str)) {
		return sl_true;
	} else {
		setZero();
		return sl_false;
	}
}

sl_bool Time::setString(const String16& str)
{
	if (parse(str)) {
		return sl_true;
	} else {
		setZero();
		return sl_false;
	}
}

sl_bool Time::setString(const SafeString8& str)
{
	if (parse(str)) {
		return sl_true;
	} else {
		setZero();
		return sl_false;
	}
}

sl_bool Time::setString(const SafeString16& str)
{
	if (parse(str)) {
		return sl_true;
	} else {
		setZero();
		return sl_false;
	}
}

sl_bool Time::setString(const sl_char8* str)
{
	if (parse(str)) {
		return sl_true;
	} else {
		setZero();
		return sl_false;
	}
}

sl_bool Time::setString(const sl_char16* str)
{
	if (parse(str)) {
		return sl_true;
	} else {
		setZero();
		return sl_false;
	}
}

template <class CT>
sl_reg _Time_parse(sl_int32* outArrayYMDHMS, const CT* sz, sl_size i, sl_size n)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	if (outArrayYMDHMS) {
		Base::resetMemory4(outArrayYMDHMS, 0, 6);
	}
	sl_size index = 0;
	sl_size posParsed = i;
	while (i < n && index < 6) {
		if (sz[i] == 0) {
			break;
		}
		do {
			CT ch = sz[i];
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
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
			if (ch >= '0' && ch <= '9') {
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
		if (i >= n) {
			break;
		}
		do {
			CT ch = sz[i];
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
				i++;
			} else {
				break;
			}
		} while (i < n);
		posParsed = i;
		if (i < n) {
			CT ch = sz[i];
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
		if (outArrayYMDHMS) {
			outArrayYMDHMS[index] = value;
		}
		index++;
	}
	if (index > 0) {
		return posParsed;
	}
	return SLIB_PARSE_ERROR;
}

sl_reg Time::parseElements(sl_int32* outArrayYMDHMS, const sl_char8* sz, sl_size posBegin, sl_size len)
{
	return _Time_parse(outArrayYMDHMS, sz, 0, len);
}

sl_reg Time::parseElements(sl_int32* outArrayYMDHMS, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	return _Time_parse(outArrayYMDHMS, sz, 0, len);
}

sl_bool Time::parseElements(const String8& time, sl_int32* outArrayYMDHMS)
{
	sl_size n = time.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _Time_parse(outArrayYMDHMS, time.getData(), 0, time.getLength()) == n;
}

sl_bool Time::parseElements(const String16& time, sl_int32* outArrayYMDHMS)
{
	sl_size n = time.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _Time_parse(outArrayYMDHMS, time.getData(), 0, time.getLength()) == n;
}

sl_bool Time::parseElements(const SafeString8& _time, sl_int32* outArrayYMDHMS)
{
	String8 time = _time;
	sl_size n = time.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _Time_parse(outArrayYMDHMS, time.getData(), 0, time.getLength()) == n;
}

sl_bool Time::parseElements(const SafeString16& _time, sl_int32* outArrayYMDHMS)
{
	String time = _time;
	sl_size n = time.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _Time_parse(outArrayYMDHMS, time.getData(), 0, time.getLength()) == n;
}

sl_bool Time::parseElements(const sl_char8* time, sl_int32* outArrayYMDHMS)
{
	sl_reg ret = _Time_parse(outArrayYMDHMS, time, 0, SLIB_SIZE_MAX);
	if (ret != SLIB_PARSE_ERROR && time[ret] == 0) {
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parseElements(const sl_char16* time, sl_int32* outArrayYMDHMS)
{
	sl_reg ret = _Time_parse(outArrayYMDHMS, time, 0, SLIB_SIZE_MAX);
	if (ret != SLIB_PARSE_ERROR && time[ret] == 0) {
		return sl_true;
	}
	return sl_false;
}

sl_reg Time::parse(Time* _out, const sl_char8* sz, sl_size posBegin, sl_size len)
{
	sl_int32 t[6];
	sl_reg ret = parseElements(t, sz, posBegin, len);
	if (ret != SLIB_PARSE_ERROR) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
	}
	return ret;
}

sl_reg Time::parse(Time* _out, const sl_char16* sz, sl_size posBegin, sl_size len)
{
	sl_int32 t[6];
	sl_reg ret = parseElements(t, sz, posBegin, len);
	if (ret != SLIB_PARSE_ERROR) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
	}
	return ret;
}

sl_bool Time::parse(const String8& str, Time* _out)
{
	sl_int32 t[6];
	if (parseElements(str, t)) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parse(const String16& str, Time* _out)
{
	sl_int32 t[6];
	if (parseElements(str, t)) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parse(const SafeString8& str, Time* _out)
{
	sl_int32 t[6];
	if (parseElements(str, t)) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parse(const SafeString16& str, Time* _out)
{
	sl_int32 t[6];
	if (parseElements(str, t)) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parse(const sl_char8* str, Time* _out)
{
	sl_int32 t[6];
	if (parseElements(str, t)) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parse(const sl_char16* str, Time* _out)
{
	sl_int32 t[6];
	if (parseElements(str, t)) {
		if (_out) {
			_out->setElements(t[0], t[1], t[2], t[3], t[4], t[5]);
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool Time::parse(const String8& str)
{
	return parse(str, this);
}

sl_bool Time::parse(const String16& str)
{
	return parse(str, this);
}

sl_bool Time::parse(const SafeString8& str)
{
	return parse(str, this);
}

sl_bool Time::parse(const SafeString16& str)
{
	return parse(str, this);
}

sl_bool Time::parse(const sl_char8* str)
{
	return parse(str, this);
}

sl_bool Time::parse(const sl_char16* str)
{
	return parse(str, this);
}

String Time::format(const String8& fmt) const
{
	return String8::format(fmt, *this);
}

String Time::format(const SafeString8& fmt) const
{
	return String8::format(fmt, *this);
}

String Time::format(const String16& fmt) const
{
	return String16::format(fmt, *this);
}

String Time::format(const SafeString16& fmt) const
{
	return String16::format(fmt, *this);
}

String Time::format(const sl_char8* fmt) const
{
	return String8::format(fmt, *this);
}

String Time::format(const sl_char16* fmt) const
{
	return String16::format(fmt, *this);
}

Time& Time::operator=(const String8& time)
{
	setString(time);
	return *this;
}

Time& Time::operator=(const String16& time)
{
	setString(time);
	return *this;
}

Time& Time::operator=(const SafeString8& time)
{
	setString(time);
	return *this;
}

Time& Time::operator=(const SafeString16& time)
{
	setString(time);
	return *this;
}

Time& Time::operator=(const sl_char8* time)
{
	setString(time);
	return *this;
}

Time& Time::operator=(const sl_char16* time)
{
	setString(time);
	return *this;
}

sl_bool Time::operator==(const Time& other) const
{
	return m_time == other.m_time;
}

sl_bool Time::operator<=(const Time& other) const
{
	return m_time <= other.m_time;
}

sl_bool Time::operator>=(const Time& other) const
{
	return m_time >= other.m_time;
}

sl_bool Time::operator!=(const Time& other) const
{
	return m_time != other.m_time;
}

sl_bool Time::operator<(const Time& other) const
{
	return m_time < other.m_time;
}

sl_bool Time::operator>(const Time& other) const
{
	return m_time > other.m_time;
}

Time Time::operator+(sl_int64 time) const
{
	return m_time + time;
}

Time Time::operator+(const Time& time) const
{
	return m_time + time.m_time;
}

Time& Time::operator+=(sl_int64 time)
{
	m_time += time;
	return *this;
}

Time& Time::operator+=(const Time& time)
{
	m_time += time.m_time;
	return *this;
}

Time Time::operator-(sl_int64 time) const
{
	return m_time - time;
}

Time Time::operator-(const Time& time) const
{
	return m_time - time.m_time;
}

Time& Time::operator-=(sl_int64 time)
{
	m_time -= time;
	return *this;
}

Time& Time::operator-=(const Time& time)
{
	m_time -= time.m_time;
	return *this;
}

SLIB_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

SLIB_NAMESPACE_BEGIN

void Time::getDate(DATE* date) const
{
	SYSTEMTIME st;
	sl_int64 n = (m_time + SLIB_INT64(11644473600000000)) * 10;  // Convert 1970 Based (time_t mode) to 1601 Based (FILETIME mode)
	FileTimeToSystemTime((PFILETIME)&n, &st);
	date->year = st.wYear;
	date->month = st.wMonth;
	date->day = st.wDay;
	date->dayOfWeek = st.wDayOfWeek;
}

void Time::setElements(int year, int month, int day, int hour, int minute, int second)
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

void Time::setNow()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sl_int64 n;
	SystemTimeToFileTime(&st, (PFILETIME)&n);
	m_time = n / 10 - SLIB_INT64(11644473600000000); // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
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

void Time::getDate(DATE* date) const
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

void Time::setElements(int year, int month, int day, int hour, int minute, int second)
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
