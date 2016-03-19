#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/canvas.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/core/queue.h"

#pragma comment(lib, "gdiplus.lib")

SLIB_UI_NAMESPACE_BEGIN

class _Gdiplus_PenInstance : public PenInstance
{
	SLIB_DECLARE_OBJECT
public:
	Gdiplus::Pen* m_pen;

public:
	_Gdiplus_PenInstance()
	{
	}

	~_Gdiplus_PenInstance()
	{
		delete m_pen;
	}

public:
	static Ref<_Gdiplus_PenInstance> _create(const PenDesc& desc)
	{
		Ref<_Gdiplus_PenInstance> ret;
		const Color& _color = desc.color;
		Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
		Gdiplus::Pen* pen = new Gdiplus::Pen(color, desc.width);
		if (pen) {
			Gdiplus::LineCap cap;
			Gdiplus::DashCap dashCap;
			switch (desc.cap) {
			case LineCap::Square:
				cap = Gdiplus::LineCapSquare;
				dashCap = Gdiplus::DashCapFlat;
				break;
			case LineCap::Round:
				cap = Gdiplus::LineCapRound;
				dashCap = Gdiplus::DashCapRound;
				break;
			case LineCap::Flat:
			default:
				cap = Gdiplus::LineCapFlat;
				dashCap = Gdiplus::DashCapFlat;
				break;
			}
			pen->SetLineCap(cap, cap, dashCap);

			Gdiplus::LineJoin join;
			switch (desc.join) {
			case LineJoin::Bevel:
				join = Gdiplus::LineJoinBevel;
				break;
			case LineJoin::Round:
				join = Gdiplus::LineJoinRound;
				break;
			case LineJoin::Miter:
			default:
				join = Gdiplus::LineJoinMiter;
				pen->SetMiterLimit(desc.miterLimit);
				break;
			}
			pen->SetLineJoin(join);
			
			Gdiplus::DashStyle style;
			switch (desc.style) {
			case PenStyle::Dot:
				style = Gdiplus::DashStyleDot;
				break;
			case PenStyle::Dash:
				style = Gdiplus::DashStyleDash;
				break;
			case PenStyle::DashDot:
				style = Gdiplus::DashStyleDashDot;
				break;
			case PenStyle::DashDotDot:
				style = Gdiplus::DashStyleDashDotDot;
				break;
			case PenStyle::Solid:
			default:
				style = Gdiplus::DashStyleSolid;
				break;
			}
			pen->SetDashStyle(style);

			ret = new _Gdiplus_PenInstance();
			if (ret.isNotNull()) {
				ret->m_pen = pen;
				return ret;
			}
			delete pen;
		}
		return ret;
	}
};

SLIB_DEFINE_OBJECT(_Gdiplus_PenInstance, PenInstance)

class _Gdiplus_BrushInstance : public BrushInstance
{
	SLIB_DECLARE_OBJECT
public:
	Gdiplus::Brush* m_brush;

public:
	_Gdiplus_BrushInstance()
	{
	}

	~_Gdiplus_BrushInstance()
	{
		delete m_brush;
	}

public:
	static Ref<_Gdiplus_BrushInstance> _create(const BrushDesc& desc)
	{
		Ref<_Gdiplus_BrushInstance> ret;
		const Color& _color = desc.color;
		Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
		if (desc.style == BrushStyle::Solid) {
			Gdiplus::Brush* brush = new Gdiplus::SolidBrush(color);
			if (brush) {
				ret = new _Gdiplus_BrushInstance();
				if (ret.isNotNull()) {
					ret->m_brush = brush;
					return ret;
				}
				delete brush;
			}
		}
		return ret;
	}
};

SLIB_DEFINE_OBJECT(_Gdiplus_BrushInstance, BrushInstance)

class _Gdiplus_Canvas : public Canvas
{
	SLIB_DECLARE_OBJECT
public:
	Gdiplus::Graphics* m_graphics;
	sl_real m_width;
	sl_real m_height;
	Rectangle m_rectDirty;
	Stack<Gdiplus::GraphicsState> m_stackState;
	sl_bool m_flagFreeOnRelease;
	Ref<Referable> m_ref;

public:
	_Gdiplus_Canvas()
	{
	}

	~_Gdiplus_Canvas()
	{
		if (m_flagFreeOnRelease) {
			delete m_graphics;			
		}
	}

public:
	static Ref<_Gdiplus_Canvas> create(Gdiplus::Graphics* graphics, sl_real width, sl_real height, const Rectangle* rectDirty, sl_bool flagFreeOnRelease, const Referable* ref)
	{
		Ref<_Gdiplus_Canvas> ret;
		if (graphics) {
			ret = new _Gdiplus_Canvas();
			if (ret.isNotNull()) {
				ret->m_context = UI::getGraphicsContext();
				ret->m_graphics = graphics;
				ret->m_width = width;
				ret->m_height = height;
				if (rectDirty) {
					ret->m_rectDirty = *rectDirty;
				} else {
					ret->m_rectDirty = Rectangle(0, 0, width, height);
				}
				ret->m_flagFreeOnRelease = flagFreeOnRelease;
				ret->m_ref = ref;
				graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
				ret->setMatrix(Matrix3::identity());
				return ret;
			}
			if (flagFreeOnRelease) {
				delete graphics;
			}
		}
		return ret;
	}

    // override
	Size getSize()
	{
		return Size(m_width, m_height);
	}

    // override
	Rectangle getInvalidatedRect()
	{
		return m_rectDirty;
	}

    // override
	void save()
	{
		Gdiplus::GraphicsState state = m_graphics->Save();
		m_stackState.push(state);
	}
    
    // override
	void restore()
	{
		Gdiplus::GraphicsState state;
		if (m_stackState.pop(&state)) {
			m_graphics->Restore(state);
		}
	}

	sl_bool isAntiAlias()
	{
		return m_graphics->GetSmoothingMode() == Gdiplus::SmoothingModeAntiAlias;
	}

    // override
	void setAntiAlias(sl_bool flag)
	{
		if (flag) {
			m_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		} else {
			m_graphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
		}
	}
	
    // override
	void clipToRectangle(const Rectangle& _rect)
	{
		Gdiplus::RectF rect(_rect.left, _rect.top, _rect.getWidth(), _rect.getHeight());
		m_graphics->SetClip(rect, Gdiplus::CombineModeIntersect);
	}

    // override
	void clipToPath(const Ref<GraphicsPath>& path)
	{
		if (path.isNotNull()) {
			Ref<GraphicsPathInstance> instance;
			Gdiplus::GraphicsPath* handle = UIPlatform::getGraphicsPath(path.ptr, instance);
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

    // override
	void concatMatrix(const Matrix3& other)
	{
		Matrix3 mat = other;
		mat.multiply(getMatrix());
		setMatrix(mat);
	}

    // override
	void drawText(const String& _text, sl_real x, sl_real y, const Ref<Font>& _font, const Color& color)
	{
		String16 text = _text;
		if (text.isNotEmpty()) {
			Gdiplus::Graphics* graphics = m_graphics;
			Ref<Font> font = _font;
			if (font.isNull()) {
				font = Font::getDefault();
			}
			if (font.isNotNull()) {
				Ref<FontInstance> fontInstance;
				Gdiplus::Font* pf = UIPlatform::getGdiplusFont(font.ptr, fontInstance);
				if (pf) {
					Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip);
					Gdiplus::SolidBrush brush(Gdiplus::Color(color.a, color.r, color.g, color.b));
					graphics->DrawString((const WCHAR*)(text.getData()), text.getLength()
						, pf
						, Gdiplus::PointF(x, y + 1)
						, &format
						, &brush);
				}
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
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			m_graphics->DrawLine(hPen, Gdiplus::PointF(pt1.x, pt1.y), Gdiplus::PointF(pt2.x, pt2.y));
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
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			Gdiplus::PointF* pts = (Gdiplus::PointF*)points;
			m_graphics->DrawLines(hPen, pts, countPoints);
		}
	}

    // override
	void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& _pen)
	{
		Ref<Pen> pen = _pen;
		if (pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			m_graphics->DrawArc(hPen, rect.left, rect.top, rect.getWidth(), rect.getHeight()
				, startDegrees, sweepDegrees);
		}
	}

    // override
	void drawRectangle(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		Gdiplus::Graphics* graphics = m_graphics;
		Ref<BrushInstance> brushInstance;
		Gdiplus::Brush* hBrush = _getBrush(brush.ptr, brushInstance);
		sl_real width = rect.getWidth();
		sl_real height = rect.getHeight();
		if (hBrush) {
			graphics->FillRectangle(hBrush, rect.left, rect.top, width, height);
		}
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			graphics->DrawRectangle(hPen, rect.left, rect.top, width, height);
		}
	}

    // override
	void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		sl_real width = rect.getWidth();
		sl_real height = rect.getHeight();
		Ref<GraphicsPath> path = GraphicsPath::create();
		if (path.isNotNull()) {
			path->addRoundRect(rect.left, rect.top, width, height, radius.x, radius.y);
			drawPath(path, _pen, brush);
		}
	}

    // override
	void drawEllipse(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		sl_real width = rect.getWidth();
		sl_real height = rect.getHeight();
		Gdiplus::Graphics* graphics = m_graphics;
		Ref<BrushInstance> brushInstance;
		Gdiplus::Brush* hBrush = _getBrush(brush.ptr, brushInstance);
		if (hBrush) {
			graphics->FillEllipse(hBrush, rect.left, rect.top, width, height);
		}
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			graphics->DrawEllipse(hPen, rect.left, rect.top, width, height);
		}
	}

    // override
	void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen, const Ref<Brush>& brush, FillMode fillMode)
	{
		if (countPoints <= 2) {
			return;
		}
		Gdiplus::Graphics* graphics = m_graphics;
		Ref<BrushInstance> brushInstance;
		Gdiplus::Brush* hBrush = _getBrush(brush.ptr, brushInstance);
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
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			Gdiplus::PointF* pts = (Gdiplus::PointF*)points;
			graphics->DrawPolygon(hPen, pts, countPoints);
		}
	}

    // override
	void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		Gdiplus::Graphics* graphics = m_graphics;
		Ref<BrushInstance> brushInstance;
		Gdiplus::Brush* hBrush = _getBrush(brush.ptr, brushInstance);
		if (hBrush) {
			graphics->FillPie(hBrush, rect.left, rect.top, rect.getWidth(), rect.getHeight()
				, startDegrees, sweepDegrees);
		}
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
		if (hPen) {
			graphics->DrawPie(hPen, rect.left, rect.top, rect.getWidth(), rect.getHeight()
				, startDegrees, sweepDegrees);
		}
	}

    // override
	void drawPath(const Ref<GraphicsPath>& _path, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		Gdiplus::Graphics* graphics = m_graphics;
		Ref<GraphicsPath> path = _path;
		if (path.isNotNull()) {
			Ref<GraphicsPathInstance> pathInstance;
			Gdiplus::GraphicsPath* pPath = UIPlatform::getGraphicsPath(path.ptr, pathInstance);
			if (pPath) {
				Ref<BrushInstance> brushInstance;
				Gdiplus::Brush* hBrush = _getBrush(brush.ptr, brushInstance);
				if (hBrush) {
					graphics->FillPath(hBrush, pPath);
				}
				Ref<Pen> pen = _pen;
				if (brush.isNull() && pen.isNull()) {
					pen = Pen::getDefault();
				}
				Ref<PenInstance> penInstance;
				Gdiplus::Pen* hPen = _getPen(pen.ptr, penInstance);
				if (hPen) {
					graphics->DrawPath(hPen, pPath);
				}
			}
		}
	}

	Gdiplus::Pen* _getPen(Pen* pen, Ref<PenInstance>& instanceOut)
	{
		if (pen) {
			Ref<PenInstance> _instance = pen->getInstance();
			Ref<_Gdiplus_PenInstance> instance;
			if (_Gdiplus_PenInstance::checkInstance(_instance.ptr)) {
				instance = Ref<_Gdiplus_PenInstance>::from(_instance);
			}
			if (instance.isNull()) {
				PenDesc desc;
				pen->getDesc(desc);
				instance = _Gdiplus_PenInstance::_create(desc);
				if (instance.isNotNull()) {
					pen->setInstance(instance);
				}
			}
			instanceOut = instance;
			if (instance.isNotNull()) {
				return instance->m_pen;
			}
		}
		return NULL;
	}

	Gdiplus::Brush* _getBrush(Brush* brush, Ref<BrushInstance>& instanceOut)
	{
		if (brush) {
			Ref<BrushInstance> _instance = brush->getInstance();
			Ref<_Gdiplus_BrushInstance> instance;
			if (_Gdiplus_BrushInstance::checkInstance(_instance.ptr)) {
				instance = Ref<_Gdiplus_BrushInstance>::from(_instance);
			}
			if (instance.isNull()) {
				BrushDesc desc;
				brush->getDesc(desc);
				instance = _Gdiplus_BrushInstance::_create(desc);
				if (instance.isNotNull()) {
					brush->setInstance(instance);
				}
			}
			instanceOut = instance;
			if (instance.isNotNull()) {
				return instance->m_brush;
			}
		}
		return NULL;
	}
};

SLIB_DEFINE_OBJECT(_Gdiplus_Canvas, Canvas)

Ref<Canvas> UIPlatform::createCanvas(Gdiplus::Graphics* graphics, sl_uint32 width, sl_uint32 height, const Rectangle* rectDirty, sl_bool flagFreeOnRelease, const Referable* ref)
{
	if (!graphics) {
		return Ref<Canvas>::null();
	}
	return _Gdiplus_Canvas::create(graphics, (sl_real)width, (sl_real)height, rectDirty, flagFreeOnRelease, ref);
}

Gdiplus::Graphics* UIPlatform::getCanvasHandle(Canvas* _canvas)
{
	if (_Gdiplus_Canvas::checkInstance(_canvas)) {
		_Gdiplus_Canvas* canvas = (_Gdiplus_Canvas*)_canvas;
		return canvas->m_graphics;
	}
	return NULL;
}

SLIB_UI_NAMESPACE_END

#endif
