#ifndef CHECKHEADER_SLIB_CORE_DETAIL_TIME
#define CHECKHEADER_SLIB_CORE_DETAIL_TIME

#include "../time.h"

namespace slib
{

	SLIB_INLINE void Time::setZero()
	{
		m_time = 0;
	}
	
	SLIB_INLINE sl_int64 Time::toInt() const
	{
		return m_time;
	}
	
	SLIB_INLINE Time& Time::operator=(const Time& other)
	{
		m_time = other.m_time;
		return *this;
	}
	
	SLIB_INLINE Time& Time::operator=(sl_int32 time)
	{
		m_time = time;
		return *this;
	}
	
	SLIB_INLINE Time& Time::operator=(sl_uint32 time)
	{
		m_time = time;
		return *this;
	}
	
	SLIB_INLINE Time& Time::operator=(sl_int64 time)
	{
		m_time = time;
		return *this;
	}
	
	SLIB_INLINE Time& Time::operator=(sl_uint64 time)
	{
		m_time = time;
		return *this;
	}
	
	SLIB_INLINE sl_bool Time::operator==(const Time& other) const
	{
		return m_time == other.m_time;
	}
	
	SLIB_INLINE sl_bool Time::operator<=(const Time& other) const
	{
		return m_time <= other.m_time;
	}
	
	SLIB_INLINE sl_bool Time::operator>=(const Time& other) const
	{
		return m_time >= other.m_time;
	}
	
	SLIB_INLINE sl_bool Time::operator!=(const Time& other) const
	{
		return m_time != other.m_time;
	}
	
	SLIB_INLINE sl_bool Time::operator<(const Time& other) const
	{
		return m_time < other.m_time;
	}
	
	SLIB_INLINE sl_bool Time::operator>(const Time& other) const
	{
		return m_time > other.m_time;
	}
	
	SLIB_INLINE Time Time::operator+(sl_int64 time) const
	{
		return m_time + time;
	}
	
	SLIB_INLINE Time Time::operator+(const Time& time) const
	{
		return m_time + time.m_time;
	}
	
	SLIB_INLINE Time& Time::operator+=(sl_int64 time)
	{
		m_time += time;
		return *this;
	}
	
	SLIB_INLINE Time& Time::operator+=(const Time& time)
	{
		m_time += time.m_time;
		return *this;
	}
	
	SLIB_INLINE Time Time::operator-(sl_int64 time) const
	{
		return m_time - time;
	}
	
	SLIB_INLINE Time Time::operator-(const Time& time) const
	{
		return m_time - time.m_time;
	}
	
	SLIB_INLINE Time& Time::operator-=(sl_int64 time)
	{
		m_time -= time;
		return *this;
	}
	
	SLIB_INLINE Time& Time::operator-=(const Time& time)
	{
		m_time -= time.m_time;
		return *this;
	}

}

#endif
