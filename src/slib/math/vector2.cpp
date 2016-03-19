#include "../../../inc/slib/math/vector2.h"

#include "../../../inc/slib/core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT>
T Vector2T<T, FT>::_zero[2] = {0, 0};

template <class T, class FT>
T Vector2T<T, FT>::dot(const Vector2T<T, FT>& other) const
{
	return x * other.x + y * other.y;
}

template <class T, class FT>
T Vector2T<T, FT>::cross(const Vector2T<T, FT>& other) const
{
	return x*other.y - y*other.x;
}

template <class T, class FT>
T Vector2T<T, FT>::getLength2p() const
{
	return x * x + y * y;
}

template <class T, class FT>
FT Vector2T<T, FT>::getLength() const
{
	return Math::sqrt((FT)(x * x + y * y));
}

template <class T, class FT>
T Vector2T<T, FT>::getLength2p(const Vector2T<T, FT>& other) const
{
	T dx = x - other.x;
	T dy = y - other.y;
	return dx * dx + dy * dy;
}

template <class T, class FT>
FT Vector2T<T, FT>::getLength(const Vector2T<T, FT>& other) const
{
	T dx = x - other.x;
	T dy = y - other.y;
	return Math::sqrt((FT)(dx * dx + dy * dy));
}

template <class T, class FT>
void Vector2T<T, FT>::normalize()
{
	T l = x * x + y * y;
	if (l > 0) {
		FT d = Math::sqrt((FT)l);
		x = (T)((FT)x / d);
		y = (T)((FT)y / d);
	}
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::getNormalized()
{
	T l = x * x + y * y;
	if (l > 0) {
		FT d = Math::sqrt((FT)l);
		return {(T)((FT)x / d), (T)((FT)y / d)};
	}
	return *this;
}

template <class T, class FT>
FT Vector2T<T, FT>::getCosBetween(const Vector2T& other) const
{
	FT ret = (FT)(dot(other)) / Math::sqrt((FT)(getLength2p() * other.getLength2p()));
	if (ret > 1) {
		ret = 1;
	}
	return ret;
}

template <class T, class FT>
FT Vector2T<T, FT>::getAbsAngleBetween(const Vector2T& other) const
{
	return Math::arccos(getCosBetween(other));
}

template <class T, class FT>
FT Vector2T<T, FT>::getAngleBetween(const Vector2T& other) const
{
	FT a = getAbsAngleBetween(other);
	if (cross(other) > 0) {
		a = -a;
	}
	return a;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator+(const Vector2T<T, FT>& other) const
{
	return {x + other.x, y + other.y};
}

template <class T, class FT>
Vector2T<T, FT>& Vector2T<T, FT>::operator+=(const Vector2T<T, FT>& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator-(const Vector2T<T, FT>& other) const
{
	return {x - other.x, y - other.y};
}

template <class T, class FT>
Vector2T<T, FT>& Vector2T<T, FT>::operator-=(const Vector2T<T, FT>& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator*(T f) const
{
	return {x * f, y * f};
}

template <class T, class FT>
Vector2T<T, FT>& Vector2T<T, FT>::operator*=(T f)
{
	x *= f;
	y *= f;
	return *this;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator*(const Vector2T<T, FT>& other) const
{
	return {x * other.x, y * other.y};
}

template <class T, class FT>
Vector2T<T, FT>& Vector2T<T, FT>::operator*=(const Vector2T<T, FT>& other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator/(T f) const
{
	return {x / f, y / f};
}

template <class T, class FT>
Vector2T<T, FT>& Vector2T<T, FT>::operator/=(T f)
{
	x /= f;
	y /= f;
	return *this;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator/(const Vector2T<T, FT>& other) const
{
	return {x / other.x, y / other.y};
}

template <class T, class FT>
Vector2T<T, FT>& Vector2T<T, FT>::operator/(const Vector2T<T, FT>& other)
{
	x /= other.x;
	y /= other.y;
	return *this;
}

template <class T, class FT>
Vector2T<T, FT> Vector2T<T, FT>::operator-() const
{
	return {-x, -y};
}

template <class T, class FT>
sl_bool Vector2T<T, FT>::operator==(const Vector2T<T, FT>& other) const
{
	return x == other.x && y == other.y;
}

template <class T, class FT>
sl_bool Vector2T<T, FT>::operator!=(const Vector2T<T, FT>& other) const
{
	return x != other.x || y != other.y;
}


SLIB_DEFINE_GEOMETRY_TYPE_EX(Vector2)

SLIB_MATH_NAMESPACE_END
