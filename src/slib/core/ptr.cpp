/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/ptr.h"

namespace slib
{
	
	struct _priv_Ptr_Const
	{
		void* _ptr;
		void* ref;
		sl_int32 lock;
	};

	const _priv_Ptr_Const _priv_Ptr_Null = {0, 0, 0};

}
