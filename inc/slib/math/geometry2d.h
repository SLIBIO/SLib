#ifndef CHECKHEADER_SLIB_MATH_GEOMETRY2D
#define CHECKHEADER_SLIB_MATH_GEOMETRY2D

#include "definition.h"
#include "vector.h"
#include "matrix.h"

#include "../core/list.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	ax + by + c = 0
*/
template <class T>
class SLIB_EXPORT Line2T
{
public:
	T a;
	T b;
	T c;

public:
	SLIB_INLINE Line2T() {}

	template <class O>
	SLIB_INLINE Line2T(const Line2T<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
	}

	SLIB_INLINE Line2T(T _a, T _b, T _c)
	{
		a = _a;
		b = _b;
		c = _c;
	}

	SLIB_INLINE Line2T(const Vector2T<T>& point, const Vector2T<T>& dir)
	{
		setFromPointAndDirection(point, dir);
	}

	template <class O>
	SLIB_INLINE Line2T<T>& operator=(const Line2T<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		return *this;
	}

	SLIB_INLINE Vector2T<T> getDirection() const
	{
		return Vector2T<T>(b, -a);
	}

	SLIB_INLINE Vector2T<T> getNormal() const
	{
		return Vector2T<T>(a, b);
	}

	SLIB_INLINE Vector2T<T> getPointProjectedFromOriginOnNormalized() const
	{
		return Vector2T<T>(-a*c, -b*c);
	}

	Vector2T<T> getPointProjectedFromOrigin() const
	{
		T L = a * a + b * b;
		if (L > 0) {
			return Vector2T<T>(-a*c / L, -b*c / L);
		} else {
			return Vector2T<T>(0, 0);
		}
	}

	SLIB_INLINE T getDistanceFromPointOnNormalized(const Vector2T<T>& pos) const
	{
		return a * pos.x + b * pos.y + c;
	}

	T getDistanceFromPoint(const Vector2T<T>& pos) const
	{
		T L = a * a + b * b;
		if (L > 0) {
			L = Math::sqrt(L);
			return (a * pos.x + b * pos.y + c) / L;
		} else {
			return c;
		}
	}

	void setFromPointAndDirection(const Vector2T<T>& point, const Vector2T<T>& dir)
	{
		a = dir.y;
		b = -dir.x;
		c = -(point.x * a + point.y * b);
	}

	void setFromPointAndNormal(const Vector2T<T>& point, const Vector2T<T>& normal)
	{
		a = normal.x;
		b = normal.y;
		c = -point.dot(normal);
	}

	void normalize()
	{
		T l = Math::sqrt(a * a + b * b);
		a /= l;
		b /= l;
		c /= l;
	}

	void transform(const Matrix3T<T>& mat)
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

};

typedef Line2T<sl_real> Line2;
typedef Line2T<float> Line2f;
typedef Line2T<double> Line2lf;

template <class T>
class SLIB_EXPORT LineSegment2T
{
public:
	Vector2T<T> point1;
	Vector2T<T> point2;

public:
	SLIB_INLINE LineSegment2T() {}

	template <class O>
	SLIB_INLINE LineSegment2T(const LineSegment2T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
	}

	SLIB_INLINE LineSegment2T(const Vector2T<T>& _point1, const Vector2T<T>& _point2)
	{
		point1 = _point1;
		point2 = _point2;
	}

	SLIB_INLINE LineSegment2T(T x1, T y1, T x2, T y2)
	{
		point1.x = x1;
		point1.y = y1;
		point2.x = x2;
		point2.y = y2;
	}

	template <class O>
	SLIB_INLINE LineSegment2T<T>& operator=(const LineSegment2T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}

	SLIB_INLINE Vector2T<T> getDirection() const
	{
		return (point2 - point1);
	}
	SLIB_INLINE Vector2T<T> direction() const
	{
		return getDirection();
	}

	SLIB_INLINE T getLength2p() const
	{
		return point1.getLength2p(point2);
	}
	SLIB_INLINE T length2p() const
	{
		return getLength2p();
	}
	SLIB_INLINE T getLength() const
	{
		return point1.getLength(point2);
	}
	SLIB_INLINE T length() const
	{
		return getLength();
	}

	void transform(const Matrix3T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
	}

	Vector2T<T> projectPoint(const Vector2T<T>& point) const
	{
		Vector2T<T> dir = point2 - point1;
		Vector2T<T> ret = point1 + (point - point1).dot(dir) * dir;
		return ret;
	}

	T getDistanceFromPoint(const Vector2T<T>& point) const
	{
		Vector2T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector2T<T> proj = point1 + f * dir;
		if (f < 0) {
			return point1.getLength(point);
		} else {
			if (f > length()) {
				return point2.getLength(point);
			} else {
				return proj.getLength(point);
			}
		}
	}

	T getDistanceFromPointOnInfiniteLine(const Vector2T<T>& point) const
	{
		Vector2T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector2T<T> proj = point1 + f * dir;
		return proj.getLength(point);
	}
};

typedef LineSegment2T<sl_real> LineSegment2;
typedef LineSegment2T<float> LineSegment2f;
typedef LineSegment2T<double> LineSegment2lf;

template <class T>
class SLIB_EXPORT Triangle2T
{
public:
	Vector2T<T> point1;
	Vector2T<T> point2;
	Vector2T<T> point3;

	SLIB_INLINE Triangle2T() {}

	template <class O>
	SLIB_INLINE Triangle2T(const Triangle2T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
	}

	SLIB_INLINE Triangle2T(const Vector2T<T>& _point1, const Vector2T<T>& _point2, const Vector2T<T>& _point3)
	{
		point1 = _point1;
		point2 = _point2;
		point3 = _point3;
	}

	template <class O>
	SLIB_INLINE Triangle2T<T>& operator=(const Triangle2T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}

	void transform(Matrix3T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
		point3 = mat.transformPosition(point3);
	}

	static T getCross(const Vector2T<T>& _point1, const Vector2T<T>& _point2, const Vector2T<T>& _point3)
	{
		return (_point1.x - _point2.x) * (_point2.y - _point3.y) - (_point2.x - _point3.x) * (_point1.y - _point2.y);
	}

	SLIB_INLINE T getSize() const
	{
		return getCross(point1, point2, point3) / 2;
	}

	SLIB_INLINE T size() const
	{
		return getSize();
	}
};

typedef Triangle2T<sl_real> Triangle2;
typedef Triangle2T<float> Triangle2f;
typedef Triangle2T<double> Triangle2lf;

typedef Vector2T<sl_real> Point;
typedef Vector2T<sl_real> Size;
typedef Vector2T<sl_int32> Pointi;
typedef Vector2T<sl_int32> Sizei;
typedef Vector2T<sl_int64> Pointli;
typedef Vector2T<sl_int64> Sizeli;
typedef Vector2T<float> Pointf;
typedef Vector2T<float> Sizef;
typedef Vector2T<double> Pointlf;
typedef Vector2T<double> Sizelf;

template <class T>
class SLIB_EXPORT RectangleT
{
public:
	T left;
	T top;
	T right;
	T bottom;

	RectangleT()
	{
	}

	template <class O>
	SLIB_INLINE RectangleT(const RectangleT<O>& other)
	{
		left = (T)(other.left);
		top = (T)(other.top);
		right = (T)(other.right);
		bottom = (T)(other.bottom);
	}

	SLIB_INLINE RectangleT(T left, T top, T right, T bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	SLIB_INLINE RectangleT(const Vector2T<T>& pt, const Vector2T<T>& size)
	{
		left = pt.x;
		top = pt.y;
		right = pt.x + size.x;
		bottom = pt.y + size.y;
	}

	template <class O>
	SLIB_INLINE RectangleT<T>& operator=(const RectangleT<O>& other)
	{
		left = (T)(other.left);
		top = (T)(other.top);
		right = (T)(other.right);
		bottom = (T)(other.bottom);
		return *this;
	}

private:
	static T _zero[4];
public:
	static const RectangleT<T>& zero()
	{
		return *((RectangleT<T>*)((void*)_zero));
	}

	SLIB_INLINE T getWidth() const
	{
		return right - left;
	}

	SLIB_INLINE T getHeight() const
	{
		return bottom - top;
	}

	SLIB_INLINE void setWidth(T width)
	{
		right = left + width;
	}

	SLIB_INLINE void setHeight(T height)
	{
		bottom = top + height;
	}

	SLIB_INLINE Vector2T<T> getSize() const
	{
		return Vector2T<T>(right - left, bottom - top);
	}
	SLIB_INLINE void setSize(T width, T height)
	{
		right = left + width;
		bottom = top + height;
	}
	SLIB_INLINE void setSize(const Vector2T<T>& size)
	{
		setSize(size.x, size.y);
	}

	SLIB_INLINE Vector2T<T> getLocation() const
	{
		return Vector2T<T>(left, top);
	}
	SLIB_INLINE void setLocation(T _x, T _y)
	{
		T w = right - left;
		T h = bottom - top;
		left = _x;
		top = _y;
		right = left + w;
		bottom = top + h;
	}
	SLIB_INLINE void setLocation(const Vector2T<T>& location)
	{
		setLocation(location.x, location.y);
	}

	SLIB_INLINE void translate(T tx, T ty)
	{
		left += tx;
		top += ty;
		right += tx;
		bottom += ty;
	}
	SLIB_INLINE void translate(const Vector2T<T>& t)
	{
		translate(t.x, t.y);
	}

	SLIB_INLINE Vector2T<T> getLeftTop() const
	{
		return Vector2T<T>(left, top);
	}
	SLIB_INLINE void setLeftTop(T _x, T _y)
	{
		left = _x;
		top = _y;
	}
	SLIB_INLINE void setLeftTop(const Vector2T<T>& pt)
	{
		left = pt.x;
		top = pt.y;
	}
	SLIB_INLINE Vector2T<T> getLeftBottom() const
	{
		return Vector2T<T>(left, bottom);
	}
	SLIB_INLINE void setLeftBottom(T _x, T _y)
	{
		left = _x;
		bottom = _y;
	}
	SLIB_INLINE void setLeftBottom(const Vector2T<T>& pt)
	{
		left = pt.x;
		bottom = pt.y;
	}
	SLIB_INLINE Vector2T<T> getRightTop() const
	{
		return Vector2T<T>(right, top);
	}
	SLIB_INLINE void setRightTop(T _x, T _y)
	{
		right = _x;
		top = _y;
	}
	SLIB_INLINE void setRightTop(const Vector2T<T>& pt)
	{
		right = pt.x;
		top = pt.y;
	}
	SLIB_INLINE Vector2T<T> getRightBottom() const
	{
		return Vector2T<T>(right, bottom);
	}
	SLIB_INLINE void setRightBottom(T _x, T _y)
	{
		right = _x;
		bottom = _y;
	}
	SLIB_INLINE void setRightBottom(const Vector2T<T>& pt)
	{
		right = pt.x;
		bottom = pt.y;
	}

	SLIB_INLINE Vector2T<T> getCenter() const
	{
		return Vector2T<T>((left + right) / 2, (top + bottom) / 2);
	}
	SLIB_INLINE Vector2T<T> center() const
	{
		return getCenter();
	}
	SLIB_INLINE void setCenter(T _x, T _y)
	{
		T w = (right - left) / 2;
		T h = (bottom - top) / 2;
		left = _x - w;
		top = _y - h;
		right = _x + w;
		bottom = _y + h;
	}
	SLIB_INLINE void setCenter(const Vector2T<T>& pt)
	{
		setCenter(pt.x, pt.y);
	}

	SLIB_INLINE void setZero()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	SLIB_INLINE sl_bool containsPoint(T x, T y) const
	{
		return x >= left && x <= right && y >= top && y <= bottom;
	}
	SLIB_INLINE sl_bool containsPoint(const Vector2T<T>& pt) const
	{
		return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom;
	}
	
	SLIB_INLINE sl_bool containsRectangle(const RectangleT<T>& other) const
	{
		return left <= other.right && right >= other.left && top <= other.bottom && bottom >= other.top;
	}

	SLIB_INLINE void setFromPoint(T x, T y)
	{
		left = right = x;
		top = bottom = y;
	}
	SLIB_INLINE void setFromPoint(const Vector2T<T>& pt)
	{
		left = right = pt.x;
		top = bottom = pt.y;
	}

	SLIB_INLINE void mergePoint(T x, T y)
	{
		if (left > x) {
			left = x;
		}
		if (right < x) {
			right = x;
		}
		if (top > y) {
			top = y;
		}
		if (bottom < y) {
			bottom = y;
		}
	}
	SLIB_INLINE void mergePoint(const Vector2T<T>& pt)
	{
		mergePoint(pt.x, pt.y);
	}

	void mergePoints(const Vector2T<T>* points, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			const Vector2T<T>& v = points[i];
			if (left > v.x) {
				left = v.x;
			}
			if (right < v.x) {
				right = v.x;
			}
			if (top > v.y) {
				top = v.y;
			}
			if (bottom < v.y) {
				bottom = v.y;
			}
		}
	}
	void mergePoints(const List< Vector2T<T> >& points)
	{
		ListLocker< Vector2T<T> > list(points);
		mergePoints(list.getBuffer(), list.getCount());
	}

	void setFromPoints(const Vector2T<T>* points, sl_size count)
	{
		if (count > 0) {
			setFromPoint(points[0]);
			if (count > 1) {
				mergePoints(points + 1, count - 1);
			}
		}
	}
	void setFromPoints(const List< Vector2T<T> >& points)
	{
		ListLocker< Vector2T<T> > list(points);
		setFromPoints(list.getBuffer(), list.getCount());
	}
	SLIB_INLINE void setFromPoints(const Vector2T<T>& pt1, const Vector2T<T>& pt2)
	{
		setFromPoint(pt1);
		mergePoint(pt2);
	}

	SLIB_INLINE void mergeRectangle(const RectangleT<T>& rect)
	{
		if (left > rect.left) {
			left = rect.left;
		}
		if (right < rect.right) {
			right = rect.right;
		}
		if (top > rect.top) {
			top = rect.top;
		}
		if (bottom < rect.bottom) {
			bottom = rect.bottom;
		}
	}

	// 4 points
	void getCornerPoints(Vector2T<T>* out) const
	{
		out[0].x = left; out[0].y = top;
		out[1].x = right; out[1].y = top;
		out[2].x = left; out[2].y = bottom;
		out[3].x = right; out[3].y = bottom;
	}
};

template <class T>
T RectangleT<T>::_zero[4] = {0, 0, 0, 0};

typedef RectangleT<sl_real> Rectangle;
typedef RectangleT<sl_int32> Rectanglei;
typedef RectangleT<sl_int64> Rectangleli;
typedef RectangleT<float> Rectanglef;
typedef RectangleT<double> Rectanglelf;

template <class T>
class SLIB_EXPORT Transform2T
{
public:
	static void setTranslation(Matrix3T<T>& out, T x, T y)
	{
		out.m00 = 1; out.m01 = 0; out.m02 = 0;
		out.m10 = 0; out.m11 = 1; out.m12 = 0;
		out.m20 = x; out.m21 = y; out.m22 = 1;
	}
	SLIB_INLINE static void setTranslation(Matrix3T<T>& out, const Vector2T<T>& v)
	{
		setTranslation(out, v.x, v.y);
	}
	SLIB_INLINE static Matrix3T<T> getTranslationMatrix(T x, T y)
	{
		Matrix3T<T> ret;
		setTranslation(ret, x, y);
		return ret;
	}
	SLIB_INLINE static Matrix3T<T> getTranslationMatrix(const Vector2T<T>& v)
	{
		Matrix3T<T> ret;
		setTranslation(ret, v);
		return ret;
	}

	static void setScaling(Matrix3T<T>& out, T sx, T sy)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1;
	}
	SLIB_INLINE static void setScaling(Matrix3T<T>& out, const Vector2T<T>& v)
	{
		setScaling(out, v.x, v.y);
	}
	SLIB_INLINE static Matrix3T<T> getScalingMatrix(T x, T y)
	{
		Matrix3T<T> ret;
		setScaling(ret, x, y);
		return ret;
	}
	SLIB_INLINE static Matrix3T<T> getScalingMatrix(const Vector2T<T>& v)
	{
		Matrix3T<T> ret;
		setScaling(ret, v);
		return ret;
	}

	static void setRotation(Matrix3T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = c; out.m01 = s; out.m02 = 0;
		out.m10 = -s; out.m11 = c; out.m12 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1;
	}
	static void setRotation(Matrix3T<T>& out, T cx, T cy, T radians)
	{
		Matrix3T<T> m;
		setTranslation(out, -cx, -cy);
		setRotation(m, radians);
		out.multiply(m);
		setTranslation(m, cx, cy);
		out.multiply(m);
	}
	SLIB_INLINE static void setRotation(Matrix3& out, const Vector2T<T>& pt, T radians)
	{
		setRotation(out, pt.x, pt.y, radians);
	}
	SLIB_INLINE static Matrix3T<T> getRotationMatrix(T radians)
	{
		Matrix3T<T> ret;
		setRotation(ret, radians);
		return ret;
	}
	SLIB_INLINE static Matrix3T<T> getRotationMatrix(T cx, T cy, T radians)
	{
		Matrix3T<T> ret;
		setRotation(ret, cx, cy, radians);
		return ret;
	}
	SLIB_INLINE static Matrix3T<T> getRotationMatrix(const Vector2T<T>& pt, T radians)
	{
		Matrix3T<T> ret;
		setRotation(ret, pt, radians);
		return ret;
	}

	SLIB_INLINE static T getRotationAngleFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to)
	{
		return to.getAngleBetween(from);
	}
	SLIB_INLINE static void setTransformFromDirToDir(Matrix3T<T>& out, const Vector2T<T>& from, const Vector2T<T>& to)
	{
		setRotation(out, getRotationAngleFromDirToDir(from, to));
	}
	SLIB_INLINE static Matrix3T<T> getTransformMatrixFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to)
	{
		Matrix3T<T> ret;
		setTransformFromDirToDir(ret, from, to);
		return ret;
	}
};

typedef Transform2T<sl_real> Transform2;
typedef Transform2T<float> Transform2f;
typedef Transform2T<double> Transform2lf;

SLIB_MATH_NAMESPACE_END

#endif
