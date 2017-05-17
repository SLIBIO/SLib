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

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "slib/core/event.h"
#include "slib/core/platform_windows.h"

namespace slib
{

	class _Win32Event : public Event
	{
	public:
		HANDLE m_hEvent;
		sl_bool m_flagCloseOnRelease;

		_Win32Event()
		{
		}

		~_Win32Event()
		{
			if (m_flagCloseOnRelease) {
				::CloseHandle(m_hEvent);
			}
		}

		static Ref<_Win32Event> create(HANDLE hEvent, sl_bool flagCloseOnRelease)
		{
			Ref<_Win32Event> ret;
			if (hEvent) {
				ret = new _Win32Event;
				if (ret.isNotNull()) {
					ret->m_hEvent = hEvent;
					ret->m_flagCloseOnRelease = flagCloseOnRelease;
					return ret;
				}
				if (flagCloseOnRelease) {
					::CloseHandle(hEvent);
				}
			}
			return ret;
		}

		static Ref<_Win32Event> create(sl_bool flagAutoReset)
		{
			HANDLE hEvent;
#if defined(SLIB_PLATFORM_IS_WIN32)
			hEvent = ::CreateEventW(NULL, flagAutoReset ? FALSE : TRUE, FALSE, NULL);
#else
			hEvent = ::CreateEventEx(NULL, NULL, flagAutoReset ? CREATE_EVENT_INITIAL_SET : CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
#endif
			return create(hEvent, sl_true);
		}

		void _native_set() override
		{
			::SetEvent(m_hEvent);
		}

		void _native_reset() override
		{
			::ResetEvent(m_hEvent);
		}

		sl_bool _native_wait(sl_int32 timeout) override
		{
			DWORD t = INFINITE;
			if (timeout >= 0) {
				t = timeout;
			}
			DWORD ret = WaitForSingleObjectEx(m_hEvent, timeout, TRUE);
			if (ret == WAIT_TIMEOUT) {
				return sl_false;
			} else {
				return sl_true;
			}
		}
	};

	Ref<Event> Windows::createEvent(HANDLE hEvent, sl_bool flagCloseOnRelease)
	{
		return _Win32Event::create(hEvent, flagCloseOnRelease);
	}

	Ref<Event> Event::create(sl_bool flagAutoReset)
	{
		return _Win32Event::create(flagAutoReset);
	}

}

#endif
