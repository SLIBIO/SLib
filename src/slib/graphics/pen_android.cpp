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

#if defined(SLIB_GRAPHICS_IS_ANDROID)

#include "slib/graphics/pen.h"

#include "slib/graphics/platform.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidPen, "slib/platform/android/ui/UiPen")
		SLIB_JNI_NEW(init, "()V");
		SLIB_JNI_METHOD(setStyle, "setStyle", "(I)V");
		SLIB_JNI_INT_FIELD(cap);
		SLIB_JNI_INT_FIELD(join);
		SLIB_JNI_FLOAT_FIELD(width);
		SLIB_JNI_INT_FIELD(color);
		SLIB_JNI_FLOAT_FIELD(miterLimit);
	SLIB_JNI_END_CLASS

	class _priv_Android_PenObject : public Referable
	{
	public:
		JniGlobal<jobject> m_pen;

	public:
		_priv_Android_PenObject(const PenDesc& desc)
		{
			JniLocal<jobject> jpen = JAndroidPen::init.newObject(sl_null);
			if (jpen.isNotNull()) {
				JniGlobal<jobject> gpen = jpen;
				if (gpen.isNotNull()) {
					JAndroidPen::cap.set(jpen, (int)(desc.cap));
					JAndroidPen::join.set(jpen, (int)(desc.join));
					JAndroidPen::color.set(jpen, desc.color.getARGB());
					JAndroidPen::width.set(jpen, desc.width);
					JAndroidPen::miterLimit.set(jpen, desc.miterLimit);
					JAndroidPen::setStyle.call(jpen, desc.style);				
					m_pen = gpen;
				}
			}
		}
	};

	class _priv_Pen : public Pen
	{
	public:
		_priv_Android_PenObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new _priv_Android_PenObject(m_desc);
				}
			}
			return (_priv_Android_PenObject*)(m_platformObject.get());;
		}

		jobject getPlatformHandle()
		{
			_priv_Android_PenObject* po = getPlatformObject();
			if (po) {
				return po->m_pen.get();
			}
			return 0;
		}
	};

	jobject GraphicsPlatform::getPenHandle(Pen* pen)
	{
		if (pen) {
			return ((_priv_Pen*)pen)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
