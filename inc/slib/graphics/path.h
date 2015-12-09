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
};

class SLIB_EXPORT GraphicsPath : public Object
{
	SLIB_DECLARE_OBJECT(GraphicsPath, Object)
protected:
	GraphicsPath();
	
public:
	static Ref<GraphicsPath> create();

public:
	void moveTo(const Point& pt);
	
	SLIB_INLINE void moveTo(sl_real x, sl_real y)
	{
		moveTo(Point(x, y));
	}
	
	void lineTo(const Point& pt);
	
	SLIB_INLINE void lineTo(sl_real x, sl_real y)
	{
		lineTo(Point(x, y));
	}
	
	void cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd);
	
	SLIB_INLINE void cubicTo(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
	{
		cubicTo(Point(xc1, yc1), Point(xc2, yc2), Point(xe, ye));
	}

	void closeSubpath();
	
	SLIB_INLINE sl_bool containsPoint(const Ref<GraphicsContext>& context, sl_real x, sl_real y)
	{
		return containsPoint(context, Point(x, y));
	}

	void addArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, sl_bool flagMoveTo = sl_true);
	
	SLIB_INLINE void addArc(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees, sl_bool flagMoveTo = sl_true)
	{
		Rectangle rect(x, y, x + width, y + height);
		addArc(rect, startDegrees, sweepDegrees, flagMoveTo);
	}
	
	void addRectangle(const Rectangle& rect);
	
	SLIB_INLINE void addRectangle(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		Rectangle rect(x, y, x + width, y + height);
		addRectangle(rect);
	}
	
	void addRoundRect(const Rectangle& rect, const Size& radius);
	
	SLIB_INLINE void addRoundRect(sl_real x, sl_real y, sl_real width, sl_real height, sl_real rx, sl_real ry)
	{
		Rectangle rect(x, y, x + width, y + height);
		Size radius(rx, ry);
		addRoundRect(rect, radius);
	}

	void addEllipse(const Rectangle& rect);
	
	SLIB_INLINE void addEllipse(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		Rectangle rect(x, y, x + width, y + height);
		addEllipse(rect);
	}
	
	void addPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees);
	
	SLIB_INLINE void addPie(sl_real x, sl_real y, sl_real width, sl_real height, sl_real startDegrees, sl_real sweepDegrees)
	{
		Rectangle rect(x, y, x + width, y + height);
		addPie(rect, startDegrees, sweepDegrees);
	}
	
	Rectangle getBounds(const Ref<GraphicsContext>& context);
	
	sl_bool containsPoint(const Ref<GraphicsContext>& context, const Point& pt);

	void invalidate();

	const Ref<GraphicsPathInstance>& getInstance();
	void setInstance(const Ref<GraphicsPathInstance>& instance);
	
public:
	List<GraphicsPathPoint> points;
	
public:
	SLIB_PROPERTY(FillMode, FillMode)

	
protected:
	sl_bool m_flagBegan;
	Point m_pointBegin;
	
	Ref<GraphicsPathInstance> m_instance;
	sl_bool m_flagInvalidated;
};

class SLIB_EXPORT GraphicsPathInstance : public Object
{
	SLIB_DECLARE_OBJECT(GraphicsPathInstance, Object)
protected:
	GraphicsPathInstance();
	
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
