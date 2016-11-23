#include "../../../inc/slib/core/thread_pool.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ThreadPool, Dispatcher)

ThreadPool::ThreadPool()
{
	setThreadStackSize(SLIB_THREAD_DEFAULT_STACK_SIZE);
	m_flagRunning = sl_true;
}

ThreadPool::~ThreadPool()
{
	release();
}

Ref<ThreadPool> ThreadPool::create(sl_uint32 minThreads, sl_uint32 maxThreads)
{
	Ref<ThreadPool> ret = new ThreadPool();
	if (ret.isNotNull()) {
		ret->setMinimumThreadsCount(minThreads);
		ret->setMaximumThreadsCount(maxThreads);
	}
	return ret;
}

void ThreadPool::release()
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	m_flagRunning = sl_false;
	
	ListItems< Ref<Thread> > threads(m_threadWorkers);
	sl_size i;
	for (i = 0; i < threads.count; i++) {
		threads[i]->finish();
	}
	for (i = 0; i < threads.count; i++) {
		threads[i]->finishAndWait();
	}
}

sl_bool ThreadPool::isRunning()
{
	return m_flagRunning;
}

sl_uint32 ThreadPool::getThreadsCount()
{
	return (sl_uint32)(m_threadWorkers.getCount());
}

sl_bool ThreadPool::addTask(const Callback& task)
{
	if (task.isNull()) {
		return sl_false;
	}
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return sl_false;
	}
	// add task
	if (!(m_tasks.push(task))) {
		return sl_false;
	}

	// wake a sleeping worker
	{
		ListItems< Ref<Thread> > threads(m_threadWorkers);
		for (sl_size i = 0; i < threads.count; i++) {
			if (threads[i]->isWaiting()) {
				threads[i]->wake();
				return sl_true;
			}
		}
	}

	// increase workers
	{
		sl_size nThreads = m_threadWorkers.getCount();
		if (nThreads == 0 || (nThreads < getMaximumThreadsCount())) {
			Ref<Thread> worker = Thread::start(SLIB_CALLBACK_CLASS(ThreadPool, onRunWorker, this), getThreadStackSize());
			if (worker.isNotNull()) {
				m_threadWorkers.add_NoLock(worker);
			}
		}
	}
	return sl_true;
}

sl_bool ThreadPool::dispatch(const Callback& callback)
{
	return addTask(callback);
}

void ThreadPool::onRunWorker()
{
	while (m_flagRunning && Thread::isNotStoppingCurrent()) {
		Callback task;
		if (m_tasks.pop(&task)) {
			task();
		} else {
			ObjectLocker lock(this);
			sl_size nThreads = m_threadWorkers.getCount();
			if (nThreads > getMinimumThreadsCount()) {
				m_threadWorkers.removeValue_NoLock(Thread::getCurrent());
				return;
			} else {
				lock.unlock();
				Thread::sleep(5000);
			}
		}
	}
}

SLIB_NAMESPACE_END
