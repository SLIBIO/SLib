/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

	class _priv_UnixEvent : public Event
	{
	public:
		pthread_cond_t m_cond;
		pthread_mutex_t m_mutex;
		sl_bool m_signal;
		sl_bool m_flagAutoReset;
		
		_priv_UnixEvent()
		{
		}

		~_priv_UnixEvent()
		{
			pthread_cond_destroy(&m_cond);
			pthread_mutex_destroy(&m_mutex);
		}

		static Ref<_priv_UnixEvent> create(sl_bool flagAutoReset)
		{
			Ref<_priv_UnixEvent> ret = new _priv_UnixEvent;
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

	Ref<Event> Event::create(sl_bool flagAutoReset)
	{
		return _priv_UnixEvent::create(flagAutoReset);
	}

}

#endif

