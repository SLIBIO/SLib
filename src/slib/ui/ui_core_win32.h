#ifndef CHECKHEADER_SLIB_UI_CORE_WIN32
#define CHECKHEADER_SLIB_UI_CORE_WIN32

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

namespace slib
{

	class Win32_UI_Shared
	{
	public:
		HINSTANCE hInstance;
		ATOM wndClassForView;
		ATOM wndClassForWindow;
		HWND hWndMessage;

	private:
		ATOM m_wndClassForMessage;

	public:
		Win32_UI_Shared();
		~Win32_UI_Shared();

	public:
		static Win32_UI_Shared* get();

	};

}

#endif

#endif
