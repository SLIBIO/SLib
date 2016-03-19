#include "../../../inc/slib/math/line.h"

#include "../../../inc/slib/core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
LineT<T>::LineT(T _a, T _b, T _c)
: a(_a), b(_b), c(_c)
{
}

template <class T>
LineT<T>::LineT(const PointT<T>& point, const Vector2T<T>& dir)
{
	setFromPointAndDirection(point, dir);
}

template <class T>
Vector2T<T> LineT<T>::getDirection() const
{
	return {b, -a};
}

template <class T>
Vector2T<T> LineT<T>::getNormal() const
{
	return {a, b};
}

template <class T>
Vector2T<T> LineT<T>::projectOriginOnNormalized() const
{
	return {-a*c, -b*c};
}

template <class T>
Vector2T<T> LineT<T>::projectOrigin() const
{
	T L = a * a + b * b;
	if (L > 0) {
		return {-a*c / L, -b*c / L};
	} else {
		return {0, 0};
	}
}

template <class T>
T LineT<T>::getDistanceFromPointOnNormalized(const PointT<T>& pos) const
{
	return a * pos.x + b * pos.y + c;
}

template <class T>
T LineT<T>::getDistanceFromPoint(const PointT<T>& pos) const
{
	T L = a * a + b * b;
	if (L > 0) {
		L = Math::sqrt(L);
		return (a * pos.x + b * pos.y + c) / L;
	} else {
		return c;
	}
}

template <class T>
Vector2T<T> LineT<T>::projectPointOnNormalized(const Vector2T<T>& pos) const
{
	T D = a * pos.x + b * pos.y + c;
	return {pos.x - D * a, pos.y - D * b};
}

template <class T>
Vector2T<T> LineT<T>::projectPoint(const Vector2T<T>& pos) const
{
	T L = a * a + b * b;
	if (L > 0) {
		T D = a * pos.x + b * pos.y + c;
		return {pos.x - D * a / L, pos.y - D * b / L};
	} else {
		return pos;
	}
}

template <class T>
void LineT<T>::setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir)
{
	a = dir.y;
	b = -dir.x;
	c = -(point.x * a + point.y * b);
}

template <class T>
void LineT<T>::setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal)
{
	a = normal.x;
	b = normal.y;
	c = -point.dot(normal);
}

template <class T>
void LineT<T>::normalize()
{
	T l = Math::sqrt(a * a + b * b);
	a /= l;
	b /= l;
	c /= l;
}

template <class T>
void LineT<T>::transform(const Matrix3T<T>& mat)
{
	T _a = a * mat.m00 + b * mat.m10;
	T _b = a * mat.m01 + b * mat.m11;
	T L = a * a + b * b;
	if (L > 0) {
		T k = c / L;
		c = (k * _a + mat.m20) * _a + (k * _b + mat.m21) * _b;
		a = _a;
		b = _b;
	} else {
		c = 0;
	}
}

SLIB_DEFINE_GEOMETRY_TYPE(Line)

SLIB_MATH_NAMESPACE_END
