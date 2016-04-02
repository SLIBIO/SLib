#ifndef CHECKHEADER_SLIB_MATH_INT128
#define CHECKHEADER_SLIB_MATH_INT128

#include "definition.h"

#include "../core/algorithm.h"
#include "../core/string.h"

SLIB_MATH_NAMESPACE_BEGIN

class SLIB_EXPORT Uint128
{
public:
	sl_uint64 high;
	sl_uint64 low;

public:
	Uint128() = default;

	Uint128(const Uint128& num) = default;;

	Uint128(sl_uint64 num);

private:
	static const sl_uint64 _zero[2];

public:
	static const Uint128& zero();
	
	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	void setZero();

	int compare(const Uint128& other) const;
	
	static sl_bool div(const Uint128& a, const Uint128& b, Uint128* quotient = sl_null, Uint128* remainder = sl_null);

	Uint128 div(const Uint128& other) const;
	
	Uint128 div(sl_uint64 num) const;
	
	Uint128 mod(const Uint128& other) const;
	
	sl_uint64 mod(sl_uint64 num) const;
	
	void shiftRight();
	
	void shiftLeft();
	
	void makeNegative();
	
	void makeBitwiseNot();

public:
	Uint128& operator=(const Uint128& other) = default;
	
	Uint128& operator=(sl_uint64 num);

	
	sl_bool operator==(const Uint128& other) const;
	
	sl_bool operator==(sl_uint64 num) const;
	
	friend sl_bool operator==(sl_uint64 num, const Uint128& v);
	
	sl_bool operator!=(const Uint128& other) const;
	
	sl_bool operator!=(sl_uint64 num) const;
	
	friend sl_bool operator!=(sl_uint64 num, const Uint128& v);
	
	sl_bool operator>=(const Uint128& other) const;
	
	sl_bool operator>=(sl_uint64 num) const;
	
	friend sl_bool operator>=(sl_uint64 num, const Uint128& v);

	sl_bool operator<=(const Uint128& other) const;
	
	sl_bool operator<=(sl_uint64 num) const;
	
	friend sl_bool operator<=(sl_uint64 num, const Uint128& v);
	
	sl_bool operator>(const Uint128& other) const;
	
	sl_bool operator>(sl_uint64 num) const;
	
	friend sl_bool operator>(sl_uint64 num, const Uint128& v);
	
	sl_bool operator<(const Uint128& other) const;
	
	sl_bool operator<(sl_uint64 num) const;
	
	friend sl_bool operator<(sl_uint64 num, const Uint128& v);

	
	Uint128 operator+(const Uint128& other) const;
	
	Uint128 operator+(sl_uint64 num) const;
	
	friend Uint128 operator+(sl_uint64 num, const Uint128& v);
	
	Uint128& operator+=(const Uint128& other);
	
	Uint128& operator+=(sl_uint32 num);

	Uint128& operator++();
	
	Uint128 operator++(int);
	
	
	Uint128 operator-(const Uint128& other) const;
	
	Uint128 operator-(sl_uint64 num) const;
	
	friend Uint128 operator-(sl_uint64 num, const Uint128& v);

	Uint128& operator-=(const Uint128& other);
	
	Uint128& operator-=(sl_uint64 num);
	
	Uint128& operator--();
	
	Uint128 operator--(int);
	
	
	Uint128 operator*(const Uint128& other) const;
	
	Uint128 operator*(sl_uint64 num) const;
	
	friend Uint128 operator*(sl_uint64 num, const Uint128& v);
	
	Uint128& operator*=(const Uint128& other);
	
	Uint128& operator*=(sl_uint64 num);
	
	
	Uint128 operator/(const Uint128& other) const;
	
	Uint128 operator/(sl_uint64 num) const;
	
	friend Uint128 operator/(sl_uint64 num, const Uint128& v);

	Uint128& operator/=(const Uint128& other);
	
	Uint128& operator/=(sl_uint64 num);

	
	Uint128 operator%(const Uint128& other) const;
	
	Uint128 operator%(sl_uint64 num) const;
	
	friend Uint128 operator%(sl_uint64 num, const Uint128& v);

	Uint128& operator%=(const Uint128& other);
	
	Uint128& operator%=(sl_uint64 num);

	
	Uint128 operator&(const Uint128& other) const;
	
	Uint128 operator&(sl_uint64 num) const;
	
	friend Uint128 operator&(sl_uint64 num, const Uint128& v);
	
	Uint128& operator&=(const Uint128& other);
	
	Uint128& operator&=(sl_uint32 num);

	
	Uint128 operator|(const Uint128& other) const;
	
	Uint128 operator|(sl_uint64 num) const;
	
	friend Uint128 operator|(sl_uint64 num, const Uint128& v);
	
	Uint128& operator|=(const Uint128& other);
	
	Uint128& operator|=(sl_uint32 num);

	
	Uint128 operator^(const Uint128& other) const;
	
	Uint128 operator^(sl_uint64 num) const;
	
	friend Uint128 operator^(sl_uint64 num, const Uint128& v);

	Uint128& operator^=(const Uint128& other);
	
	Uint128& operator^=(sl_uint32 num);

	
	Uint128 operator>>(sl_uint32 n) const;
	
	Uint128& operator>>=(sl_uint32 n);

	
	Uint128 operator<<(sl_uint32 n) const;
	
	Uint128& operator<<=(sl_uint32 n);

	
	Uint128 operator-() const;

	Uint128 operator~() const;

public:
	sl_uint32 getMostSignificantBits() const;
	
	sl_uint32 getLeastSignificantBits() const;
	
	// 16 bytes
	void getBytesBE(void* buf);
	
	// 16 bytes
	void setBytesBE(const void* buf);
	
	// 16 bytes
	void getBytesLE(void* buf);

	// 16 bytes
	void setBytesLE(const void* buf);
	
	static sl_reg parseString(Uint128* out, const char* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX, sl_uint32 radix = 10);
	
	static sl_reg parseString(Uint128* out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX, sl_uint32 radix = 10);
	
	sl_bool parseString(const String& str, sl_uint32 radix = 10);
	
	static Uint128 fromString(const String& str, sl_uint32 radix = 10);
	
	String toString(sl_uint32 radix = 10) const;

	static Uint128 fromHexString(const String& str);
	
	String toHexString() const;

};

template<>
int Compare<Uint128>::compare(const Uint128& a, const Uint128& b);

template<>
sl_bool Compare<Uint128>::equals(const Uint128& a, const Uint128& b);

template<>
sl_uint32 Hash<Uint128>::hash(const Uint128& v);

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

SLIB_INLINE Uint128::Uint128(sl_uint64 num) : high(0), low(num)
{
}

SLIB_INLINE const Uint128& Uint128::zero()
{
	return *((Uint128*)((void*)_zero));
}

SLIB_INLINE sl_bool Uint128::isZero() const
{
	return high == 0 && low == 0;
}

SLIB_INLINE sl_bool Uint128::isNotZero() const
{
	return high != 0 || low != 0;
}

SLIB_INLINE void Uint128::setZero()
{
	high = 0;
	low = 0;
}

SLIB_INLINE Uint128& Uint128::operator=(sl_uint64 num)
{
	high = 0;
	low = num;
	return *this;
}

SLIB_MATH_NAMESPACE_END

#endif

