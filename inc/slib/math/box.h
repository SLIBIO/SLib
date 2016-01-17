#ifndef CHECKHEADER_SLIB_MATH_BOX
#define CHECKHEADER_SLIB_MATH_BOX

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

#include "../core/list.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT BoxT
{
public:
	T x1, y1, z1;
	T x2, y2, z2;
	
public:
	SLIB_INLINE BoxT()
	{
	}
	
	template <class O>
	SLIB_INLINE BoxT(const BoxT<O>& other)
	{
		x1 = (T)(other.x1); x2 = (T)(other.x2);
		y1 = (T)(other.y1); y2 = (T)(other.y2);
		z1 = (T)(other.z1); z2 = (T)(other.z2);
	}
	
	SLIB_INLINE BoxT(T _x1, T _y1, T _z1
					 , T _x2, T _y2, T _z2)
	{
		x1 = _x1; y1 = _y1; z1 = _z1;
		x2 = _x2; y2 = _y2; z2 = _z2;
	}
	
public:
	template <class O>
	SLIB_INLINE BoxT<T>& operator=(const BoxT<O>& other)
	{
		x1 = (T)(other.x1); x2 = (T)(other.x2);
		y1 = (T)(other.y1); y2 = (T)(other.y2);
		z1 = (T)(other.z1); z2 = (T)(other.z2);
		return *this;
	}
	
private:
	static T _zero[6];
	
public:
	SLIB_INLINE static const BoxT<T>& zero()
	{
		return *((BoxT*)((void*)_zero));
	}
	
	SLIB_INLINE Vector3T<T> getStart() const
	{
		return Vector3T<T>(x1, y1, z1);
	}
	
	SLIB_INLINE void setStart(T x, T y, T z)
	{
		x1 = x;
		y1 = y;
		z1 = z;
	}
	
	SLIB_INLINE void setStart(const Vector3T<T>& v)
	{
		setStart(v.x, v.y, v.z);
	}
	
	SLIB_INLINE Vector3T<T> getEnd() const
	{
		return Vector3T<T>(x2, y2, z2);
	}
	
	SLIB_INLINE void setEnd(T x, T y, T z)
	{
		x2 = x;
		y2 = y;
		z2 = z;
	}
	
	SLIB_INLINE void setEnd(const Vector3T<T>& v)
	{
		setEnd(v.x, v.y, v.z);
	}
	
	SLIB_INLINE Vector3T<T> getSize() const
	{
		return Vector3T<T>(x2 - x1, y2 - y1, z2 - z1);
	}
	
	SLIB_INLINE Vector3T<T> size() const
	{
		return getSize();
	}
	
	SLIB_INLINE Vector3T<T> getCenter() const
	{
		return Vector3T<T>((x2 + x1) / 2, (y2 + y1) / 2, (z2 + z1) / 2);
	}
	
	SLIB_INLINE Vector3T<T> center() const
	{
		return getCenter();
	}
	
	SLIB_INLINE void setZero()
	{
		x1 = 0; y1 = 0; z1 = 0;
		x2 = 0; y2 = 0; z2 = 0;
	}
	
	SLIB_INLINE sl_bool containsPoint(T x, T y, T z) const
	{
		return x >= x1 && x <= x2
		&& y >= y1 && y <= y2
		&& z >= z1 && z <= z2;
	}
	
	SLIB_INLINE sl_bool containsPoint(const Vector3T<T>& pt) const
	{
		return pt.x >= x1 && pt.x <= x2
		&& pt.y >= y1 && pt.y <= y2
		&& pt.z >= z1 && pt.z <= z2;
	}
	
	SLIB_INLINE sl_bool containsBox(const BoxT<T>& other) const
	{
		return x1 <= other.x2 && x2 >= other.x1
		&& y1 <= other.y2 && y2 >= other.y1
		&& z1 <= other.z2 && z2 >= other.z1;
	}
	
	SLIB_INLINE void setFromPoint(T x, T y, T z)
	{
		x1 = x2 = x;
		y1 = y2 = y;
		z1 = z2 = z;
	}
	
	SLIB_INLINE void setFromPoint(const Vector3T<T>& pt)
	{
		setFromPoint(pt.x, pt.y, pt.z);
	}
	
	void mergePoint(T x, T y, T z)
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
	
	SLIB_INLINE void mergePoint(const Vector3T<T>& pt)
	{
		mergePoint(pt.x, pt.y, pt.z);
	}
	
	void mergePoints(const Vector3T<T>* points, sl_size count)
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
	
	void mergePoints(const List< Vector3T<T> >& points)
	{
		ListLocker< Vector3T<T> > list(points);
		mergePoints(list.data(), list.getCount());
	}
	
	void setFromPoints(const Vector3T<T>* points, sl_size count)
	{
		if (count > 0) {
			setFromPoint(points[0]);
			if (count > 1) {
				mergePoints(points + 1, count - 1);
			}
		}
	}
	
	void setFromPoints(const List< Vector3T<T> >& points)
	{
		ListLocker< Vector3T<T> > list(points);
		setFromPoints(list.data(), list.getCount());
	}
	
	SLIB_INLINE void setFromPoints(const Vector3T<T>& pt1, const Vector3T<T>& pt2)
	{
		setFromPoint(pt1);
		mergePoint(pt2);
	}
	
	SLIB_INLINE void mergeBox(const BoxT<T>& other)
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
	
	// 8 points
	void getCornerPoints(Vector3T<T>* _out) const
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
};

template <class T>
T BoxT<T>::_zero[6] = {0, 0, 0, 0, 0, 0};

typedef BoxT<sl_real> Box;
typedef BoxT<float> Boxf;
typedef BoxT<double> Boxlf;

SLIB_MATH_NAMESPACE_END

#endif
