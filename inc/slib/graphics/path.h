#ifndef CHECKHEADER_SLIB_GRAPHICS_PATH
#define CHECKHEADER_SLIB_GRAPHICS_PATH

#include "definition.h"

#include "constants.h"

#include "../core/object.h"
#include "../core/list.h"
#include "../math/rectangle.h"

namespace slib
{
	
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

		~GraphicsPath();

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


		Rectangle getBounds();

		sl_bool containsPoint(sl_real x, sl_real y);

		sl_bool containsPoint(const Point& pt);

	protected:
		void _initialize_PO();

		void _moveTo_PO(sl_real x, sl_real y);

		void _lineTo_PO(sl_real x, sl_real y);

		void _cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye);

		void _closeSubpath_PO();

		void _setFillMode_PO(FillMode mode);
	
		Rectangle _getBounds_PO();
	
		sl_bool _containsPoint_PO(sl_real x, sl_real y);
	
	protected:
		CList<GraphicsPathPoint> points;
		sl_bool m_flagBegan;
		Point m_pointBegin;
		FillMode m_fillMode;

		Ref<Referable> m_platformObject;

	};

}

#endif
