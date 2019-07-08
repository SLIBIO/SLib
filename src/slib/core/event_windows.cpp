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

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "slib/core/event.h"
#include "slib/core/platform_windows.h"

namespace slib
{

	namespace priv
	{
		namespace event
		{

			class EventImpl : public Event
			{
			public:
				HANDLE m_hEvent;
				sl_bool m_flagCloseOnRelease;

				EventImpl()
				{
				}

				~EventImpl()
				{
					if (m_flagCloseOnRelease) {
						::CloseHandle(m_hEvent);
					}
				}

				static Ref<EventImpl> create(HANDLE hEvent, sl_bool flagCloseOnRelease)
				{
					Ref<EventImpl> ret;
					if (hEvent) {
						ret = new EventImpl;
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

				static Ref<EventImpl> create(sl_bool flagAutoReset)
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

		}
	}

	Ref<Event> Windows::createEvent(HANDLE hEvent, sl_bool flagCloseOnRelease)
	{
		return priv::event::EventImpl::create(hEvent, flagCloseOnRelease);
	}

	Ref<Event> Event::create(sl_bool flagAutoReset)
	{
		return priv::event::EventImpl::create(flagAutoReset);
	}

}

#endif
