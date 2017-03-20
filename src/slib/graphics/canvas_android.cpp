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

#include "../../../inc/slib/graphics/canvas.h"

#include "../../../inc/slib/graphics/platform.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidRect, "android/graphics/Rect")
		SLIB_JNI_INT_FIELD(left);
		SLIB_JNI_INT_FIELD(top);
		SLIB_JNI_INT_FIELD(right);
		SLIB_JNI_INT_FIELD(bottom);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(_JAndroidGraphics, "slib/platform/android/ui/Graphics")
		SLIB_JNI_METHOD(getWidth, "getWidth", "()I");
		SLIB_JNI_METHOD(getHeight, "getHeight", "()I");
		SLIB_JNI_METHOD(save, "save", "()V");
		SLIB_JNI_METHOD(restore, "restore", "()V");
		SLIB_JNI_METHOD(getClipBounds, "getClipBounds", "()Landroid/graphics/Rect;");
		SLIB_JNI_METHOD(clipToRectangle, "clipToRectangle", "(FFFF)V");
		SLIB_JNI_METHOD(clipToPath, "clipToPath", "(Lslib/platform/android/ui/UiPath;)V");
		SLIB_JNI_METHOD(concatMatrix, "concatMatrix", "(FFFFFFFFF)V");
		SLIB_JNI_METHOD(drawText, "drawText", "(Ljava/lang/String;FFLslib/platform/android/ui/UiFont;I)V");
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

	class _Android_Canvas : public Canvas
	{
		SLIB_DECLARE_OBJECT
	public:
		JniGlobal<jobject> m_canvas;

	public:
		static Ref<_Android_Canvas> create(CanvasType type, jobject jcanvas) {
			JniGlobal<jobject> canvas = jcanvas;
			if (canvas.isNotNull()) {
				int width = _JAndroidGraphics::getWidth.callInt(jcanvas);
				int height = _JAndroidGraphics::getHeight.callInt(jcanvas);
				Ref<_Android_Canvas> ret = new _Android_Canvas();
				if (ret.isNotNull()) {
					ret->m_canvas = canvas;
					ret->setType(type);
					ret->setSize(Size((sl_real)width, (sl_real)height));
					return ret;
				}
			}
			return sl_null;
		}

		// override
		void save()
		{
			_JAndroidGraphics::save.call(m_canvas);
		}
		
		// override
		void restore()
		{
			_JAndroidGraphics::restore.call(m_canvas);
		}

		// override
		Rectangle getClipBounds()
		{
			JniLocal<jobject> rect(_JAndroidGraphics::getClipBounds.callObject(m_canvas));
			if (rect.isNotNull()) {
				Rectangle ret;
				ret.left = _JAndroidRect::left.get(rect);
				ret.top = _JAndroidRect::top.get(rect);
				ret.right = _JAndroidRect::right.get(rect);
				ret.bottom = _JAndroidRect::bottom.get(rect);
				return ret;
			}
			Size size = getSize();
			return Rectangle(0, 0, size.x, size.y);
		}

		// override
		void clipToRectangle(const Rectangle& _rect)
		{
			_JAndroidGraphics::clipToRectangle.call(m_canvas, (float)(_rect.left), (float)(_rect.top), (float)(_rect.right), (float)(_rect.bottom));
		}

		// override
		void clipToPath(const Ref<GraphicsPath>& path)
		{
			jobject handle = GraphicsPlatform::getGraphicsPath(path.get());
			if (handle) {
				_JAndroidGraphics::clipToPath.call(m_canvas, handle);
			}
		}

		// override
		void concatMatrix(const Matrix3& matrix)
		{
			_JAndroidGraphics::concatMatrix.call(m_canvas,
					(float)(matrix.m00), (float)(matrix.m10), (float)(matrix.m20),
					(float)(matrix.m01), (float)(matrix.m11), (float)(matrix.m21),
					(float)(matrix.m02), (float)(matrix.m12), (float)(matrix.m22));
		}

		// override
		void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& _font, const Color& color)
		{
			if (text.isNotEmpty()) {
				Ref<Font> font = _font;
				if (font.isNull()) {
					font = Font::getDefault();
				}
				jobject hFont = GraphicsPlatform::getNativeFont(font.get());
				if (hFont) {
					JniLocal<jstring> jtext = Jni::getJniString(text);
					_JAndroidGraphics::drawText.call(m_canvas, jtext.value, (float)x, (float)y, hFont, color.getARGB());
				}
			}
		}

		// override
		void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& _pen)
		{
			Ref<Pen> pen = _pen;
			if (pen.isNull()) {
				pen = Pen::getDefault();
			}
			jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
			if (hPen) {
				_JAndroidGraphics::drawLine.call(m_canvas
						, (float)(pt1.x), (float)(pt1.y), (float)(pt2.x), (float)(pt2.y)
						, hPen);
			}
		}

		// override
		void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen)
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
					_JAndroidGraphics::drawLines.call(m_canvas, jarr.value, hPen);
				}
			}
		}

		// override
		void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real endDegrees, const Ref<Pen>& _pen)
		{
			Ref<Pen> pen = _pen;
			if (pen.isNull()) {
				pen = Pen::getDefault();
			}
			jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
			if (hPen) {
				_JAndroidGraphics::drawArc.call(m_canvas
						, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
						, (float)(startDegrees), (float)(endDegrees)
						, hPen);
			}
		}

		// override
		void drawRectangle(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush)
		{
			Ref<Pen> pen = _pen;
			if (brush.isNull() && pen.isNull()) {
				pen = Pen::getDefault();
			}
			jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
			jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
			if (hPen || hBrush) {
				_JAndroidGraphics::drawRectangle.call(m_canvas
						, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
						, hPen, hBrush);
			}
		}

		// override
		void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& _pen, const Ref<Brush>& brush)
		{
			Ref<Pen> pen = _pen;
			if (brush.isNull() && pen.isNull()) {
				pen = Pen::getDefault();
			}
			jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
			jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
			if (hPen || hBrush) {
				_JAndroidGraphics::drawRoundRectangle.call(m_canvas
						, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
						, (float)(radius.x), (float)(radius.y), hPen, hBrush);
			}
		}

		// override
		void drawEllipse(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush)
		{
			Ref<Pen> pen = _pen;
			if (brush.isNull() && pen.isNull()) {
				pen = Pen::getDefault();
			}
			jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
			jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
			if (hPen || hBrush) {
				_JAndroidGraphics::drawEllipse.call(m_canvas
						, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
						, hPen, hBrush);
			}
		}

		// override
		void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen, const Ref<Brush>& brush, FillMode fillMode)
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
					_JAndroidGraphics::drawPolygon.call(m_canvas, jarr.value, hPen, hBrush, fillMode);
				}
			}
		}

		// override
		void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real endDegrees, const Ref<Pen>& _pen, const Ref<Brush>& brush)
		{
			Ref<Pen> pen = _pen;
			if (brush.isNull() && pen.isNull()) {
				pen = Pen::getDefault();
			}
			jobject hPen = GraphicsPlatform::getPenHandle(pen.get());
			jobject hBrush = GraphicsPlatform::getBrushHandle(brush.get());
			if (hPen || hBrush) {
				_JAndroidGraphics::drawPie.call(m_canvas
						, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
						, (float)(startDegrees), (float)(endDegrees)
						, hPen, hBrush);
			}
		}

		// override
		void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& _pen, const Ref<Brush>& brush)
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
					_JAndroidGraphics::drawPath.call(m_canvas
							, hPath
							, hPen, hBrush);
				}
			}
		}

		// override
		void _setAlpha(sl_real alpha)
		{
			_JAndroidGraphics::setAlpha.call(m_canvas, (float)alpha);
		}

		// override
		void _setAntiAlias(sl_bool flag)
		{
			_JAndroidGraphics::setAntiAlias.call(m_canvas, flag);
		}

	};

	SLIB_DEFINE_OBJECT(_Android_Canvas, Canvas)

	Ref<Canvas> GraphicsPlatform::createCanvas(CanvasType type, jobject jcanvas)
	{
		if (!jcanvas) {
			return sl_null;
		}
		return _Android_Canvas::create(type, jcanvas);
	}

	jobject GraphicsPlatform::getCanvasHandle(Canvas* _canvas)
	{
		if (_Android_Canvas* canvas = CastInstance<_Android_Canvas>(_canvas)) {
			return canvas->m_canvas;
		} else {
			return 0;
		}
	}

}

#endif
