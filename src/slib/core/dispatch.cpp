#include "../../../inc/slib/core/dispatch.h"

#include "../../../inc/slib/core/safe_static.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN

/*************************************
			Dispatch
*************************************/
sl_bool Dispatch::dispatch(const Ref<Dispatcher>& dispatcher, const Function<void()>& task, sl_uint64 delay_ms)
{
	if (dispatcher.isNotNull()) {
		return dispatcher->dispatch(task, delay_ms);
	}
	return sl_false;
}

sl_bool Dispatch::dispatch(const Function<void()>& task, sl_uint64 delay_ms)
{
	return Dispatch::dispatch(Dispatcher::getDefault(), task, delay_ms);
}

Ref<Timer> Dispatch::addTimer(const Ref<Dispatcher>& dispatcher, const Function<void()>& task, sl_uint64 interval_ms)
{
	if (dispatcher.isNotNull()) {
		return dispatcher->addTimer(task, interval_ms);
	}
	return sl_null;
}

Ref<Timer> Dispatch::addTimer(const Function<void()>& task, sl_uint64 interval_ms)
{
	return Dispatch::addTimer(Dispatcher::getDefault(), task, interval_ms);
}

void Dispatch::removeTimer(const Ref<Dispatcher>& dispatcher, const Ref<Timer>& timer)
{
	if (dispatcher.isNotNull()) {
		return dispatcher->removeTimer(timer);
	}
}

void Dispatch::removeTimer(const Ref<Timer>& timer)
{
	Dispatch::removeTimer(Dispatcher::getDefault(), timer);
}

/*************************************
			Dispatcher
*************************************/

SLIB_DEFINE_OBJECT(Dispatcher, Executor)

Dispatcher::Dispatcher()
{
	m_flagInit = sl_false;
	m_flagRunning = sl_false;
}

Dispatcher::~Dispatcher()
{
	release();
}

Ref<Dispatcher> Dispatcher::getDefault()
{
	SLIB_SAFE_STATIC(Ref<Dispatcher>, ret, create())
	if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
		return sl_null;
	}
	return ret;
}

void Dispatcher::releaseDefault()
{
	Ref<Dispatcher> loop = getDefault();
	if (loop.isNotNull()) {
		loop->release();
	}
}

Ref<Dispatcher> Dispatcher::create(sl_bool flagAutoStart)
{
	Ref<Dispatcher> ret = new Dispatcher;
	if (ret.isNotNull()) {
		ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(Dispatcher, _runLoop, ret.get()));
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

void Dispatcher::release()
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

void Dispatcher::start()
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

sl_bool Dispatcher::isRunning()
{
	return m_flagRunning;
}

void Dispatcher::_wake()
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	m_thread->wake();
}

sl_int32 Dispatcher::_getTimeout()
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

sl_bool Dispatcher::dispatch(const Function<void()>& task, sl_uint64 delay_ms)
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

sl_bool Dispatcher::execute(const Function<void()>& callback)
{
	return dispatch(callback);
}

sl_int32 Dispatcher::_getTimeout_TimeTasks()
{
	MutexLocker lock(&m_lockTimeTasks);
	sl_uint64 rel = getElapsedMilliseconds();
	if (m_timeTasks.getCount() == 0) {
		return -1;
	}
	sl_int32 timeout = -1;
	TreePosition pos;
	TimeTask timeTask;
	LinkedQueue< Function<void()> > tasks;
	sl_uint64 t;
	while (m_timeTasks.getFirstPosition(pos, &t, &timeTask)) {
		if (rel >= t) {
			tasks.push(timeTask.task);
			m_timeTasks.removeAt(pos);
		} else {
			t = t - rel;
			timeout = (sl_int32)t;
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

sl_int32 Dispatcher::_getTimeout_Timer()
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
				if (timeout < 0 || t < timeout) {
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

sl_bool Dispatcher::TimerTask::operator==(const Dispatcher::TimerTask& other) const
{
	return timer == other.timer;
}

sl_bool Dispatcher::addTimer(const Ref<Timer>& timer)
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

void Dispatcher::removeTimer(const Ref<Timer>& timer)
{
	MutexLocker lock(&m_lockTimer);
	TimerTask t;
	t.timer = timer;
	m_queueTimers.removeValue(t);
}

Ref<Timer> Dispatcher::addTimer(const Function<void()>& task, sl_uint64 interval_ms)
{
	Ref<Timer> timer = Timer::create(task, interval_ms);
	if (timer.isNotNull()) {
		if (addTimer(timer)) {
			return timer;
		}
	}
	return sl_null;
}

sl_uint64 Dispatcher::getElapsedMilliseconds()
{
	return m_timeCounter.getElapsedMilliseconds();
}

void Dispatcher::_runLoop()
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

/*************************************
	Timer
**************************************/

SLIB_DEFINE_OBJECT(Timer, Object)

Timer::Timer()
{
	m_flagStarted = sl_false;
	m_nCountRun = 0;
	setLastRunTime(0);
	setMaxConcurrentThread(1);
}

Ref<Timer> Timer::create(const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart)
{
	Ref<Timer> ret;
	if (task.isNull()) {
		return ret;
	}
	ret = new Timer();
	if (ret.isNotNull()) {
		ret->m_flagStarted = flagStart;
		ret->m_task = task;
		ret->m_interval = interval_ms;
	}
	return ret;
}

void Timer::start()
{
	m_flagStarted = sl_true;
}

void Timer::stop()
{
	m_flagStarted = sl_false;
}

sl_bool Timer::isStarted()
{
	return m_flagStarted;
}

Function<void()> Timer::getTask()
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
			m_task();
		}
		Base::interlockedDecrement32(&m_nCountRun);
	}
}

void Timer::stopAndWait()
{
	ObjectLocker lock(this);
	stop();
	sl_uint32 n = 0;
	while (m_nCountRun > 0) {
		System::yield(++n);
	}
}

SLIB_NAMESPACE_END
