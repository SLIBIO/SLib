/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_PREFERENCE
#define CHECKHEADER_SLIB_CORE_PREFERENCE

#include "definition.h"

#include "variant.h"
#include "json.h"

namespace slib
{
	
	class SLIB_EXPORT Preference
	{
	public:
		static void setValue(const String& key, const Json& value);
		
		static Json getValue(const String& key);

		template <class T>
		SLIB_INLINE static void getValue(const String& key, T& _out)
		{
			getValue(key).get(_out);
		}
		
		template <class T>
		SLIB_INLINE static void getValue(const String& key, T& _out, const T& _def)
		{
			getValue(key).get(_out, _def);
		}

		// used for Win32 applications
		static String getApplicationKeyName();

		static void setApplicationKeyName(const String& name);
	
	};

}

#endif
