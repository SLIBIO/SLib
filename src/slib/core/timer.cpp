#include "../../../inc/slib/core/timer.h"

#include "../../../inc/slib/core/dispatch_loop.h"
#include "../../../inc/slib/core/system.h"

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
		SLIB_REFERABLE_DESTRUCTOR
		stop();
	}

	Ref<Timer> Timer::createWithLoop(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart)
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
			
			if (flagStart) {
				ret->start();
			}
			return ret;
		}
		return sl_null;
	}

	Ref<Timer> Timer::createWithDispatcher(const Ref<Dispatcher>& dispatcher, const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart)
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
			
			if (flagStart) {
				ret->start();
			}
			return ret;
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
		stop();
		sl_uint32 n = 0;
		while (m_nCountRun > 0) {
			System::yield(++n);
		}
	}

	void Timer::_runFromDispatcher()
	{
		if (m_flagStarted) {
			m_task();
		}
		ObjectLocker lock(this);
		if (m_flagStarted) {
			m_dispatcher->dispatch(SLIB_FUNCTION_WEAKREF(Timer, _runFromDispatcher, this), m_interval);
		} else {
			m_flagDispatched = sl_false;
		}
	}

}
