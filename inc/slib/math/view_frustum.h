#ifndef CHECKHEADER_SLIB_MATH_VIEW_FRUSTUM
#define CHECKHEADER_SLIB_MATH_VIEW_FRUSTUM

#include "definition.h"

#include "vector3.h"
#include "plane.h"
#include "sphere.h"
#include "box.h"
#include "matrix4.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	ViewFrustum defined on View Coordinate
 */
template <class T>
class SLIB_EXPORT ViewFrustumT
{
public:
	Matrix4T<T> MVP;
	
public:
	ViewFrustumT() = default;
	
	ViewFrustumT(const ViewFrustumT<T>& other) = default;
	
	template <class O>
	ViewFrustumT(const ViewFrustumT<O>& other);
	
public:
	static const ViewFrustumT<T>& fromMVP(const Matrix4T<T>& MVP);
	
	void getPlanes(PlaneT<T>& near, PlaneT<T>& far, PlaneT<T>& left, PlaneT<T>& right, PlaneT<T>& top, PlaneT<T>& bottom) const;
	
	void getPlanes(PlaneT<T>* _out) const;
	
	sl_bool containsPoint(const Vector3T<T>& pt, sl_bool flagSkipNearFar = sl_true) const;
	
	sl_bool containsFacets(const Vector3T<T>* pts, sl_uint32 n, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const;
	
	sl_bool containsSphere(const SphereT<T>& sphere, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const;
	
	sl_bool containsBox(const BoxT<T>& box, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const;
	
public:
	ViewFrustumT<T>& operator=(const ViewFrustumT<T>& other) = default;
	
	template <class O>
	ViewFrustumT<T>& operator=(const ViewFrustumT<O>& other);

};

extern template class ViewFrustumT<float>;
extern template class ViewFrustumT<double>;
typedef ViewFrustumT<sl_real> ViewFrustum;
typedef ViewFrustumT<float> ViewFrustumf;
typedef ViewFrustumT<double> ViewFrustumlf;

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
ViewFrustumT<T>::ViewFrustumT(const ViewFrustumT<O>& other) : MVP(other.MVP)
{
}

template <class T>
SLIB_INLINE const ViewFrustumT<T>& ViewFrustumT<T>::fromMVP(const Matrix4T<T>& MVP)
{
	return *((ViewFrustumT<T>*)(void*)&MVP);
}

template <class T>
template <class O>
ViewFrustumT<T>& ViewFrustumT<T>::operator=(const ViewFrustumT<O>& other)
{
	MVP = other.MVP;
	return *this;
}

SLIB_MATH_NAMESPACE_END

#endif
