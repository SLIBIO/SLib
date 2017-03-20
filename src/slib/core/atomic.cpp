/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/atomic.h"

#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/time.h"
#include "../../../inc/slib/core/system.h"

namespace slib
{

	Atomic<sl_int32>::Atomic()
	{
	}

	Atomic<sl_int32>::Atomic(sl_int32 value)
	: m_value(value)
	{
	}

	sl_int32 Atomic<sl_int32>::operator=(sl_int32 value)
	{
		m_value = value;
		return m_value;
	}

	Atomic<sl_int32>::operator sl_int32 () const
	{
		return m_value;
	}

	sl_int32 Atomic<sl_int32>::increase()
	{
		return Base::interlockedIncrement32((sl_int32*)&m_value);
	}

	sl_int32 Atomic<sl_int32>::decrease()
	{
		return Base::interlockedDecrement32((sl_int32*)&m_value);
	}

	sl_int32 Atomic<sl_int32>::add(sl_int32 other)
	{
		return Base::interlockedAdd32((sl_int32*)&m_value, other);
	}

	sl_bool Atomic<sl_int32>::waitZero(sl_int32 timeout)
	{
		TimeCounter t;
		sl_uint32 count = 0;
		while (timeout < 0 || t.getElapsedMilliseconds() < (sl_uint64)(timeout)) {
			if (Base::interlockedCompareExchange32((sl_int32*)&m_value, 0, 0)) {
				return sl_true;
			}
			System::yield(count);
			count++;
		}
		return sl_false;
	}

}
