#ifndef CHECKHEADER_SLIB_CORE_THREAD_POOL
#define CHECKHEADER_SLIB_CORE_THREAD_POOL

#include "definition.h"

#include "queue.h"
#include "thread.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT ThreadPool : public Dispatcher
{
	SLIB_DECLARE_OBJECT
	
private:
	ThreadPool();
	
	~ThreadPool();
	
public:
	static Ref<ThreadPool> create(sl_uint32 minThreads = 0, sl_uint32 maxThreads = 30);

public:
	void release();
	
	sl_bool isRunning();
	
	sl_uint32 getThreadsCount();

	sl_bool addTask(const Function<void()>& task);
	
	// override
	sl_bool dispatch(const Function<void()>& callback);

public:
	SLIB_PROPERTY(sl_uint32, MinimumThreadsCount)
	SLIB_PROPERTY(sl_uint32, MaximumThreadsCount)
	SLIB_PROPERTY(sl_uint32, ThreadStackSize)

protected:
	void onRunWorker();

protected:
	CList< Ref<Thread> > m_threadWorkers;
	LinkedQueue< Function<void()> > m_tasks;

	sl_bool m_flagRunning;
	
};

SLIB_NAMESPACE_END

#endif
