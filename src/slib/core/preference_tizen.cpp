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

#ifdef SLIB_PLATFORM_IS_TIZEN

#include "slib/core/preference.h"

#include "slib/core/json.h"

#include <app_preference.h>
#include <stdlib.h>

namespace slib
{

	void Preference::setValue(const String& key, const Json& value)
	{
		if (key.isEmpty()) {
			return;
		}
		String json = value.toJsonString();
		::preference_set_string(key.getData(), json.getData());
	}

	Json Preference::getValue(const String& key)
	{
		if (key.isEmpty()) {
			return sl_null;
		}
		char* buf = sl_null;
		::preference_get_string(key.getData(), &buf);
		if (buf) {
			String s = buf;
			Json ret;
			if (s.isNotEmpty()) {
				ret = Json::parseJson(s);
			}
			::free(buf);
			return ret;
		}
		return sl_null;
	}

}

#endif
