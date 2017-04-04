/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/resource.h"

namespace slib
{

	static Locale _g_resource_current_locale = Locale::Unknown;

	Locale Resources::getCurrentLocale()
	{
		return _g_resource_current_locale;
	}

	void Resources::setCurrentLocale(const Locale& locale)
	{
		_g_resource_current_locale = locale;
	}

	String Resources::makeResourceName(const String& path)
	{
		String name = path.duplicate();
		sl_char8* szName = name.getData();
		sl_size lenName = name.getLength();
		for (sl_size i = 0; i < lenName; i++) {
			if (!(SLIB_CHAR_IS_C_NAME(szName[i]))) {
				szName[i] = '_';
			}
		}
		return name;
	}

}
