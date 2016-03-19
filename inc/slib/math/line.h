#ifndef CHECKHEADER_SLIB_MATH_LINE
#define CHECKHEADER_SLIB_MATH_LINE

#include "definition.h"

#include "point.h"
#include "matrix3.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	ax + by + c = 0
*/
template <class T>
class SLIB_EXPORT LineT
{
public:
	T a;
	T b;
	T c;
	
public:
	LineT() = default;
	
	LineT(const LineT<T>& other) = default;
	
	template <class O>
	LineT(const LineT<O>& other);
	
	LineT(T a, T b, T c);
	
	LineT(const PointT<T>& point, const Vector2T<T>& dir);
	
public:
	Vector2T<T> getDirection() const;
	
	Vector2T<T> getNormal() const;
	
	Vector2T<T> projectOriginOnNormalized() const;
	
	Vector2T<T> projectOrigin() const;
	
	T getDistanceFromPointOnNormalized(const PointT<T>& pos) const;
	
	T getDistanceFromPoint(const PointT<T>& pos) const;
	
	Vector2T<T> projectPointOnNormalized(const PointT<T>& pos) const;
	
	Vector2T<T> projectPoint(const PointT<T>& pos) const;
	
	void setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir);
	
	void setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal);
	
	void normalize();
	
	void transform(const Matrix3T<T>& mat);
	
public:
	LineT<T>& operator=(const LineT<T>& other) = default;
	
	template <class O>
	LineT<T>& operator=(const LineT<O>& other);
	
};

SLIB_DECLARE_GEOMETRY_TYPE(Line)

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
SLIB_INLINE LineT<T>::LineT(const LineT<O>& other)
: a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
{
}

template <class T>
template <class O>
SLIB_INLINE LineT<T>& LineT<T>::operator=(const LineT<O>& other)
{
	a = (T)(other.a);
	b = (T)(other.b);
	c = (T)(other.c);
	return *this;
}

SLIB_MATH_NAMESPACE_END

#endif
