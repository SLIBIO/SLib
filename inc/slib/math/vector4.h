#ifndef CHECKHEADER_SLIB_MATH_VECTOR4
#define CHECKHEADER_SLIB_MATH_VECTOR4

#include "definition.h"

#include "vector3.h"

#include "../core/interpolation.h"

namespace slib
{
	
	template <class T, class FT = T>
	class SLIB_EXPORT Vector4T
	{
	public:
		T x;
		T y;
		T z;
		T w;

	public:
		SLIB_INLINE Vector4T() = default;

		constexpr Vector4T(const Vector4T<T, FT>& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
	
		template <class O, class FO>
		constexpr Vector4T(const Vector4T<O, FO>& other) : x((T)(other.x)), y((T)(other.y)), z((T)(other.z)), w((T)(other.w)) {}
	
		constexpr Vector4T(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	
		constexpr Vector4T(const Vector3T<T, FT>& xyz, T _w) : x(xyz.x), y(xyz.y), z(xyz.z), w(_w) {}
	
	public:
		static const Vector4T<T, FT>& zero();

		static const Vector4T<T, FT>& fromArray(const T arr[4]);
	
		static Vector4T<T, FT>& fromArray(T arr[4]);
	
	public:
		const Vector3T<T, FT>& xyz() const;

		Vector3T<T, FT>& xyz();

		static Vector4T<T, FT> fromLocation(const Vector3T<T, FT>& v);

		static Vector4T<T, FT> fromDirection(const Vector3T<T, FT>& v);

		T dot(const Vector4T<T, FT>& other) const;

		T getLength2p() const;

		FT getLength() const;

		T getLength2p(const Vector4T<T, FT>& other) const;

		FT getLength(const Vector4T<T, FT>& other) const;

		void normalize();

		Vector4T<T, FT> getNormalized();

		FT getCosBetween(const Vector4T<T, FT>& other) const;

		FT getAngleBetween(const Vector4T<T, FT>& other) const;

		sl_bool equals(const Vector4T<T, FT>& other) const;
	
		sl_bool isAlmostEqual(const Vector4T<T, FT>& other) const;

		Vector4T<T, FT> lerp(const Vector4T<T, FT>& target, float factor) const;
	
	public:
		Vector4T<T, FT>& operator=(const Vector4T<T, FT>& other) = default;

		template <class O, class FO>
		Vector4T<T, FT>& operator=(const Vector4T<O, FO>& other);

		Vector4T<T, FT> operator+(const Vector4T<T, FT>& other) const;

		Vector4T<T, FT>& operator+=(const Vector4T<T, FT>& other);

		Vector4T<T, FT> operator-(const Vector4T<T, FT>& other) const;

		Vector4T<T, FT>& operator-=(const Vector4T<T, FT>& other);

		Vector4T<T, FT> operator*(T f) const;

		Vector4T<T, FT>& operator*=(T f);

		Vector4T<T, FT> operator*(const Vector4T<T, FT>& other) const;

		Vector4T<T, FT>& operator*=(const Vector4T<T, FT>& other);

		Vector4T<T, FT> operator/(T f) const;

		Vector4T<T, FT>& operator/=(T f);

		Vector4T<T, FT> operator/(const Vector4T<T, FT>& other) const;

		Vector4T<T, FT>& operator/(const Vector4T<T, FT>& other);

		Vector4T<T, FT> operator-() const;

		sl_bool operator==(const Vector4T<T, FT>& other) const;

		sl_bool operator!=(const Vector4T<T, FT>& other) const;

	private:
		static T _zero[4];

	};

	extern template class Vector4T<float>;
	extern template class Vector4T<double>;
	extern template class Vector4T<sl_int32, float>;
	extern template class Vector4T<sl_int64, double>;
	typedef Vector4T<sl_real> Vector4;
	typedef Vector4T<float> Vector4f;
	typedef Vector4T<double> Vector4lf;
	typedef Vector4T<sl_int32, float> Vector4i;
	typedef Vector4T<sl_int64, double> Vector4li;

	template <class T, class FT>
	Vector4T<T, FT> operator*(T f, const Vector4T<T, FT>& v);

	template <class T, class FT>
	Vector4T<T, FT> operator/(T f, const Vector4T<T, FT>& v);

	template <class T, class FT>
	class Interpolation< Vector4T<T, FT> >
	{
	public:
		static Vector4T<T, FT> interpolate(const Vector4T<T, FT>& a, const Vector4T<T, FT>& b, float factor);
	};

}

#include "detail/vector4.h"

#endif
