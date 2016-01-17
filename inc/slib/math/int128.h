#ifndef CHECKHEADER_SLIB_MATH_INT128
#define CHECKHEADER_SLIB_MATH_INT128

#include "definition.h"

#include "../core/math.h"
#include "../core/algorithm.h"
#include "../core/mio.h"
#include "../core/string.h"

SLIB_MATH_NAMESPACE_BEGIN

class SLIB_EXPORT Uint128
{
public:
	sl_uint64 high;
	sl_uint64 low;

public:
	SLIB_INLINE Uint128()
	{
	}

	SLIB_INLINE Uint128(const Uint128& num)
	{
		high = num.high;
		low = num.low;
	}

	SLIB_INLINE Uint128(sl_uint64 num)
	{
		high = 0;
		low = num;
	}

private:
	static const sl_uint64 _zero[2];

public:
	SLIB_INLINE static const Uint128& zero()
	{
		return *((Uint128*)((void*)_zero));
	}
	
	SLIB_INLINE sl_bool isZero() const
	{
		return high == 0 && low == 0;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return high != 0 || low != 0;
	}
	
	SLIB_INLINE void setZero()
	{
		high = 0;
		low = 0;
	}

public:
	SLIB_INLINE Uint128& operator=(const Uint128& other)
	{
		high = other.high;
		low = other.low;
		return *this;
	}
	
	SLIB_INLINE Uint128& operator=(sl_uint64 num)
	{
		high = 0;
		low = num;
		return *this;
	}

public:
	SLIB_INLINE int compare(const Uint128& other) const
	{
		if (high > other.high) {
			return 1;
		}
		if (high < other.high) {
			return -1;
		}
		return (low < other.low) ? -1 : (low > other.low);
	}

	
	SLIB_INLINE sl_bool operator==(const Uint128& other) const
	{
		return high == other.high && low == other.low;
	}
	
	SLIB_INLINE sl_bool operator==(sl_uint64 num) const
	{
		return high == 0 && low == num;
	}
	
	SLIB_INLINE friend sl_bool operator==(sl_uint64 num, const Uint128& v)
	{
		return v.high == 0 && v.low == num;
	}

	
	SLIB_INLINE sl_bool operator!=(const Uint128& other) const
	{
		return high != other.high || low != other.low;
	}
	
	SLIB_INLINE sl_bool operator!=(sl_uint64 num) const
	{
		return high != 0 || low != num;
	}
	
	SLIB_INLINE friend sl_bool operator!=(sl_uint64 num, const Uint128& v)
	{
		return v.high != 0 || v.low != num;
	}

	
	SLIB_INLINE sl_bool operator>=(const Uint128& other) const
	{
		if (high > other.high) {
			return sl_true;
		}
		if (high < other.high) {
			return sl_false;
		}
		return low >= other.low;
	}
	
	SLIB_INLINE sl_bool operator>=(sl_uint64 num) const
	{
		return high > 0 || low >= num;
	}
	
	SLIB_INLINE friend sl_bool operator>=(sl_uint64 num, const Uint128& v)
	{
		return v.high == 0 && v.low <= num;
	}

	
	SLIB_INLINE sl_bool operator<=(const Uint128& other) const
	{
		if (high < other.high) {
			return sl_true;
		}
		if (high > other.high) {
			return sl_false;
		}
		return low <= other.low;
	}
	
	SLIB_INLINE sl_bool operator<=(sl_uint64 num) const
	{
		return high == 0 && low <= num;
	}
	
	SLIB_INLINE friend sl_bool operator<=(sl_uint64 num, const Uint128& v)
	{
		return v.high > 0 || v.low >= num;
	}

	
	SLIB_INLINE sl_bool operator>(const Uint128& other) const
	{
		if (high > other.high) {
			return sl_true;
		}
		if (high < other.high) {
			return sl_false;
		}
		return low > other.low;
	}
	
	SLIB_INLINE sl_bool operator>(sl_uint64 num) const
	{
		return high > 0 || low > num;
	}
	
	SLIB_INLINE friend sl_bool operator>(sl_uint64 num, const Uint128& v)
	{
		return v.high == 0 && v.low < num;
	}

	
	SLIB_INLINE sl_bool operator<(const Uint128& other) const
	{
		if (high < other.high) {
			return sl_true;
		}
		if (high > other.high) {
			return sl_false;
		}
		return low < other.low;
	}
	
	SLIB_INLINE sl_bool operator<(sl_uint64 num) const
	{
		return high == 0 && low < num;
	}
	
	SLIB_INLINE friend sl_bool operator<(sl_uint64 num, const Uint128& v)
	{
		return v.high > 0 || v.low > num;
	}

public:
	SLIB_INLINE Uint128 operator+(const Uint128& other) const
	{
		Uint128 ret;
		ret.high = high + other.high;
		ret.low = low + other.low;
		if (ret.low < low) {
			ret.high++;
		}
		return ret;
	}
	
	SLIB_INLINE Uint128 operator+(sl_uint64 num) const
	{
		Uint128 ret;
		ret.high = high;
		ret.low = low + num;
		if (ret.low < num) {
			ret.high++;
		}
		return ret;
	}
	
	SLIB_INLINE friend Uint128 operator+(sl_uint64 num, const Uint128& v)
	{
		Uint128 ret;
		ret.high = v.high;
		ret.low = v.low + num;
		if (ret.low < num) {
			ret.high++;
		}
		return ret;
	}

	
	SLIB_INLINE Uint128& operator+=(const Uint128& other)
	{
		high += other.high;
		low += other.low;
		if (low < other.low) {
			high++;
		}
		return *this;
	}
	
	SLIB_INLINE Uint128& operator+=(sl_uint32 num)
	{
		low += num;
		if (low < num) {
			high++;
		}
		return *this;
	}
	

	SLIB_INLINE Uint128& operator++()
	{
		low++;
		if (low == 0) {
			high++;
		}
		return *this;
	}
	
	SLIB_INLINE Uint128 operator++(int)
	{
		Uint128 ret = *this;
		low++;
		if (low == 0) {
			high++;
		}
		return ret;
	}

	
	SLIB_INLINE Uint128 operator-(const Uint128& other) const
	{
		Uint128 ret;
		ret.high = high - other.high;
		if (low < other.low) {
			ret.high--;
		}
		ret.low = low - other.low;
		return ret;
	}
	
	SLIB_INLINE Uint128 operator-(sl_uint64 num) const
	{
		Uint128 ret;
		ret.high = high;
		if (low < num) {
			ret.high--;
		}
		ret.low = low - num;
		return ret;
	}
	
	SLIB_INLINE friend Uint128 operator-(sl_uint64 num, const Uint128& v)
	{
		Uint128 ret;
		ret.high = - (sl_int64)(v.high);
		if (num < v.low) {
			ret.high--;
		}
		ret.low = num - v.low;
		return ret;
	}

	
	SLIB_INLINE Uint128& operator-=(const Uint128& other)
	{
		high -= other.high;
		if (low < other.low) {
			high--;
		}
		low -= other.low;
		return *this;
	}
	
	SLIB_INLINE Uint128& operator-=(sl_uint64 num)
	{
		if (low < num) {
			high--;
		}
		low -= num;
		return *this;
	}

	
	SLIB_INLINE Uint128& operator--()
	{
		if (low == 0) {
			high--;
		}
		low--;
		return *this;
	}
	
	SLIB_INLINE Uint128 operator--(int)
	{
		Uint128 ret = *this;
		if (low == 0) {
			high--;
		}
		low--;
		return ret;
	}

	
	SLIB_INLINE Uint128 operator*(const Uint128& other) const
	{
		Uint128 ret;
		Math::mul64(low, other.low, ret.high, ret.low);
		ret.high += (high * other.low);
		ret.high += (low * other.high);
		return ret;
	}
	
	SLIB_INLINE Uint128 operator*(sl_uint64 num) const
	{
		Uint128 ret;
		Math::mul64(low, num, ret.high, ret.low);
		ret.high += (high * num);
		return ret;
	}
	
	SLIB_INLINE friend Uint128 operator*(sl_uint64 num, const Uint128& v)
	{
		Uint128 ret;
		Math::mul64(v.low, num, ret.high, ret.low);
		ret.high += (v.high * num);
		return ret;
	}

	
	SLIB_INLINE Uint128& operator*=(const Uint128& other)
	{
		sl_uint64 ml, mh;
		Math::mul64(low, other.low, mh, ml);
		mh += (high * other.low);
		mh += (low * other.high);
		high = mh;
		low = ml;
		return *this;
	}
	
	SLIB_INLINE Uint128& operator*=(sl_uint64 num)
	{
		sl_uint64 ml, mh;
		Math::mul64(low, num, mh, ml);
		mh += (high * num);
		high = mh;
		low = ml;
		return *this;
	}
	
	
	sl_bool static div(const Uint128& a, const Uint128& b, Uint128* quotient = sl_null, Uint128* remainder = sl_null);

	SLIB_INLINE Uint128 div(const Uint128& other) const
	{
		Uint128 ret;
		if (div(*this, other, &ret, sl_null)) {
			return ret;
		} else {
			return Uint128::zero();
		}
	}

	SLIB_INLINE Uint128 div(sl_uint64 num) const
	{
		Uint128 ret;
		if (div(*this, num, &ret, sl_null)) {
			return ret;
		} else {
			return Uint128::zero();
		}
	}

	SLIB_INLINE Uint128 mod(const Uint128& other) const
	{
		Uint128 ret;
		if (div(*this, other, sl_null, &ret)) {
			return ret;
		} else {
			return Uint128::zero();
		}
	}
	
	SLIB_INLINE sl_uint64 mod(sl_uint64 num) const
	{
		Uint128 ret;
		if (div(*this, num, sl_null, &ret)) {
			return ret.low;
		} else {
			return 0;
		}
	}

	
	SLIB_INLINE Uint128 operator/(const Uint128& other) const
	{
		return div(other);
	}
	
	SLIB_INLINE Uint128 operator/(sl_uint64 num) const
	{
		return div(num);
	}
	
	SLIB_INLINE friend Uint128 operator/(sl_uint64 num, const Uint128& v)
	{
		Uint128 a(num);
		return a.div(v);
	}

	
	SLIB_INLINE Uint128& operator/=(const Uint128& other)
	{
		*this = div(other);
		return *this;
	}
	
	SLIB_INLINE Uint128& operator/=(sl_uint64 num)
	{
		*this = div(num);
		return *this;
	}

	
	SLIB_INLINE Uint128 operator%(const Uint128& other) const
	{
		return mod(other);
	}
	
	SLIB_INLINE Uint128 operator%(sl_uint64 num) const
	{
		return mod(num);
	}
	
	SLIB_INLINE friend Uint128 operator%(sl_uint64 num, const Uint128& v)
	{
		Uint128 a(num);
		return a.mod(v);
	}

	
	SLIB_INLINE Uint128& operator%=(const Uint128& other)
	{
		*this = mod(other);
		return *this;
	}
	
	SLIB_INLINE Uint128& operator%=(sl_uint64 num)
	{
		*this = mod(num);
		return *this;
	}

	
	SLIB_INLINE Uint128 operator&(const Uint128& other) const
	{
		Uint128 ret;
		ret.high = high & other.high;
		ret.low = low & other.low;
		return ret;
	}
	
	SLIB_INLINE Uint128 operator&(sl_uint64 num) const
	{
		Uint128 ret;
		ret.high = high;
		ret.low = low & num;
		return ret;
	}
	
	SLIB_INLINE friend Uint128 operator&(sl_uint64 num, const Uint128& v)
	{
		Uint128 ret;
		ret.high = v.high;
		ret.low = v.low & num;
		return ret;
	}

	
	SLIB_INLINE Uint128& operator&=(const Uint128& other)
	{
		high &= other.high;
		low &= other.low;
		return *this;
	}
	
	SLIB_INLINE Uint128& operator&=(sl_uint32 num)
	{
		low &= num;
		return *this;
	}

	
	SLIB_INLINE Uint128 operator|(const Uint128& other) const
	{
		Uint128 ret;
		ret.high = high | other.high;
		ret.low = low | other.low;
		return ret;
	}
	
	SLIB_INLINE Uint128 operator|(sl_uint64 num) const
	{
		Uint128 ret;
		ret.high = high;
		ret.low = low | num;
		return ret;
	}
	
	SLIB_INLINE friend Uint128 operator|(sl_uint64 num, const Uint128& v)
	{
		Uint128 ret;
		ret.high = v.high;
		ret.low = v.low | num;
		return ret;
	}

	
	SLIB_INLINE Uint128& operator|=(const Uint128& other)
	{
		high |= other.high;
		low |= other.low;
		return *this;
	}
	
	SLIB_INLINE Uint128& operator|=(sl_uint32 num)
	{
		low |= num;
		return *this;
	}

	
	SLIB_INLINE Uint128 operator^(const Uint128& other) const
	{
		Uint128 ret;
		ret.high = high ^ other.high;
		ret.low = low ^ other.low;
		return ret;
	}
	
	SLIB_INLINE Uint128 operator^(sl_uint64 num) const
	{
		Uint128 ret;
		ret.high = high;
		ret.low = low ^ num;
		return ret;
	}
	
	SLIB_INLINE friend Uint128 operator^(sl_uint64 num, const Uint128& v)
	{
		Uint128 ret;
		ret.high = v.high;
		ret.low = v.low ^ num;
		return ret;
	}
	

	SLIB_INLINE Uint128& operator^=(const Uint128& other)
	{
		high ^= other.high;
		low ^= other.low;
		return *this;
	}
	
	SLIB_INLINE Uint128& operator^=(sl_uint32 num)
	{
		low ^= num;
		return *this;
	}

	
	SLIB_INLINE Uint128 operator>>(sl_uint32 n) const
	{
		Uint128 ret;
		if (n) {
			if (n >= 64) {
				ret.low = high >> (n - 64);
				ret.high = 0;
			} else {
				ret.low = (high << (64 - n)) | (low >> n);
				ret.high = high >> n;
			}
		} else {
			ret.low = low;
			ret.high = high;
		}
		return ret;
	}
	
	SLIB_INLINE Uint128& operator>>=(sl_uint32 n)
	{
		if (n) {
			if (n >= 64) {
				low = high >> (n - 64);
				high = 0;
			} else {
				low = (high << (64 - n)) | (low >> n);
				high >>= n;
			}
		}
		return *this;
	}

	
	SLIB_INLINE Uint128 operator<<(sl_uint32 n) const
	{
		Uint128 ret;
		if (n) {
			if (n >= 64) {
				ret.high = low << (n - 64);
				ret.low = 0;
			} else {
				ret.high = (low >> (64 - n)) | (high << n);
				ret.low = low << n;
			}
		} else {
			ret.high = high;
			ret.low = low;
		}
		return ret;
	}
	
	SLIB_INLINE Uint128& operator<<=(sl_uint32 n)
	{
		if (n) {
			if (n >= 64) {
				high = low << (n - 64);
				low = 0;
			} else {
				high = (low >> (64 - n)) | (high << n);
				low <<= n;
			}
		}
		return *this;
	}

	
	SLIB_INLINE void shiftRight()
	{
		low = (high << 63) | (low >> 1);
		high >>= 1;
	}

	SLIB_INLINE void shiftLeft()
	{
		high = (low >> 63) | (high << 1);
		low <<= 1;
	}

	
	SLIB_INLINE void makeNegative()
	{
		low = -(sl_int64)(low);
		high = -(sl_int64)(high);
		if (low != 0) {
			high--;
		}
	}
	
	SLIB_INLINE Uint128 operator-() const
	{
		Uint128 ret;
		ret.low = -(sl_int64)(low);
		ret.high = -(sl_int64)(high);
		if (low != 0) {
			ret.high--;
		}
		return ret;
	}

	
	SLIB_INLINE void makeBitwiseNot()
	{
		low = ~low;
		high = ~high;
	}
	
	SLIB_INLINE Uint128 operator~() const
	{
		Uint128 ret;
		ret.low = ~low;
		ret.high = ~high;
		return ret;
	}

public:
	sl_uint32 getMostSignificantBits() const;
	
	sl_uint32 getLeastSignificantBits() const;

	
	// 16 bytes
	SLIB_INLINE void getBytesBE(void* _buf)
	{
		char* buf = (char*)_buf;
		MIO::writeUint64BE(buf, high);
		MIO::writeUint64BE(buf + 8, low);
	}
	
	// 16 bytes
	SLIB_INLINE void setBytesBE(const void* _buf)
	{
		const char* buf = (const char*)_buf;
		high = MIO::readUint64BE(buf);
		low = MIO::readUint64BE(buf + 8);
	}

	// 16 bytes
	SLIB_INLINE void getBytesLE(void* _buf)
	{
		char* buf = (char*)_buf;
		MIO::writeUint64LE(buf, low);
		MIO::writeUint64LE(buf + 8, high);
	}

	// 16 bytes
	SLIB_INLINE void setBytesLE(const void* _buf)
	{
		const char* buf = (const char*)_buf;
		low = MIO::readUint64LE(buf);
		high = MIO::readUint64LE(buf + 8);
	}

	
	static sl_int32 parseString(Uint128* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_uint32 radix = 10);
	
	static sl_int32 parseString(Uint128* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_uint32 radix = 10);
	
	sl_bool parseString(const String& str, sl_uint32 radix = 10);
	
	static Uint128 fromString(const String& str, sl_uint32 radix = 10);
	
	String toString(sl_uint32 radix = 10) const;

	SLIB_INLINE static Uint128 fromHexString(const String& str)
	{
		return fromString(str, 16);
	}
	
	SLIB_INLINE String toHexString() const
	{
		return toString(16);
	}

};

template<>
SLIB_INLINE int Compare<Uint128>::compare(const Uint128& a, const Uint128& b)
{
	return a.compare(b);
}

template<>
SLIB_INLINE sl_bool Compare<Uint128>::equals(const Uint128& a, const Uint128& b)
{
	return a == b;
}

template<>
SLIB_INLINE sl_uint32 Hash<Uint128>::hash(const Uint128& v)
{
	return sl_rehash((sl_uint32)(v.high ^ (v.high >> 32) ^ v.low ^ (v.low >> 32)));
}

SLIB_MATH_NAMESPACE_END

#endif

