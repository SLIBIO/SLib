#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#import <Foundation/Foundation.h>

#include "../../../inc/slib/core/thread.h"

@interface _slib_apple_ThreadObject : NSObject
@end

@implementation _slib_apple_ThreadObject
-(void)_slib_thread_callback: (NSMutableDictionary*)parameter
{
	long long _thread = [[parameter objectForKey:@"thread_object"] longLongValue];
	slib::Thread* pThread = (slib::Thread*)_thread;
	pThread->_run();
	pThread->decreaseReference();
}
@end

SLIB_NAMESPACE_BEGIN
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

	_slib_apple_ThreadObject *obj = [[_slib_apple_ThreadObject alloc] init];
	NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
	params[@"thread_object"] = [NSNumber numberWithLong:(long)this];
	NSThread* thread=[[NSThread alloc]initWithTarget:obj
											selector:@selector(_slib_thread_callback:) object:params];
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
        case threadPriority_Lowest:
            return min;
        case threadPriority_BelowNormal:
            return (min * 3 + max) / 4;
        case threadPriority_Normal:
            return (min + max) / 2;
        case threadPriority_AboveNormal:
            return (min + max * 3) / 4;
        case threadPriority_Highest:
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

SLIB_NAMESPACE_END

#endif
