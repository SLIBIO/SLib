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

class SLIB_EXPORT Thread : public Object
{
	SLIB_DECLARE_OBJECT(Thread, Object)
public:
	enum Priority {
		priorityLowest = -2,
		priorityBelowNormal = -1,
		priorityNormal = 0,
		priorityAboveNormal = 1,
		priorityHighest = 2
	};

protected:
	Thread();
	
public:
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

	void setPriority(Priority priority);
	
	Priority getPriority();

public:
	SLIB_INLINE sl_bool isRunning()
	{
		return m_flagRunning;
	}
	
	SLIB_INLINE sl_bool isNotRunning()
	{
		return !m_flagRunning;
	}
	
	
	SLIB_INLINE sl_bool isStopping()
	{
		return m_flagRequestStop;
	}
	
	SLIB_INLINE sl_bool isNotStopping()
	{
		return !m_flagRequestStop;
	}
	
	
	SLIB_INLINE sl_bool isWaiting()
	{
		return m_eventWaiting.isNotNull();
	}
	
	SLIB_INLINE sl_bool isNotWaiting()
	{
		return m_eventWaiting.isNull();
	}
	
	
	SLIB_INLINE const Ref<Runnable>& getRunnable()
	{
		return m_runnable;
	}
	
	
	sl_bool isCurrentThread();
	

	Variant getProperty(const String& name);
	
	void setProperty(const String& name, const Variant& value);
	
	void clearProperty(const String& name);

	
	// attached objects are removed when the thread is exited
	Ref<Referable> getAttachedObject(const String& name);
	
	void attachObject(const String& name, const Referable* object);
	
	void removeAttachedObject(const String& name);
	
public:
	static Ref<Thread> getCurrent();
	
	static sl_bool sleep(sl_uint32 ms);
	
	static sl_bool isStoppingCurrent();
	
	static sl_bool isNotStoppingCurrent();
	
	static sl_uint64 getCurrentThreadUniqueId();
	

private:
	void* m_handle;
	Thread::Priority m_priority;

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
