/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/math/box.h"

namespace slib
{

	template <class T>
	SLIB_ALIGN(8) T BoxT<T>::_zero[6] = {0, 0, 0, 0, 0, 0};

	template <class T>
	BoxT<T>::BoxT(T _x1, T _y1, T _z1,
				  T _x2, T _y2, T _z2)
	: x1(_x1), y1(_y1), z1(_z1),
	  x2(_x2), y2(_y2), z2(_z2)
	{
	}

	template <class T>
	Vector3T<T> BoxT<T>::getStart() const
	{
		return {x1, y1, z1};
	}

	template <class T>
	void BoxT<T>::setStart(T x, T y, T z)
	{
		x1 = x;
		y1 = y;
		z1 = z;
	}

	template <class T>
	void BoxT<T>::setStart(const Vector3T<T>& v)
	{
		x1 = v.x;
		y1 = v.y;
		z1 = v.z;
	}

	template <class T>
	Vector3T<T> BoxT<T>::getEnd() const
	{
		return {x2, y2, z2};
	}

	template <class T>
	void BoxT<T>::setEnd(T x, T y, T z)
	{
		x2 = x;
		y2 = y;
		z2 = z;
	}

	template <class T>
	void BoxT<T>::setEnd(const Vector3T<T>& v)
	{
		x2 = v.x;
		y2 = v.y;
		z2 = v.z;
	}

	template <class T>
	Vector3T<T> BoxT<T>::getSize() const
	{
		return {x2 - x1, y2 - y1, z2 - z1};
	}

	template <class T>
	Vector3T<T> BoxT<T>::getCenter() const
	{
		return {(x2 + x1) / 2, (y2 + y1) / 2, (z2 + z1) / 2};
	}

	template <class T>
	void BoxT<T>::setZero()
	{
		x1 = 0; y1 = 0; z1 = 0;
		x2 = 0; y2 = 0; z2 = 0;
	}

	template <class T>
	sl_bool BoxT<T>::containsPoint(T x, T y, T z) const
	{
		return
			x >= x1 && x <= x2 &&
			y >= y1 && y <= y2 &&
			z >= z1 && z <= z2;
	}

	template <class T>
	sl_bool BoxT<T>::containsPoint(const Vector3T<T>& pt) const
	{
		return
			pt.x >= x1 && pt.x <= x2 &&
			pt.y >= y1 && pt.y <= y2 &&
			pt.z >= z1 && pt.z <= z2;
	}

	template <class T>
	sl_bool BoxT<T>::containsBox(const BoxT<T>& other) const
	{
		return
			x1 <= other.x2 && x2 >= other.x1 &&
			y1 <= other.y2 && y2 >= other.y1 &&
			z1 <= other.z2 && z2 >= other.z1;
	}

	template <class T>
	void BoxT<T>::setFromPoint(T x, T y, T z)
	{
		x1 = x2 = x;
		y1 = y2 = y;
		z1 = z2 = z;
	}

	template <class T>
	void BoxT<T>::setFromPoint(const Vector3T<T>& pt)
	{
		setFromPoint(pt.x, pt.y, pt.z);
	}

	template <class T>
	void BoxT<T>::mergePoint(T x, T y, T z)
	{
		if (x1 > x) {
			x1 = x;
		}
		if (x2 < x) {
			x2 = x;
		}
		if (y1 > y) {
			y1 = y;
		}
		if (y2 < y) {
			y2 = y;
		}
		if (z1 > z) {
			z1 = z;
		}
		if (z2 < z) {
			z2 = z;
		}
	}

	template <class T>
	void BoxT<T>::mergePoint(const Vector3T<T>& pt)
	{
		mergePoint(pt.x, pt.y, pt.z);
	}

	template <class T>
	void BoxT<T>::mergePoints(const Vector3T<T>* points, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			const Vector3T<T>& v = points[i];
			if (x1 > v.x) {
				x1 = v.x;
			}
			if (x2 < v.x) {
				x2 = v.x;
			}
			if (y1 > v.y) {
				y1 = v.y;
			}
			if (y2 < v.y) {
				y2 = v.y;
			}
			if (z1 > v.z) {
				z1 = v.z;
			}
			if (z2 < v.z) {
				z2 = v.z;
			}
		}
	}

	template <class T>
	void BoxT<T>::mergePoints(const List< Vector3T<T> >& points)
	{
		ListLocker< Vector3T<T> > list(points);
		mergePoints(list.data, list.count);
	}

	template <class T>
	void BoxT<T>::setFromPoints(const Vector3T<T>* points, sl_size count)
	{
		if (count > 0) {
			setFromPoint(points[0]);
			if (count > 1) {
				mergePoints(points + 1, count - 1);
			}
		}
	}

	template <class T>
	void BoxT<T>::setFromPoints(const List< Vector3T<T> >& points)
	{
		ListLocker< Vector3T<T> > list(points);
		setFromPoints(list.data, list.count);
	}

	template <class T>
	void BoxT<T>::setFromPoints(const Vector3T<T>& pt1, const Vector3T<T>& pt2)
	{
		setFromPoint(pt1);
		mergePoint(pt2);
	}

	template <class T>
	void BoxT<T>::mergeBox(const BoxT<T>& other)
	{
		if (x1 > other.x1) {
			x1 = other.x1;
		}
		if (x2 < other.x2) {
			x2 = other.x2;
		}
		if (y1 > other.y1) {
			y1 = other.y1;
		}
		if (y2 < other.y2) {
			y2 = other.y2;
		}
		if (z1 > other.z1) {
			z1 = other.z1;
		}
		if (z2 < other.z2) {
			z2 = other.z2;
		}
	}

	template <class T>
	void BoxT<T>::getCornerPoints(Vector3T<T>* _out) const
	{
		_out[0].x = x1; _out[0].y = y1; _out[0].z = z1;
		_out[1].x = x2; _out[1].y = y1; _out[1].z = z1;
		_out[2].x = x1; _out[2].y = y2; _out[2].z = z1;
		_out[3].x = x2; _out[3].y = y2; _out[3].z = z1;
		_out[4].x = x1; _out[4].y = y1; _out[4].z = z2;
		_out[5].x = x2; _out[5].y = y1; _out[5].z = z2;
		_out[6].x = x1; _out[6].y = y2; _out[6].z = z2;
		_out[7].x = x2; _out[7].y = y2; _out[7].z = z2;
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Box)

}
