/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/brush.h"

#include "../../../inc/slib/graphics/platform.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidBrush, "slib/platform/android/ui/UiBrush")
		SLIB_JNI_NEW(init, "()V");
		SLIB_JNI_INT_FIELD(style);
		SLIB_JNI_INT_FIELD(color);
	SLIB_JNI_END_CLASS

	class Android_BrushObject : public Referable
	{
	public:
		JniGlobal<jobject> m_brush;

	public:
		Android_BrushObject(const BrushDesc& desc)
		{
			JniLocal<jobject> jbrush = _JAndroidBrush::init.newObject(sl_null);
			if (jbrush.isNotNull()) {
				JniGlobal<jobject> gbrush = jbrush;
				if (gbrush.isNotNull()) {
					_JAndroidBrush::color.set(jbrush, desc.color.getARGB());
					_JAndroidBrush::style.set(jbrush, (int)(desc.style));
					m_brush = gbrush;
				}
			}
		}
	};

	class Brush_Ext : public Brush
	{
	public:
		Android_BrushObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new Android_BrushObject(m_desc);
				}
			}
			return (Android_BrushObject*)(m_platformObject.get());;
		}

		jobject getPlatformHandle()
		{
			Android_BrushObject* po = getPlatformObject();
			if (po) {
				return po->m_brush.get();
			}
			return 0;
		}
	};

	jobject GraphicsPlatform::getBrushHandle(Brush* brush)
	{
		if (brush) {
			return ((Brush_Ext*)brush)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
