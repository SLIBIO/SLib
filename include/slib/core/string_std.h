/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_STRING_STD
#define CHECKHEADER_SLIB_CORE_STRING_STD

#include "string.h"

#include <cstddef>

namespace slib
{
	String operator "" _s(const char*, std::size_t);
	
	String operator "" _s(const sl_char16*, std::size_t);
	
}

#endif
