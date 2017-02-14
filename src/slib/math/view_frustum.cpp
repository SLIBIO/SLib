#include "../../../inc/slib/math/view_frustum.h"

namespace slib
{

	template <class T>
	void ViewFrustumT<T>::getPlanes(PlaneT<T>& near, PlaneT<T>& far, PlaneT<T>& left, PlaneT<T>& right, PlaneT<T>& top, PlaneT<T>& bottom) const
	{
		const Matrix4T<T>& m = MVP;
		left.a = m.m03 + m.m00;
		left.b = m.m13 + m.m10;
		left.c = m.m23 + m.m20;
		left.d = m.m33 + m.m30;
		right.a = m.m03 - m.m00;
		right.b = m.m13 - m.m10;
		right.c = m.m23 - m.m20;
		right.d = m.m33 - m.m30;
		bottom.a = m.m03 + m.m01;
		bottom.b = m.m13 + m.m11;
		bottom.c = m.m23 + m.m21;
		bottom.d = m.m33 + m.m31;
		top.a = m.m03 - m.m01;
		top.b = m.m13 - m.m11;
		top.c = m.m23 - m.m21;
		top.d = m.m33 - m.m31;
		near.a = m.m02;
		near.b = m.m12;
		near.c = m.m22;
		near.d = m.m32;
		far.a = m.m03 - m.m02;
		far.b = m.m13 - m.m12;
		far.c = m.m23 - m.m22;
		far.d = m.m33 - m.m32;
	}

	template <class T>
	void ViewFrustumT<T>::getPlanes(PlaneT<T>* _out) const
	{
		getPlanes(_out[0], _out[1], _out[2], _out[3], _out[4], _out[5]);
	}

	template <class T>
	sl_bool ViewFrustumT<T>::containsPoint(const Vector3T<T>& pt, sl_bool flagSkipNearFar) const
	{
		PlaneT<T> planes[6];
		getPlanes(planes);
		for (sl_uint32 i = flagSkipNearFar ? 2 : 0; i < 6; i++) {
			if (planes[i].getDistanceFromPointOnNormalized(pt) < 0) {
				return sl_false;
			}
		}
		return sl_true;
	}

	template <class T>
	sl_bool ViewFrustumT<T>::containsFacets(const Vector3T<T>* pts, sl_uint32 n, sl_bool* pFlagIntersect, sl_bool flagSkipNearFar) const
	{
		PlaneT<T> planes[6];
		getPlanes(planes);
		sl_bool flagIntersect = sl_false;
		for (sl_uint32 i = flagSkipNearFar ? 2 : 0; i < 6; i++) {
			sl_uint32 nIn = 0;
			for (sl_uint32 k = 0; k < n; k++) {
				if (planes[i].getDistanceFromPointOnNormalized(pts[k]) >= 0) {
					nIn++;
				}
			}
			if (nIn == 0) {
				return sl_false;
			}
			if (nIn != n) {
				flagIntersect = sl_true;
			}
		}
		if (pFlagIntersect) {
			*pFlagIntersect = flagIntersect;
		}
		return sl_true;
	}

	template <class T>
	sl_bool ViewFrustumT<T>::containsSphere(const SphereT<T>& sphere, sl_bool* pFlagIntersect, sl_bool flagSkipNearFar) const
	{
		PlaneT<T> planes[6];
		getPlanes(planes);
		sl_bool flagIntersect = sl_false;
		for (sl_uint32 i = flagSkipNearFar ? 2 : 0; i < 6; i++) {
			T d = planes[i].getDistanceFromPoint(sphere.center);
			if (d < -sphere.radius) {
				return sl_false;
			}
			if (d < sphere.radius) {
				flagIntersect = sl_true;
			}
		}
		if (pFlagIntersect) {
			*pFlagIntersect = flagIntersect;
		}
		return sl_true;
	}

	template <class T>
	sl_bool ViewFrustumT<T>::containsBox(const BoxT<T>& box, sl_bool* pFlagIntersect, sl_bool flagSkipNearFar) const
	{
		Vector3T<T> corners[8];
		box.getCornerPoints(corners);
		return containsFacets(corners, 8, pFlagIntersect, flagSkipNearFar);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(ViewFrustum)

}
