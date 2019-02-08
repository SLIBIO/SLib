/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_MATH_VIEW_FRUSTUM
#define CHECKHEADER_SLIB_MATH_VIEW_FRUSTUM

#include "definition.h"

#include "vector3.h"
#include "plane.h"
#include "sphere.h"
#include "box.h"
#include "matrix4.h"

/*
	ViewFrustum defined on View Coordinate
 */

namespace slib
{

	template <class T>
	class SLIB_EXPORT ViewFrustumT
	{
	public:
		Matrix4T<T> MVP;

	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(ViewFrustumT)
		
		ViewFrustumT() noexcept = default;

		template <class O>
		ViewFrustumT(const ViewFrustumT<O>& other) noexcept;

	public:
		static const ViewFrustumT<T>& fromMVP(const Matrix4T<T>& MVP) noexcept;

		void getPlanes(PlaneT<T>& near, PlaneT<T>& far, PlaneT<T>& left, PlaneT<T>& right, PlaneT<T>& top, PlaneT<T>& bottom) const noexcept;

		void getPlanes(PlaneT<T>* _out) const noexcept;

		sl_bool containsPoint(const Vector3T<T>& pt, sl_bool flagSkipNearFar = sl_true) const noexcept;

		sl_bool containsFacets(const Vector3T<T>* pts, sl_uint32 n, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const noexcept;

		sl_bool containsSphere(const SphereT<T>& sphere, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const noexcept;

		sl_bool containsBox(const BoxT<T>& box, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const noexcept;

	public:
		template <class O>
		ViewFrustumT<T>& operator=(const ViewFrustumT<O>& other) noexcept;

	};

	extern template class ViewFrustumT<float>;
	extern template class ViewFrustumT<double>;
	typedef ViewFrustumT<sl_real> ViewFrustum;
	typedef ViewFrustumT<float> ViewFrustumf;
	typedef ViewFrustumT<double> ViewFrustumlf;

}

#include "detail/view_frustum.inc"

#endif
