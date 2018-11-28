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
#include "parse.h"

/*******************************************************
 Time contains an integral value holding the number of
 microseconds since 00:00, Jan 1 1970 UTC, corresponding
 to POSIX time (seconds).
*******************************************************/

namespace slib
{

	struct SLIB_EXPORT DATE
	{
		int year;
		int month;
		int day;
		int dayOfWeek;
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

		Time(int year, int month, int date) noexcept;

		Time(int year, int month, int date, int hour, int minute, int second) noexcept;

		Time(const String& str) noexcept;

		Time(const String16& str) noexcept;

		Time(const AtomicString& str) noexcept;

		Time(const AtomicString16& str) noexcept;

		Time(const sl_char8* str) noexcept;

		Time(const sl_char16* str) noexcept;
	
	public:
		static Time now() noexcept;

		SLIB_INLINE constexpr static Time zero()
		{
			return 0;
		}

		void setZero() noexcept;

		SLIB_INLINE constexpr sl_bool isZero() const
		{
			return m_time == 0;
		}

		SLIB_INLINE constexpr sl_bool isNotZero() const
		{
			return m_time != 0;
		}

		sl_int64 toInt() const noexcept;
	
		void setInt(sl_int64 time) noexcept;

		void add(sl_int64 time) noexcept;

		void add(const Time& other) noexcept;
	
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
		void setElements(int year, int month, int date, int hour, int minute, int second) noexcept;

		void setNow() noexcept;

		void setToSystem() noexcept;

		void getDate(DATE* date) const noexcept;

		void setDate(int year, int month, int day) noexcept;

		int getYear() const noexcept;

		void setYear(int year) noexcept;

		void addYears(int years) noexcept;

		int getMonth() const noexcept;

		void setMonth(int month) noexcept;

		void addMonths(int months) noexcept;

		int getDay() const noexcept;

		void setDay(int day) noexcept;

		void addDays(sl_int64 days) noexcept;

		double getDayf() const noexcept;

		void setDayf(double day) noexcept;

		void addDaysf(double days) noexcept;

		int getHour() const noexcept;

		void setHour(int hour) noexcept;

		void addHours(sl_int64 hours) noexcept;

		double getHourf() const noexcept;

		void setHourf(double hour) noexcept;

		void addHoursf(double hours) noexcept;

		int getMinute() const noexcept;

		void setMinute(int minute) noexcept;

		void addMinutes(sl_int64 minutes) noexcept;

		double getMinutef() const noexcept;
	
		void setMinutef(double minute) noexcept;

		void addMinutesf(double minutes) noexcept;

		int getSecond() const noexcept;

		void setSecond(int second) noexcept;

		void addSeconds(sl_int64 seconds) noexcept;

		double getSecondf() const noexcept;

		void setSecondf(double second) noexcept;

		void addSecondsf(double seconds) noexcept;

		int getMillisecond() const noexcept;

		void setMillisecond(int millis) noexcept;

		void addMilliseconds(sl_int64 millis) noexcept;

		double getMillisecondf() const noexcept;

		void setMillisecondf(double millis) noexcept;

		void addMillisecondsf(double millis) noexcept;

		int getMicrosecond() const noexcept;

		void setMicrosecond(int micros) noexcept;

		void addMicroseconds(sl_int64 micros) noexcept;

		double getMicrosecondf() const noexcept;
	
		void setMicrosecondf(double micros) noexcept;

		void addMicrosecondsf(double micros) noexcept;

		int getDayOfWeek() const noexcept;

		void setDayOfWeek(int day) noexcept;

		int getDayOfYear() const noexcept;

		void setDayOfYear(int day) noexcept;

		sl_int64 getDaysCount() const noexcept;

		void setDaysCount(sl_int64 days) noexcept;

		double getDaysCountf() const noexcept;

		void setDaysCountf(double days) noexcept;

		sl_int64 getHoursCount() const noexcept;

		void setHoursCount(sl_int64 hours) noexcept;

		double getHoursCountf() const noexcept;

		void setHoursCountf(double hours) noexcept;

		sl_int64 getMinutesCount() const noexcept;

		void setMinutesCount(sl_int64 minutes) noexcept;

		double getMinutesCountf() const noexcept;

		void setMinutesCountf(double minutes) noexcept;

		sl_int64 getSecondsCount() const noexcept;

		void setSecondsCount(sl_int64 seconds) noexcept;

		double getSecondsCountf() const noexcept;

		void setSecondsCountf(double seconds) noexcept;

		sl_int64 getMillisecondsCount() const noexcept;

		void setMillisecondsCount(sl_int64 milis) noexcept;

		double getMillisecondsCountf() const noexcept;

		void setMillisecondsCountf(double milis) noexcept;

		sl_int64 getMicrosecondsCount() const noexcept;

		void setMicrosecondsCount(sl_int64 micros) noexcept;

		double getMicrosecondsCountf() const noexcept;
	
		void setMicrosecondsCountf(double micros) noexcept;
	

		int getDaysCountInMonth() const noexcept;

		int getDaysCountInYear() const noexcept;

		int getQuarter() const noexcept;
	
		String getWeekday(sl_bool flagShort = sl_true) const noexcept;
	
		Time getTimeOnly() const noexcept;


		String toString() const noexcept;

		String16 toString16() const noexcept;

		String getDateString() const noexcept;

		String16 getDateString16() const noexcept;

		String getTimeString() const noexcept;

		String16 getTimeString16() const noexcept;

		sl_bool setString(const String& str) noexcept;

		sl_bool setString(const String16& str) noexcept;

		sl_bool setString(const AtomicString& str) noexcept;

		sl_bool setString(const AtomicString16& str) noexcept;

		sl_bool setString(const sl_char8* str) noexcept;

		sl_bool setString(const sl_char16* str) noexcept;

		String format(const String& fmt) const noexcept;

		String format(const AtomicString& fmt) const noexcept;

		String format(const String16& fmt) const noexcept;

		String format(const AtomicString16& fmt) const noexcept;

		String format(const sl_char8* fmt) const noexcept;

		String format(const sl_char16* fmt) const noexcept;
	

		static sl_reg parseElements(sl_int32* outArrayYMDHMS, const sl_char8* sz, sl_size posStart = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;

		static sl_reg parseElements(sl_int32* outArrayYMDHMS, const sl_char16* sz, sl_size posStart = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;

		static sl_bool parseElements(const String& time, sl_int32* outArrayYMDHMS) noexcept;

		static sl_bool parseElements(const String16& time, sl_int32* outArrayYMDHMS) noexcept;

		static sl_bool parseElements(const AtomicString& time, sl_int32* outArrayYMDHMS) noexcept;

		static sl_bool parseElements(const AtomicString16& time, sl_int32* outArrayYMDHMS) noexcept;

		static sl_bool parseElements(const sl_char8* time, sl_int32* outArrayYMDHMS) noexcept;

		static sl_bool parseElements(const sl_char16* time, sl_int32* outArrayYMDHMS) noexcept;

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

		/* platform functions */
	protected:
		sl_bool _getDate(DATE* date) const noexcept;

		void _setElements(int year, int month, int date, int hour, int minute, int second) noexcept;

		void _setNow() noexcept;

		void _setToSystem() noexcept;

	};
	
	template <>
	sl_reg Parser<Time, sl_char8>::parse(Time* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<Time, sl_char16>::parse(Time* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	class SLIB_EXPORT TimeCounter
	{
	public:
		TimeCounter() noexcept;

		~TimeCounter() noexcept;
	
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

		~TimeKeeper() noexcept;

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
