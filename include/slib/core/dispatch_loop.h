/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_DISPATCH_LOOP
#define CHECKHEADER_SLIB_CORE_DISPATCH_LOOP

#include "definition.h"

#include "dispatch.h"
#include "thread.h"
#include "time.h"
#include "map.h"

namespace slib
{
	
	class SLIB_EXPORT DispatchLoop : public Dispatcher
	{
		SLIB_DECLARE_OBJECT

	private:
		DispatchLoop();

		~DispatchLoop();
	
	public:
		static Ref<DispatchLoop> getDefault();
	
		static void releaseDefault();
	
		static Ref<DispatchLoop> create(sl_bool flagAutoStart = sl_true);
	
	public:
		void release();
	
		void start();

		sl_bool isRunning();

		sl_bool dispatch(const Function<void()>& task, sl_uint64 delay_ms = 0) override;

		sl_bool addTimer(const Ref<Timer>& timer);
		
		void removeTimer(const Ref<Timer>& timer);

		sl_uint64 getElapsedMilliseconds();

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
		CMap<sl_uint64, TimeTask> m_timeTasks;
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

}

#endif
