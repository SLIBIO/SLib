/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
