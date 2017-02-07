#ifndef CHECKHEADER_SLIB_MATH_TRANSFORM3D
#define CHECKHEADER_SLIB_MATH_TRANSFORM3D

#include "definition.h"

#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#include "line3.h"
#include "rectangle.h"
#include "matrix4.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Transform3T
{
public:
	static void setTranslation(Matrix4T<T>& _out, T x, T y, T z);
	
	static void setTranslation(Matrix4T<T>& _out, const Vector3T<T>& v);
	
	static Matrix4T<T> getTranslationMatrix(T x, T y, T z);
	
	static Matrix4T<T> getTranslationMatrix(const Vector3T<T>& v);
	
	static void translate(Matrix4T<T>& mat, T x, T y, T z);
	
	static void translate(Matrix4T<T>& mat, const Vector3T<T>& v);

	
	static void setScaling(Matrix4T<T>& _out, T sx, T sy, T sz);
	
	static void setScaling(Matrix4T<T>& _out, const Vector3T<T>& v);
	
	static Matrix4T<T> getScalingMatrix(T x, T y, T z);
	
	static Matrix4T<T> getScalingMatrix(const Vector3T<T>& v);
	
	static void scale(Matrix4T<T>& mat, T sx, T sy, T sz);
	
	static void scale(Matrix4T<T>& mat, const Vector3T<T>& v);

	
	static void setRotationX(Matrix4T<T>& _out, T radians);
	
	static Matrix4T<T> getRotationXMatrix(T radians);
	
	static void rotateX(Matrix4T<T>& mat, T radians);
	
	static void setRotationY(Matrix4T<T>& _out, T radians);
	
	static Matrix4T<T> getRotationYMatrix(T radians);
	
	static void rotateY(Matrix4T<T>& mat, T radians);
	
	static void setRotationZ(Matrix4T<T>& _out, T radians);
	
	static Matrix4T<T> getRotationZMatrix(T radians);
	
	static void rotateZ(Matrix4T<T>& mat, T radians);
	
	static void setRotation(Matrix4T<T>& _out, const QuaternionT<T>& q);
	
	static Matrix4T<T> getRotationMatrix(const QuaternionT<T>& q);
	
	static void rotate(Matrix4T<T>& mat, const QuaternionT<T>& q);
	
	static void setRotation(Matrix4T<T>& _out, const Vector3T<T>& vAxis, T fAngle);
	
	static Matrix4T<T> getRotationMatrix(const Vector3T<T>& vAxis, T fAngle);
	
	static void rotate(Matrix4T<T>& mat, const Vector3T<T>& vAxis, T fAngle);
	
	
	// Slib uses Left-Handed coordinate system
	static void setPerspectiveProjection(Matrix4T<T>& _out, T sx, T sy, T zNear, T zFar);
	
	static Matrix4T<T> getPerspectiveProjectionMatrix(T sx, T sy, T zNear, T zFar);
	
	static void setPerspectiveProjectionFovY(Matrix4T<T>& _out, T fovY, T fAspectWH, T zNear, T zFar);
	
	static Matrix4T<T> getPerspectiveProjectionFovYMatrix(T fovY, T fAspectWH, T zNear, T zFar);
	
	static void setOrthogonalProjection(Matrix4T<T>& _out, T sx, T sy, T zNear, T zFar);
	
	static Matrix4T<T> getOrthogonalProjectionMatrix(T sx, T sy, T zNear, T zFar);

	static void lookAt(Matrix4T<T>& _out, const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up);
	
	static Matrix4T<T> getLookAtMatrix(const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up);

	static void makeTransform(Matrix4T<T>& _out, const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation);
	
	static Matrix4T<T> getTransformMatrix(const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation);

	static void getRotationFromDirToDir(Vector3T<T>& outAxis, T& outAngle, const Vector3T<T>& from, const Vector3T<T>& to);

	static void setQuaternionFromDirToDir(QuaternionT<T>& _out, const Vector3T<T>& from, const Vector3T<T>& to);
	
	static QuaternionT<T> getQuaternionRotationFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to);

	static void setTransformFromDirToDir(Matrix4T<T>& _out, const Vector3T<T>& from, const Vector3T<T>& to);
	
	static Matrix4T<T> getTransformMatrixFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to);

	static Vector3T<T> getTransformedOrigin(const Matrix4T<T>& transform);

	static Vector3T<T> getTransformedAxisX(const Matrix4T<T>& transform);

	static Vector3T<T> getTransformedAxisY(const Matrix4T<T>& transform);

	static Vector3T<T> getTransformedAxisZ(const Matrix4T<T>& transform);

	static Vector3T<T> projectToViewport(const Matrix4T<T>& matViewProjection, const Vector3T<T>& point);

	static Line3T<T> unprojectViewportPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt);
	
	static Line3T<T> unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, T viewportWidth, T viewportHeight);
	
	static Line3T<T> unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, const RectangleT<T>& viewport);

	static Vector2T<T> convertViewportToScreen(const Vector2T<T>& ptViewport, const RectangleT<T>& viewport);
	
	static Vector2T<T> convertViewportToScreen(const Vector2T<T>& ptViewport, T viewportWidth, T viewportHeight);
	
	static Vector2T<T> convertScreenToViewport(const Vector2T<T>& ptScreen, const RectangleT<T>& viewport);
	
	static Vector2T<T> convertScreenToViewport(const Vector2T<T>& ptScreen, T viewportWidth, T viewportHeight);

	static RectangleT<T> convertViewportToScreen(const RectangleT<T>& rcInViewport, const RectangleT<T>& viewport);
	
	static RectangleT<T> convertViewportToScreen(const RectangleT<T>& rcInViewport, T viewportWidth, T viewportHeight);
	
	static RectangleT<T> convertScreenToViewport(const RectangleT<T>& rcInScreen, const RectangleT<T>& viewport);
	
	static RectangleT<T> convertScreenToViewport(const RectangleT<T>& rcInScreen, T viewportWidth, T viewportHeight);
	
};

extern template class Transform3T<float>;
extern template class Transform3T<double>;
typedef Transform3T<sl_real> Transform3;
typedef Transform3T<float> Transform3f;
typedef Transform3T<double> Transform3lf;

SLIB_MATH_NAMESPACE_END

#endif
