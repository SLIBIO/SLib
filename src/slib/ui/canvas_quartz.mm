#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/canvas.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#include <CoreText/CoreText.h>

SLIB_UI_NAMESPACE_BEGIN
class _Quartz_Canvas : public Canvas
{
	SLIB_DECLARE_OBJECT(_Quartz_Canvas, Canvas)
public:
	CGContextRef m_graphics;
	sl_real m_width;
	sl_real m_height;
	Rectangle m_rectDirty;
	
	_Quartz_Canvas()
	{
	}
	
	~_Quartz_Canvas()
	{
		CGContextRelease(m_graphics);
	}
	
	static Ref<_Quartz_Canvas> _create(CGContextRef graphics, sl_real width, sl_real height, const Rectangle* rectDirty)
	{
		Ref<_Quartz_Canvas> ret;
		if (graphics) {
			CGContextRetain(graphics);
			ret = new _Quartz_Canvas();
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
				return ret;
			}
			CGContextRelease(graphics);
		}
		return ret;
	}
	
	Size getSize()
	{
		return Size(m_width, m_height);
	}
	
	Rectangle getInvalidatedRect()
	{
		return m_rectDirty;
	}
	
	void save()
	{
		CGContextSaveGState(m_graphics);
	}
	void restore()
	{
		CGContextRestoreGState(m_graphics);
	}
	
	void setAntiAlias(sl_bool flag)
	{
		if (flag) {
			CGContextSetAllowsAntialiasing(m_graphics, sl_true);
			CGContextSetShouldAntialias(m_graphics, sl_true);
		} else {
			CGContextSetAllowsAntialiasing(m_graphics, sl_false);
			CGContextSetShouldAntialias(m_graphics, sl_false);
		}
	}
	
	void clipToRectangle(const Rectangle& rect)
	{
		CGRect rc;
		rc.origin.x = rect.left;
		rc.origin.y = rect.top;
		rc.size.width = rect.getWidth();
		rc.size.height = rect.getHeight();
		CGContextClipToRect(m_graphics, rc);
	}
	
	void clipToPath(const Ref<GraphicsPath>& path)
	{
		if (path.isNotNull()) {
			Ref<GraphicsPathInstance> instance;
			CGPathRef handle = UIPlatform::getGraphicsPath(path.get(), instance);
			if (handle) {
				_clipToPath(handle, path->getFillMode());				
			}
		}
	}
	
	void _clipToPath(CGPathRef path, FillMode fillMode)
	{
		CGContextBeginPath(m_graphics);
		CGContextAddPath(m_graphics, path);
		if (fillMode != fillMode_Winding) {
			CGContextEOClip(m_graphics);
		} else {
			CGContextClip(m_graphics);
		}
	}
	
	Matrix3 getMatrix()
	{
		Matrix3 ret;
		CGAffineTransform t = CGContextGetCTM(m_graphics);
		ret.m00 = (sl_real)(t.a);
		ret.m01 = (sl_real)(t.b);
		ret.m02 = 0;
		ret.m10 = (sl_real)(t.c);
		ret.m11 = (sl_real)(t.d);
		ret.m12 = 0;
		ret.m20 = (sl_real)(t.tx);
		ret.m21 = (sl_real)(t.ty);
		ret.m22 = 1;
		return ret;
	}
	
	void concatMatrix(const Matrix3& other)
	{
		CGAffineTransform t;
		t.a = other.m00;
		t.b = other.m01;
		t.c = other.m10;
		t.d = other.m11;
		t.tx = other.m20;
		t.ty = other.m21;
		CGContextConcatCTM(m_graphics, t);
	}
	
	void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& __font, const Color& _color)
	{		
		if (text.isNotEmpty()) {
			
			Ref<Font> _font = __font;
			if (_font.isNull()) {
				_font = Font::getDefault();
			}
			if (_font.isNotNull()) {
				
				Ref<FontInstance> fontInstance;
				CTFontRef font = UIPlatform::getCoreTextFont(_font.get(), fontInstance);
				
				if (font) {
					
					NSString* ns_text = Apple::getNSStringFromString(text);
					CFStringRef string = (__bridge CFStringRef)ns_text;
					
					SInt32 _underline = _font->isUnderline() ? kCTUnderlineStyleSingle: kCTUnderlineStyleNone;
					CFNumberRef underline = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &_underline);
					
					CGFloat colorComponents[4] = {_color.getRedF(), _color.getGreenF(), _color.getBlueF(), _color.getAlphaF()};
					CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
					CGColorRef color = CGColorCreate(colorSpace, colorComponents);
					CFRelease(colorSpace);
					
					CFStringRef keys[] = { kCTFontAttributeName, kCTUnderlineStyleAttributeName, kCTForegroundColorAttributeName };
					CFTypeRef values[] = { font, underline, color };
					CFDictionaryRef attributes = CFDictionaryCreate(
																	kCFAllocatorDefault
																	, (const void**)&keys, (const void**)&values
																	, sizeof(keys) / sizeof(keys[0])
																	, &kCFCopyStringDictionaryKeyCallBacks
																	, &kCFTypeDictionaryValueCallBacks);
					if (attributes) {
						CFAttributedStringRef attrString = CFAttributedStringCreate(kCFAllocatorDefault, string, attributes);
						if (attrString) {
							CTLineRef line = CTLineCreateWithAttributedString(attrString);
							if (line) {
								CGRect rect = CTLineGetBoundsWithOptions(line, kCTLineBoundsUseGlyphPathBounds);
								CGAffineTransform trans;
								trans.a = 1;
								trans.b = 0;
								trans.c = 0;
								trans.d = -1;
								trans.tx = x;
								trans.ty = y + rect.size.height;
								CGContextSetTextMatrix(m_graphics, trans);
								CTLineDraw(line, m_graphics);
								CFRelease(line);
							}
							CFRelease(attrString);
						}
						CFRelease(attributes);
					}
					
					CFRelease(underline);
					CFRelease(color);
				}
				
			}
		}
	}
	
	void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& _pen)
	{
		Ref<Pen> pen = _pen;
		if (pen.isNull()) {
			pen = Pen::getDefault();
		}
		if (pen.isNotNull()) {
			CGContextBeginPath(m_graphics);
			CGContextMoveToPoint(m_graphics, pt1.x, pt1.y);
			CGContextAddLineToPoint(m_graphics, pt2.x, pt2.y);
			_applyPen(pen.get());
			CGContextStrokePath(m_graphics);
		}
	}
	
	void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen)
	{
		if (countPoints < 2) {
			return;
		}
		Ref<Pen> pen = _pen;
		if (pen.isNull()) {
			pen = Pen::getDefault();
		}
		if (pen.isNotNull()) {
			CGContextBeginPath(m_graphics);
			CGContextMoveToPoint(m_graphics, points[0].x, points[0].y);
			for (sl_uint32 i = 1; i < countPoints; i++) {
				CGContextAddLineToPoint(m_graphics, points[i].x, points[i].y);
			}
			_applyPen(pen.get());
			CGContextStrokePath(m_graphics);
		}
	}
	
	void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& _pen)
	{
		Ref<GraphicsPath> path = GraphicsPath::create();
		if (path.isNotNull()) {
			path->addArc(rect, startDegrees, sweepDegrees);
			drawPath(path, _pen, Ref<Brush>::null());
		}
	}
	
	void drawRectangle(const Rectangle& _rect, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		CGRect rect;
		rect.origin.x = _rect.left;
		rect.origin.y = _rect.top;
		rect.size.width = _rect.getWidth();
		rect.size.height = _rect.getHeight();
		if (brush.isNotNull()) {
			_applyBrush(brush.get());
			CGContextFillRect(m_graphics, rect);
		}
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		if (pen.isNotNull()) {
			_applyPen(pen.get());
			rect.origin.y++;
			CGContextStrokeRect(m_graphics, rect);
		}
	}
	
	void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		Ref<GraphicsPath> path = GraphicsPath::create();
		if (path.isNotNull()) {
			path->addRoundRect(rect, radius);
			drawPath(path, _pen, brush);
		}
	}
	
	void drawEllipse(const Rectangle& _rect, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		CGRect rect;
		rect.origin.x = _rect.left;
		rect.origin.y = _rect.top;
		rect.size.width = _rect.getWidth();
		rect.size.height = _rect.getHeight();
		if (brush.isNotNull()) {
			_applyBrush(brush.get());
			CGContextFillEllipseInRect(m_graphics, rect);
		}
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		if (pen.isNotNull()) {
			_applyPen(pen.get());
			CGContextStrokeEllipseInRect(m_graphics, rect);
		}
	}
	
	void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen, const Ref<Brush>& brush, FillMode fillMode)
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
			drawPath(path, _pen, brush);
		}
	}
	
	void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		Ref<GraphicsPath> path = GraphicsPath::create();
		if (path.isNotNull()) {
			path->addPie(rect, startDegrees, sweepDegrees);
			drawPath(path, _pen, brush);
		}
	}
	
	void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& _pen, const Ref<Brush>& brush)
	{
		if (path.isNotNull()) {
			Ref<GraphicsPathInstance> instance;
			CGPathRef handle = UIPlatform::getGraphicsPath(path.get(), instance);
			if (handle) {
				_drawPath(handle, _pen, brush, path->getFillMode());
			}
		}
	}
	
	void _drawPath(CGPathRef path, const Ref<Pen>& _pen, const Ref<Brush>& brush, FillMode fillMode)
	{
		if (brush.isNotNull()) {
			_applyBrush(brush.get());
			CGContextBeginPath(m_graphics);
			CGContextAddPath(m_graphics, path);
			switch (fillMode) {
				case fillMode_Winding:
					CGContextFillPath(m_graphics);
					break;
				case fillMode_Alternate:
				default:
					CGContextEOFillPath(m_graphics);
					break;
			}
		}
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		if (pen.isNotNull()) {
			_applyPen(pen.get());
			CGContextBeginPath(m_graphics);
			CGContextAddPath(m_graphics, path);
			CGContextStrokePath(m_graphics);
		}
	}
	
	void _applyPen(Pen* pen)
	{
		CGContextRef graphics = m_graphics;
		
		CGFloat _width;
		CGLineCap _cap;
		CGLineJoin _join;
		CGFloat _miterLimit;
		CGFloat* _dash;
		sl_uint32 _dashLen;
		
		_width = pen->getWidth();
		
		switch (pen->getCap()) {
			case lineCap_Square:
				_cap = kCGLineCapSquare;
				break;
			case lineCap_Round:
				_cap = kCGLineCapRound;
				break;
			case lineCap_Flat:
			default:
				_cap = kCGLineCapButt;
				break;
		}
		
		switch (pen->getJoin()) {
				break;
			case lineJoin_Bevel:
				_join = kCGLineJoinBevel;
				break;
			case lineJoin_Round:
				_join = kCGLineJoinRound;
				break;
			case lineJoin_Miter:
			default:
				_join = kCGLineJoinMiter;
				break;
		}
		
		_miterLimit = pen->getMiterLimit();
		
#define W _width
		CGFloat dashStyleDot[] = {W, 2*W};
		CGFloat dashStyleDash[] = {3*W, 3*W};
		CGFloat dashStyleDashDot[] = {3*W, 2*W, W, 2*W};
		CGFloat dashStyleDashDotDot[] = {3*W, 2*W, W, 2*W, W, 2*W};
		switch (pen->getStyle()) {
			case penStyle_Dot:
				_dash = dashStyleDot;
				_dashLen = sizeof(dashStyleDot) / sizeof(CGFloat);
				break;
			case penStyle_Dash:
				_dash = dashStyleDash;
				_dashLen = sizeof(dashStyleDash) / sizeof(CGFloat);
				break;
			case penStyle_DashDot:
				_dash = dashStyleDashDot;
				_dashLen = sizeof(dashStyleDashDot) / sizeof(CGFloat);
				break;
			case penStyle_DashDotDot:
				_dash = dashStyleDashDotDot;
				_dashLen = sizeof(dashStyleDashDotDot) / sizeof(CGFloat);
				break;
			case penStyle_Solid:
			default:
				_dash = 0;
				_dashLen = 0;
				break;
		}
		
		CGContextSetLineWidth(graphics, _width);
		CGContextSetLineCap(graphics, _cap);
		CGContextSetLineJoin(graphics, _join);
		CGContextSetMiterLimit(graphics, _miterLimit);
		CGContextSetLineDash(graphics, 0, _dash, _dashLen);
		
		Color _color = pen->getColor();
		CGContextSetRGBStrokeColor(graphics, _color.getRedF(), _color.getGreenF(), _color.getBlueF(), _color.getAlphaF());
	}

	void _applyBrush(Brush* brush)
	{
		CGContextRef graphics = m_graphics;
		Color _color = brush->getColor();
		CGContextSetRGBFillColor(graphics, _color.getRedF(), _color.getGreenF(), _color.getBlueF(), _color.getAlphaF());
	}
};

Ref<Canvas> UIPlatform::createCanvas(CGContextRef graphics, sl_uint32 width, sl_uint32 height, const Rectangle* rectDirty)
{
	if (!graphics) {
		return Ref<Canvas>::null();
	}
	return _Quartz_Canvas::_create(graphics, (sl_real)width, (sl_real)height, rectDirty);
}

CGContextRef UIPlatform::getCanvasHandle(Canvas* _canvas)
{
	if (_Quartz_Canvas::checkInstance(_canvas)) {
		_Quartz_Canvas* canvas = (_Quartz_Canvas*)_canvas;
		return canvas->m_graphics;
	}
	return NULL;
}
SLIB_UI_NAMESPACE_END

#endif
