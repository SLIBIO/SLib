/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/event.h"

#include "slib/core/thread.h"

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
		_native_set();
	}

	void Event::reset()
	{
		_native_reset();
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
		sl_bool ret = _native_wait(timeout);
		if (thread.isNotNull()) {
			thread->clearWaitingEvent();
		}
		return ret;
	}

}
