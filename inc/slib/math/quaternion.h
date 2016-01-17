#ifndef CHECKHEADER_SLIB_MATH_QUATERNION
#define CHECKHEADER_SLIB_MATH_QUATERNION

#include "definition.h"

#include "vector4.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT QuaternionT
{
public:
	T x;
	T y;
	T z;
	T w;
	
public:
	SLIB_INLINE QuaternionT()
	{
	}
	
	SLIB_INLINE QuaternionT(T x, T y, T z, T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	
	template <class O>
	SLIB_INLINE QuaternionT(const QuaternionT<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
	}
	
	SLIB_INLINE QuaternionT(const Vector4T<T>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}
	
public:
	SLIB_INLINE const Vector4T<T>& toVector4() const
	{
		return *((Vector4T<T>*)this);
	}
	
	SLIB_INLINE Vector4T<T>& toVector4()
	{
		return *((Vector4T<T>*)this);
	}
	
private:
	SLIB_EXPORT static T _identity[4];
	
public:
	SLIB_INLINE static const QuaternionT<T>& identity()
	{
		return *((QuaternionT<T>*)((void*)_identity));
	}
	
public:
	SLIB_INLINE sl_bool operator==(const QuaternionT<T>& other) const
	{
		return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
	}
	
	SLIB_INLINE sl_bool operator!=(const QuaternionT<T>& other) const
	{
		return !(*this == other);
	}
	
	template <class O>
	SLIB_INLINE QuaternionT<T>& operator=(const QuaternionT<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
		return *this;
	}
	
	SLIB_INLINE QuaternionT<T>& operator=(const Vector4T<T>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
	
	SLIB_INLINE QuaternionT<T>& operator*=(const QuaternionT<T>& other)
	{
		multiply(other);
		return *this;
	}
	
	SLIB_INLINE QuaternionT<T> operator*(const QuaternionT<T>& other) const
	{
		QuaternionT<T> ret = *this;
		ret.multiply(other);
		return ret;
	}
	
	SLIB_INLINE QuaternionT<T>& operator/=(const QuaternionT<T>& other)
	{
		divide(other);
		return *this;
	}
	
	SLIB_INLINE QuaternionT<T> operator/(QuaternionT<T>& other) const
	{
		QuaternionT<T> ret = *this;
		ret.divide(other);
		return ret;
	}
	
public:
	SLIB_INLINE T getLength2p() const
	{
		return x * x + y * y + z * z + w * w;
	}
	
	SLIB_INLINE T length2p() const
	{
		return getLength2p();
	}
	
	SLIB_INLINE T getLength() const
	{
		return Math::sqrt(getLength2p());
	}
	
	SLIB_INLINE T length() const
	{
		return getLength();
	}
	
	void multiply(const QuaternionT<T>& other)
	{
		T ox = other.x, oy = other.y, oz = other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}
	
	void divide(const QuaternionT<T>& other)
	{
		T ox = -other.x, oy = -other.y, oz = -other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}
	
	void setRotation(const Vector3T<T>& vAxis, T fAngle)
	{
		T f = Math::sin(fAngle / 2) / vAxis.getLength();
		x = vAxis.x * f;
		y = vAxis.y * f;
		z = vAxis.z * f;
		w = Math::cos(fAngle / 2);
	}
	
	SLIB_INLINE static QuaternionT<T> getRotation(const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> ret;
		ret.setRotation(vAxis, fAngle);
		return ret;
	}
	
public:
	SLIB_INLINE T getAngle() const
	{
		return 2 * Math::arccos(w);
	}
	
	SLIB_INLINE Vector4T<T> getAxis() const
	{
		return Vector4T<T>(x, y, z, 0);
	}
	
	SLIB_INLINE void makeInverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	
	SLIB_INLINE QuaternionT<T> inverse() const
	{
		QuaternionT<T> ret = *this;
		ret.makeInverse();
		return ret;
	}
};

template <class T>
T QuaternionT<T>::_identity[4] = { 0, 0, 0, 1 };

typedef QuaternionT<sl_real> Quaternion;
typedef QuaternionT<float> Quaternionf;
typedef QuaternionT<double> Quaternionlf;

SLIB_MATH_NAMESPACE_END

#endif
