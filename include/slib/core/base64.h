/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_BASE64
#define CHECKHEADER_SLIB_CORE_BASE64

#include "definition.h"

#include "string.h"

namespace slib
{
	
	class SLIB_EXPORT Base64
	{
	public:
		static String encode(const void* byte, sl_size size);

		static String encode(const Memory& mem);

		static sl_size decode(const String& base64, void* buf, sl_size size);

		static Memory decode(const String& base64);
	
	};

}

#endif
