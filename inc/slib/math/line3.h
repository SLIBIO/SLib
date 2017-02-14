#ifndef CHECKHEADER_SLIB_MATH_LINE3
#define CHECKHEADER_SLIB_MATH_LINE3

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Line3T
	{
	public:
		Vector3T<T> point1;
		Vector3T<T> point2;

	public:
		Line3T() = default;

		Line3T(const Line3T<T>& other) = default;

		template <class O>
		Line3T(const Line3T<O>& other);

		Line3T(const Vector3T<T>& point1, const Vector3T<T>& point2);

		Line3T(T x1, T y1, T z1, T x2, T y2, T z2);

	public:
		Vector3T<T> getDirection() const;

		T getLength2p() const;

		T getLength() const;

		Vector3T<T> projectPoint(const Vector3T<T>& point) const;

		T getDistanceFromPoint(const Vector3T<T>& point) const;

		T getDistanceFromPointOnInfiniteLine(const Vector3T<T>& point) const;

		void transform(const Matrix4T<T>& mat);

	public:
		Line3T<T>& operator=(const Line3T<T>& other) = default;

		template <class O>
		Line3T<T>& operator=(const Line3T<O>& other);

	};
	
	extern template class Line3T<float>;
	extern template class Line3T<double>;
	typedef Line3T<sl_real> Line3;
	typedef Line3T<float> Line3f;
	typedef Line3T<double> Line3lf;

}

#include "detail/line3.h"

#endif
