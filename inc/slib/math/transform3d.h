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
	static void setTranslation(Matrix4T<T>& out, T x, T y, T z)
	{
		out.m00 = 1; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = 1; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1; out.m23 = 0;
		out.m30 = x; out.m31 = y; out.m32 = z; out.m33 = 1;
	}
	
	SLIB_INLINE static void setTranslation(Matrix4T<T>& out, const Vector3T<T>& v)
	{
		setTranslation(out, v.x, v.y, v.z);
	}
	
	SLIB_INLINE static Matrix4T<T> getTranslationMatrix(T x, T y, T z)
	{
		Matrix4T<T> ret;
		setTranslation(ret, x, y, z);
		return ret;
	}
	
	SLIB_INLINE static Matrix4T<T> getTranslationMatrix(const Vector3T<T>& v)
	{
		Matrix4T<T> ret;
		setTranslation(ret, v);
		return ret;
	}

	static void setScaling(Matrix4T<T>& out, T sx, T sy, T sz)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = sz; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	
	SLIB_INLINE static void setScaling(Matrix4T<T>& out, const Vector3T<T>& v)
	{
		setScaling(out, v.x, v.y, v.z);
	}
	
	SLIB_INLINE static Matrix4T<T> getScalingMatrix(T x, T y, T z)
	{
		Matrix4T<T> ret;
		setScaling(ret, x, y, z);
		return ret;
	}
	
	SLIB_INLINE static Matrix4T<T> getScalingMatrix(const Vector3T<T>& v)
	{
		Matrix4T<T> ret;
		setScaling(ret, v);
		return ret;
	}

	static void setRotationX(Matrix4T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = 1; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = c; out.m12 = s; out.m13 = 0;
		out.m20 = 0; out.m21 = -s; out.m22 = c; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	
	static void setRotationY(Matrix4T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = c; out.m01 = 0; out.m02 = -s; out.m03 = 0;
		out.m10 = 0; out.m11 = 1; out.m12 = 0; out.m13 = 0;
		out.m20 = s; out.m21 = 0; out.m22 = c; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	
	static void setRotationZ(Matrix4T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = c; out.m01 = s; out.m02 = 0; out.m03 = 0;
		out.m10 = -s; out.m11 = c; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	
	static void setRotation(Matrix4T<T>& out, const QuaternionT<T>& q)
	{
		T d = q.getLength2p();
		T s = 2 / d;
		T x = q.x * s, y = q.y * s, z = q.z * s;
		T wx = q.w * x, wy = q.w * y, wz = q.w * z;
		T xx = q.x * x, xy = q.x * y, xz = q.x * z;
		T yy = q.y * y, yz = q.y * z, zz = q.z * z;

		out.m00 = 1 - (yy + zz);
		out.m01 = xy - wz;
		out.m02 = xz + wy;
		out.m03 = 0;
		out.m10 = xy + wz;
		out.m11 = 1 - (xx + zz);
		out.m12 = yz - wx;
		out.m13 = 0;
		out.m20 = xz - wy;
		out.m21 = yz + wx;
		out.m22 = 1 - (xx + yy);
		out.m23 = 0;
		out.m30 = 0;
		out.m31 = 0;
		out.m32 = 0;
		out.m33 = 1;
	}
	
	static void setRotation(Matrix4T<T>& out, const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> q;
		q.setRotation(vAxis, fAngle);
		setRotation(out, q);
	}
	
	SLIB_INLINE static Matrix4T<T> getRotationXMatrix(T radians)
	{
		Matrix4T<T> ret;
		setRotationX(ret, radians);
		return ret;
	}
	
	SLIB_INLINE static Matrix4T<T> getRotationYMatrix(T radians)
	{
		Matrix4T<T> ret;
		setRotationY(ret, radians);
		return ret;
	}
	
	SLIB_INLINE static Matrix4T<T> getRotationZMatrix(T radians)
	{
		Matrix4T<T> ret;
		setRotationZ(ret, radians);
		return ret;
	}
	
	SLIB_INLINE static Matrix4T<T> getRotationMatrix(const QuaternionT<T>& q)
	{
		Matrix4T<T> ret;
		setRotation(ret, q);
		return ret;
	}
	
	SLIB_INLINE static Matrix4T<T> getRotationMatrix(const Vector3T<T>& vAxis, T fAngle)
	{
		Matrix4T<T> ret;
		setRotation(ret, vAxis, fAngle);
		return ret;
	}

	static void setPerspectiveProjectionFovY(Matrix4T<T>& out, T fovY, T fAspectWH, T zNear, T zFar)
	{
		T sy = Math::cot(fovY / 2);
		T sx = sy / fAspectWH;
		setPerspectiveProjection(out, sx, sy, zNear, zFar);
	}
	
	SLIB_INLINE static Matrix4T<T> getPerspectiveProjectionFovYMatrix(T fovY, T fAspectWH, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setPerspectiveProjectionFovY(ret, fovY, fAspectWH, zNear, zFar);
		return ret;
	}

	// Slib uses Left-Handed coordinate system
	static void setPerspectiveProjection(Matrix4T<T>& out, T sx, T sy, T zNear, T zFar)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = zFar / (zFar - zNear); out.m23 = 1;
		out.m30 = 0; out.m31 = 0; out.m32 = -zNear * zFar / (zFar - zNear); out.m33 = 0;
	}
	
	SLIB_INLINE static Matrix4T<T> getPerspectiveProjectionMatrix(T sx, T sy, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setPerspectiveProjection(ret, sx, sy, zNear, zFar);
		return ret;
	}

	static void setOrthogonalProjection(Matrix4T<T>& out, T sx, T sy, T zNear, T zFar)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1 / (zFar - zNear); out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = zNear / (zFar - zNear); out.m33 = 1;
	}
	
	SLIB_INLINE static Matrix4T<T> getOrthogonalProjectionMatrix(T sx, T sy, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setOrthogonalProjection(ret, sx, sy, zNear, zFar);
		return ret;
	}

	static void lookAt(Matrix4T<T>& out, const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up)
	{
		Vector3T<T> xAxis, yAxis, zAxis;
		zAxis = (at - eye);
		zAxis.normalize();
		if (Math::isLessThanEpsilon(zAxis.length2p())) {
			zAxis = Vector3T<T>::axisZ();
		}
		xAxis = up.cross(zAxis);
		xAxis.normalize();
		if (Math::isLessThanEpsilon(xAxis.length2p())) {
			xAxis = Vector3T<T>::axisX();
		}
		yAxis = zAxis.cross(xAxis);
		if (Math::isLessThanEpsilon(yAxis.length2p())) {
			yAxis = Vector3T<T>::axisY();
		}
		out.m00 = xAxis.x; out.m01 = yAxis.x; out.m02 = zAxis.x; out.m03 = 0;
		out.m10 = xAxis.y; out.m11 = yAxis.y; out.m12 = zAxis.y; out.m13 = 0;
		out.m20 = xAxis.z; out.m21 = yAxis.z; out.m22 = zAxis.z; out.m23 = 0;
		out.m30 = -xAxis.dot(eye); out.m31 = -yAxis.dot(eye); out.m32 = -zAxis.dot(eye); out.m33 = 1;
	}
	
	SLIB_INLINE static Matrix4T<T> getLookAtMatrix(const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up)
	{
		Matrix4T<T> ret;
		lookAt(ret, eye, at, up);
		return ret;
	}

	static void makeTransform(Matrix4T<T>& out, const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation)
	{
		setRotation(out, rotation);
		out.m00 *= scaling.x; out.m01 *= scaling.x; out.m02 *= scaling.x;
		out.m10 *= scaling.y; out.m11 *= scaling.y; out.m12 *= scaling.y;
		out.m20 *= scaling.z; out.m21 *= scaling.z; out.m22 *= scaling.z;
		out.m30 = position.x; out.m31 = position.y; out.m32 *= position.z;
	}
	
	SLIB_INLINE static Matrix4T<T> getTransformMatrix(const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation)
	{
		Matrix4T<T> ret;
		makeTransform(ret, position, scaling, rotation);
		return ret;
	}

	static void getRotationFromDirToDir(Vector3T<T>& outAxis, T& outAngle, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Vector3T<T> dirBefore = from;
		dirBefore.normalize();
		Vector3T<T> dirNext = to;
		dirNext.normalize();
		outAxis = dirBefore.cross(dirNext);
		outAngle = -Math::arccos(dirBefore.dot(dirNext));
	}

	SLIB_INLINE static void setQuaternionFromDirToDir(QuaternionT<T>& out, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Vector3T<T> dirAxisRotation;
		T angleRotation;
		getRotationFromDirToDir(dirAxisRotation, angleRotation, from, to);
		out.setRotation(dirAxisRotation, angleRotation);
	}
	
	SLIB_INLINE static QuaternionT<T> getQuaternionRotationFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to)
	{
		QuaternionT<T> ret;
		setQuaternionFromDirToDir(from, to);
		return ret;
	}

	SLIB_INLINE static void setTransformFromDirToDir(Matrix4T<T>& out, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		QuaternionT<T> q;
		setQuaternionFromDirToDir(q, from, to);
		setRotation(out, q);
	}
	
	SLIB_INLINE static Matrix4T<T> getTransformMatrixFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Matrix4T<T> ret;
		setTransformFromDirToDir(ret, from, to);
		return ret;
	}

	SLIB_INLINE static Vector3T<T> getTransformedOrigin(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m30, transform.m31, transform.m32);
	}

	SLIB_INLINE static Vector3T<T> getTransformedAxisX(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m00, transform.m01, transform.m02);
	}

	SLIB_INLINE static Vector3T<T> getTransformedAxisY(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m10, transform.m11, transform.m12);
	}

	SLIB_INLINE static Vector3T<T> getTransformedAxisZ(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m20, transform.m21, transform.m22);
	}

	static Vector3T<T> projectToViewport(const Matrix4T<T>& matViewProjection, const Vector3T<T>& point)
	{
		Vector4T<T> v = Vector4T<T>(point, 1) * matViewProjection;
		if (v.w >= 0 && Math::isLessThanEpsilon(v.w)) {
			Math::getEpsilon(&(v.w));
		}
		if (v.w <= 0 && Math::isLessThanEpsilon(-v.w)) {
			Math::getEpsilon(&(v.w));
			v.w = -v.w;
		}
		return Vector3T<T>(v.x / v.w, v.y / v.w, v.z / v.w);
	}

	static Line3T<T> unprojectViewportPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt)
	{
		Vector4T<T> vTest1(1, 1, 1, 1);
		Vector4T<T> vTest2(1, 1, 2, 1);
		Vector4T<T> sTest1, sTest2;
		sTest1 = vTest1 * matProjection;
		sTest2 = vTest2 * matProjection;
		vTest1.x = pt.x / sTest1.x * sTest1.w;
		vTest1.y = pt.y / sTest1.y * sTest1.w;
		vTest2.x = pt.x / sTest2.x * sTest2.w;
		vTest2.y = pt.y / sTest2.y * sTest2.w;
		Line3T<T> ret(vTest1.xyz(), vTest2.xyz());
		return ret;
	}
	
	SLIB_INLINE static Line3T<T> unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, T viewportWidth, T viewportHeight)
	{
		return unprojectViewportPoint(matProjection, Vector2T<T>((pt.x / viewportWidth * 2) - 1, 1 - (pt.y / viewportHeight * 2)));
	}
	
	SLIB_INLINE static Line3T<T> unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, const RectangleT<T>& viewport)
	{
		return unprojectScreenPoint(matProjection, Vector2T<T>(pt.x - viewport.left, pt.y - viewport.top), viewport.getWidth(), viewport.getHeight());
	}

	SLIB_INLINE static Vector2T<T> convertViewportToScreen(const Vector2T<T>& ptViewport, const RectangleT<T>& viewport)
	{
		return Vector2T<T>(
			((viewport.left + viewport.right) + ptViewport.x * (viewport.right - viewport.left)) / 2
			, ((viewport.top + viewport.bottom) - ptViewport.y * (viewport.bottom - viewport.top)) / 2
			);
	}
	
	SLIB_INLINE static Vector2T<T> convertViewportToScreen(const Vector2T<T>& ptViewport, T viewportWidth, T viewportHeight)
	{
		return Vector2T<T>(
			(1 + ptViewport.x) * viewportWidth / 2
			, (1 - ptViewport.y) * viewportHeight / 2
			);
	}
	
	SLIB_INLINE static Vector2T<T> convertScreenToViewport(const Vector2T<T>& ptScreen, const RectangleT<T>& viewport)
	{
		return Vector2T<T>(
			(ptScreen.x - viewport.left) * 2 / (viewport.right - viewport.left) - 1
			, 1 - (ptScreen.y - viewport.top) * 2 / (viewport.bottom - viewport.top)
			);
	}
	
	SLIB_INLINE static Vector2T<T> convertScreenToViewport(const Vector2T<T>& ptScreen, T viewportWidth, T viewportHeight)
	{
		return Vector2T<T>(
			ptScreen.x * 2 / viewportWidth - 1
			, 1 - ptScreen.y * 2 / viewportHeight
			);
	}

	SLIB_INLINE static RectangleT<T> convertViewportToScreen(const RectangleT<T>& rcInViewport, const RectangleT<T>& viewport)
	{
		return RectangleT<T>(
			((viewport.left + viewport.right) + rcInViewport.left * (viewport.right - viewport.left)) / 2
			, ((viewport.top + viewport.bottom) - rcInViewport.bottom * (viewport.bottom - viewport.top)) / 2
			, ((viewport.left + viewport.right) + rcInViewport.right * (viewport.right - viewport.left)) / 2
			, ((viewport.top + viewport.bottom) - rcInViewport.top * (viewport.bottom - viewport.top)) / 2
			);
	}
	
	SLIB_INLINE static RectangleT<T> convertViewportToScreen(const RectangleT<T>& rcInViewport, T viewportWidth, T viewportHeight)
	{
		return RectangleT<T>(
			(1 + rcInViewport.left) * viewportWidth / 2
			, (1 - rcInViewport.bottom) * viewportHeight / 2
			, (1 + rcInViewport.right) * viewportWidth / 2
			, (1 - rcInViewport.top) * viewportHeight / 2
			);
	}
	
	SLIB_INLINE static RectangleT<T> convertScreenToViewport(const RectangleT<T>& rcInScreen, const RectangleT<T>& viewport)
	{
		return RectangleT<T>(
			(rcInScreen.left - viewport.left) * 2 / (viewport.right - viewport.left) - 1
			, 1 - (rcInScreen.bottom - viewport.top) * 2 / (viewport.bottom - viewport.top)
			, (rcInScreen.right - viewport.left) * 2 / (viewport.right - viewport.left) - 1
			, 1 - (rcInScreen.top - viewport.top) * 2 / (viewport.bottom - viewport.top)
			);
	}
	
	SLIB_INLINE static RectangleT<T> convertScreenToViewport(const RectangleT<T>& rcInScreen, T viewportWidth, T viewportHeight)
	{
		return RectangleT<T>(
			rcInScreen.left * 2 / viewportWidth - 1
			, 1 - rcInScreen.bottom * 2 / viewportHeight
			, rcInScreen.right * 2 / viewportWidth - 1
			, 1 - rcInScreen.top * 2 / viewportHeight
			);
	}
};

typedef Transform3T<sl_real> Transform3;
typedef Transform3T<float> Transform3f;
typedef Transform3T<double> Transform3lf;

SLIB_MATH_NAMESPACE_END

#endif
