#include "../../../inc/slib/core/event.h"

#include "../../../inc/slib/core/thread.h"

namespace slib
{

	SLIB_DEFINE_ROOT_OBJECT(Event)

	Event::Event()
	{
	}

	Event::~Event()
	{
	}

	void Event::set()
	{
		__set();
	}

	void Event::reset()
	{
		__reset();
	}

	sl_bool Event::wait(sl_int32 timeout)
	{
		Ref<Thread> thread = Thread::getCurrent();
		if (thread.isNotNull()) {
			if (thread->isStopping()) {
				return sl_false;
			}
			thread->setWaitingEvent(this);
		}
		sl_bool ret = __wait(timeout);
		if (thread.isNotNull()) {
			thread->clearWaitingEvent();
		}
		return ret;
	}

}
