#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_WINDOWS
#define CHECKHEADER_SLIB_CORE_PLATFORM_WINDOWS

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

#include "string.h"
#include "event.h"
#include "object.h"

SLIB_NAMESPACE_BEGIN

typedef BOOL(WINAPI *WINAPI_GetQueuedCompletionStatusEx)
(
	HANDLE CompletionPort,
	LPOVERLAPPED_ENTRY lpCompletionPortEntries,
	ULONG ulCount,
	PULONG ulNumEntriesRemoved,
	DWORD dwMilliseconds,
	BOOL fAlertable
);

class SLIB_EXPORT Windows
{
public:
	static String getStringFromGUID(const GUID& guid);
	
	static sl_bool getGUIDFromString(const String& str, GUID* pguid);

	
	static sl_bool isWindowVisible(HWND hWnd);
	
	static HWND getRootWindow(HWND hWnd);

	
	static String getWindowText(HWND hWnd);
	
	static void setWindowText(HWND hWnd, const String& text);

	
	static sl_bool processWindowHorizontalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel);
	
	static sl_bool processWindowVerticalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel);

	static void setWindowHorizontalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage);
	
	static void setWindowVerticalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage);
	

	static void setDebugFlags();

	static HMODULE loadLibrary(const String& path);
	
	static HMODULE loadLibrary_kernel32();
	
	static WINAPI_GetQueuedCompletionStatusEx getAPI_GetQueuedCompletionStatusEx();
	

	static Ref<Event> createEvent(HANDLE hEvent, sl_bool flagCloseOnRelease = sl_true);

};

SLIB_NAMESPACE_END

#endif

#endif
