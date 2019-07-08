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

#if defined(SLIB_PLATFORM_IS_UNIX)

#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "slib/core/event.h"

#include "slib/core/thread.h"

namespace slib
{
	
	namespace priv
	{
		namespace event
		{
			
			class EventImpl : public Event
			{
			public:
				pthread_cond_t m_cond;
				pthread_mutex_t m_mutex;
				sl_bool m_signal;
				sl_bool m_flagAutoReset;
				
				EventImpl()
				{
				}
				
				~EventImpl()
				{
					pthread_cond_destroy(&m_cond);
					pthread_mutex_destroy(&m_mutex);
				}
				
				static Ref<EventImpl> create(sl_bool flagAutoReset)
				{
					Ref<EventImpl> ret = new EventImpl;
					if (ret.isNotNull()) {
						pthread_cond_init(&(ret->m_cond), NULL);
						pthread_mutex_init(&(ret->m_mutex), NULL);
						ret->m_flagAutoReset = flagAutoReset;
						ret->m_signal = sl_false;
					}
					return ret;
				}
				
				void _native_set() override
				{
					pthread_mutex_lock(&m_mutex);
					m_signal = sl_true;
					if (m_flagAutoReset) {
						pthread_cond_signal(&m_cond);
					} else {
						pthread_cond_broadcast(&m_cond);
					}
					pthread_mutex_unlock(&m_mutex);
				}
				
				void _native_reset() override
				{
					pthread_mutex_lock(&m_mutex);
					m_signal = sl_false;
					pthread_mutex_unlock(&m_mutex);
				}
				
				sl_bool _native_wait(sl_int32 timeout) override
				{
					sl_bool ret = sl_true;
					
					pthread_mutex_lock(&m_mutex);
					
					struct timeval now;
					struct timespec to;
					sl_int32 _t = timeout;
					if (timeout < 0) {
						_t = 0;
					}
					{
						gettimeofday(&now, NULL);
						long ms = (now.tv_usec / 1000) + _t;
						long s = now.tv_sec + (ms / 1000);
						ms = ms % 1000;
						to.tv_sec = s;
						to.tv_nsec = ms * 1000000;
					}
					
					while (! m_signal) {
						if (Thread::isStoppingCurrent()) {
							break;
						}
						if (timeout >= 0) {
							if (pthread_cond_timedwait(&m_cond, &m_mutex, &to)) {
								ret = sl_false;
								break;
							}
						} else {
#ifdef SLIB_PLATFORM_IS_BLACKBERRY
							_t = 10000;
							{
								gettimeofday(&now, NULL);
								long ms = (now.tv_usec / 1000) + _t;
								long s = now.tv_sec + (ms / 1000);
								ms = ms % 1000;
								to.tv_sec = s;
								to.tv_nsec = ms * 1000000;
							}
							pthread_cond_timedwait(&m_cond, &m_mutex, &to);
#else
							pthread_cond_wait(&m_cond, &m_mutex);
#endif
						}
					}
					if (m_flagAutoReset) {
						m_signal = sl_false;
					}
					pthread_mutex_unlock(&m_mutex);
					return ret;
				}
			};
			
		}
	}

	Ref<Event> Event::create(sl_bool flagAutoReset)
	{
		return priv::event::EventImpl::create(flagAutoReset);
	}

}

#endif

