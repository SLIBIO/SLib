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
	static void setTranslation(Matrix3T<T>& _out, T x, T y);
	
	static void setTranslation(Matrix3T<T>& _out, const Vector2T<T>& v);
	
	static Matrix3T<T> getTranslationMatrix(T x, T y);
	
	static Matrix3T<T> getTranslationMatrix(const Vector2T<T>& v);

	static void setScaling(Matrix3T<T>& _out, T sx, T sy);
	
	static void setScaling(Matrix3T<T>& _out, const Vector2T<T>& v);
	
	static Matrix3T<T> getScalingMatrix(T x, T y);
	
	static Matrix3T<T> getScalingMatrix(const Vector2T<T>& v);

	static void setRotation(Matrix3T<T>& _out, T radians);
	
	static void setRotation(Matrix3T<T>& _out, T cx, T cy, T radians);
	
	static void setRotation(Matrix3T<T>& _out, const Vector2T<T>& pt, T radians);
	
	static Matrix3T<T> getRotationMatrix(T radians);
	
	static Matrix3T<T> getRotationMatrix(T cx, T cy, T radians);
	
	static Matrix3T<T> getRotationMatrix(const Vector2T<T>& pt, T radians);

	static T getRotationAngleFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to);
	
	static void setTransformFromDirToDir(Matrix3T<T>& _out, const Vector2T<T>& from, const Vector2T<T>& to);
	
	static Matrix3T<T> getTransformMatrixFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to);
	
};

SLIB_DECLARE_GEOMETRY_TYPE(Transform2)

SLIB_MATH_NAMESPACE_END

#endif
