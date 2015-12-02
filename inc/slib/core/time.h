#ifndef CHECKHEADER_SLIB_CORE_TIME
#define CHECKHEADER_SLIB_CORE_TIME

#include "definition.h"
#include "string.h"

SLIB_NAMESPACE_BEGIN

struct SLIB_EXPORT DATE {
	int year;
	int month;
	int day;
	int dayOfWeek;
};

class SLIB_EXPORT Time {
private:
	sl_int64 m_time; // microseconds
public:
	/* Constructors & De-constructor */
	SLIB_INLINE Time() {}
	SLIB_INLINE ~Time() {}

	SLIB_INLINE Time(const Time& other)
	{
		set(other.m_time);
	}
	SLIB_INLINE Time(sl_int64 time)
	{
		set(time);
	}

	SLIB_INLINE Time(int year, int month, int date)
	{
		set(year, month, date, 0, 0, 0);
	}
	SLIB_INLINE Time(int year, int month, int date, int hour, int minute, int second)
	{
		set(year, month, date, hour, minute, second);
	}
	SLIB_INLINE Time(const String& str)
	{
		set(str);
	}

	/* Static Functions */
	SLIB_INLINE static Time now()
	{
		Time ret;
		ret.setNow();
		return ret;
	}
	SLIB_INLINE static Time zero()
	{
		return 0;
	}
	SLIB_INLINE void setZero()
	{
		m_time = 0;
	}

	/* Operators */
	SLIB_INLINE Time& operator=(const Time& other)
	{
		set(other.m_time);
		return *this;
	}
	SLIB_INLINE Time& operator=(sl_int64 time)
	{
		set(time);
		return *this;
	}
	SLIB_INLINE Time& operator=(const String& time)
	{
		set(time);
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const Time& other) const
	{
		return m_time == other.m_time;
	}
	SLIB_INLINE sl_bool operator<=(const Time& other) const
	{
		return m_time <= other.m_time;
	}
	SLIB_INLINE sl_bool operator>=(const Time& other) const
	{
		return m_time >= other.m_time;
	}
	SLIB_INLINE sl_bool operator!=(const Time& other) const
	{
		return m_time != other.m_time;
	}
	SLIB_INLINE sl_bool operator<(const Time& other) const
	{
		return m_time < other.m_time;
	}
	SLIB_INLINE sl_bool operator>(const Time& other) const
	{
		return m_time > other.m_time;
	}

	SLIB_INLINE sl_int64 toInt() const
	{
		return m_time;
	}

	SLIB_INLINE Time operator+(sl_int64 time)
	{
		Time ret = *this;
		ret.add(time);
		return ret;
	}
	SLIB_INLINE Time operator+(Time time)
	{
		return m_time + time.m_time;
	}
	SLIB_INLINE Time operator-(sl_int64 time)
	{
		Time ret = *this;
		ret.add(-time);
		return ret;
	}
	SLIB_INLINE Time operator-(Time time)
	{
		return m_time - time.m_time;
	}
	SLIB_INLINE Time& operator+=(sl_int64 time)
	{
		add(time);
		return *this;
	}
	SLIB_INLINE Time& operator-=(sl_int64 time)
	{
		add(-time);
		return *this;
	}

	/* Attributes */
	SLIB_INLINE sl_bool isZero() const
	{
		return (m_time == 0);
	}
	SLIB_INLINE sl_bool isNotZero() const
	{
		return (m_time != 0);
	}

	SLIB_INLINE void set(Time& other)
	{
		m_time = other.m_time;
	}
	SLIB_INLINE void set(sl_int64 time)
	{
		m_time = time;
	}

	void get(DATE* date) const;
	void set(int year, int month, int date, int hour, int minute, int second);
	void setNow();
	void setToSystem();

	void set(int year, int month, int day);
	void set(const String& str);
	void setYear(int year);
	void setMonth(int month);
	void setDay(int day);
	void setHour(int hour);
	void setMinute(int minute);
	void setSecond(int second);
	void setMillisecond(int millis);
	void setDayOfWeek(int day);
	void setDayOfYear(int day);
	void setDaysCount(sl_int64 count);
	void setHoursCount(sl_int64 count);
	void setMinutesCount(sl_int64 count);
	void setSecondsCount(sl_int64 count);
	void setMillisecondsCount(sl_int64 count);

	int getYear() const;
	int getMonth() const;
	int getDay() const;
	int getHour() const;
	int getMinute() const;
	int getSecond() const;
	int getMillisecond() const;
	int getDayOfWeek() const;
	int getDayOfYear() const;
	sl_int64 getDaysCount() const;
	sl_int64 getHoursCount() const;
	sl_int64 getMinutesCount() const;
	sl_int64 getSecondsCount() const;
	sl_int64 getMillisecondsCount() const;
	int getQuarter() const;

	String getWeekday(sl_bool flagShort = sl_true) const;

	int getDaysCountInMonth() const;
	int getDaysCountInYear() const;

	SLIB_INLINE void add(sl_int64 time)
	{
		m_time += time;
	}
	void addYear(int year);
	void addMonth(int month);
	void addDay(int day);
	void addHour(int hour);
	void addMinute(int minute);
	void addSecond(int second);
	void addMillisecond(int millis);

	Time getTimeOnly() const;

	String format(const String& fmt) const;
	static void parseTime(const String& date, int* out);

	String toString() const;
	static Time parse(const String& str)
	{
		Time ret(str);
		return ret;
	}
};
SLIB_NAMESPACE_END

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT TimeCounter
{
protected:
	Time m_timeStart;
	sl_uint64 m_relStart;

public:
	TimeCounter();

public:
	void reset();

	void update();

	sl_uint64 getEllapsedMilliseconds() const;

	Time getRelative() const;

};

SLIB_NAMESPACE_END

#endif// CHECKHEADER_SLIB_CORE_TIME
