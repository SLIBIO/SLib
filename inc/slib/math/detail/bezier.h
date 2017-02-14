#ifndef CHECKHEADER_SLIB_MATH_DETAIL_BEZIER
#define CHECKHEADER_SLIB_MATH_DETAIL_BEZIER

#include "../bezier.h"

namespace slib
{

	template <class T>
	template <class O>
	CubicBezierCurveT<T>::CubicBezierCurveT(const CubicBezierCurveT<O>& other)
	: x0((T)(other.x0)), y0((T)(other.y0)),
	  x1((T)(other.x1)), y1((T)(other.y1)),
	  x2((T)(other.x2)), y2((T)(other.y2)),
	  x3((T)(other.x3)), y3((T)(other.y3))
	{
	}

	template <class T>
	template <class O>
	CubicBezierCurveT<T>& CubicBezierCurveT<T>::operator=(const CubicBezierCurveT<O>& other)
	{
		x0 = (T)(other.x0);
		y0 = (T)(other.y0);
		x1 = (T)(other.x1);
		y1 = (T)(other.y1);
		x2 = (T)(other.x2);
		y2 = (T)(other.y2);
		x3 = (T)(other.x3);
		y3 = (T)(other.y3);
		return *this;
	}

}

#endif
