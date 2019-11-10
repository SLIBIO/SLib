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

#ifndef CHECKHEADER_SLIB_CORE_THREAD
#define CHECKHEADER_SLIB_CORE_THREAD

#include "definition.h"

#include "mutex.h"
#include "event.h"
#include "object.h"
#include "function.h"
#include "string.h"
#include "hash_map.h"

#define SLIB_THREAD_DEFAULT_STACK_SIZE 1048576 // 1MB

namespace slib
{
	
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
		static Ref<Thread> create(const Function<void()>& callback);

		static Ref<Thread> start(const Function<void()>& callback, sl_uint32 stackSize = SLIB_THREAD_DEFAULT_STACK_SIZE);

	public:
		sl_bool start(sl_uint32 stackSize = SLIB_THREAD_DEFAULT_STACK_SIZE);

		void finish();

		sl_bool join(sl_int32 timeout = -1);

		sl_bool finishAndWait(sl_int32 timeout = -1);

		sl_bool wait(sl_int32 timeout = -1);
		
		void wakeSelfEvent();

		const Ref<Event>& getSelfEvent();
		
		void wake();
		
		Ref<Event> getWaitingEvent();
	
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

		const Function<void()>& getCallback();

		static sl_bool sleep(sl_uint32 ms);

		sl_bool isCurrentThread();

		static Ref<Thread> getCurrent();

		static sl_bool isStoppingCurrent();

		static sl_bool isNotStoppingCurrent();
		
		static sl_uint64 getCurrentThreadId();

		static sl_uint64 getCurrentThreadUniqueId();

		// attached objects are removed when the thread is exited
		Ref<Referable> getAttachedObject(const String& name);

		void attachObject(const String& name, Referable* object);

		void removeAttachedObject(const String& name);

	private:
		void* m_handle;
		ThreadPriority m_priority;
	
		sl_bool m_flagRequestStop;
		sl_bool m_flagRunning;
		Function<void()> m_callback;
	
		Ref<Event> m_eventWake;
		Ref<Event> m_eventExit;
		AtomicRef<Event> m_eventWaiting;
	
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
	
}

#define SLIB_SAFE_SLEEP(ms) { slib::Thread::sleep(ms); if (slib::Thread::isStoppingCurrent()) return; }

#endif
