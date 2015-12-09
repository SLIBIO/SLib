#ifndef CHECKHEADER_SLIB_MATH_TRANSFORM2D
#define CHECKHEADER_SLIB_MATH_TRANSFORM2D

#include "definition.h"
#include "vector2.h"
#include "matrix3.h"

SLIB_MATH_NAMESPACE_BEGIN

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
