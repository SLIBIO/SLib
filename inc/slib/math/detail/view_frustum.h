/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
