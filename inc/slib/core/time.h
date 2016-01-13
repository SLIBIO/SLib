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

class SLIB_EXPORT Time
{
public:
	SLIB_INLINE Time()
	{
	}
	
	SLIB_INLINE Time(Time&& other)
	{
		m_time = other.m_time;
	}

	SLIB_INLINE Time(const Time& other)
	{
		m_time = other.m_time;
	}
	
	SLIB_INLINE Time(sl_int64 time)
	{
		m_time = time;
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

public:
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
	
	SLIB_INLINE sl_bool isZero() const
	{
		return m_time == 0;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return m_time != 0;
	}
	
	SLIB_INLINE sl_int64 toInt() const
	{
		return m_time;
	}

	SLIB_INLINE void set(sl_int64 time)
	{
		m_time = time;
	}

	SLIB_INLINE void set(const Time& other)
	{
		m_time = other.m_time;
	}
	
	SLIB_INLINE void add(sl_int64 time)
	{
		m_time += time;
	}
	
	SLIB_INLINE void add(const Time& other)
	{
		m_time += other.m_time;
	}
	
public:
	SLIB_INLINE Time& operator=(Time&& other)
	{
		m_time = other.m_time;
		return *this;
	}
	
	SLIB_INLINE Time& operator=(const Time& other)
	{
		m_time = other.m_time;
		return *this;
	}
	
	SLIB_INLINE Time& operator=(sl_int64 time)
	{
		m_time = time;
		return *this;
	}
	
	SLIB_INLINE Time& operator=(const String& time)
	{
		set(time);
		return *this;
	}

public:
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

public:
	SLIB_INLINE Time operator+(sl_int64 time) const
	{
		return m_time + time;
	}
	
	SLIB_INLINE Time operator+(const Time& time) const
	{
		return m_time + time.m_time;
	}
	
	SLIB_INLINE Time& operator+=(sl_int64 time)
	{
		m_time += time;
		return *this;
	}
	
	SLIB_INLINE Time& operator+=(const Time& time)
	{
		m_time += time.m_time;
		return *this;
	}

public:
	SLIB_INLINE Time operator-(sl_int64 time) const
	{
		return m_time - time;
	}
	
	SLIB_INLINE Time operator-(const Time& time) const
	{
		return m_time - time.m_time;
	}
	
	SLIB_INLINE Time& operator-=(sl_int64 time)
	{
		m_time -= time;
		return *this;
	}
	
	SLIB_INLINE Time& operator-=(const Time& time)
	{
		m_time -= time.m_time;
		return *this;
	}

public:
	void get(DATE* date) const;
	
	void set(int year, int month, int date, int hour, int minute, int second);
	
	void setNow();
	
	void setToSystem();

	void set(int year, int month, int day);
	
	void set(const String& str);
	
	void setYear(int year);
	
	void setMonth(int month);
	
	void setDay(int day);
	
	void setDayf(double day);
	
	void setHour(int hour);
	
	void setHourf(double hour);
	
	void setMinute(int minute);
	
	void setMinutef(double minute);
	
	void setSecond(int second);
	
	void setSecondf(double second);
	
	void setMillisecond(int millis);
	
	void setMillisecondf(double millis);
	
	void setMicrosecond(int micros);
	
	void setMicrosecondf(double micros);
	
	void setDayOfWeek(int day);
	
	void setDayOfYear(int day);
	
	void setDaysCount(sl_int64 days);
	
	void setDaysCountf(double days);
	
	void setHoursCount(sl_int64 hours);
	
	void setHoursCountf(double hours);
	
	void setMinutesCount(sl_int64 minutes);
	
	void setMinutesCountf(double minutes);
	
	void setSecondsCount(sl_int64 seconds);
	
	void setSecondsCountf(double seconds);
	
	void setMillisecondsCount(sl_int64 milis);
	
	void setMillisecondsCountf(double milis);
	
	void setMicrosecondsCount(sl_int64 micros);
	
	void setMicrosecondsCountf(double micros);

	
	int getYear() const;
	
	int getMonth() const;
	
	int getDay() const;
	
	double getDayf() const;
	
	int getHour() const;
	
	double getHourf() const;
	
	int getMinute() const;
	
	double getMinutef() const;
	
	int getSecond() const;
	
	double getSecondf() const;
	
	int getMillisecond() const;
	
	double getMillisecondf() const;
	
	int getMicrosecond() const;
	
	double getMicrosecondf() const;

	
	int getDayOfWeek() const;
	
	int getDayOfYear() const;

	
	sl_int64 getDaysCount() const;
	
	double getDaysCountf() const;
	
	sl_int64 getHoursCount() const;
	
	double getHoursCountf() const;
	
	sl_int64 getMinutesCount() const;
	
	double getMinutesCountf() const;
	
	sl_int64 getSecondsCount() const;
	
	double getSecondsCountf() const;
	
	sl_int64 getMillisecondsCount() const;
	
	double getMillisecondsCountf() const;
	
	sl_int64 getMicrosecondsCount() const;
	
	double getMicrosecondsCountf() const;

	
	int getQuarter() const;

	String getWeekday(sl_bool flagShort = sl_true) const;

	
	int getDaysCountInMonth() const;
	
	int getDaysCountInYear() const;


	void addYears(int years);
	
	void addMonths(int months);
	
	void addDays(sl_int64 days);
	
	void addDaysf(double days);
	
	void addHours(sl_int64 hours);
	
	void addHoursf(double hours);
	
	void addMinutes(sl_int64 minutes);
	
	void addMinutesf(double minutes);
	
	void addSeconds(sl_int64 seconds);
	
	void addSecondsf(double seconds);
	
	void addMilliseconds(sl_int64 millis);
	
	void addMillisecondsf(double millis);
	
	void addMicroseconds(sl_int64 micros);
	
	void addMicrosecondsf(double micros);

	
	Time getTimeOnly() const;

	
	String format(const String& fmt) const;
	
	static void parseTime(const String& date, int* out);

	
	String toString() const;
	
	SLIB_INLINE static Time parse(const String& str)
	{
		return str;
	}
	
private:
	sl_int64 m_time; // microseconds
};


class SLIB_EXPORT TimeCounter
{
public:
	TimeCounter();

public:
	void reset();

	void update();

	sl_uint64 getEllapsedMilliseconds() const;

	Time getRelative() const;

protected:
	Time m_timeStart;
	sl_uint64 m_relStart;
	
};

SLIB_NAMESPACE_END

#endif
