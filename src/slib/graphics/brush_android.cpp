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

#include "slib/graphics/brush.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace android
		{

			SLIB_JNI_BEGIN_CLASS(JBrush, "slib/platform/android/ui/UiBrush")
				SLIB_JNI_NEW(init, "()V");
				SLIB_JNI_INT_FIELD(style);
				SLIB_JNI_INT_FIELD(color);
			SLIB_JNI_END_CLASS

			class BrushPlatformObject : public Referable
			{
			public:
				JniGlobal<jobject> m_brush;

			public:
				BrushPlatformObject(const BrushDesc& desc)
				{
					JniLocal<jobject> jbrush = JBrush::init.newObject(sl_null);
					if (jbrush.isNotNull()) {
						JniGlobal<jobject> gbrush = jbrush;
						if (gbrush.isNotNull()) {
							JBrush::color.set(jbrush, desc.color.getARGB());
							JBrush::style.set(jbrush, (int)(desc.style));
							m_brush = gbrush;
						}
					}
				}
			};

			class BrushHelper : public Brush
			{
			public:
				BrushPlatformObject* getPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new BrushPlatformObject(m_desc);
						}
					}
					return (BrushPlatformObject*)(m_platformObject.get());;
				}

				jobject getPlatformHandle()
				{
					BrushPlatformObject* po = getPlatformObject();
					if (po) {
						return po->m_brush.get();
					}
					return 0;
				}
			};

		}
	}

	using namespace priv::android;

	jobject GraphicsPlatform::getBrushHandle(Brush* brush)
	{
		if (brush) {
			return ((BrushHelper*)brush)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
