#include "../../../inc/slib/math/int128.h"

#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/core/mio.h"

/*************************************
	Uint128
**************************************/
SLIB_MATH_NAMESPACE_BEGIN

template<>
int Compare<Uint128>::compare(const Uint128& a, const Uint128& b)
{
	return a.compare(b);
}

template<>
sl_bool Compare<Uint128>::equals(const Uint128& a, const Uint128& b)
{
	return a == b;
}

template<>
sl_uint32 Hash<Uint128>::hash(const Uint128& v)
{
	return sl_rehash((sl_uint32)(v.high ^ (v.high >> 32) ^ v.low ^ (v.low >> 32)));
}


const sl_uint64 Uint128::_zero[2] = { 0, 0 };

int Uint128::compare(const Uint128& other) const
{
	if (high > other.high) {
		return 1;
	}
	if (high < other.high) {
		return -1;
	}
	return (low < other.low) ? -1 : (low > other.low);
}

sl_bool Uint128::div(const Uint128& a, const Uint128& b, Uint128* quotient, Uint128* remainder)
{
	if (b.isZero()) {
		return sl_false;
	}
	if (a < b) {
		if (remainder) {
			*remainder = a;
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	sl_uint32 nba = a.getMostSignificantBits();
	sl_uint32 nbb = b.getMostSignificantBits();
	if (nba < nbb) {
		if (remainder) {
			*remainder = a;
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	sl_uint32 shift = nba - nbb;
	Uint128 r = a;
	Uint128 t = b;
	t <<= shift;
	Uint128 q;
	q.setZero();
	do {
		if (r >= t) {
			q |= 1;
			r -= t;
		}
		if (shift == 0) {
			break;
		}
		q.shiftLeft();
		t.shiftRight();
		shift--;
	} while (1);
	if (quotient) {
		*quotient = q;
	}
	if (remainder) {
		*remainder = r;
	}
	return sl_true;
}

Uint128 Uint128::div(const Uint128& other) const
{
	Uint128 ret;
	if (div(*this, other, &ret, sl_null)) {
		return ret;
	} else {
		return Uint128::zero();
	}
}

Uint128 Uint128::div(sl_uint64 num) const
{
	Uint128 ret;
	if (div(*this, num, &ret, sl_null)) {
		return ret;
	} else {
		return Uint128::zero();
	}
}

Uint128 Uint128::mod(const Uint128& other) const
{
	Uint128 ret;
	if (div(*this, other, sl_null, &ret)) {
		return ret;
	} else {
		return Uint128::zero();
	}
}

sl_uint64 Uint128::mod(sl_uint64 num) const
{
	Uint128 ret;
	if (div(*this, num, sl_null, &ret)) {
		return ret.low;
	} else {
		return 0;
	}
}

void Uint128::shiftRight()
{
	low = (high << 63) | (low >> 1);
	high >>= 1;
}

void Uint128::shiftLeft()
{
	high = (low >> 63) | (high << 1);
	low <<= 1;
}

void Uint128::makeNegative()
{
	low = -(sl_int64)(low);
	high = -(sl_int64)(high);
	if (low != 0) {
		high--;
	}
}

void Uint128::makeBitwiseNot()
{
	low = ~low;
	high = ~high;
}

sl_bool Uint128::operator==(const Uint128& other) const
{
	return high == other.high && low == other.low;
}

sl_bool Uint128::operator==(sl_uint64 num) const
{
	return high == 0 && low == num;
}

sl_bool operator==(sl_uint64 num, const Uint128& v)
{
	return v.high == 0 && v.low == num;
}

sl_bool Uint128::operator!=(const Uint128& other) const
{
	return high != other.high || low != other.low;
}

sl_bool Uint128::operator!=(sl_uint64 num) const
{
	return high != 0 || low != num;
}

sl_bool operator!=(sl_uint64 num, const Uint128& v)
{
	return v.high != 0 || v.low != num;
}

sl_bool Uint128::operator>=(const Uint128& other) const
{
	if (high > other.high) {
		return sl_true;
	}
	if (high < other.high) {
		return sl_false;
	}
	return low >= other.low;
}

sl_bool Uint128::operator>=(sl_uint64 num) const
{
	return high > 0 || low >= num;
}

sl_bool operator>=(sl_uint64 num, const Uint128& v)
{
	return v.high == 0 && v.low <= num;
}

sl_bool Uint128::operator<=(const Uint128& other) const
{
	if (high < other.high) {
		return sl_true;
	}
	if (high > other.high) {
		return sl_false;
	}
	return low <= other.low;
}

sl_bool Uint128::operator<=(sl_uint64 num) const
{
	return high == 0 && low <= num;
}

sl_bool operator<=(sl_uint64 num, const Uint128& v)
{
	return v.high > 0 || v.low >= num;
}

sl_bool Uint128::operator>(const Uint128& other) const
{
	if (high > other.high) {
		return sl_true;
	}
	if (high < other.high) {
		return sl_false;
	}
	return low > other.low;
}

sl_bool Uint128::operator>(sl_uint64 num) const
{
	return high > 0 || low > num;
}

sl_bool operator>(sl_uint64 num, const Uint128& v)
{
	return v.high == 0 && v.low < num;
}

sl_bool Uint128::operator<(const Uint128& other) const
{
	if (high < other.high) {
		return sl_true;
	}
	if (high > other.high) {
		return sl_false;
	}
	return low < other.low;
}

sl_bool Uint128::operator<(sl_uint64 num) const
{
	return high == 0 && low < num;
}

sl_bool operator<(sl_uint64 num, const Uint128& v)
{
	return v.high > 0 || v.low > num;
}

Uint128 Uint128::operator+(const Uint128& other) const
{
	Uint128 ret;
	ret.high = high + other.high;
	ret.low = low + other.low;
	if (ret.low < low) {
		ret.high++;
	}
	return ret;
}

Uint128 Uint128::operator+(sl_uint64 num) const
{
	Uint128 ret;
	ret.high = high;
	ret.low = low + num;
	if (ret.low < num) {
		ret.high++;
	}
	return ret;
}

Uint128 operator+(sl_uint64 num, const Uint128& v)
{
	Uint128 ret;
	ret.high = v.high;
	ret.low = v.low + num;
	if (ret.low < num) {
		ret.high++;
	}
	return ret;
}

Uint128& Uint128::operator+=(const Uint128& other)
{
	high += other.high;
	low += other.low;
	if (low < other.low) {
		high++;
	}
	return *this;
}

Uint128& Uint128::operator+=(sl_uint32 num)
{
	low += num;
	if (low < num) {
		high++;
	}
	return *this;
}


Uint128& Uint128::operator++()
{
	low++;
	if (low == 0) {
		high++;
	}
	return *this;
}

Uint128 Uint128::operator++(int)
{
	Uint128 ret = *this;
	low++;
	if (low == 0) {
		high++;
	}
	return ret;
}

Uint128 Uint128::operator-(const Uint128& other) const
{
	Uint128 ret;
	ret.high = high - other.high;
	if (low < other.low) {
		ret.high--;
	}
	ret.low = low - other.low;
	return ret;
}

Uint128 Uint128::operator-(sl_uint64 num) const
{
	Uint128 ret;
	ret.high = high;
	if (low < num) {
		ret.high--;
	}
	ret.low = low - num;
	return ret;
}

Uint128 operator-(sl_uint64 num, const Uint128& v)
{
	Uint128 ret;
	ret.high = - (sl_int64)(v.high);
	if (num < v.low) {
		ret.high--;
	}
	ret.low = num - v.low;
	return ret;
}

Uint128& Uint128::operator-=(const Uint128& other)
{
	high -= other.high;
	if (low < other.low) {
		high--;
	}
	low -= other.low;
	return *this;
}

Uint128& Uint128::operator-=(sl_uint64 num)
{
	if (low < num) {
		high--;
	}
	low -= num;
	return *this;
}

Uint128& Uint128::operator--()
{
	if (low == 0) {
		high--;
	}
	low--;
	return *this;
}

Uint128 Uint128::operator--(int)
{
	Uint128 ret = *this;
	if (low == 0) {
		high--;
	}
	low--;
	return ret;
}

Uint128 Uint128::operator*(const Uint128& other) const
{
	Uint128 ret;
	Math::mul64(low, other.low, ret.high, ret.low);
	ret.high += (high * other.low);
	ret.high += (low * other.high);
	return ret;
}

Uint128 Uint128::operator*(sl_uint64 num) const
{
	Uint128 ret;
	Math::mul64(low, num, ret.high, ret.low);
	ret.high += (high * num);
	return ret;
}

Uint128 operator*(sl_uint64 num, const Uint128& v)
{
	Uint128 ret;
	Math::mul64(v.low, num, ret.high, ret.low);
	ret.high += (v.high * num);
	return ret;
}

Uint128& Uint128::operator*=(const Uint128& other)
{
	sl_uint64 ml, mh;
	Math::mul64(low, other.low, mh, ml);
	mh += (high * other.low);
	mh += (low * other.high);
	high = mh;
	low = ml;
	return *this;
}

Uint128& Uint128::operator*=(sl_uint64 num)
{
	sl_uint64 ml, mh;
	Math::mul64(low, num, mh, ml);
	mh += (high * num);
	high = mh;
	low = ml;
	return *this;
}

Uint128 Uint128::operator/(const Uint128& other) const
{
	return div(other);
}

Uint128 Uint128::operator/(sl_uint64 num) const
{
	return div(num);
}

Uint128 operator/(sl_uint64 num, const Uint128& v)
{
	Uint128 a(num);
	return a.div(v);
}

Uint128& Uint128::operator/=(const Uint128& other)
{
	*this = div(other);
	return *this;
}

Uint128& Uint128::operator/=(sl_uint64 num)
{
	*this = div(num);
	return *this;
}

Uint128 Uint128::operator%(const Uint128& other) const
{
	return mod(other);
}

Uint128 Uint128::operator%(sl_uint64 num) const
{
	return mod(num);
}

Uint128 operator%(sl_uint64 num, const Uint128& v)
{
	Uint128 a(num);
	return a.mod(v);
}

Uint128& Uint128::operator%=(const Uint128& other)
{
	*this = mod(other);
	return *this;
}

Uint128& Uint128::operator%=(sl_uint64 num)
{
	*this = mod(num);
	return *this;
}

Uint128 Uint128::operator&(const Uint128& other) const
{
	Uint128 ret;
	ret.high = high & other.high;
	ret.low = low & other.low;
	return ret;
}

Uint128 Uint128::operator&(sl_uint64 num) const
{
	Uint128 ret;
	ret.high = high;
	ret.low = low & num;
	return ret;
}

Uint128 operator&(sl_uint64 num, const Uint128& v)
{
	Uint128 ret;
	ret.high = v.high;
	ret.low = v.low & num;
	return ret;
}

Uint128& Uint128::operator&=(const Uint128& other)
{
	high &= other.high;
	low &= other.low;
	return *this;
}

Uint128& Uint128::operator&=(sl_uint32 num)
{
	low &= num;
	return *this;
}

Uint128 Uint128::operator|(const Uint128& other) const
{
	Uint128 ret;
	ret.high = high | other.high;
	ret.low = low | other.low;
	return ret;
}

Uint128 Uint128::operator|(sl_uint64 num) const
{
	Uint128 ret;
	ret.high = high;
	ret.low = low | num;
	return ret;
}

Uint128 operator|(sl_uint64 num, const Uint128& v)
{
	Uint128 ret;
	ret.high = v.high;
	ret.low = v.low | num;
	return ret;
}

Uint128& Uint128::operator|=(const Uint128& other)
{
	high |= other.high;
	low |= other.low;
	return *this;
}

Uint128& Uint128::operator|=(sl_uint32 num)
{
	low |= num;
	return *this;
}

Uint128 Uint128::operator^(const Uint128& other) const
{
	Uint128 ret;
	ret.high = high ^ other.high;
	ret.low = low ^ other.low;
	return ret;
}

Uint128 Uint128::operator^(sl_uint64 num) const
{
	Uint128 ret;
	ret.high = high;
	ret.low = low ^ num;
	return ret;
}

Uint128 operator^(sl_uint64 num, const Uint128& v)
{
	Uint128 ret;
	ret.high = v.high;
	ret.low = v.low ^ num;
	return ret;
}

Uint128& Uint128::operator^=(const Uint128& other)
{
	high ^= other.high;
	low ^= other.low;
	return *this;
}

Uint128& Uint128::operator^=(sl_uint32 num)
{
	low ^= num;
	return *this;
}

Uint128 Uint128::operator>>(sl_uint32 n) const
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

Uint128& Uint128::operator>>=(sl_uint32 n)
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

Uint128 Uint128::operator<<(sl_uint32 n) const
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

Uint128& Uint128::operator<<=(sl_uint32 n)
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

Uint128 Uint128::operator-() const
{
	Uint128 ret;
	ret.low = -(sl_int64)(low);
	ret.high = -(sl_int64)(high);
	if (low != 0) {
		ret.high--;
	}
	return ret;
}

Uint128 Uint128::operator~() const
{
	Uint128 ret;
	ret.low = ~low;
	ret.high = ~high;
	return ret;
}

sl_uint32 Uint128::getMostSignificantBits() const
{
	if (high != 0) {
		return 64 + Math::getMostSignificantBits(high);
	}
	if (low != 0) {
		return Math::getMostSignificantBits(low);
	}
	return 0;
}

sl_uint32 Uint128::getLeastSignificantBits() const
{
	if (low != 0) {
		return Math::getLeastSignificantBits(low);
	}
	if (high != 0) {
		return 64 + Math::getLeastSignificantBits(high);
	}
	return 0;
}

void Uint128::getBytesBE(void* _buf)
{
	char* buf = (char*)_buf;
	MIO::writeUint64BE(buf, high);
	MIO::writeUint64BE(buf + 8, low);
}

void Uint128::setBytesBE(const void* _buf)
{
	const char* buf = (const char*)_buf;
	high = MIO::readUint64BE(buf);
	low = MIO::readUint64BE(buf + 8);
}

void Uint128::getBytesLE(void* _buf)
{
	char* buf = (char*)_buf;
	MIO::writeUint64LE(buf, low);
	MIO::writeUint64LE(buf + 8, high);
}

void Uint128::setBytesLE(const void* _buf)
{
	const char* buf = (const char*)_buf;
	low = MIO::readUint64LE(buf);
	high = MIO::readUint64LE(buf + 8);
}

Uint128 Uint128::fromString(const String& str, sl_uint32 radix)
{
	Uint128 ret;
	if (ret.parse(str, radix)) {
		return ret;
	}
	ret.setZero();
	return ret;
}

String Uint128::toString(sl_uint32 radix) const
{
	if (radix < 2 || radix > 64) {
		return String::null();
	}
	Uint128 m = *this;
	if (m.isZero()) {
		SLIB_STATIC_STRING(s, "0");
		return s;
	}
	if (radix == 16) {
		char buf[33];
		buf[32] = 0;
		sl_uint32 posBuf = 32;
		while (m.isNotZero() && posBuf > 0) {
			posBuf--;
			sl_uint32 v = m.low & 15;
			if (v < 10) {
				buf[posBuf] = (sl_char8)(v + 0x30);
			} else {
				buf[posBuf] = (sl_char8)(v + 0x37);
			}
			m >>= 4;
		}
		return String(buf + posBuf, 32 - posBuf);
	} else {
		char buf[129];
		buf[128] = 0;
		sl_uint32 posBuf = 128;
		Uint128 _radix = radix;
		Uint128 r;
		while (m.isNotZero() && posBuf > 0) {
			posBuf--;
			if (div(m, _radix, &m, &r)) {
				sl_uint32 v = (sl_uint32)(r.low);
				if (v < radix) {
					buf[posBuf] = _StringConv_radixPatternUpper[v];
				} else {
					buf[posBuf] = '?';
				}
			} else {
				buf[posBuf] = '?';
			}			
		}
		return String(buf + posBuf, 128 - posBuf);
	}
}

Uint128 Uint128::fromHexString(const String& str)
{
	return fromString(str, 16);
}

String Uint128::toHexString() const
{
	return toString(16);
}


template <class CT, class ST>
static sl_reg _Uint128_parseString(Uint128* out, const CT* sz, sl_size posBegin, sl_size len, sl_uint32 radix)
{
	if (radix < 2 || radix > 64) {
		return SLIB_PARSE_ERROR;
	}
	sl_size pos = posBegin;
	Uint128 m;
	m.setZero();
	const sl_uint8* pattern = radix <= 36 ? _StringConv_radixInversePatternSmall : _StringConv_radixInversePatternBig;
	if (radix == 16) {
		for (; pos < len; pos++) {
			sl_uint32 c = (sl_uint8)(sz[pos]);
			sl_uint32 v = c < 128 ? pattern[c] : 255;
			if (v >= 16) {
				break;
			}
			m <<= 4;
			m |= v;
		}
	} else {
		for (; pos < len; pos++) {
			sl_uint32 c = (sl_uint8)(sz[pos]);
			sl_uint32 v = c < 128 ? pattern[c] : 255;
			if (v >= radix) {
				break;
			}
			m *= radix;
			m += v;
		}
	}
	if (pos == posBegin) {
		return SLIB_PARSE_ERROR;
	}
	if (out) {
		*out = m;
	}
	return pos;
}

SLIB_DEFINE_PARSE_FUNCTIONS_ARG(Uint128, _Uint128_parseString, sl_uint32, radix, 10)


SLIB_MATH_NAMESPACE_END
