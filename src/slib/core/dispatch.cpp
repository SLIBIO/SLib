/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/dispatch.h"
#include "slib/core/dispatch_loop.h"

#include "slib/core/safe_static.h"
#include "slib/core/system.h"

namespace slib
{

/*************************************
			Dispatch
*************************************/
	sl_bool Dispatch::dispatch(const Ref<DispatchLoop>& loop, const Function<void()>& task)
	{
		if (loop.isNotNull()) {
			return loop->dispatch(task);
		}
		return sl_false;
	}

	sl_bool Dispatch::dispatch(const Function<void()>& task)
	{
		return Dispatch::dispatch(DispatchLoop::getDefault(), task);
	}

	sl_bool Dispatch::setTimeout(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 delay_ms)
	{
		if (loop.isNotNull()) {
			return loop->dispatch(task, delay_ms);
		}
		return sl_false;
	}

	sl_bool Dispatch::setTimeout(const Function<void()>& task, sl_uint64 delay_ms)
	{
		return Dispatch::setTimeout(DispatchLoop::getDefault(), task, delay_ms);
	}

	Ref<Timer> Dispatch::setInterval(const Ref<DispatchLoop>& loop, const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		if (loop.isNotNull()) {
			return Timer::startWithLoop(loop, task, interval_ms);
		}
		return sl_null;
	}

	Ref<Timer> Dispatch::setInterval(const Function<void(Timer*)>& task, sl_uint64 interval_ms)
	{
		return Dispatch::setInterval(DispatchLoop::getDefault(), task, interval_ms);
	}

/*************************************
			Dispatcher
*************************************/

	SLIB_DEFINE_OBJECT(Dispatcher, Object)

	Dispatcher::Dispatcher()
	{
	}

	Dispatcher::~Dispatcher()
	{
	}


/*************************************
			DispatchLoop
*************************************/

	SLIB_DEFINE_OBJECT(DispatchLoop, Dispatcher)

	DispatchLoop::DispatchLoop()
	{
		m_flagInit = sl_false;
		m_flagRunning = sl_false;
	}

	DispatchLoop::~DispatchLoop()
	{
		release();
	}

	Ref<DispatchLoop> DispatchLoop::getDefault()
	{
		SLIB_SAFE_STATIC(Ref<DispatchLoop>, ret, create())
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		return ret;
	}

	void DispatchLoop::releaseDefault()
	{
		Ref<DispatchLoop> loop = getDefault();
		if (loop.isNotNull()) {
			loop->release();
		}
	}

	Ref<DispatchLoop> DispatchLoop::create(sl_bool flagAutoStart)
	{
		Ref<DispatchLoop> ret = new DispatchLoop;
		if (ret.isNotNull()) {
			ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(DispatchLoop, _runLoop, ret.get()));
			if (ret->m_thread.isNotNull()) {
				ret->m_flagInit = sl_true;
				if (flagAutoStart) {
					ret->start();
				}
				return ret;
			}
		}
		return sl_null;
	}

	void DispatchLoop::release()
	{
		ObjectLocker lock(this);
		if (!m_flagInit) {
			return;
		}
		m_flagInit = sl_false;

		if (m_flagRunning) {
			m_flagRunning = sl_false;
			lock.unlock();
			m_thread->finishAndWait();
		}

		m_queueTasks.removeAll();
		
		MutexLocker lockTime(&m_lockTimeTasks);
		m_timeTasks.removeAll();
	}

	void DispatchLoop::start()
	{
		ObjectLocker lock(this);
		if (!m_flagInit) {
			return;
		}
		if (m_flagRunning) {
			return;
		}
		m_flagRunning = sl_true;
		if (!(m_thread->start())) {
			m_flagRunning = sl_false;
		}
	}

	sl_bool DispatchLoop::isRunning()
	{
		return m_flagRunning;
	}

	void DispatchLoop::_wake()
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		m_thread->wake();
	}

	sl_int32 DispatchLoop::_getTimeout()
	{
		m_timeCounter.update();
		if (m_queueTasks.isNotEmpty()) {
			return 0;
		}
		sl_int32 t1 = _getTimeout_TimeTasks();
		sl_int32 t2 = _getTimeout_Timer();
		if (t1 < 0) {
			return t2;
		} else {
			if (t2 < 0) {
				return t1;
			} else {
				return SLIB_MIN(t1, t2);
			}
		}
	}

	sl_bool DispatchLoop::dispatch(const Function<void()>& task, sl_uint64 delay_ms)
	{
		if (task.isNull()) {
			return sl_false;
		}
		if (delay_ms == 0) {
			if (m_queueTasks.push(task)) {
				_wake();
				return sl_true;
			}
		} else {
			MutexLocker lock(&m_lockTimeTasks);
			TimeTask tt;
			tt.time = getElapsedMilliseconds() + delay_ms;
			tt.task = task;
			if (m_timeTasks.put(tt.time, tt, MapPutMode::AddAlways)) {
				_wake();
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_int32 DispatchLoop::_getTimeout_TimeTasks()
	{
		MutexLocker lock(&m_lockTimeTasks);
		sl_uint64 rel = getElapsedMilliseconds();
		if (m_timeTasks.getCount() == 0) {
			return -1;
		}
		sl_int32 timeout = -1;
		LinkedQueue< Function<void()> > tasks;
		while (RedBlackTreeNode<sl_uint64, TimeTask>* node = m_timeTasks.getFirstNode()) {
			if (rel >= node->data.key) {
				tasks.push(node->data.value.task);
				m_timeTasks.removeNode(node);
			} else {
				timeout = (sl_int32)(node->data.key - rel);
				break;
			}
		}
		lock.unlock();

		Function<void()> task;
		while (tasks.pop(&task)) {
			task();
		}
		return timeout;
	}

	sl_int32 DispatchLoop::_getTimeout_Timer()
	{
		MutexLocker lock(&m_lockTimer);

		sl_int32 timeout = -1;
		LinkedQueue< Ref<Timer> > tasks;

		sl_uint64 rel = getElapsedMilliseconds();
		Link<TimerTask>* link = m_queueTimers.getFront();
		while (link) {
			Ref<Timer> timer(link->value.timer);
			if (timer.isNotNull()) {
				if (timer->isStarted()) {
					sl_uint64 t = rel - timer->getLastRunTime();
					if (t >= timer->getInterval()) {
						tasks.push(timer);
						timer->setLastRunTime(rel);
						t = timer->getInterval();
					} else {
						t = timer->getInterval() - t;
					}
					if (timeout < 0 || t < (sl_uint64)timeout) {
						timeout = (sl_uint32)t;
					}
				}
				link = link->next;
			} else {
				Link<TimerTask>* linkRemove = link;
				link = link->next;
				m_queueTimers.removeItem(linkRemove);
			}
		}

		lock.unlock();

		Ref<Timer> task;
		while (tasks.pop(&task)) {
			if (task.isNotNull()) {
				task->run();
			}
		}
		return timeout;
	}

	sl_bool DispatchLoop::TimerTask::operator==(const DispatchLoop::TimerTask& other) const
	{
		return timer == other.timer;
	}

	sl_bool DispatchLoop::addTimer(const Ref<Timer>& timer)
	{
		if (timer.isNull()) {
			return sl_false;
		}
		TimerTask t;
		t.timer = timer;
		MutexLocker lock(&m_lockTimer);
		if (m_queueTimers.push(t)) {
			_wake();
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void DispatchLoop::removeTimer(const Ref<Timer>& timer)
	{
		MutexLocker lock(&m_lockTimer);
		TimerTask t;
		t.timer = timer;
		m_queueTimers.removeValue(t);
	}

	sl_uint64 DispatchLoop::getElapsedMilliseconds()
	{
		return m_timeCounter.getElapsedMilliseconds();
	}

	void DispatchLoop::_runLoop()
	{
		while (Thread::isNotStoppingCurrent()) {

			// Async Tasks
			{
				LinkedQueue< Function<void()> > tasks;
				tasks.merge(&m_queueTasks);
				Function<void()> task;
				while (tasks.pop_NoLock(&task)) {
					task();
				}
			}
			
			sl_int32 _t = _getTimeout();
			if (_t < 0) {
				_t = 10000;
			}
			if (_t > 10000) {
				_t = 10000;
			}
			Thread::sleep(_t);

		}
	}

}
