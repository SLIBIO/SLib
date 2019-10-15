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

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

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
				SLIB_JNI_FLOAT_FIELD(x)
				SLIB_JNI_FLOAT_FIELD(y)
				SLIB_JNI_FLOAT_FIELD(x2)
				SLIB_JNI_FLOAT_FIELD(y2)
				SLIB_JNI_FLOAT_FIELD(radius)
				SLIB_JNI_OBJECT_FIELD(colors, "[I")
				SLIB_JNI_OBJECT_FIELD(locations, "[F")
				SLIB_JNI_OBJECT_FIELD(pattern, "Lslib/platform/android/ui/UiBitmap;")
			SLIB_JNI_END_CLASS

			class BrushPlatformObject : public Referable
			{
			public:
				JniGlobal<jobject> m_brush;
				Ref<Bitmap> m_bitmapCache;

			public:
				BrushPlatformObject(const BrushDesc& desc)
				{
					JniLocal<jobject> jbrush = JBrush::init.newObject(sl_null);
					if (jbrush.isNotNull()) {
						JniGlobal<jobject> gbrush = jbrush;
						if (gbrush.isNotNull()) {
							JBrush::style.set(jbrush, (int)(desc.style));
							if (desc.style == BrushStyle::Solid) {
								JBrush::color.set(jbrush, desc.color.getARGB());
								m_brush = gbrush;
							} else if (desc.style == BrushStyle::LinearGradient || desc.style == BrushStyle::RadialGradient) {
								GradientBrushDetail* detail = (GradientBrushDetail*)(desc.detail.get());
								if (detail) {
									ListElements<Color> _colors(detail->colors);
									ListElements<sl_real> _locations(detail->locations);
									sl_size n = _colors.count;
									JniLocal<jintArray> jcolors = Jni::newIntArray(n);
									JniLocal<jfloatArray> jlocations = Jni::newFloatArray(n);
									if (jcolors.isNotNull() && jlocations.isNotNull()) {
										SLIB_SCOPED_BUFFER(jint, 128, colors, n);
										SLIB_SCOPED_BUFFER(float, 128, locations, n);
										for (sl_size i = 0; i < n; i++) {
											colors[i] = (jint)(_colors[i].getARGB());
											locations[i] = (jfloat)(_locations[i]);
										}
										Jni::setIntArrayRegion(jcolors, 0, n, colors);
										Jni::setFloatArrayRegion(jlocations, 0, n, locations);
										JBrush::colors.set(jbrush, jcolors);
										JBrush::locations.set(jbrush, jlocations);
										JBrush::x.set(jbrush, (jfloat)(detail->point1.x));
										JBrush::y.set(jbrush, (jfloat)(detail->point1.y));
										if (desc.style == BrushStyle::LinearGradient) {
											JBrush::x2.set(jbrush, (jfloat)(detail->point2.x));
											JBrush::y2.set(jbrush, (jfloat)(detail->point2.y));
										} else {
											JBrush::radius.set(jbrush, (jfloat)(detail->radius));
										}
										m_brush = gbrush;
									}
								}
							} else if (desc.style == BrushStyle::Texture) {
								TextureBrushDetail* detail = (TextureBrushDetail*)(desc.detail.get());
								if (detail) {
									Bitmap* pattern = detail->pattern.get();
									if (pattern->isImage()) {
										Ref<Bitmap> bitmap = Bitmap::create(((Image*)pattern));
										if (bitmap.isNotNull()) {											
											jobject jbitmap = GraphicsPlatform::getBitmapHandle(bitmap.get());
											if (jbitmap) {
												JBrush::pattern.set(jbrush, jbitmap);
												m_bitmapCache = bitmap;
												m_brush = gbrush;
											}
										}
									} else {
										jobject jbitmap = GraphicsPlatform::getBitmapHandle(pattern);
										if (jbitmap) {
											JBrush::pattern.set(jbrush, jbitmap);
											m_brush = gbrush;
										}
									}
								}
							}
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
