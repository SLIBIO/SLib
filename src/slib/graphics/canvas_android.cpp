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

#include "slib/graphics/canvas.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace android
		{

			SLIB_JNI_BEGIN_CLASS(JRect, "android/graphics/Rect")
				SLIB_JNI_INT_FIELD(left);
				SLIB_JNI_INT_FIELD(top);
				SLIB_JNI_INT_FIELD(right);
				SLIB_JNI_INT_FIELD(bottom);
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JGraphics, "slib/platform/android/ui/Graphics")
				SLIB_JNI_METHOD(getWidth, "getWidth", "()I");
				SLIB_JNI_METHOD(getHeight, "getHeight", "()I");
				SLIB_JNI_METHOD(save, "save", "()V");
				SLIB_JNI_METHOD(restore, "restore", "()V");
				SLIB_JNI_METHOD(getClipBounds, "getClipBounds", "()Landroid/graphics/Rect;");
				SLIB_JNI_METHOD(clipToRectangle, "clipToRectangle", "(FFFF)V");
				SLIB_JNI_METHOD(clipToPath, "clipToPath", "(Lslib/platform/android/ui/UiPath;)V");
				SLIB_JNI_METHOD(concatMatrix, "concatMatrix", "(FFFFFFFFF)V");
				SLIB_JNI_METHOD(drawText, "drawText", "(Ljava/lang/String;FFLslib/platform/android/ui/UiFont;I)V");
				SLIB_JNI_METHOD(drawText2, "drawText", "(Ljava/lang/String;FFLslib/platform/android/ui/UiFont;IIFFF)V");
				SLIB_JNI_METHOD(drawLine, "drawLine", "(FFFFLslib/platform/android/ui/UiPen;)V");
				SLIB_JNI_METHOD(drawLines, "drawLines", "([FLslib/platform/android/ui/UiPen;)V");
				SLIB_JNI_METHOD(drawArc, "drawArc", "(FFFFFFLslib/platform/android/ui/UiPen;)V");
				SLIB_JNI_METHOD(drawRectangle, "drawRectangle", "(FFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
				SLIB_JNI_METHOD(drawRoundRectangle, "drawRoundRectangle", "(FFFFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
				SLIB_JNI_METHOD(drawEllipse, "drawEllipse", "(FFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
				SLIB_JNI_METHOD(drawPolygon, "drawPolygon", "([FLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;I)V");
				SLIB_JNI_METHOD(drawPie, "drawPie", "(FFFFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
				SLIB_JNI_METHOD(drawPath, "drawPath", "(Lslib/platform/android/ui/UiPath;Lslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
				SLIB_JNI_METHOD(setAlpha, "setAlpha", "(F)V");
				SLIB_JNI_METHOD(setAntiAlias, "setAntiAlias", "(Z)V");
			SLIB_JNI_END_CLASS

			class CanvasImpl : public CanvasExt
			{
				SLIB_DECLARE_OBJECT
			public:
				JniGlobal<jobject> m_canvas;

			public:
				static Ref<CanvasImpl> create(CanvasType type, jobject jcanvas) {
					JniGlobal<jobject> canvas = jcanvas;
					if (canvas.isNotNull()) {
						int width = JGraphics::getWidth.callInt(jcanvas);
						int height = JGraphics::getHeight.callInt(jcanvas);
						Ref<CanvasImpl> ret = new CanvasImpl();
						if (ret.isNotNull()) {
							ret->m_canvas = canvas;
							ret->setType(type);
							ret->setSize(Size((sl_real)width, (sl_real)height));
							return ret;
						}
					}
					return sl_null;
				}

				void save() override
				{
					JGraphics::save.call(m_canvas);
				}
				
				void restore() override
				{
					JGraphics::restore.call(m_canvas);
				}

				Rectangle getClipBounds() override
				{
					JniLocal<jobject> rect(JGraphics::getClipBounds.callObject(m_canvas));
					if (rect.isNotNull()) {
						Rectangle ret;
						ret.left = JRect::left.get(rect);
						ret.top = JRect::top.get(rect);
						ret.right = JRect::right.get(rect);
						ret.bottom = JRect::bottom.get(rect);
						return ret;
					}
					Size size = getSize();
					return Rectangle(0, 0, size.x, size.y);
				}

				void clipToRectangle(const Rectangle& _rect) override
				{
					JGraphics::clipToRectangle.call(m_canvas, (float)(_rect.left), (float)(_rect.top), (float)(_rect.right), (float)(_rect.bottom));
				}

				void clipToPath(const Ref<GraphicsPath>& path) override
				{
					jobject handle = GraphicsPlatform::getGraphicsPath(path.get());
					if (handle) {
						JGraphics::clipToPath.call(m_canvas, handle);
					}
				}

				void concatMatrix(const Matrix3& matrix) override
				{
					JGraphics::concatMatrix.call(m_canvas,
							(float)(matrix.m00), (float)(matrix.m10), (float)(matrix.m20),
							(float)(matrix.m01), (float)(matrix.m11), (float)(matrix.m21),
							(float)(matrix.m02), (float)(matrix.m12), (float)(matrix.m22));
				}

				void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& _pen) override
				{
					Ref<Pen> pen = _pen;
					if (pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					if (hPen) {
						JGraphics::drawLine.call(m_canvas
								, (float)(pt1.x), (float)(pt1.y), (float)(pt2.x), (float)(pt2.y)
								, hPen);
					}
				}

				void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen) override
				{
					if (countPoints < 2) {
						return;
					}
					Ref<Pen> pen = _pen;
					if (pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					if (hPen) {
						JniLocal<jfloatArray> jarr = Jni::newFloatArray(countPoints*2);
						if (jarr.isNotNull()) {
							Jni::setFloatArrayRegion(jarr, 0, countPoints*2, (jfloat*)(points));
							JGraphics::drawLines.call(m_canvas, jarr.value, hPen);
						}
					}
				}

				void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real endDegrees, const Ref<Pen>& _pen) override
				{
					Ref<Pen> pen = _pen;
					if (pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					if (hPen) {
						JGraphics::drawArc.call(m_canvas
								, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
								, (float)(startDegrees), (float)(endDegrees)
								, hPen);
					}
				}

				void drawRectangle(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					Ref<Pen> pen = _pen;
					if (brush.isNull() && pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
					if (hPen || hBrush) {
						JGraphics::drawRectangle.call(m_canvas
								, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
								, hPen, hBrush);
					}
				}

				void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					Ref<Pen> pen = _pen;
					if (brush.isNull() && pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
					if (hPen || hBrush) {
						JGraphics::drawRoundRectangle.call(m_canvas
								, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
								, (float)(radius.x), (float)(radius.y), hPen, hBrush);
					}
				}

				void drawEllipse(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					Ref<Pen> pen = _pen;
					if (brush.isNull() && pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
					if (hPen || hBrush) {
						JGraphics::drawEllipse.call(m_canvas
								, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
								, hPen, hBrush);
					}
				}

				void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen, const Ref<Brush>& brush, FillMode fillMode) override
				{
					if (countPoints <= 2) {
						return;
					}
					Ref<Pen> pen = _pen;
					if (brush.isNull() && pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
					if (hPen || hBrush) {
						JniLocal<jfloatArray> jarr = Jni::newFloatArray(countPoints*2);
						if (jarr.isNotNull()) {
							Jni::setFloatArrayRegion(jarr, 0, countPoints*2, (jfloat*)(points));
							JGraphics::drawPolygon.call(m_canvas, jarr.value, hPen, hBrush, fillMode);
						}
					}
				}

				void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real endDegrees, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					Ref<Pen> pen = _pen;
					if (brush.isNull() && pen.isNull()) {
						pen = Pen::getDefault();
					}
					jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
					jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
					if (hPen || hBrush) {
						JGraphics::drawPie.call(m_canvas
								, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
								, (float)(startDegrees), (float)(endDegrees)
								, hPen, hBrush);
					}
				}
				
				void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					jobject hPath = GraphicsPlatform::getGraphicsPath(path.get());
					if (hPath) {
						Ref<Pen> pen = _pen;
						if (brush.isNull() && pen.isNull()) {
							pen = Pen::getDefault();
						}
						jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
						jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
						if (hPen || hBrush) {
							JGraphics::drawPath.call(m_canvas
									, hPath
									, hPen, hBrush);
						}
					}
				}

				void onDrawText(const StringParam& _text, sl_real x, sl_real y, const Ref<Font>& font, const DrawTextParam& param) override
				{
					StringData16 text(_text);
					if (text.isNotEmpty()) {
						jobject hFont = GraphicsPlatform::getNativeFont(font.get());
						if (hFont) {
							JniLocal<jstring> jtext = Jni::getJniString(text.getData(), text.getLength());
							sl_real shadowOpacity = param.shadowOpacity;
							if (shadowOpacity > 0.0001f) {
								Color shadowColor = param.shadowColor;
								shadowColor.multiplyAlpha((float)shadowOpacity);
								JGraphics::drawText2.call(m_canvas, jtext.value, (jfloat)x, (jfloat)y, hFont, (jint)(param.color.getARGB()),
									(jint)(shadowColor.getARGB()), (jfloat)(param.shadowRadius), (jfloat)(param.shadowOffset.x), (jfloat)(param.shadowOffset.y));
							} else {
								JGraphics::drawText.call(m_canvas, jtext.value, (jfloat)x, (jfloat)y, hFont, (jint)(param.color.getARGB()));
							}
						}
					}
				}

				void _setAlpha(sl_real alpha) override
				{
					JGraphics::setAlpha.call(m_canvas, (float)alpha);
				}

				void _setAntiAlias(sl_bool flag) override
				{
					JGraphics::setAntiAlias.call(m_canvas, flag);
				}

			};

			SLIB_DEFINE_OBJECT(CanvasImpl, Canvas)

		}
	}

	using namespace priv::android;

	Ref<Canvas> GraphicsPlatform::createCanvas(CanvasType type, jobject jcanvas)
	{
		if (!jcanvas) {
			return sl_null;
		}
		return CanvasImpl::create(type, jcanvas);
	}

	jobject GraphicsPlatform::getCanvasHandle(Canvas* _canvas)
	{
		if (CanvasImpl* canvas = CastInstance<CanvasImpl>(_canvas)) {
			return canvas->m_canvas;
		} else {
			return 0;
		}
	}

}

#endif
