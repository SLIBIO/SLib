#ifndef CHECKHEADER_SLIB_CORE_TIMER
#define CHECKHEADER_SLIB_CORE_TIMER

#include "definition.h"

#include "object.h"
#include "function.h"

SLIB_NAMESPACE_BEGIN

class DispatchLoop;

class SLIB_EXPORT Timer : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	Timer();
	
	~Timer();
	
public:
	static Ref<Timer> createWithLoop(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart = sl_true);
	
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
	
	sl_bool m_flagUseLoop;
	WeakRef<DispatchLoop> m_loop;
	
};

SLIB_NAMESPACE_END

#endif
