/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_GRAPHICS_IS_ANDROID)

#include "slib/graphics/pen.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace android
		{

			SLIB_JNI_BEGIN_CLASS(JPen, "slib/platform/android/ui/UiPen")
				SLIB_JNI_NEW(init, "()V");
				SLIB_JNI_METHOD(setStyle, "setStyle", "(I)V");
				SLIB_JNI_INT_FIELD(cap);
				SLIB_JNI_INT_FIELD(join);
				SLIB_JNI_FLOAT_FIELD(width);
				SLIB_JNI_INT_FIELD(color);
				SLIB_JNI_FLOAT_FIELD(miterLimit);
			SLIB_JNI_END_CLASS

			class PenPlatformObject : public Referable
			{
			public:
				JniGlobal<jobject> m_pen;

			public:
				PenPlatformObject(const PenDesc& desc)
				{
					JniLocal<jobject> jpen = JPen::init.newObject(sl_null);
					if (jpen.isNotNull()) {
						JniGlobal<jobject> gpen = jpen;
						if (gpen.isNotNull()) {
							JPen::cap.set(jpen, (int)(desc.cap));
							JPen::join.set(jpen, (int)(desc.join));
							JPen::color.set(jpen, desc.color.getARGB());
							JPen::width.set(jpen, desc.width);
							JPen::miterLimit.set(jpen, desc.miterLimit);
							JPen::setStyle.call(jpen, desc.style);				
							m_pen = gpen;
						}
					}
				}
			};

			class PenHelper : public Pen
			{
			public:
				PenPlatformObject* getPenPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new PenPlatformObject(m_desc);
						}
					}
					return (PenPlatformObject*)(m_platformObject.get());;
				}

				jobject getPlatformHandle()
				{
					PenPlatformObject* po = getPenPlatformObject();
					if (po) {
						return po->m_pen.get();
					}
					return 0;
				}
			};

		}
	}

	jobject GraphicsPlatform::getPenHandle(Pen* pen)
	{
		if (pen) {
			return ((priv::android::PenHelper*)pen)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
