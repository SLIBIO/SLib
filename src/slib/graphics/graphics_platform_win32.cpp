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

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/graphics/platform.h"

namespace slib
{

	COLORREF GraphicsPlatform::getColorRef(const Color& color)
	{
		return (COLORREF)(color.getBGR());
	}

	Color GraphicsPlatform::getColorFromColorRef(COLORREF cr)
	{
		Color ret;
		ret.setBGR((sl_uint32)cr);
		ret.a = 255;
		return ret;
	}

}

#endif
