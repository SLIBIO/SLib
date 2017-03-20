/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/math/transform3d.h"

#include "../../../inc/slib/core/math.h"

namespace slib
{

	template <class T>
	void Transform3T<T>::setTranslation(Matrix4T<T>& _out, T x, T y, T z)
	{
		_out.m00 = 1; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = 1; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1; _out.m23 = 0;
		_out.m30 = x; _out.m31 = y; _out.m32 = z; _out.m33 = 1;
	}

	template <class T>
	void Transform3T<T>::setTranslation(Matrix4T<T>& _out, const Vector3T<T>& v)
	{
		_out.m00 = 1; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = 1; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1; _out.m23 = 0;
		_out.m30 = v.x; _out.m31 = v.y; _out.m32 = v.z; _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getTranslationMatrix(T x, T y, T z)
	{
		return {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				x, y, z, 1};
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getTranslationMatrix(const Vector3T<T>& v)
	{
		return {1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				v.x, v.y, v.z, 1};
	}

	template <class T>
	void Transform3T<T>::translate(Matrix4T<T>& mat, T x, T y, T z)
	{
		mat.m30 += x;
		mat.m31 += y;
		mat.m32 += z;
	}

	template <class T>
	void Transform3T<T>::translate(Matrix4T<T>& mat, const Vector3T<T>& v)
	{
		mat.m30 += v.x;
		mat.m31 += v.y;
		mat.m32 += v.z;
	}

	template <class T>
	void Transform3T<T>::setScaling(Matrix4T<T>& _out, T sx, T sy, T sz)
	{
		_out.m00 = sx; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = sy; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = sz; _out.m23 = 0;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = 0; _out.m33 = 1;
	}

	template <class T>
	void Transform3T<T>::setScaling(Matrix4T<T>& _out, const Vector3T<T>& v)
	{
		_out.m00 = v.x; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = v.y; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = v.z; _out.m23 = 0;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = 0; _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getScalingMatrix(T x, T y, T z)
	{
		return {x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1};
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getScalingMatrix(const Vector3T<T>& v)
	{
		return {v.x, 0, 0, 0,
				0, v.y, 0, 0,
				0, 0, v.z, 0,
				0, 0, 0, 1};
	}

	template <class T>
	void Transform3T<T>::scale(Matrix4T<T>& mat, T sx, T sy, T sz)
	{
		mat.m00 *= sx;
		mat.m10 *= sx;
		mat.m20 *= sx;
		mat.m30 *= sx;
		mat.m01 *= sy;
		mat.m11 *= sy;
		mat.m21 *= sy;
		mat.m31 *= sy;
		mat.m02 *= sz;
		mat.m12 *= sz;
		mat.m22 *= sz;
		mat.m32 *= sz;
	}

	template <class T>
	void Transform3T<T>::scale(Matrix4T<T>& mat, const Vector3T<T>& v)
	{
		scale(mat, v.x, v.y, v.z);
	}

	template <class T>
	void Transform3T<T>::setRotationX(Matrix4T<T>& _out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		_out.m00 = 1; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = c; _out.m12 = s; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = -s; _out.m22 = c; _out.m23 = 0;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = 0; _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getRotationXMatrix(T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		return {1, 0, 0, 0,
				0, c, s, 0,
				0, -s, c, 0,
				0, 0, 0, 1};
	}

	template <class T>
	void Transform3T<T>::rotateX(Matrix4T<T>& mat, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		T v1, v2;
		v1 = mat.m01 * c - mat.m02 * s;
		v2 = mat.m01 * s + mat.m02 * c;
		mat.m01 = v1; mat.m02 = v2;
		v1 = mat.m11 * c - mat.m12 * s;
		v2 = mat.m11 * s + mat.m12 * c;
		mat.m11 = v1; mat.m12 = v2;
		v1 = mat.m21 * c - mat.m22 * s;
		v2 = mat.m21 * s + mat.m22 * c;
		mat.m21 = v1; mat.m22 = v2;
		v1 = mat.m31 * c - mat.m32 * s;
		v2 = mat.m31 * s + mat.m32 * c;
		mat.m31 = v1; mat.m32 = v2;
	}

	template <class T>
	void Transform3T<T>::setRotationY(Matrix4T<T>& _out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		_out.m00 = c; _out.m01 = 0; _out.m02 = -s; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = 1; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = s; _out.m21 = 0; _out.m22 = c; _out.m23 = 0;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = 0; _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getRotationYMatrix(T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		return {c, 0, -s, 0,
				0, 1, 0, 0,
				s, 0, c, 0,
				0, 0, 0, 1};
	}

	template <class T>
	void Transform3T<T>::rotateY(Matrix4T<T>& mat, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		T v0, v2;
		v0 = mat.m00 * c + mat.m02 * s;
		v2 = - mat.m00 * s + mat.m02 * c;
		mat.m00 = v0; mat.m02 = v2;
		v0 = mat.m10 * c + mat.m12 * s;
		v2 = - mat.m10 * s + mat.m12 * c;
		mat.m10 = v0; mat.m12 = v2;
		v0 = mat.m20 * c + mat.m22 * s;
		v2 = - mat.m20 * s + mat.m22 * c;
		mat.m20 = v0; mat.m22 = v2;
		v0 = mat.m30 * c + mat.m32 * s;
		v2 = - mat.m30 * s + mat.m32 * c;
		mat.m30 = v0; mat.m32 = v2;
	}

	template <class T>
	void Transform3T<T>::setRotationZ(Matrix4T<T>& _out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		_out.m00 = c; _out.m01 = s; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = -s; _out.m11 = c; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1; _out.m23 = 0;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = 0; _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getRotationZMatrix(T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		return {c, s, 0, 0,
				-s, c, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};
	}

	template <class T>
	void Transform3T<T>::rotateZ(Matrix4T<T>& mat, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		T v0, v1;
		v0 = mat.m00 * c - mat.m01 * s;
		v1 = mat.m00 * s + mat.m01 * c;
		mat.m00 = v0; mat.m01 = v1;
		v0 = mat.m10 * c - mat.m11 * s;
		v1 = mat.m10 * s + mat.m11 * c;
		mat.m10 = v0; mat.m11 = v1;
		v0 = mat.m20 * c - mat.m21 * s;
		v1 = mat.m20 * s + mat.m21 * c;
		mat.m20 = v0; mat.m21 = v1;
		v0 = mat.m30 * c - mat.m31 * s;
		v1 = mat.m30 * s + mat.m31 * c;
		mat.m30 = v0; mat.m31 = v1;
	}

	template <class T>
	void Transform3T<T>::setRotation(Matrix4T<T>& _out, const QuaternionT<T>& q)
	{
		T d = q.getLength2p();
		T s = 2 / d;
		T x = q.x * s, y = q.y * s, z = q.z * s;
		T wx = q.w * x, wy = q.w * y, wz = q.w * z;
		T xx = q.x * x, xy = q.x * y, xz = q.x * z;
		T yy = q.y * y, yz = q.y * z, zz = q.z * z;
		
		_out.m00 = 1 - (yy + zz);
		_out.m01 = xy - wz;
		_out.m02 = xz + wy;
		_out.m03 = 0;
		_out.m10 = xy + wz;
		_out.m11 = 1 - (xx + zz);
		_out.m12 = yz - wx;
		_out.m13 = 0;
		_out.m20 = xz - wy;
		_out.m21 = yz + wx;
		_out.m22 = 1 - (xx + yy);
		_out.m23 = 0;
		_out.m30 = 0;
		_out.m31 = 0;
		_out.m32 = 0;
		_out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getRotationMatrix(const QuaternionT<T>& q)
	{
		T d = q.getLength2p();
		T s = 2 / d;
		T x = q.x * s, y = q.y * s, z = q.z * s;
		T wx = q.w * x, wy = q.w * y, wz = q.w * z;
		T xx = q.x * x, xy = q.x * y, xz = q.x * z;
		T yy = q.y * y, yz = q.y * z, zz = q.z * z;
		
		return {1 - (yy + zz), xy - wz, xz + wy, 0,
				xy + wz, 1 - (xx + zz), yz - wx, 0,
				xz - wy, yz + wx, 1 - (xx + yy), 0,
				0, 0, 0, 1};
	}

	template <class T>
	void Transform3T<T>::rotate(Matrix4T<T>& mat, const QuaternionT<T>& q)
	{
		T d = q.getLength2p();
		T s = 2 / d;
		T x = q.x * s, y = q.y * s, z = q.z * s;
		T wx = q.w * x, wy = q.w * y, wz = q.w * z;
		T xx = q.x * x, xy = q.x * y, xz = q.x * z;
		T yy = q.y * y, yz = q.y * z, zz = q.z * z;
		
		T o00 = 1 - (yy + zz);
		T o01 = xy - wz;
		T o02 = xz + wy;
		T o10 = xy + wz;
		T o11 = 1 - (xx + zz);
		T o12 = yz - wx;
		T o20 = xz - wy;
		T o21 = yz + wx;
		T o22 = 1 - (xx + yy);
		
		T v0, v1, v2;
		v0 = mat.m00 * o00 + mat.m01 * o10 + mat.m02 * o20;
		v1 = mat.m00 * o01 + mat.m01 * o11 + mat.m02 * o21;
		v2 = mat.m00 * o02 + mat.m01 * o12 + mat.m02 * o22;
		mat.m00 = v0; mat.m01 = v1; mat.m02 = v2;
		v0 = mat.m10 * o00 + mat.m11 * o10 + mat.m12 * o20;
		v1 = mat.m10 * o01 + mat.m11 * o11 + mat.m12 * o21;
		v2 = mat.m10 * o02 + mat.m11 * o12 + mat.m12 * o22;
		mat.m10 = v0; mat.m11 = v1; mat.m12 = v2;
		v0 = mat.m20 * o00 + mat.m21 * o10 + mat.m22 * o20;
		v1 = mat.m20 * o01 + mat.m21 * o11 + mat.m22 * o21;
		v2 = mat.m20 * o02 + mat.m21 * o12 + mat.m22 * o22;
		mat.m20 = v0; mat.m21 = v1; mat.m22 = v2;
		v0 = mat.m30 * o00 + mat.m31 * o10 + mat.m32 * o20;
		v1 = mat.m30 * o01 + mat.m31 * o11 + mat.m32 * o21;
		v2 = mat.m30 * o02 + mat.m31 * o12 + mat.m32 * o22;
		mat.m30 = v0; mat.m31 = v1; mat.m32 = v2;
	}

	template <class T>
	void Transform3T<T>::setRotation(Matrix4T<T>& _out, const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> q;
		q.setRotation(vAxis, fAngle);
		setRotation(_out, q);
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getRotationMatrix(const Vector3T<T>& vAxis, T fAngle)
	{
		Matrix4T<T> ret;
		setRotation(ret, vAxis, fAngle);
		return ret;
	}

	template <class T>
	void Transform3T<T>::rotate(Matrix4T<T>& mat, const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> q;
		q.setRotation(vAxis, fAngle);
		rotate(mat, q);
	}

	template <class T>
	void Transform3T<T>::setPerspectiveProjection(Matrix4T<T>& _out, T sx, T sy, T zNear, T zFar)
	{
		_out.m00 = sx; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = sy; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = zFar / (zFar - zNear); _out.m23 = 1;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = -zNear * zFar / (zFar - zNear); _out.m33 = 0;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getPerspectiveProjectionMatrix(T sx, T sy, T zNear, T zFar)
	{
		return {sx, 0, 0, 0,
				0, sy, 0, 0,
				0, 0, zFar / (zFar - zNear), 1,
				0, 0, -zNear * zFar / (zFar - zNear), 0};
	}

	template <class T>
	void Transform3T<T>::setPerspectiveProjectionFovY(Matrix4T<T>& _out, T fovY, T fAspectWH, T zNear, T zFar)
	{
		T sy = Math::cot(fovY / 2);
		T sx = sy / fAspectWH;
		setPerspectiveProjection(_out, sx, sy, zNear, zFar);
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getPerspectiveProjectionFovYMatrix(T fovY, T fAspectWH, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setPerspectiveProjectionFovY(ret, fovY, fAspectWH, zNear, zFar);
		return ret;
	}

	template <class T>
	void Transform3T<T>::setOrthogonalProjection(Matrix4T<T>& _out, T sx, T sy, T zNear, T zFar)
	{
		_out.m00 = sx; _out.m01 = 0; _out.m02 = 0; _out.m03 = 0;
		_out.m10 = 0; _out.m11 = sy; _out.m12 = 0; _out.m13 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1 / (zFar - zNear); _out.m23 = 0;
		_out.m30 = 0; _out.m31 = 0; _out.m32 = zNear / (zFar - zNear); _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getOrthogonalProjectionMatrix(T sx, T sy, T zNear, T zFar)
	{
		return {sx, 0, 0, 0,
				0, sy, 0, 0,
				0, 0, 1 / (zFar - zNear), 0,
				0, 0, zNear / (zFar - zNear), 1};
	}

	template <class T>
	void Transform3T<T>::lookAt(Matrix4T<T>& _out, const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up)
	{
		Vector3T<T> xAxis, yAxis, zAxis;
		zAxis = (at - eye);
		zAxis.normalize();
		if (Math::isLessThanEpsilon(zAxis.getLength2p())) {
			zAxis = {0, 0, 1};
		}
		xAxis = up.cross(zAxis);
		xAxis.normalize();
		if (Math::isLessThanEpsilon(xAxis.getLength2p())) {
			xAxis = {1, 0, 0};
		}
		yAxis = zAxis.cross(xAxis);
		if (Math::isLessThanEpsilon(yAxis.getLength2p())) {
			yAxis = {0, 1, 0};
		}
		_out.m00 = xAxis.x; _out.m01 = yAxis.x; _out.m02 = zAxis.x; _out.m03 = 0;
		_out.m10 = xAxis.y; _out.m11 = yAxis.y; _out.m12 = zAxis.y; _out.m13 = 0;
		_out.m20 = xAxis.z; _out.m21 = yAxis.z; _out.m22 = zAxis.z; _out.m23 = 0;
		_out.m30 = -xAxis.dot(eye); _out.m31 = -yAxis.dot(eye); _out.m32 = -zAxis.dot(eye); _out.m33 = 1;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getLookAtMatrix(const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up)
	{
		Matrix4T<T> ret;
		lookAt(ret, eye, at, up);
		return ret;
	}

	template <class T>
	void Transform3T<T>::makeTransform(Matrix4T<T>& _out, const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation)
	{
		setRotation(_out, rotation);
		_out.m00 *= scaling.x; _out.m01 *= scaling.x; _out.m02 *= scaling.x;
		_out.m10 *= scaling.y; _out.m11 *= scaling.y; _out.m12 *= scaling.y;
		_out.m20 *= scaling.z; _out.m21 *= scaling.z; _out.m22 *= scaling.z;
		_out.m30 = position.x; _out.m31 = position.y; _out.m32 *= position.z;
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getTransformMatrix(const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation)
	{
		Matrix4T<T> ret;
		makeTransform(ret, position, scaling, rotation);
		return ret;
	}

	template <class T>
	void Transform3T<T>::getRotationFromDirToDir(Vector3T<T>& outAxis, T& outAngle, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Vector3T<T> dirBefore = from;
		dirBefore.normalize();
		Vector3T<T> dirNext = to;
		dirNext.normalize();
		outAxis = dirBefore.cross(dirNext);
		outAngle = -Math::arccos(dirBefore.dot(dirNext));
	}

	template <class T>
	void Transform3T<T>::setQuaternionFromDirToDir(QuaternionT<T>& _out, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Vector3T<T> dirAxisRotation;
		T angleRotation;
		getRotationFromDirToDir(dirAxisRotation, angleRotation, from, to);
		_out.setRotation(dirAxisRotation, angleRotation);
	}

	template <class T>
	QuaternionT<T> Transform3T<T>::getQuaternionRotationFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to)
	{
		QuaternionT<T> ret;
		setQuaternionFromDirToDir(ret, from, to);
		return ret;
	}

	template <class T>
	void Transform3T<T>::setTransformFromDirToDir(Matrix4T<T>& _out, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		QuaternionT<T> q;
		setQuaternionFromDirToDir(q, from, to);
		setRotation(_out, q);
	}

	template <class T>
	Matrix4T<T> Transform3T<T>::getTransformMatrixFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Matrix4T<T> ret;
		setTransformFromDirToDir(ret, from, to);
		return ret;
	}

	template <class T>
	Vector3T<T> Transform3T<T>::getTransformedOrigin(const Matrix4T<T>& transform)
	{
		return {transform.m30, transform.m31, transform.m32};
	}

	template <class T>
	Vector3T<T> Transform3T<T>::getTransformedAxisX(const Matrix4T<T>& transform)
	{
		return {transform.m00, transform.m01, transform.m02};
	}

	template <class T>
	Vector3T<T> Transform3T<T>::getTransformedAxisY(const Matrix4T<T>& transform)
	{
		return {transform.m10, transform.m11, transform.m12};
	}

	template <class T>
	Vector3T<T> Transform3T<T>::getTransformedAxisZ(const Matrix4T<T>& transform)
	{
		return {transform.m20, transform.m21, transform.m22};
	}

	template <class T>
	Vector3T<T> Transform3T<T>::projectToViewport(const Matrix4T<T>& matViewProjection, const Vector3T<T>& point)
	{
		Vector4T<T> v = Vector4T<T>(point, 1) * matViewProjection;
		if (v.w >= 0 && Math::isLessThanEpsilon(v.w)) {
			v.w = Math::Epsilon<T>();
		}
		if (v.w <= 0 && Math::isLessThanEpsilon(-v.w)) {
			v.w = - Math::Epsilon<T>();
		}
		return {v.x / v.w, v.y / v.w, v.z / v.w};
	}

	template <class T>
	Line3T<T> Transform3T<T>::unprojectViewportPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt)
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
		return {vTest1.xyz(), vTest2.xyz()};
	}

	template <class T>
	Line3T<T> Transform3T<T>::unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, T viewportWidth, T viewportHeight)
	{
		return unprojectViewportPoint(matProjection, Vector2T<T>((pt.x / viewportWidth * 2) - 1, 1 - (pt.y / viewportHeight * 2)));
	}

	template <class T>
	Line3T<T> Transform3T<T>::unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, const RectangleT<T>& viewport)
	{
		return unprojectScreenPoint(matProjection, Vector2T<T>(pt.x - viewport.left, pt.y - viewport.top), viewport.getWidth(), viewport.getHeight());
	}

	template <class T>
	Vector2T<T> Transform3T<T>::convertViewportToScreen(const Vector2T<T>& ptViewport, const RectangleT<T>& viewport)
	{
		return {
			((viewport.left + viewport.right) + ptViewport.x * (viewport.right - viewport.left)) / 2,
			((viewport.top + viewport.bottom) - ptViewport.y * (viewport.bottom - viewport.top)) / 2};
	}

	template <class T>
	Vector2T<T> Transform3T<T>::convertViewportToScreen(const Vector2T<T>& ptViewport, T viewportWidth, T viewportHeight)
	{
		return {
			(1 + ptViewport.x) * viewportWidth / 2,
			(1 - ptViewport.y) * viewportHeight / 2};
	}

	template <class T>
	Vector2T<T> Transform3T<T>::convertScreenToViewport(const Vector2T<T>& ptScreen, const RectangleT<T>& viewport)
	{
		return {
			(ptScreen.x - viewport.left) * 2 / (viewport.right - viewport.left) - 1,
			1 - (ptScreen.y - viewport.top) * 2 / (viewport.bottom - viewport.top)};
	}

	template <class T>
	Vector2T<T> Transform3T<T>::convertScreenToViewport(const Vector2T<T>& ptScreen, T viewportWidth, T viewportHeight)
	{
		return {
			ptScreen.x * 2 / viewportWidth - 1,
			1 - ptScreen.y * 2 / viewportHeight};
	}

	template <class T>
	RectangleT<T> Transform3T<T>::convertViewportToScreen(const RectangleT<T>& rcInViewport, const RectangleT<T>& viewport)
	{
		return {
			((viewport.left + viewport.right) + rcInViewport.left * (viewport.right - viewport.left)) / 2,
			((viewport.top + viewport.bottom) - rcInViewport.bottom * (viewport.bottom - viewport.top)) / 2,
			((viewport.left + viewport.right) + rcInViewport.right * (viewport.right - viewport.left)) / 2,
			((viewport.top + viewport.bottom) - rcInViewport.top * (viewport.bottom - viewport.top)) / 2};
	}

	template <class T>
	RectangleT<T> Transform3T<T>::convertViewportToScreen(const RectangleT<T>& rcInViewport, T viewportWidth, T viewportHeight)
	{
		return {
			(1 + rcInViewport.left) * viewportWidth / 2,
			(1 - rcInViewport.bottom) * viewportHeight / 2,
			(1 + rcInViewport.right) * viewportWidth / 2,
			(1 - rcInViewport.top) * viewportHeight / 2};
	}

	template <class T>
	RectangleT<T> Transform3T<T>::convertScreenToViewport(const RectangleT<T>& rcInScreen, const RectangleT<T>& viewport)
	{
		return {
			(rcInScreen.left - viewport.left) * 2 / (viewport.right - viewport.left) - 1,
			1 - (rcInScreen.bottom - viewport.top) * 2 / (viewport.bottom - viewport.top),
			(rcInScreen.right - viewport.left) * 2 / (viewport.right - viewport.left) - 1,
			1 - (rcInScreen.top - viewport.top) * 2 / (viewport.bottom - viewport.top)};
	}

	template <class T>
	RectangleT<T> Transform3T<T>::convertScreenToViewport(const RectangleT<T>& rcInScreen, T viewportWidth, T viewportHeight)
	{
		return {
			rcInScreen.left * 2 / viewportWidth - 1,
			1 - rcInScreen.bottom * 2 / viewportHeight,
			rcInScreen.right * 2 / viewportWidth - 1,
			1 - rcInScreen.top * 2 / viewportHeight};
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Transform3)

}
