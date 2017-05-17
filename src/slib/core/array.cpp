/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/array.h"
#include "slib/core/array2d.h"

namespace slib
{
	
	SLIB_DEFINE_ROOT_OBJECT(CArrayBase)
	
	CArrayBase::CArrayBase() noexcept
	{
	}
	
	CArrayBase::~CArrayBase() noexcept
	{
	}

	
	SLIB_DEFINE_ROOT_OBJECT(CArray2DBase)
	
	CArray2DBase::CArray2DBase() noexcept
	{
	}
	
	CArray2DBase::~CArray2DBase() noexcept
	{
	}

}
