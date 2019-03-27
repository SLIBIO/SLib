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

#include "slib/core/timer.h"

#include "slib/core/dispatch_loop.h"
#include "slib/core/system.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Timer, Object)

	Timer::Timer()
	{
		m_flagStarted = sl_false;
		m_nCountRun = 0;
		
		m_flagDispatched = sl_false;
		
		setLastRunTime(0);
		setMaxConcurrentThread(1);
		
	}

	Timer::~Timer()
	{
	}
	
	Ref<Timer> Timer::create(const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		return Timer::createWithLoop(DispatchLoop::getDefault(), task, interval_ms);
	}
	
	Ref<Timer> Timer::start(const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		return Timer::startWithLoop(DispatchLoop::getDefault(), task, interval_ms);
	}

	Ref<Timer> Timer::createWithLoop(const Ref<DispatchLoop>& loop, const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		if (task.isNull()) {
			return sl_null;
		}
		if (loop.isNull()) {
			return sl_null;
		}
		Ref<Timer> ret = new Timer();
		if (ret.isNotNull()) {
			ret->m_loop = loop;
			
			ret->m_task = task;
			ret->m_interval = interval_ms;
			
			return ret;
		}
		return sl_null;
	}
	
	Ref<Timer> Timer::startWithLoop(const Ref<DispatchLoop>& loop, const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		Ref<Timer> timer = Timer::createWithLoop(loop, task, interval_ms);
		if (timer.isNotNull()) {
			timer->start();
			return timer;
		}
		return sl_null;
	}

	Ref<Timer> Timer::createWithDispatcher(const Ref<Dispatcher>& dispatcher, const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		if (task.isNull()) {
			return sl_null;
		}
		if (dispatcher.isNull()) {
			return sl_null;
		}
		Ref<Timer> ret = new Timer();
		if (ret.isNotNull()) {
			ret->m_dispatcher = dispatcher;
			
			ret->m_task = task;
			ret->m_interval = interval_ms;
			
			return ret;
		}
		return sl_null;
	}
	
	Ref<Timer> Timer::startWithDispatcher(const Ref<Dispatcher>& dispatcher, const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		Ref<Timer> timer = Timer::createWithDispatcher(dispatcher, task, interval_ms);
		if (timer.isNotNull()) {
			timer->start();
			return timer;
		}
		return sl_null;
	}
	
	void Timer::start()
	{
		ObjectLocker lock(this);
		if (!m_flagStarted) {
			if (m_dispatcher.isNotNull()) {
				m_flagStarted = sl_true;
				if (!m_flagDispatched) {
					m_flagDispatched = sl_true;
					m_dispatcher->dispatch(SLIB_FUNCTION_WEAKREF(Timer, _runFromDispatcher, this), m_interval);
				}
			} else {
				Ref<DispatchLoop> loop = m_loop;
				if (loop.isNotNull()) {
					m_flagStarted = sl_true;
					lock.unlock();
					loop->addTimer(this);
				}
			}
		}
	}

	void Timer::stop()
	{
		ObjectLocker lock(this);
		if (m_flagStarted) {
			m_flagStarted = sl_false;
			if (m_dispatcher.isNull()) {
				Ref<DispatchLoop> loop = m_loop;
				if (loop.isNotNull()) {
					lock.unlock();
					loop->removeTimer(this);
				}
			}
		}
	}

	sl_bool Timer::isStarted()
	{
		return m_flagStarted;
	}

	Function<void(Timer*)> Timer::getTask()
	{
		return m_task;
	}

	sl_uint64 Timer::getInterval()
	{
		return m_interval;
	}

	void Timer::run()
	{
		ObjectLocker lock(this);
		if (m_flagStarted) {
			sl_int32 n = Base::interlockedIncrement32(&m_nCountRun);
			if (n <= (sl_int32)(getMaxConcurrentThread())) {
				lock.unlock();
				m_task(this);
			}
			Base::interlockedDecrement32(&m_nCountRun);
		}
	}

	void Timer::stopAndWait()
	{
		stop();
		sl_uint32 n = 0;
		while (m_nCountRun > 0) {
			System::yield(++n);
		}
	}

	void Timer::_runFromDispatcher()
	{
		if (m_flagStarted) {
			m_task(this);
		}
		ObjectLocker lock(this);
		if (m_flagStarted) {
			m_dispatcher->dispatch(SLIB_FUNCTION_WEAKREF(Timer, _runFromDispatcher, this), m_interval);
		} else {
			m_flagDispatched = sl_false;
		}
	}

}
