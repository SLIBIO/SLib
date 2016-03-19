#ifndef CHECKHEADER_SLIB_CORE_THREAD
#define CHECKHEADER_SLIB_CORE_THREAD

#include "definition.h"

#include "mutex.h"
#include "event.h"
#include "object.h"
#include "variant.h"
#include "callback.h"

#define SLIB_THREAD_DEFAULT_STACK_SIZE 1048576 // 1MB

SLIB_NAMESPACE_BEGIN

enum class ThreadPriority
{
	Lowest = -2,
	BelowNormal = -1,
	Normal = 0,
	AboveNormal = 1,
	Highest = 2
};

class SLIB_EXPORT Thread : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	Thread();
	
	~Thread();

public:
	static Ref<Thread> create(const Ref<Runnable>& runnable);
	
	static Ref<Thread> start(const Ref<Runnable>& runnable, sl_uint32 stackSize = SLIB_THREAD_DEFAULT_STACK_SIZE);
	
public:
	sl_bool start(sl_uint32 stackSize = SLIB_THREAD_DEFAULT_STACK_SIZE);
	
	void finish();
	
	sl_bool join(sl_int32 timeout = -1);
	
	sl_bool finishAndWait(sl_int32 timeout = -1);
	
	sl_bool wait(sl_int32 timeout = -1);
	
	void wake();

	void setWaitingEvent(Event* ev);
	
	void clearWaitingEvent();

	ThreadPriority getPriority();

	void setPriority(ThreadPriority priority);

	sl_bool isRunning();
	
	sl_bool isNotRunning();
	
	sl_bool isStopping();
	
	sl_bool isNotStopping();
	
	sl_bool isWaiting();
	
	sl_bool isNotWaiting();
	
	const Ref<Runnable>& getRunnable();
	
	static sl_bool sleep(sl_uint32 ms);
	
	sl_bool isCurrentThread();
	
	static Ref<Thread> getCurrent();
	
	static sl_bool isStoppingCurrent();
	
	static sl_bool isNotStoppingCurrent();
	
	static sl_uint64 getCurrentThreadUniqueId();

	
	Variant getProperty(const String& name);
	
	void setProperty(const String& name, const Variant& value);
	
	void clearProperty(const String& name);
	
	// attached objects are removed when the thread is exited
	Ref<Referable> getAttachedObject(const String& name);
	
	void attachObject(const String& name, const Referable* object);
	
	void removeAttachedObject(const String& name);
	
private:
	void* m_handle;
	ThreadPriority m_priority;

	sl_bool m_flagRequestStop;
	sl_bool m_flagRunning;
	Ref<Runnable> m_runnable;

	Ref<Event> m_eventWake;
	Ref<Event> m_eventExit;
	SafeRef<Event> m_eventWaiting;

	HashMap<String, Variant> m_properties;
	HashMap< String, Ref<Referable> > m_attachedObjects;

private:
	static Thread* _nativeGetCurrentThread();
	static void _nativeSetCurrentThread(Thread* thread);
	static sl_uint64 _nativeGetCurrentThreadUniqueId();
	static void _nativeSetCurrentThreadUniqueId(sl_uint64 n);
	void _nativeStart(sl_uint32 stackSize);
	void _nativeClose();
	void _nativeSetPriority();

public:
	void _run();

};

#define SLIB_SAFE_SLEEP(ms) { slib::Thread::sleep(ms); if (slib::Thread::isStoppingCurrent()) return; }

SLIB_NAMESPACE_END

#endif
