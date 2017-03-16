#ifndef CHECKHEADER_SLIB_CORE_TIMER
#define CHECKHEADER_SLIB_CORE_TIMER

#include "definition.h"

#include "object.h"
#include "function.h"

namespace slib
{
	
	class DispatchLoop;
	class Dispatcher;
	
	class SLIB_EXPORT Timer : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		Timer();

		~Timer();

	public:
		static Ref<Timer> create(const Function<void(Timer*)>& task, sl_uint64 interval_ms);
		
		static Ref<Timer> start(const Function<void(Timer*)>& task, sl_uint64 interval_ms);
		
		static Ref<Timer> createWithLoop(const Ref<DispatchLoop>& loop, const Function<void(Timer*)>& task, sl_uint64 interval_ms);

		static Ref<Timer> startWithLoop(const Ref<DispatchLoop>& loop, const Function<void(Timer*)>& task, sl_uint64 interval_ms);
		
		static Ref<Timer> createWithDispatcher(const Ref<Dispatcher>& dispatcher, const Function<void(Timer*)>& task, sl_uint64 interval_ms);
		
		static Ref<Timer> startWithDispatcher(const Ref<Dispatcher>& dispatcher, const Function<void(Timer*)>& task, sl_uint64 interval_ms);

	public:
		void start();

		void stop();

		sl_bool isStarted();

		Function<void(Timer*)> getTask();

		sl_uint64 getInterval();

		void run();

		void stopAndWait();

	public:
		SLIB_PROPERTY(sl_uint64, LastRunTime)

		SLIB_PROPERTY(sl_uint32, MaxConcurrentThread)

	protected:
		void _runFromDispatcher();

	protected:
		sl_bool m_flagStarted;
		Function<void(Timer*)> m_task;
		sl_uint64 m_interval;
		sl_int32 m_nCountRun;

		Ref<Dispatcher> m_dispatcher;
		WeakRef<DispatchLoop> m_loop;

		sl_bool m_flagDispatched;

	};

}

#endif
