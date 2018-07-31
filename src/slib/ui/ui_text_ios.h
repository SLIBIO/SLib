/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_TEXT_IOS
#define CHECKHEADER_SLIB_UI_TEXT_IOS

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/platform.h"
#include "slib/ui/text.h"

namespace slib
{
	
	class iOS_TextInput
	{
	public:
		static UIResponder* getNativeInstance(TextInput* input);
		
	};
	
}


#endif

#endif
