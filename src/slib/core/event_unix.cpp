#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_UNIX)

#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "../../../inc/slib/core/event.h"
#include "../../../inc/slib/core/thread.h"

SLIB_NAMESPACE_BEGIN

class _UnixEvent : public Event
{
public:
	pthread_cond_t m_cond;
	pthread_mutex_t m_mutex;
	sl_bool m_signal;
	sl_bool m_flagAutoReset;
	
	_UnixEvent()
	{
	}

	~_UnixEvent()
	{
		pthread_cond_destroy(&m_cond);
		pthread_mutex_destroy(&m_mutex);
	}

	static Ref<_UnixEvent> create(sl_bool flagAutoReset)
	{
		Ref<_UnixEvent> ret = new _UnixEvent;
		if (ret.isNotNull()) {
			pthread_cond_init(&(ret->m_cond), NULL);
			pthread_mutex_init(&(ret->m_mutex), NULL);
			ret->m_flagAutoReset = flagAutoReset;
			ret->m_signal = sl_false;
		}
		return ret;
	}

	// override
	void __set()
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

	// override
	void __reset()
	{
		pthread_mutex_lock(&m_mutex);
		m_signal = sl_false;
		pthread_mutex_unlock(&m_mutex);
	}

	// override
	sl_bool __wait(sl_int32 timeout)
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
			to.tv_sec = now.tv_sec + _t / 1000;
			to.tv_nsec = now.tv_usec * 1000 + (_t % 1000) * 1000 * 1000;
			if ((to.tv_nsec / 1000000000) >= 1) {
				to.tv_sec += to.tv_nsec / 1000000000;
				to.tv_nsec = to.tv_nsec % 1000000000;
			}
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
					to.tv_sec = now.tv_sec + _t / 1000;
					to.tv_nsec = now.tv_usec * 1000 + (_t % 1000) * 1000 * 1000;
					if ((to.tv_nsec / 1000000000) >= 1) {
						to.tv_sec += to.tv_nsec / 1000000000;
						to.tv_nsec = to.tv_nsec % 1000000000;
					}
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
	return _UnixEvent::create(flagAutoReset);
}

SLIB_NAMESPACE_END

#endif

