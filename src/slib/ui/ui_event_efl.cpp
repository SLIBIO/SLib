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

#if defined(SLIB_UI_IS_EFL)

#include "slib/ui/event.h"
#include "slib/ui/core.h"
#include "slib/ui/platform.h"

namespace slib
{

	sl_uint32 UIEvent::getSystemKeycode(Keycode key)
	{
		return -1;
	}
	
	Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
	{
		return Keycode::Unknown;
	}
	
}

#endif
