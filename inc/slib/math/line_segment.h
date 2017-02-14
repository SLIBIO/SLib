#ifndef CHECKHEADER_SLIB_MATH_LINE_SEGMENT
#define CHECKHEADER_SLIB_MATH_LINE_SEGMENT

#include "definition.h"

#include "point.h"
#include "matrix3.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT LineSegmentT
	{
	public:
		PointT<T> point1;
		PointT<T> point2;

	public:
		LineSegmentT() = default;

		LineSegmentT(const LineSegmentT<T>& other) = default;

		template <class O>
		LineSegmentT(const LineSegmentT<O>& other);

		LineSegmentT(const PointT<T>& point1, const PointT<T>& point2);

		LineSegmentT(T x1, T y1, T x2, T y2);

	public:
		Vector2T<T> getDirection() const;

		T getLength2p() const;

		T getLength() const;

		void transform(const Matrix3T<T>& mat);

		PointT<T> projectPoint(const PointT<T>& point) const;

		T getDistanceFromPoint(const PointT<T>& point) const;

		T getDistanceFromPointOnInfiniteLine(const PointT<T>& point) const;

	public:
		LineSegmentT<T>& operator=(const LineSegmentT<T>& other) = default;

		template <class O>
		LineSegmentT<T>& operator=(const LineSegmentT<O>& other);

	};
	
	extern template class LineSegmentT<float>;
	extern template class LineSegmentT<double>;
	typedef LineSegmentT<sl_real> LineSegment;
	typedef LineSegmentT<float> LineSegmentf;
	typedef LineSegmentT<double> LineSegmentlf;

}

#include "detail/line_segment.h"

#endif
