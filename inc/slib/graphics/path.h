#ifndef CHECKHEADER_SLIB_GRAPHICS_PATH
#define CHECKHEADER_SLIB_GRAPHICS_PATH

#include "definition.h"

#include "constants.h"

#include "../core/object.h"
#include "../core/list.h"
#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class GraphicsContext;

class GraphicsPathInstance;

struct SLIB_EXPORT GraphicsPathPoint
{
	Point pt;

	// flags from GraphicsPathPointType
	sl_uint8 type;
	
	enum : sl_uint8
	{
		Begin = 0,
		Line = 1,
		BezierCubic = 3,
		FlagClose = 0x80
	};
	
};

class SLIB_EXPORT GraphicsPath : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	GraphicsPath();
	
public:
	static Ref<GraphicsPath> create();

public:
	void moveTo(sl_real x, sl_real y);
	
	void moveTo(const Point& pt);
	
	void lineTo(sl_real x, sl_real y);
	
	void lineTo(const Point& pt);
	
	void cubicTo(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye);

	void cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd);
	
	void closeSubpath();
	
	void addArc(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, sl_bool flagMoveTo = sl_true);
	
	void addArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, sl_bool flagMoveTo = sl_true);
	
	void addRectangle(sl_real x, sl_real y, sl_real width, sl_real height);

	void addRectangle(const Rectangle& rect);
	
	void addRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry);
	
	void addRoundRect(const Rectangle& rect, const Size& radius);
	
	void addEllipse(sl_real x, sl_real y, sl_real width, sl_real height);
	
	void addEllipse(const Rectangle& rect);
	
	void addPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees);
	
	void addPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees);
	
	FillMode getFillMode();
	
	void setFillMode(FillMode mode);
	
	Rectangle getBounds(const Ref<GraphicsContext>& context);
	
	sl_bool containsPoint(const Ref<GraphicsContext>& context, sl_real x, sl_real y);
	
	sl_bool containsPoint(const Ref<GraphicsContext>& context, const Point& pt);
	
	void invalidate();
	
public:
	static sl_bool containsPointInRoundRect(const Point& pt, const Rectangle& rect, const Size& radius);
	
	static sl_bool containsPointInEllipse(const Point& pt, const Rectangle& rect);

public:
	CList<GraphicsPathPoint> points;
	
protected:
	sl_bool m_flagBegan;
	Point m_pointBegin;
	FillMode m_fillMode;
	
	sl_bool m_flagInvalidated;
	
	SLIB_REF_PROPERTY(GraphicsPathInstance, Instance)
};

class SLIB_EXPORT GraphicsPathInstance : public Object
{
	SLIB_DECLARE_OBJECT

public:
	virtual void moveTo(const Point& pt) = 0;
	virtual void lineTo(const Point& pt) = 0;
	virtual void cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd) = 0;
	virtual void closeSubpath() = 0;

public:
	void buildFrom(const Ref<GraphicsPath>& path);
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
