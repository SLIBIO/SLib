/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_WIN32_COM
#define CHECKHEADER_SLIB_CORE_WIN32_COM

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

#include "object.h"
#include "memory.h"

namespace slib
{

	class Win32_COM
	{
	public:
		static Memory readAllBytesFromStream(IStream* stream);
	};

}

#define SLIB_WIN32_COM_SAFE_RELEASE(x) {if (x) {x->Release(); x=NULL;}}

#endif

#endif
