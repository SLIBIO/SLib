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

#include "slib/core/atomic.h"

#include "slib/core/base.h"
#include "slib/core/time.h"
#include "slib/core/system.h"

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
