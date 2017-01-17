#ifndef CHECKHEADER_SLIB_CORE_DISPATCH
#define CHECKHEADER_SLIB_CORE_DISPATCH

#include "definition.h"

#include "object.h"
#include "thread.h"

SLIB_NAMESPACE_BEGIN

class Dispatcher;
class Timer;

class SLIB_EXPORT Dispatch
{
public:
	static sl_bool dispatch(const Ref<Dispatcher>& dispatcher, const Function<void()>& task, sl_uint64 delay_ms = 0);
	
	static sl_bool dispatch(const Function<void()>& task, sl_uint64 delay_ms = 0);
	
	static Ref<Timer> addTimer(const Ref<Dispatcher>& dispatcher, const Function<void()>& task, sl_uint64 interval_ms);
	
	static Ref<Timer> addTimer(const Function<void()>& task, sl_uint64 interval_ms);
	
	static void removeTimer(const Ref<Dispatcher>& dispatcher, const Ref<Timer>& timer);
	
	static void removeTimer(const Ref<Timer>& timer);
	
};

class SLIB_EXPORT Dispatcher : public Executor
{
	SLIB_DECLARE_OBJECT
	
private:
	Dispatcher();
	
	~Dispatcher();

public:
	static Ref<Dispatcher> getDefault();

	static void releaseDefault();

	static Ref<Dispatcher> create(sl_bool flagAutoStart = sl_true);

public:
	void release();

	void start();
	
	sl_bool isRunning();
	
	sl_bool dispatch(const Function<void()>& task, sl_uint64 delay_ms = 0);

	Ref<Timer> addTimer(const Function<void()>& task, sl_uint64 interval_ms);
	
	sl_bool addTimer(const Ref<Timer>& timer);
	
	void removeTimer(const Ref<Timer>& timer);

	sl_uint64 getElapsedMilliseconds();
	
	// override
	sl_bool execute(const Function<void()>& callback);

protected:
	sl_bool m_flagInit;
	sl_bool m_flagRunning;
	Ref<Thread> m_thread;

	TimeCounter m_timeCounter;

	LinkedQueue< Function<void()> > m_queueTasks;

	struct TimeTask
	{
		sl_uint64 time;
		Function<void()> task;
	};
	BTree<sl_uint64, TimeTask> m_timeTasks;
	Mutex m_lockTimeTasks;

	class TimerTask
	{
	public:
		WeakRef<Timer> timer;
		
	public:
		sl_bool operator==(const TimerTask& other) const;
	};
	LinkedQueue<TimerTask> m_queueTimers;
	Mutex m_lockTimer;

protected:
	void _wake();
	sl_int32 _getTimeout();
	sl_int32 _getTimeout_TimeTasks();
	sl_int32 _getTimeout_Timer();
	void _runLoop();

};


class SLIB_EXPORT Timer : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	Timer();
	
public:
	static Ref<Timer> create(const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart = sl_true);
	
public:
	void start();
	
	void stop();
	
	sl_bool isStarted();
	
	Function<void()> getTask();
	
	sl_uint64 getInterval();
	
	void run();
	
	void stopAndWait();
	
public:
	SLIB_PROPERTY(sl_uint64, LastRunTime)
	
	SLIB_PROPERTY(sl_uint32, MaxConcurrentThread)
	
protected:
	sl_bool m_flagStarted;
	Function<void()> m_task;
	sl_uint64 m_interval;
	sl_int32 m_nCountRun;
	
};

SLIB_NAMESPACE_END

#endif
