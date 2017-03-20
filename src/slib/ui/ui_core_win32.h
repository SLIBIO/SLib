/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
