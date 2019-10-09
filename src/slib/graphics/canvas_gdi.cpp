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

#if defined(SLIB_GRAPHICS_IS_GDI)

#include "slib/graphics/canvas.h"

#include "slib/graphics/platform.h"
#include "slib/core/queue.h"

namespace slib
{

	namespace priv
	{
		namespace gdi
		{

			static void ApplyAlphaToColor(Gdiplus::Color& color, sl_real alpha)
			{
				BYTE a = color.GetA();
				BYTE r = color.GetR();
				BYTE g = color.GetG();
				BYTE b = color.GetB();
				int t = color.GetA();
				t = (int)(t * alpha);
				if (t < 0) {
					t = 0;
				}
				if (t > 255) {
					t = 255;
				}
				color.SetValue(Gdiplus::Color::MakeARGB((BYTE)t, r, g, b));
			}


#define DRAW_PEN_BEGIN \
			Gdiplus::Graphics* graphics = m_graphics; \
			sl_real alpha = getAlpha(); \
			Ref<Pen> pen = _pen; \
			if (pen.isNull()) { \
				pen = Pen::getDefault(); \
			} \
			Gdiplus::Pen* hPen = GraphicsPlatform::getPenHandle(pen.get()); \
			Gdiplus::Pen* hPenClone = NULL; \
			if (alpha < 0.995f) { \
				if (hPen) { \
					hPenClone = hPen->Clone(); \
					Gdiplus::Color color; \
					hPenClone->GetColor(&color); \
					ApplyAlphaToColor(color, alpha); \
					hPenClone->SetColor(color); \
					hPen = hPenClone; \
				} \
			}

#define DRAW_PEN_END \
			if (hPenClone) { \
				delete hPenClone; \
			}

#define DRAW_PEN_BRUSH_BEGIN \
			Gdiplus::Graphics* graphics = m_graphics; \
			sl_real alpha = getAlpha(); \
			Gdiplus::Brush* hBrush = GraphicsPlatform::getBrushHandle(brush.get()); \
			Ref<Pen> pen = _pen; \
			if (brush.isNull() && pen.isNull()) { \
				pen = Pen::getDefault(); \
			} \
			Gdiplus::Pen* hPen = GraphicsPlatform::getPenHandle(pen.get()); \
			Gdiplus::Brush* hBrushClone = NULL; \
			Gdiplus::Pen* hPenClone = NULL; \
			if (alpha < 0.995f) { \
				if (hBrush && hBrush->GetType() == Gdiplus::BrushTypeSolidColor) { \
					hBrushClone = hBrush->Clone(); \
					Gdiplus::Color color; \
					((Gdiplus::SolidBrush*)hBrushClone)->GetColor(&color); \
					ApplyAlphaToColor(color, alpha); \
					((Gdiplus::SolidBrush*)hBrushClone)->SetColor(color); \
					hBrush = hBrushClone; \
				} \
				if (hPen) { \
					hPenClone = hPen->Clone(); \
					Gdiplus::Color color; \
					hPenClone->GetColor(&color); \
					ApplyAlphaToColor(color, alpha); \
					hPenClone->SetColor(color); \
					hPen = hPenClone; \
				} \
			}

#define DRAW_PEN_BRUSH_END \
			if (hBrushClone) { \
				delete hBrushClone; \
			} \
			if (hPenClone) { \
				delete hPenClone; \
			}

			class CanvasImpl : public CanvasExt
			{
				SLIB_DECLARE_OBJECT
			public:
				Gdiplus::Graphics* m_graphics;
				LinkedStack<Gdiplus::GraphicsState> m_stackState;
				sl_bool m_flagFreeOnRelease;
				Ref<Referable> m_ref;

			public:
				CanvasImpl()
				{
				}

				~CanvasImpl()
				{
					if (m_flagFreeOnRelease) {
						delete m_graphics;			
					}
				}

			public:
				static Ref<CanvasImpl> create(CanvasType type, Gdiplus::Graphics* graphics, sl_real width, sl_real height, sl_bool flagFreeOnRelease, Referable* ref)
				{
					if (graphics) {
						Ref<CanvasImpl> ret = new CanvasImpl();
						if (ret.isNotNull()) {
							ret->m_graphics = graphics;
							ret->m_flagFreeOnRelease = flagFreeOnRelease;
							ret->m_ref = ref;

							ret->setType(type);
							ret->setSize(Size(width, height));

							ret->_setAntiAlias(sl_true);
							return ret;
						}
						if (flagFreeOnRelease) {
							delete graphics;
						}
					}
					return sl_null;
				}

				void save() override
				{
					Gdiplus::GraphicsState state = m_graphics->Save();
					m_stackState.push(state);
				}
				
				void restore() override
				{
					Gdiplus::GraphicsState state;
					if (m_stackState.pop(&state)) {
						m_graphics->Restore(state);
					}
				}

				Rectangle getClipBounds() override
				{
					Gdiplus::RectF rc;
					Gdiplus::Status status = m_graphics->GetClipBounds(&rc);
					if (status == Gdiplus::Ok) {
						return Rectangle(rc.X, rc.Y, rc.X + rc.Width, rc.Y + rc.Height);
					}

					Size size = getSize();
					return Rectangle(0, 0, size.x, size.y);
				}

				void clipToRectangle(const Rectangle& _rect) override
				{
					Gdiplus::RectF rect(_rect.left, _rect.top, _rect.getWidth(), _rect.getHeight());
					m_graphics->IntersectClip(rect);
				}

				void clipToPath(const Ref<GraphicsPath>& path) override
				{
					if (path.isNotNull()) {
						Gdiplus::GraphicsPath* handle = GraphicsPlatform::getGraphicsPath(path.get());
						if (handle) {
							m_graphics->SetClip(handle, Gdiplus::CombineModeIntersect);
						}
					}
				}

				Matrix3 getMatrix()
				{
					Matrix3 ret;
					Gdiplus::Matrix m;
					m_graphics->GetTransform(&m);
					float f[6];
					m.GetElements(f);
					ret.m00 = f[0];
					ret.m01 = f[1];
					ret.m02 = 0;
					ret.m10 = f[2];
					ret.m11 = f[3];
					ret.m12 = 0;
					ret.m20 = f[4];
					ret.m21 = f[5];
					ret.m22 = 1;
					return ret;
				}

				void setMatrix(const Matrix3& matrix)
				{
					Gdiplus::Matrix m(matrix.m00, matrix.m01, matrix.m10, matrix.m11, matrix.m20, matrix.m21);
					m_graphics->SetTransform(&m);
				}

				void concatMatrix(const Matrix3& other) override
				{
					Matrix3 mat = other;
					mat.multiply(getMatrix());
					setMatrix(mat);
				}

				void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& font, const Color& color) override
				{
					CanvasImpl::drawText16(text, x, y, font, color);
				}
				
				void drawText16(const String16& text, sl_real x, sl_real y, const Ref<Font>& _font, const Color& color) override
				{
					if (text.isNotEmpty()) {
						Gdiplus::Graphics* graphics = m_graphics;
						Ref<Font> font = _font;
						if (font.isNull()) {
							font = Font::getDefault();
						}
						if (font.isNotNull()) {
							Gdiplus::Font* pf = GraphicsPlatform::getGdiplusFont(font.get());
							if (pf) {
								Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip);
								int a = color.a;
								sl_real alpha = getAlpha();
								if (alpha < 0.995f) {
									a = (int)(a * alpha);
									if (a < 0) {
										a = 0;
									}
									if (a > 255) {
										a = 255;
									}
								}
								Gdiplus::SolidBrush brush(Gdiplus::Color((BYTE)a, color.r, color.g, color.b));
								graphics->DrawString((const WCHAR*)(text.getData()), (INT)(text.getLength())
									, pf
									, Gdiplus::PointF(x, y + 1)
									, Gdiplus::StringFormat::GenericTypographic()
									, &brush);
							}
						}
					}
				}

				void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& _pen) override
				{
					DRAW_PEN_BEGIN
					if (hPen) {
						m_graphics->DrawLine(hPen, Gdiplus::PointF(pt1.x, pt1.y), Gdiplus::PointF(pt2.x, pt2.y));
					}
					DRAW_PEN_END
				}

				void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen) override
				{
					if (countPoints < 2) {
						return;
					}
					DRAW_PEN_BEGIN
					if (hPen) {
						Gdiplus::PointF* pts = (Gdiplus::PointF*)points;
						m_graphics->DrawLines(hPen, pts, countPoints);
					}
					DRAW_PEN_END
				}

				void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& _pen) override
				{
					DRAW_PEN_BEGIN
					if (hPen) {
						m_graphics->DrawArc(hPen, rect.left, rect.top, rect.getWidth(), rect.getHeight()
							, startDegrees, sweepDegrees);
					}
					DRAW_PEN_END
				}

				void drawRectangle(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					sl_real width = rect.getWidth();
					sl_real height = rect.getHeight();

					DRAW_PEN_BRUSH_BEGIN
					if (hBrush) {
						graphics->FillRectangle(hBrush, rect.left, rect.top, width, height);
					}
					if (hPen) {
						graphics->DrawRectangle(hPen, rect.left, rect.top, width, height);
					}
					DRAW_PEN_BRUSH_END
				}

				void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					sl_real width = rect.getWidth();
					sl_real height = rect.getHeight();
					Ref<GraphicsPath> path = GraphicsPath::create();
					if (path.isNotNull()) {
						path->addRoundRect(rect.left, rect.top, width, height, radius.x, radius.y);
						drawPath(path, _pen, brush);
					}
				}

				void drawEllipse(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					sl_real width = rect.getWidth();
					sl_real height = rect.getHeight();
					
					DRAW_PEN_BRUSH_BEGIN
					if (hBrush) {
						graphics->FillEllipse(hBrush, rect.left, rect.top, width, height);
					}
					if (hPen) {
						graphics->DrawEllipse(hPen, rect.left, rect.top, width, height);
					}
					DRAW_PEN_BRUSH_END
				}

				void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen, const Ref<Brush>& brush, FillMode fillMode) override
				{
					if (countPoints <= 2) {
						return;
					}

					DRAW_PEN_BRUSH_BEGIN
					if (hBrush) {
						Gdiplus::FillMode mode;
						switch (fillMode) {
						case FillMode::Winding:
							mode = Gdiplus::FillModeWinding;
							break;
						case FillMode::Alternate:
						default:
							mode = Gdiplus::FillModeAlternate;
							break;
						}
						Gdiplus::PointF* pts = (Gdiplus::PointF*)points;
						graphics->FillPolygon(hBrush, pts, countPoints, mode);
					}
					if (hPen) {
						Gdiplus::PointF* pts = (Gdiplus::PointF*)points;
						graphics->DrawPolygon(hPen, pts, countPoints);
					}
					DRAW_PEN_BRUSH_END

				}

				void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					DRAW_PEN_BRUSH_BEGIN
					if (hBrush) {
						graphics->FillPie(hBrush, rect.left, rect.top, rect.getWidth(), rect.getHeight()
							, startDegrees, sweepDegrees);
					}
					if (hPen) {
						graphics->DrawPie(hPen, rect.left, rect.top, rect.getWidth(), rect.getHeight()
							, startDegrees, sweepDegrees);
					}
					DRAW_PEN_BRUSH_END
				}

				void drawPath(const Ref<GraphicsPath>& _path, const Ref<Pen>& _pen, const Ref<Brush>& brush) override
				{
					Ref<GraphicsPath> path = _path;
					if (path.isNotNull()) {
						Gdiplus::GraphicsPath* pPath = GraphicsPlatform::getGraphicsPath(path.get());
						if (pPath) {
							DRAW_PEN_BRUSH_BEGIN
							if (hBrush) {
								graphics->FillPath(hBrush, pPath);
							}
							if (hPen) {
								graphics->DrawPath(hPen, pPath);
							}
							DRAW_PEN_BRUSH_END
						}
					}
				}

				void _setAntiAlias(sl_bool flag) override
				{
					if (flag) {
						m_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
						m_graphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
					} else {
						m_graphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
						m_graphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
					}
				}

			};

			SLIB_DEFINE_OBJECT(CanvasImpl, Canvas)

		}
	}

	using namespace priv::gdi;

	Ref<Canvas> GraphicsPlatform::createCanvas(CanvasType type, Gdiplus::Graphics* graphics, sl_uint32 width, sl_uint32 height, sl_bool flagFreeOnRelease, Referable* ref)
	{
		if (!graphics) {
			return sl_null;
		}
		return Ref<Canvas>::from(CanvasImpl::create(type, graphics, (sl_real)width, (sl_real)height, flagFreeOnRelease, ref));
	}

	Gdiplus::Graphics* GraphicsPlatform::getCanvasHandle(Canvas* _canvas)
	{
		if (CanvasImpl* canvas = CastInstance<CanvasImpl>(_canvas)) {
			return canvas->m_graphics;
		}
		return NULL;
	}

	void GraphicsPlatform::drawImage(Canvas* canvas, const Rectangle& rectDst, Gdiplus::Image* image, const Rectangle& rectSrc, const DrawParam& param)
	{
		Gdiplus::Graphics* graphics = GraphicsPlatform::getCanvasHandle(canvas);
		if (!graphics) {
			return;
		}

		int sw = (int)(rectSrc.getWidth());
		if (sw <= 0) {
			return;
		}
		int sh = (int)(rectSrc.getHeight());
		if (sh <= 0) {
			return;
		}
		int sx = (int)(rectSrc.left);
		int sy = (int)(rectSrc.top);

		Gdiplus::ImageAttributes* pia = NULL;
		sl_real canvasAlpha = canvas->getAlpha();
		if (param.useAlpha || param.useColorMatrix || canvasAlpha < 0.995f) {
			Gdiplus::REAL alpha;
			if (param.useAlpha) {
				alpha = (Gdiplus::REAL)(param.alpha * canvasAlpha);
			} else {
				alpha = (Gdiplus::REAL)(canvasAlpha);
			}
			Gdiplus::ColorMatrix cm;
			if (param.useColorMatrix) {
				const ColorMatrix& c = param.colorMatrix;
				cm.m[0][0] = (Gdiplus::REAL)(c.red.x); cm.m[0][1] = (Gdiplus::REAL)(c.green.x); cm.m[0][2] = (Gdiplus::REAL)(c.blue.x); cm.m[0][3] = (Gdiplus::REAL)(c.alpha.x * alpha); cm.m[0][4] = 0;
				cm.m[1][0] = (Gdiplus::REAL)(c.red.y); cm.m[1][1] = (Gdiplus::REAL)(c.green.y); cm.m[1][2] = (Gdiplus::REAL)(c.blue.y); cm.m[1][3] = (Gdiplus::REAL)(c.alpha.y * alpha); cm.m[1][4] = 0;
				cm.m[2][0] = (Gdiplus::REAL)(c.red.z); cm.m[2][1] = (Gdiplus::REAL)(c.green.z); cm.m[2][2] = (Gdiplus::REAL)(c.blue.z); cm.m[2][3] = (Gdiplus::REAL)(c.alpha.z * alpha); cm.m[2][4] = 0;
				cm.m[3][0] = (Gdiplus::REAL)(c.red.w); cm.m[3][1] = (Gdiplus::REAL)(c.green.w); cm.m[3][2] = (Gdiplus::REAL)(c.blue.w); cm.m[3][3] = (Gdiplus::REAL)(c.alpha.w * alpha); cm.m[3][4] = 0;
				cm.m[4][0] = (Gdiplus::REAL)(c.bias.x); cm.m[4][1] = (Gdiplus::REAL)(c.bias.y); cm.m[4][2] = (Gdiplus::REAL)(c.bias.z); cm.m[4][3] = (Gdiplus::REAL)(c.bias.w * alpha); cm.m[4][4] = 1;
			} else {
				cm.m[0][0] = 1; cm.m[0][1] = 0; cm.m[0][2] = 0; cm.m[0][3] = 0; cm.m[0][4] = 0;
				cm.m[1][0] = 0; cm.m[1][1] = 1; cm.m[1][2] = 0; cm.m[1][3] = 0; cm.m[1][4] = 0;
				cm.m[2][0] = 0; cm.m[2][1] = 0; cm.m[2][2] = 1; cm.m[2][3] = 0; cm.m[2][4] = 0;
				cm.m[3][0] = 0; cm.m[3][1] = 0; cm.m[3][2] = 0; cm.m[3][3] = alpha; cm.m[3][4] = 0;
				cm.m[4][0] = 0; cm.m[4][1] = 0; cm.m[4][2] = 0; cm.m[4][3] = 0; cm.m[4][4] = 1;
			}
			pia = new Gdiplus::ImageAttributes();
			if (pia) {
				pia->SetColorMatrix(&cm);
			}
		}

		if (param.tiled) {
			if (!pia) {
				pia = new Gdiplus::ImageAttributes();
			}
			if (pia) {
				pia->SetWrapMode(Gdiplus::WrapModeTile);
			}
		}

		do {
			if (param.isBlur() && Windows::getAPI_GdipCreateEffect()) {
				Gdiplus::Blur blur;
				Gdiplus::BlurParams bp;
				bp.expandEdge = FALSE;
				bp.radius = param.blurRadius;
				if (blur.SetParameters(&bp) == Gdiplus::Ok) {
					Gdiplus::RectF rcSrc((Gdiplus::REAL)(sx), (Gdiplus::REAL)(sy), (Gdiplus::REAL)(sw), (Gdiplus::REAL)(sh));
					Gdiplus::Matrix xForm((Gdiplus::REAL)(rectDst.getWidth() / sw), 0, 0, (Gdiplus::REAL)(rectDst.getHeight() / sh), (Gdiplus::REAL)(rectDst.left), (Gdiplus::REAL)(rectDst.top));
					graphics->DrawImage(image, &rcSrc, &xForm, &blur, pia, Gdiplus::UnitPixel);
					break;
				}
			}
			Gdiplus::RectF rcDst((Gdiplus::REAL)(rectDst.left), (Gdiplus::REAL)(rectDst.top), (Gdiplus::REAL)(rectDst.getWidth()), (Gdiplus::REAL)(rectDst.getHeight()));
			if (pia) {
				graphics->DrawImage(image, rcDst,
					(Gdiplus::REAL)(sx), (Gdiplus::REAL)(sy), (Gdiplus::REAL)(sw), (Gdiplus::REAL)(sh),
					Gdiplus::UnitPixel, pia);
			} else {
				graphics->DrawImage(image, rcDst,
					(Gdiplus::REAL)(sx), (Gdiplus::REAL)(sy), (Gdiplus::REAL)(sw), (Gdiplus::REAL)(sh),
					Gdiplus::UnitPixel);
			}
		} while (0);

		if (pia) {
			delete pia;
		}
	}

}

#endif
