#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "../../../inc/slib/core/event.h"
#include "../../../inc/slib/core/platform_windows.h"

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

		// override
		void __set()
		{
			::SetEvent(m_hEvent);
		}

		// override
		void __reset()
		{
			::ResetEvent(m_hEvent);
		}

		// override
		sl_bool __wait(sl_int32 timeout)
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
