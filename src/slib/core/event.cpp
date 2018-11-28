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

#include "slib/core/event.h"

#include "slib/core/thread.h"

namespace slib
{

	SLIB_DEFINE_ROOT_OBJECT(Event)

	Event::Event()
	{
	}

	Event::~Event()
	{
	}

	void Event::set()
	{
		_native_set();
	}

	void Event::reset()
	{
		_native_reset();
	}

	sl_bool Event::wait(sl_int32 timeout)
	{
		Ref<Thread> thread = Thread::getCurrent();
		if (thread.isNotNull()) {
			if (thread->isStopping()) {
				return sl_false;
			}
			thread->setWaitingEvent(this);
		}
		sl_bool ret = _native_wait(timeout);
		if (thread.isNotNull()) {
			thread->clearWaitingEvent();
		}
		return ret;
	}

}
