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

#include "slib/graphics/platform.h"

#if defined(SLIB_GRAPHICS_IS_CAIRO)

namespace slib
{

	namespace priv
	{

		namespace cairo
		{

			class CanvasImpl : public CanvasExt
			{
				SLIB_DECLARE_OBJECT

			public:
				cairo_t* m_graphics;

			public:
				CanvasImpl()
				{
					m_graphics = sl_null;
				}

				~CanvasImpl()
				{
					if (m_graphics) {
						::cairo_destroy(m_graphics);
					}
				}

			public:
				static Ref<CanvasImpl> create(CanvasType type, cairo_t* graphics, sl_real width, sl_real height)
				{
					if (graphics) {

						Ref<CanvasImpl> ret = new CanvasImpl();

						if (ret.isNotNull()) {

							ret->m_graphics = graphics;

							ret->setType(type);
							ret->setSize(Size(width, height));

							ret->_setAntiAlias(sl_true);

							return ret;
						}

						::cairo_destroy(graphics);
					}
					return sl_null;

				}

				sl_bool _applyPath(cairo_t* graphics, GraphicsPath* path)
				{
					sl_size n = path->getPointsCount();
					if (n > 0) {
						GraphicsPathPoint* data = path->getPoints();
						for (sl_size i = 0; i < n; i++) {
							switch (data[i].type & GraphicsPathPoint::TypeMask) {
								case GraphicsPathPoint::Begin:
									::cairo_move_to(graphics, data[i].pt.x, data[i].pt.y);
									break;
								case GraphicsPathPoint::Line:
									::cairo_line_to(graphics, data[i].pt.x, data[i].pt.y);
									break;
								case GraphicsPathPoint::BezierCubic:
									if (i + 2 < n) {
										::cairo_curve_to(graphics, data[i].pt.x, data[i].pt.y, data[i+1].pt.x, data[i+1].pt.y, data[i+2].pt.x, data[i+2].pt.y);
										i += 2;
									}
									break;
							}
							if (data[i].type & GraphicsPathPoint::FlagClose) {
								::cairo_close_path(graphics);
							}
						}
						if (path->getFillMode() == FillMode::Winding) {
							::cairo_set_fill_rule(m_graphics, CAIRO_FILL_RULE_WINDING);
						} else {
							::cairo_set_fill_rule(m_graphics, CAIRO_FILL_RULE_EVEN_ODD);
						}
						return sl_true;
					}
					return sl_false;
				}

				void _applyPen(Pen* pen)
				{
					cairo_t* graphics = m_graphics;

					double _width;
					cairo_line_cap_t _cap;
					cairo_line_join_t _join;
					double _miterLimit;
					double _dash[6];
					sl_uint32 _dashLen;

					_width = pen->getWidth();

					switch (pen->getCap()) {
						case LineCap::Square:
							_cap = CAIRO_LINE_CAP_SQUARE;
							break;
						case LineCap::Round:
							_cap = CAIRO_LINE_CAP_ROUND;
							break;
						case LineCap::Flat:
						default:
							_cap = CAIRO_LINE_CAP_BUTT;
							break;
					}

					switch (pen->getJoin()) {
							break;
						case LineJoin::Bevel:
							_join = CAIRO_LINE_JOIN_BEVEL;
							break;
						case LineJoin::Round:
							_join = CAIRO_LINE_JOIN_ROUND;
							break;
						case LineJoin::Miter:
						default:
							_join = CAIRO_LINE_JOIN_MITER;
							break;
					}

					_miterLimit = pen->getMiterLimit();

					switch (pen->getStyle()) {
						case PenStyle::Dot:
							_dash[0] = _width;
							_dash[1] = 2 * _width;
							_dashLen = 2;
							break;
						case PenStyle::Dash:
							_dash[0] = 3 * _width;
							_dash[1] = _dash[0];
							_dashLen = 2;
							break;
						case PenStyle::DashDot:
							_dash[0] = 3 * _width;
							_dash[1] = 2 * _width;
							_dash[2] = _width;
							_dash[3] = _dash[1];
							_dashLen = 4;
							break;
						case PenStyle::DashDotDot:
							_dash[0] = 3 * _width;
							_dash[1] = 2 * _width;
							_dash[2] = _width;
							_dash[3] = _dash[1];
							_dash[4] = _width;
							_dash[5] = _dash[1];
							_dashLen = 6;
							break;
						case PenStyle::Solid:
						default:
							_dashLen = 0;
							break;
					}

					::cairo_set_line_width(graphics, _width);
					::cairo_set_line_cap(graphics, _cap);
					::cairo_set_line_join(graphics, _join);
					::cairo_set_miter_limit(graphics, _miterLimit);
					::cairo_set_dash(graphics, _dash, _dashLen, 0);

					Color4f _color = pen->getColor();
					::cairo_set_source_rgba(graphics, _color.x, _color.y, _color.z, _color.w * getAlpha());
				}

				void _applyBrush(Brush* brush)
				{
					cairo_t* graphics = m_graphics;
					Color4f _color = brush->getColor();
					::cairo_set_source_rgba(graphics, _color.x, _color.y, _color.z, _color.w * getAlpha());
				}

			public:
				void save() override
				{
					::cairo_save(m_graphics);
				}

				void restore() override
				{
					::cairo_restore(m_graphics);
				}

				Rectangle getClipBounds() override
				{
					double x1 = 0;
					double y1 = 0;
					double x2 = 0;
					double y2 = 0;
					::cairo_clip_extents(m_graphics, &x1, &y1, &x2, &y2);
					return Rectangle((sl_real)x1, (sl_real)y1, (sl_real)x2, (sl_real)y2);
				}

				void clipToRectangle(const Rectangle& rect) override
				{
					::cairo_rectangle(m_graphics, rect.left, rect.top, rect.getWidth(), rect.getHeight());
					::cairo_clip(m_graphics);
				}

				void clipToPath(const Ref<GraphicsPath>& path) override
				{
					if (path.isNotNull()) {
						if (_applyPath(m_graphics, path.get())) {
							::cairo_clip(m_graphics);
						}
					}
				}

				void concatMatrix(const Matrix3& other) override
				{
					cairo_matrix_t t;
					t.xx = other.m00;
					t.xy = other.m10;
					t.x0 = other.m20;
					t.yx = other.m01;
					t.yy = other.m11;
					t.y0 = other.m21;
					::cairo_transform(m_graphics, &t);
				}

				void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& in_font, const Color& _color) override
				{
					if (text.isNotEmpty()) {
						Ref<Font> _font = in_font;
						if (_font.isNull()) {
							_font = Font::getDefault();
						}
						if (_font.isNotNull()) {

							cairo_scaled_font_t* font = GraphicsPlatform::getCairoFont(_font.get());

							if (font) {

								::cairo_set_font_face(m_graphics, ::cairo_scaled_font_get_font_face(font));
								::cairo_set_font_size(m_graphics, _font->getSize());

								Color4f color = _color;
								::cairo_set_source_rgba(m_graphics, color.x, color.y, color.z, color.w * getAlpha());

								FontMetrics fm;
								if (_font->getFontMetrics(fm)) {
									y += (fm.leading + fm.ascent);
								}
								::cairo_move_to(m_graphics, x, y);

								::cairo_show_text(m_graphics, text.getData());

								if (_font->isStrikeout() || _font->isUnderline()) {
									sl_real width = _font->measureText(text).x;
									if (_font->isUnderline()) {
										sl_real yLine = y;
										::cairo_move_to(m_graphics, 0, yLine);
										::cairo_line_to(m_graphics, width, yLine);
										::cairo_stroke(m_graphics);
									}
									if (_font->isStrikeout()) {
										sl_real yLine = y - fm.ascent / 2;
										::cairo_move_to(m_graphics, 0, yLine);
										::cairo_line_to(m_graphics, width, yLine);
										::cairo_stroke(m_graphics);
									}
								}

							}

						}
					}
				}

				void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& _pen) override
				{
					Ref<Pen> pen = _pen;
					if (pen.isNull()) {
						pen = Pen::getDefault();
					}
					if (pen.isNotNull()) {
						_applyPen(pen.get());
						::cairo_move_to(m_graphics, pt1.x, pt1.y);
						::cairo_line_to(m_graphics, pt2.x, pt2.y);
						::cairo_stroke(m_graphics);
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
					if (pen.isNotNull()) {
						_applyPen(pen.get());
						::cairo_move_to(m_graphics, points[0].x, points[0].y);
						for (sl_uint32 i = 1; i < countPoints; i++) {
							::cairo_line_to(m_graphics, points[i].x, points[i].y);
						}
						::cairo_stroke(m_graphics);
					}
				}

				void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen) override
				{
					Ref<GraphicsPath> path = GraphicsPath::create();
					if (path.isNotNull()) {
						path->addArc(rect, startDegrees, sweepDegrees);
						drawPath(path, pen, Ref<Brush>::null());
					}
				}

				void drawRectangle(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) override
				{
					::cairo_rectangle(m_graphics, rect.left, rect.top, rect.getWidth(), rect.getHeight());
					_drawPath(pen, brush);
				}

				void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& pen, const Ref<Brush>& brush) override
				{
					Ref<GraphicsPath> path = GraphicsPath::create();
					if (path.isNotNull()) {
						path->addRoundRect(rect, radius);
						drawPath(path, pen, brush);
					}
				}

				void drawEllipse(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) override
				{
					Ref<GraphicsPath> path = GraphicsPath::create();
					if (path.isNotNull()) {
						path->addArc(rect, 0, 360);
						drawPath(path, pen, brush);
					}
				}

				void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode) override
				{
					if (countPoints <= 2) {
						return;
					}
					Ref<GraphicsPath> path = GraphicsPath::create();
					if (path.isNotNull()) {
						path->moveTo(points[0]);
						for (sl_uint32 i = 1; i < countPoints; i++) {
							path->lineTo(points[i]);
						}
						path->closeSubpath();
						path->setFillMode(fillMode);
						drawPath(path, pen, brush);
					}
				}

				void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush) override
				{
					Ref<GraphicsPath> path = GraphicsPath::create();
					if (path.isNotNull()) {
						path->addPie(rect, startDegrees, sweepDegrees);
						drawPath(path, pen, brush);
					}
				}

				void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen, const Ref<Brush>& brush) override
				{
					if (path.isNotNull()) {
						if (_applyPath(m_graphics, path.get())) {
							_drawPath(pen, brush);
						}
					}
				}

				void _drawPath(const Ref<Pen>& _pen, const Ref<Brush>& brush)
				{
					Ref<Pen> pen = _pen;
					if (brush.isNull() && pen.isNull()) {
						pen = Pen::getDefault();
					}
					if (brush.isNotNull()) {
						_applyBrush(brush.get());
						if (pen.isNotNull()) {
							::cairo_fill_preserve(m_graphics);
						} else {
							::cairo_fill(m_graphics);
							return;
						}
					}
					if (pen.isNotNull()) {
						_applyPen(pen.get());
						::cairo_stroke(m_graphics);
					}
				}

				void _setAlpha(sl_real alpha) override
				{
				}

				void _setAntiAlias(sl_bool flag) override
				{
					if (flag) {
						::cairo_set_antialias(m_graphics, CAIRO_ANTIALIAS_GOOD);
					} else {
						::cairo_set_antialias(m_graphics, CAIRO_ANTIALIAS_NONE);
					}
				}

			};

			SLIB_DEFINE_OBJECT(CanvasImpl, Canvas)

		}
	}

	using namespace priv::cairo;

	Ref<Canvas> GraphicsPlatform::createCanvas(CanvasType type, cairo_t* graphics, sl_uint32 width, sl_uint32 height)
	{
		if (graphics) {
			return CanvasImpl::create(type, graphics, (sl_real)width, (sl_real)height);
		}
		return sl_null;
	}

	cairo_t* GraphicsPlatform::getCanvasHandle(Canvas* _canvas)
	{
		if (CanvasImpl* canvas = CastInstance<CanvasImpl>(_canvas)) {
			return canvas->m_graphics;
		}
		return sl_null;
	}

	void GraphicsPlatform::drawImage(Canvas* _canvas, const Rectangle& rectDst, cairo_surface_t* image, const DrawParam& param)
	{
		if (!image) {
			return;
		}

		CanvasImpl* canvas = CastInstance<CanvasImpl>(_canvas);
		if (!canvas) {
			return;
		}

		sl_real dw = rectDst.getWidth();
		sl_real dh = rectDst.getHeight();
		if (dw < SLIB_EPSILON || dh < SLIB_EPSILON) {
			return;
		}

		sl_real sw = cairo_image_surface_get_width(image);
		sl_real sh = cairo_image_surface_get_height(image);
		if (sw < SLIB_EPSILON || sh < SLIB_EPSILON) {
			return;
		}

		cairo_t* graphics = canvas->m_graphics;

		sl_bool flagOpaque = param.isOpaque();
		sl_real alpha = 1;
		if (param.useAlpha && !flagOpaque) {
			alpha = param.alpha;
		}
		
		cairo_save(graphics);
		cairo_translate(graphics, rectDst.left, rectDst.top);
		cairo_scale(graphics, dw / sw, dh / sh);
		cairo_set_source_surface(graphics, image, 0, 0);
		if (alpha == 1) {
			cairo_paint(graphics);
		} else {
			cairo_paint_with_alpha(graphics, alpha);
		}
		cairo_restore(graphics);

	}

	void GraphicsPlatform::drawImage(Canvas* canvas, const Rectangle& rectDst, cairo_surface_t* image, const Rectangle& rectSrc, const DrawParam& param)
	{
		sl_real dw = rectDst.getWidth();
		sl_real dh = rectDst.getHeight();
		if (dw < SLIB_EPSILON || dh < SLIB_EPSILON) {
			return;
		}
		sl_real sw = rectSrc.getWidth();
		sl_real sh = rectSrc.getHeight();
		if (sw < SLIB_EPSILON || sh < SLIB_EPSILON) {
			return;
		}
		cairo_surface_t* sub = cairo_surface_create_for_rectangle(image, rectSrc.left, rectSrc.top, sw, sh);
		if (sub) {
			drawImage(canvas, rectDst, sub, param);
			cairo_surface_destroy(sub);
		}
	}

}

#endif
