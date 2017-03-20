/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_WIN32

#include "../../../inc/slib/core/preference.h"

#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/platform_windows.h"

namespace slib
{

	void Preference::setValue(const String& key, const Json& value)
	{
		if (key.isEmpty()) {
			return;
		}
		String appName = getApplicationKeyName();
		if (appName.isEmpty()) {
			return;
		}
		Windows::setRegistryValue(HKEY_CURRENT_USER, "Software\\" + appName, key, value.toJsonString());
	}

	Json Preference::getValue(const String& key)
	{
		if (key.isEmpty()) {
			return sl_null;
		}
		String appName = getApplicationKeyName();
		if (appName.isEmpty()) {
			return sl_null;
		}
		Variant v;
		if (Windows::getRegistryValue(HKEY_CURRENT_USER, "Software\\" + appName, key, &v)) {
			String s = v.getString();
			if (s.isNotEmpty()) {
				return Json::parseJson(s);
			}
		}
		return sl_null;
	}

}

#endif