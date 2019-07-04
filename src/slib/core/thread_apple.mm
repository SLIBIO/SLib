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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#import <Foundation/Foundation.h>

#include "slib/core/thread.h"

@interface SLIBThread : NSObject
@end

@implementation SLIBThread
-(void)run_callback: (NSMutableDictionary*)parameter
{
	long long _thread = [[parameter objectForKey:@"thread_object"] longLongValue];
	slib::Thread* pThread = (slib::Thread*)_thread;
	@autoreleasepool {
		pThread->_run();
	}
	pThread->decreaseReference();
}
@end

namespace slib
{

	Thread* Thread::_nativeGetCurrentThread() {
		NSThread *thread_this = [NSThread currentThread];
		long long thread_id = 0;
		if (thread_this != NULL) {
			NSMutableDictionary *dict = [thread_this threadDictionary];
			thread_id = [[dict objectForKey:@"thread_current"] longLongValue];
		}
		return (Thread*)thread_id;
	}

	void Thread::_nativeSetCurrentThread(Thread* thread) {
		NSThread *thread_this = [NSThread currentThread];
		if (thread_this != NULL) {
			NSMutableDictionary *dict = [thread_this threadDictionary];
			dict[@"thread_current"] = [NSNumber numberWithLongLong:(long long)thread];
		}
	}

	sl_uint64 Thread::_nativeGetCurrentThreadUniqueId()
	{
		NSThread *thread_this = [NSThread currentThread];
		long long thread_id = 0;
		if (thread_this != NULL) {
			NSMutableDictionary *dict = [thread_this threadDictionary];
			thread_id = [[dict objectForKey:@"thread_unique_id"] longLongValue];
		}
		return thread_id;
	}

	void Thread::_nativeSetCurrentThreadUniqueId(sl_uint64 n)
	{
		NSThread *thread_this = [NSThread currentThread];
		if (thread_this != NULL) {
			NSMutableDictionary *dict = [thread_this threadDictionary];
			dict[@"thread_unique_id"] = [NSNumber numberWithLongLong:n];
		}
	}

	void Thread::_nativeStart(sl_uint32 stackSize)
	{
		this->increaseReference();

		SLIBThread *obj = [[SLIBThread alloc] init];
		NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
		params[@"thread_object"] = [NSNumber numberWithLong:(long)this];
		NSThread* thread=[[NSThread alloc]initWithTarget:obj
												selector:@selector(run_callback:) object:params];
		if (thread != NULL) {
			m_handle = (__bridge void*)thread;
			stackSize = ((stackSize - 1) | 4095) + 1;
			[thread setStackSize:stackSize];
			[thread start];
		} else {
			m_handle = 0;
			this->decreaseReference();
		}
	}

	void Thread::_nativeClose()
	{
	}

	static double _thread_getMacPriority(ThreadPriority priority)
	{
		double min = 0;
		double max = 1;
		if (min < 0 || max < 0) {
			return -1;
		}
		switch (priority) {
			case ThreadPriority::Lowest:
				return min;
			case ThreadPriority::BelowNormal:
				return (min * 3 + max) / 4;
			case ThreadPriority::Normal:
				return (min + max) / 2;
			case ThreadPriority::AboveNormal:
				return (min + max * 3) / 4;
			case ThreadPriority::Highest:
				return max;
		}
		return -1;
	}

	void Thread::_nativeSetPriority()
	{
		double p = _thread_getMacPriority(m_priority);
		NSThread* thread = (__bridge NSThread*)m_handle;
		if(thread != NULL) {
			[thread setThreadPriority:p];
		}
	}

}

#endif
