#include "../../../inc/slib/core/thread_pool.h"

SLIB_NAMESPACE_BEGIN
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
	if (!m_flagRunning) {
		return;
	}
	m_flagRunning = sl_false;

	MutexLocker lock(getLocker());
	ListLocker< Ref<Thread> > threads(m_threadWorkers.duplicate());
	for (sl_size i = 0; i < threads.count(); i++) {
		threads[i]->finish();
	}
	for (sl_size i = 0; i < threads.count(); i++) {
		threads[i]->finishAndWait();
	}
}

sl_bool ThreadPool::isRunning()
{
	return m_flagRunning;
}

sl_uint32 ThreadPool::getThreadsCount()
{
	return (sl_uint32)(m_threadWorkers.count());
}

sl_bool ThreadPool::addTask(const Ref<Runnable>& _task)
{
	Ref<Runnable> task = _task;
	if (task.isNull()) {
		return sl_false;
	}
	if (!m_flagRunning) {
		return sl_false;
	}
	MutexLocker lock(getLocker());
	// add task
	if (!(m_tasks.push(task))) {
		return sl_false;
	}

	// wake a sleeping worker
	{
		ListLocker< Ref<Thread> > threads(m_threadWorkers);
		for (sl_size i = 0; i < threads.count(); i++) {
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
				m_threadWorkers.add(worker);
			}
		}
	}
	return sl_true;
}

void ThreadPool::onRunWorker()
{
	while (m_flagRunning && Thread::isNotStoppingCurrent()) {
		Ref<Runnable> task;
		if (m_tasks.pop(&task)) {
			task->run();
		} else {
			sl_size nThreads = m_threadWorkers.getCount();
			if (nThreads > getMinimumThreadsCount()) {
				m_threadWorkers.removeValue(Thread::getCurrent());
				return;
			} else {
				Thread::sleep(5000);
			}
		}
	}
}
SLIB_NAMESPACE_END
