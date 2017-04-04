/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_CORE_COMMON
#define CHECKHEADER_SLIB_UI_CORE_COMMON

#include "slib/ui/core.h"

namespace slib
{
	class _UIDispatcher
	{
	public:
		static sl_bool addCallback(const Function<void()>& callback);
		
		static void processCallbacks();
		
		static sl_bool addDelayedCallback(const Function<void()>& callback, sl_reg& ptr);
		
		static void processDelayedCallback(sl_reg ptr);
		
		static void removeAllCallbacks();
		
	};	
}

#endif
