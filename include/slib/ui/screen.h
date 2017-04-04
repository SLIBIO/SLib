/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_SCREEN
#define CHECKHEADER_SLIB_UI_SCREEN

#include "definition.h"

#include "../core/object.h"
#include "../math/rectangle.h"

namespace slib
{

	class SLIB_EXPORT Screen : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		Screen();

		~Screen();

	public:
		virtual UIRect getRegion() = 0;
		
	};

}

#endif
