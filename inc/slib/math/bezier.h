/*******************************
		CubicBezierCurve

P(t)	= B(3,0)*P0 + B(3,1)*P1 + B(3,2)*P2 + B(3,3)*P3
		= (1-t)^3 * P0 + 3*(1-t)^2*t * P1 + 3*(1-t)*t^2 * P2 + t^3 * P3
0 <= t <= 1

B(n,m)	= m th coefficient of nth degree Bernstein polynomial
		= C(n,m) * t^(m) * (1 - t)^(n-m)
C(n,m)	= Combinations of n things, taken m at a time
		= n! / (m! * (n-m)!)
*******************************/

#ifndef CHECKHEADER_SLIB_MATH_BEZIER
#define CHECKHEADER_SLIB_MATH_BEZIER

#include "definition.h"

#include "point.h"
#include "rectangle.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT CubicBezierCurveT
	{
	public:
		T x0, y0;
		T x1, y1;
		T x2, y2;
		T x3, y3;

	public:
		CubicBezierCurveT() = default;

		CubicBezierCurveT(const CubicBezierCurveT<T>& other) = default;

		template <class O>
		CubicBezierCurveT(const CubicBezierCurveT<O>& other);

		CubicBezierCurveT(T x0, T y0, T x1, T y1, T x2, T y2, T x3, T y3);

		CubicBezierCurveT(const PointT<T>& P0, const PointT<T>& P1, const PointT<T>& P2, const PointT<T>& P3);

	public:
		void getPoint(T t, T& x, T& y);
	
		void getPoint(T t, PointT<T>& pt);
	
		PointT<T> getPoint(T t);

		// returns 1 + 3 * NumberOfArcSections points
		static sl_uint32 convertArcToBezier(PointT<T> pts[13], const RectangleT<T>& rc, T startDegrees, T sweepDegrees);
	
		void describeArc(T cx, T cy, T rx, T ry, T startRadian, T endRadian);

	public:
		CubicBezierCurveT<T>& operator=(const CubicBezierCurveT<T>& other) = default;

		template <class O>
		CubicBezierCurveT<T>& operator=(const CubicBezierCurveT<O>& other);

	};
	
	extern template class CubicBezierCurveT<float>;
	extern template class CubicBezierCurveT<double>;
	typedef CubicBezierCurveT<sl_real> CubicBezierCurve;
	typedef CubicBezierCurveT<float> CubicBezierCurvef;
	typedef CubicBezierCurveT<double> CubicBezierCurvelf;

}

#include "detail/bezier.h"

#endif
