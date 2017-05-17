/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_DISPATCH_LOOP
#define CHECKHEADER_SLIB_CORE_DISPATCH_LOOP

#include "definition.h"

#include "dispatch.h"
#include "thread.h"
#include "time.h"

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
		RedBlackTree<sl_uint64, TimeTask> m_timeTasks;
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
