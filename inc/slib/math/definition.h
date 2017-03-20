/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_DEFINITION
#define CHECKHEADER_SLIB_MATH_DEFINITION

#include "../core/definition.h"

#define SLIB_DEFINE_GEOMETRY_TYPE(TYPE) \
	template class TYPE##T<float>; \
	template class TYPE##T<double>;

#define SLIB_DEFINE_GEOMETRY_TYPE_EX(TYPE) \
	template class TYPE##T<float>; \
	template class TYPE##T<double>; \
	template class TYPE##T<sl_int32, float>; \
	template class TYPE##T<sl_int64, double>;

#endif
