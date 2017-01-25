#include "../../../inc/slib/core/timer.h"

#include "../../../inc/slib/core/dispatch_loop.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Timer, Object)

Timer::Timer()
{
	m_flagUseLoop = sl_false;
	
	m_flagStarted = sl_false;
	m_nCountRun = 0;
	setLastRunTime(0);
	setMaxConcurrentThread(1);
}

Timer::~Timer()
{
}

Ref<Timer> Timer::createWithLoop(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart)
{
	Ref<Timer> ret;
	if (task.isNull()) {
		return ret;
	}
	ret = new Timer();
	if (ret.isNotNull()) {
		ret->m_flagUseLoop = sl_true;
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

void Timer::start()
{
	ObjectLocker lock(this);
	if (!m_flagStarted) {
		if (m_flagUseLoop) {
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
		if (m_flagUseLoop) {
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

SLIB_NAMESPACE_END
