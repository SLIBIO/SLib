#ifndef CHECKHEADER_SLIB_MATH_DETAIL_VIEW_FRUSTUM
#define CHECKHEADER_SLIB_MATH_DETAIL_VIEW_FRUSTUM

#include "../view_frustum.h"

namespace slib
{

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

}

#endif
