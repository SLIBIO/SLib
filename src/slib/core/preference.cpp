/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/preference.h"

#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_STATIC_ZERO_INITIALIZED(AtomicString, _g_preference_app_key)

	String Preference::getApplicationKeyName()
	{
		return _g_preference_app_key;
	}

	void Preference::setApplicationKeyName(const String& name)
	{
		_g_preference_app_key = name;
	}

}
