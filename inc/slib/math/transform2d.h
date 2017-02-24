#ifndef CHECKHEADER_SLIB_MATH_TRANSFORM2D
#define CHECKHEADER_SLIB_MATH_TRANSFORM2D

#include "definition.h"

#include "vector2.h"
#include "matrix3.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Transform2T
	{
	public:
		static void setTranslation(Matrix3T<T>& _out, T x, T y);

		static void setTranslation(Matrix3T<T>& _out, const Vector2T<T>& v);

		static Matrix3T<T> getTranslationMatrix(T x, T y);

		static Matrix3T<T> getTranslationMatrix(const Vector2T<T>& v);

		static void translate(Matrix3T<T>& mat, T x, T y);

		static void translate(Matrix3T<T>& mat, const Vector2T<T>& v);
		
		static void preTranslate(Matrix3T<T>& mat, T x, T y);
		
		static void preTranslate(Matrix3T<T>& mat, const Vector2T<T>& v);
		
		static sl_bool isTranslation(const Matrix3T<T>& mat);


		static void setScaling(Matrix3T<T>& _out, T sx, T sy);

		static void setScaling(Matrix3T<T>& _out, const Vector2T<T>& v);

		static Matrix3T<T> getScalingMatrix(T x, T y);

		static Matrix3T<T> getScalingMatrix(const Vector2T<T>& v);

		static void scale(Matrix3T<T>& mat, T sx, T sy);

		static void scale(Matrix3T<T>& mat, const Vector2T<T>& v);
	

		static void setRotation(Matrix3T<T>& _out, T radians);

		static void setRotation(Matrix3T<T>& _out, T cx, T cy, T radians);

		static void setRotation(Matrix3T<T>& _out, const Vector2T<T>& pt, T radians);

		static Matrix3T<T> getRotationMatrix(T radians);

		static Matrix3T<T> getRotationMatrix(T cx, T cy, T radians);

		static Matrix3T<T> getRotationMatrix(const Vector2T<T>& pt, T radians);

		static void rotate(Matrix3T<T>& mat, T radians);

		static void rotate(Matrix3T<T>& mat, T cx, T cy, T radians);

		static void rotate(Matrix3T<T>& mat, const Vector2T<T>& pt, T radians);
	

		static T getRotationAngleFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to);

		static void setTransformFromDirToDir(Matrix3T<T>& _out, const Vector2T<T>& from, const Vector2T<T>& to);

		static Matrix3T<T> getTransformMatrixFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to);

	};
	
	extern template class Transform2T<float>;
	extern template class Transform2T<double>;
	typedef Transform2T<sl_real> Transform2;
	typedef Transform2T<float> Transform2f;
	typedef Transform2T<double> Transform2lf;

}

#endif
