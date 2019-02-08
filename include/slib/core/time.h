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

#ifndef CHECKHEADER_SLIB_CORE_TIME
#define CHECKHEADER_SLIB_CORE_TIME

#include "definition.h"

#include "string.h"
#include "object.h"
#include "parse.h"
#include "locale.h"

/*******************************************************
 Time contains an integral value holding the number of
 microseconds since 00:00, Jan 1 1970 UTC, corresponding
 to POSIX time (seconds).
*******************************************************/

namespace slib
{
	
	class Time;
	class TimeZone;
	
	class CTimeZone: public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CTimeZone() noexcept;
		
		~CTimeZone() noexcept;
		
	public:
		// In seconds
		sl_int64 getOffset();
		
		// In seconds
		virtual sl_int64 getOffset(const Time& time) = 0;
		
	};
	
	class GenericTimeZone : public CTimeZone
	{
		SLIB_DECLARE_OBJECT
		
	public:
		GenericTimeZone(sl_int64 offsetSeconds = 0);
		
		~GenericTimeZone();
		
	public:
		using CTimeZone::getOffset;
		
		sl_int64 getOffset(const Time& time) override;
		
	protected:
		sl_int64 m_offset;
		
	};
	
	
	template <>
	class SLIB_EXPORT Atomic<TimeZone>
	{
		SLIB_ATOMIC_REF_WRAPPER(CTimeZone)
		
	public:
		sl_bool isLocal() const noexcept;
		
		sl_bool isUTC() const noexcept;
		
		// In seconds
		sl_int64 getOffset() const;
		
		// In seconds
		sl_int64 getOffset(const Time& time) const;
		
	public:
		AtomicRef<CTimeZone> ref;
		
	};
	
	typedef Atomic<TimeZone> AtomicTimeZone;
	
	class TimeZone
	{
		SLIB_REF_WRAPPER(TimeZone, CTimeZone)
		
	public:
		static const TimeZone& Local;
		
		static const TimeZone& UTC() noexcept;
		
		static TimeZone create(sl_int64 offset) noexcept;
		
	public:
		sl_bool isLocal() const noexcept;
		
		sl_bool isUTC() const noexcept;

		// In seconds
		sl_int64 getOffset() const;
		
		// In seconds
		sl_int64 getOffset(const Time& time) const;

	public:
		Ref<CTimeZone> ref;
		
	};
	
	enum class TimeTextType
	{
		Long = 0,
		Short = 1,
		SingleChar = 2
	};

	class SLIB_EXPORT TimeComponents
	{
	public:
		sl_int32 year;
		sl_uint8 month;
		sl_uint8 day;
		sl_uint8 dayOfWeek;
		sl_uint8 hour;
		sl_uint8 minute;
		sl_uint8 second;
		sl_uint16 milliseconds;
		sl_uint16 microseconds;
		
	public:
		TimeComponents() noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TimeComponents)
		
	};
	
	class SLIB_EXPORT Time
	{
	protected:
		sl_int64 m_time; // microseconds

	public:
		SLIB_INLINE constexpr Time() noexcept: m_time(0) {}

		SLIB_INLINE constexpr Time(const Time& other) noexcept: m_time(other.m_time) {}

		SLIB_INLINE constexpr Time(sl_int32 time) noexcept: m_time(time) {}

		SLIB_INLINE constexpr Time(sl_uint32 time) noexcept: m_time(time) {}

		SLIB_INLINE constexpr Time(sl_int64 time) noexcept: m_time(time) {}

		SLIB_INLINE constexpr Time(sl_uint64 time) noexcept: m_time(time) {}

		Time(sl_int32 year, sl_int32 month, sl_int32 date, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(sl_int32 year, sl_int32 month, sl_int32 date, sl_int32 hour, sl_int32 minute, sl_int32 second, sl_int32 milliseconds = 0, sl_int32 microseconds = 0, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const TimeComponents& comps, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const String& str, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const String16& str, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const AtomicString& str, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const AtomicString16& str, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const sl_char8* str, const TimeZone& zone = TimeZone::Local) noexcept;

		Time(const sl_char16* str, const TimeZone& zone = TimeZone::Local) noexcept;
	
	public:
		static Time now() noexcept;

		SLIB_INLINE constexpr static Time zero()
		{
			return 0;
		}

		Time& setZero() noexcept;

		SLIB_INLINE constexpr sl_bool isZero() const
		{
			return m_time == 0;
		}

		SLIB_INLINE constexpr sl_bool isNotZero() const
		{
			return m_time != 0;
		}

		sl_int64 toInt() const noexcept;
	
		Time& setInt(sl_int64 time) noexcept;
		
		static Time fromInt(sl_int64 time) noexcept;
		
		sl_int64 toUnixTime() const noexcept;

		Time& setUnixTime(sl_int64 time) noexcept;
		
		static Time fromUnixTime(sl_int64 time) noexcept;
		
		Time& add(sl_int64 time) noexcept;

		Time& add(const Time& other) noexcept;
	
	public:
		Time& operator=(const Time& other) noexcept;

		Time& operator=(sl_int32 time) noexcept;

		Time& operator=(sl_uint32 time) noexcept;

		Time& operator=(sl_int64 time) noexcept;

		Time& operator=(sl_uint64 time) noexcept;

		Time& operator=(const String& str) noexcept;

		Time& operator=(const String16& str) noexcept;

		Time& operator=(const AtomicString& str) noexcept;

		Time& operator=(const AtomicString16& str) noexcept;

		Time& operator=(const sl_char8* str) noexcept;

		Time& operator=(const sl_char16* str) noexcept;
	

		sl_bool operator==(const Time& other) const noexcept;

		sl_bool operator<=(const Time& other) const noexcept;

		sl_bool operator>=(const Time& other) const noexcept;

		sl_bool operator!=(const Time& other) const noexcept;

		sl_bool operator<(const Time& other) const noexcept;

		sl_bool operator>(const Time& other) const noexcept;
	

		Time operator+(sl_int64 time) const noexcept;

		Time operator+(const Time& time) const noexcept;

		Time& operator+=(sl_int64 time) noexcept;

		Time& operator+=(const Time& time) noexcept;
	

		Time operator-(sl_int64 time) const noexcept;

		Time operator-(const Time& time) const noexcept;

		Time& operator-=(sl_int64 time) noexcept;

		Time& operator-=(const Time& time) noexcept;

	public:
		Time& setNow() noexcept;

		static sl_bool setSystemTime(const Time& time) noexcept;

		void get(TimeComponents& output, const TimeZone& zone = TimeZone::Local) const noexcept;
		
		void getUTC(TimeComponents& output) const noexcept;
		
		Time& set(const TimeComponents& comps, const TimeZone& zone = TimeZone::Local) noexcept;
		
		Time& setUTC(const TimeComponents& comps) noexcept;

		Time& set(sl_int32 year, sl_int32 month, sl_int32 date, sl_int32 hour = 0, sl_int32 minute = 0, sl_int32 second = 0, sl_int32 milliseconds = 0, sl_int32 microseconds = 0, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& setUTC(sl_int32 year, sl_int32 month, sl_int32 date, sl_int32 hour = 0, sl_int32 minute = 0, sl_int32 second = 0, sl_int32 milliseconds = 0, sl_int32 microseconds = 0) noexcept;

		Time& setDate(sl_int32 year, sl_int32 month, sl_int32 day, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_int32 getYear(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setYear(sl_int32 year, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addYears(sl_int32 years, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_int32 getMonth(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setMonth(sl_int32 month, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addMonths(sl_int32 months, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_int32 getDay(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setDay(sl_int32 day, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addDays(sl_int64 days) noexcept;

		double getDayf(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setDayf(double day, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addDaysf(double days) noexcept;

		sl_int32 getHour(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setHour(sl_int32 hour, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addHours(sl_int64 hours) noexcept;

		double getHourf(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setHourf(double hour, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addHoursf(double hours) noexcept;

		sl_int32 getMinute(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setMinute(sl_int32 minute, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addMinutes(sl_int64 minutes) noexcept;

		double getMinutef(const TimeZone& zone = TimeZone::Local) const noexcept;
	
		Time& setMinutef(double minute, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addMinutesf(double minutes) noexcept;

		sl_int32 getSecond(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setSecond(sl_int32 second, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addSeconds(sl_int64 seconds) noexcept;

		double getSecondf(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setSecondf(double second, const TimeZone& zone = TimeZone::Local) noexcept;

		Time& addSecondsf(double seconds) noexcept;

		sl_int32 getMillisecond() const noexcept;

		Time& setMillisecond(sl_int32 millis) noexcept;

		Time& addMilliseconds(sl_int64 millis) noexcept;

		double getMillisecondf() const noexcept;

		Time& setMillisecondf(double millis) noexcept;

		Time& addMillisecondsf(double millis) noexcept;

		sl_int32 getMicrosecond() const noexcept;

		Time& setMicrosecond(sl_int32 micros) noexcept;

		Time& addMicroseconds(sl_int64 micros) noexcept;

		double getMicrosecondf() const noexcept;
	
		Time& setMicrosecondf(double micros) noexcept;

		Time& addMicrosecondsf(double micros) noexcept;

		sl_int32 getDayOfWeek(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setDayOfWeek(sl_int32 day, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_int32 getDayOfYear(const TimeZone& zone = TimeZone::Local) const noexcept;

		Time& setDayOfYear(sl_int32 day, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_int64 getDaysCount() const noexcept;

		Time& setDaysCount(sl_int64 days) noexcept;

		double getDaysCountf() const noexcept;

		Time& setDaysCountf(double days) noexcept;

		sl_int64 getHoursCount() const noexcept;

		Time& setHoursCount(sl_int64 hours) noexcept;

		double getHoursCountf() const noexcept;

		Time& setHoursCountf(double hours) noexcept;

		sl_int64 getMinutesCount() const noexcept;

		Time& setMinutesCount(sl_int64 minutes) noexcept;

		double getMinutesCountf() const noexcept;

		Time& setMinutesCountf(double minutes) noexcept;

		sl_int64 getSecondsCount() const noexcept;

		Time& setSecondsCount(sl_int64 seconds) noexcept;

		double getSecondsCountf() const noexcept;

		Time& setSecondsCountf(double seconds) noexcept;

		sl_int64 getMillisecondsCount() const noexcept;

		Time& setMillisecondsCount(sl_int64 milis) noexcept;

		double getMillisecondsCountf() const noexcept;

		Time& setMillisecondsCountf(double milis) noexcept;

		sl_int64 getMicrosecondsCount() const noexcept;

		Time& setMicrosecondsCount(sl_int64 micros) noexcept;

		double getMicrosecondsCountf() const noexcept;
	
		Time& setMicrosecondsCountf(double micros) noexcept;
	
		// In Seconds
		sl_int64 getLocalTimeOffset() const noexcept;

		sl_int32 getDaysCountInMonth(const TimeZone& zone = TimeZone::Local) const noexcept;

		sl_int32 getDaysCountInYear(const TimeZone& zone = TimeZone::Local) const noexcept;

		sl_int32 getQuarter(const TimeZone& zone = TimeZone::Local) const noexcept;
		
		Time getTimeOnly(const TimeZone& zone = TimeZone::Local) const noexcept;

		
		static String getWeekdayText(sl_int32 weekday, TimeTextType type, const Locale& locale) noexcept;

		String getWeekdayShort(const TimeZone& zone, const Locale& locale = Locale::Unknown) const noexcept;
		
		String getWeekdayShort(const Locale& locale = Locale::Unknown) const noexcept;

		String getWeekdayLong(const TimeZone& zone, const Locale& locale = Locale::Unknown) const noexcept;
		
		String getWeekdayLong(const Locale& locale = Locale::Unknown) const noexcept;
		
		String getWeekday(const TimeZone& zone, const Locale& locale = Locale::Unknown) const noexcept;
		
		String getWeekday(const Locale& locale = Locale::Unknown) const noexcept;

		
		// English
		static String getMonthText(sl_int32 month, TimeTextType type) noexcept;
		
		String getMonthShort(const TimeZone& zone = TimeZone::Local) const noexcept;
		
		String getMonthLong(const TimeZone& zone = TimeZone::Local) const noexcept;
		
		
		// IMF-fixdate
		String toHttpDate() const noexcept;
		
		static sl_reg parseHttpDate(Time* _output, const sl_char8* buf, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		sl_bool parseHttpDate(const String& date) noexcept;
		
		
		String toString(const TimeZone& zone = TimeZone::Local) const noexcept;

		String16 toString16(const TimeZone& zone = TimeZone::Local) const noexcept;

		String getDateString(const TimeZone& zone = TimeZone::Local) const noexcept;

		String16 getDateString16(const TimeZone& zone = TimeZone::Local) const noexcept;

		String getDateString(sl_char8 delimiter, const TimeZone& zone = TimeZone::Local) const noexcept;
		
		String16 getDateString16(sl_char16 delimiter, const TimeZone& zone = TimeZone::Local) const noexcept;

		String getTimeString(const TimeZone& zone = TimeZone::Local) const noexcept;

		String16 getTimeString16(const TimeZone& zone = TimeZone::Local) const noexcept;
		

		sl_bool setString(const String& str, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_bool setString(const String16& str, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_bool setString(const AtomicString& str, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_bool setString(const AtomicString16& str, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_bool setString(const sl_char8* str, const TimeZone& zone = TimeZone::Local) noexcept;

		sl_bool setString(const sl_char16* str, const TimeZone& zone = TimeZone::Local) noexcept;

		String format(const String& fmt) const noexcept;

		String format(const AtomicString& fmt) const noexcept;

		String format(const String16& fmt) const noexcept;

		String format(const AtomicString16& fmt) const noexcept;

		String format(const sl_char8* fmt) const noexcept;

		String format(const sl_char16* fmt) const noexcept;
	
		template <class ST>
		static sl_bool parse(const ST& str, TimeComponents* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		static sl_bool parse(const ST& str, Time* _out) noexcept
		{
			return Parse(str, _out);
		}

		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}
		
		template <class ST>
		static sl_bool parse(const ST& str, const TimeZone& zone, Time* _out) noexcept
		{
			return Parse(str, zone, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str, const TimeZone& zone) noexcept
		{
			return Parse(str, zone, this);
		}
		
		/* platform functions */
	protected:
		sl_bool _get(TimeComponents& output, sl_bool flagUTC) const noexcept;

		static sl_int64 _set(sl_int32 year, sl_int32 month, sl_int32 date, sl_int32 hour, sl_int32 minute, sl_int32 second, sl_bool flagUTC) noexcept;

		void _setNow() noexcept;

		sl_bool _setToSystem() const noexcept;

	};
	
	template <>
	sl_reg Parser<TimeComponents, sl_char8>::parse(TimeComponents* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<TimeComponents, sl_char16>::parse(TimeComponents* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<Time, sl_char8>::parse(Time* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<Time, sl_char16>::parse(Time* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser2<Time, sl_char8, TimeZone>::parse(Time* _out, const TimeZone& zone, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser2<Time, sl_char16, TimeZone>::parse(Time* _out, const TimeZone& zone, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;

	class SLIB_EXPORT TimeCounter
	{
	public:
		TimeCounter() noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TimeCounter)
	
	public:
		Time getTime() const noexcept;

		Time getTime(const Time& current) const noexcept;
	
		sl_uint64 getElapsedMilliseconds() const noexcept;

		sl_uint64 getElapsedMilliseconds(const Time& current) const noexcept;
	
		void reset() noexcept;

		void reset(const Time& current) noexcept;

		void update() noexcept;

		void update(const Time& current) noexcept;

	protected:
		Time m_timeLast;
		Time m_timeElapsed;

	};
	
	
	class SLIB_EXPORT TimeKeeper
	{
	public:
		TimeKeeper() noexcept;

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TimeKeeper)

	public:
		void start() noexcept;

		void start(const Time& current) noexcept;

		void startAndSetTime(const Time& initialTimeValue) noexcept;

		void startAndSetTime(const Time& initialTimeValue, const Time& current) noexcept;

		void restart() noexcept;

		void restart(const Time& current) noexcept;

		void restartAndSetTime(const Time& initialTimeValue) noexcept;

		void restartAndSetTime(const Time& initialTimeValue, const Time& current) noexcept;
	
		void stop() noexcept;

		void resume() noexcept;

		void resume(const Time& current) noexcept;

		void pause() noexcept;

		void pause(const Time& current) noexcept;
	
		Time getTime() const noexcept;

		Time getTime(const Time& current) const noexcept;

		void setTime(const Time& time) noexcept;

		void setTime(const Time& time, const Time& current) noexcept;

		void update() noexcept;

		void update(const Time& current) noexcept;

		sl_bool isStarted() const noexcept;

		sl_bool isStopped() const noexcept;

		sl_bool isRunning() const noexcept;

		sl_bool isNotRunning() const noexcept;
	
		sl_bool isPaused() const noexcept;
	
	protected:
		sl_bool m_flagStarted;
		sl_bool m_flagRunning;
		Time m_timeLast;
		Time m_timeElapsed;
		SpinLock m_lock;

	};

}

#include "detail/time.inc"

#endif
