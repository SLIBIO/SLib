#include "../../../inc/slib/math/bigint.h"

#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/mio.h"
#include "../../../inc/slib/core/scoped.h"

#define STACK_BUFFER_SIZE 4096

/*
	CBigInt
*/

#define CBIGINT_INT32(o, v) \
	CBigInt o; \
	sl_uint32 __m__##o; \
	if (v < 0) { \
		__m__##o = -v; \
		o.sign = -1; \
	} else { \
		__m__##o = v; \
		o.sign = 1; \
	} \
	o.setUserDataElements((sl_uint32*)&__m__##o, 1);

#define CBIGINT_UINT32(o, v) \
	CBigInt o; \
	o.sign = 1; \
	sl_uint32 __m__##o = v; \
	o.setUserDataElements((sl_uint32*)&__m__##o, 1);

#define CBIGINT_INT64(o, v) \
	CBigInt o; \
	if (v < 0) { \
		o.sign = -1; \
		v = -v; \
	} else { \
		o.sign = 1; \
	} \
	sl_uint32 __m__##o[2]; \
	__m__##o[0] = (sl_uint32)((sl_uint64)v); \
	__m__##o[1] = (sl_uint32)(((sl_uint64)v) >> 32); \
	o.setUserDataElements(__m__##o, 2);

#define CBIGINT_UINT64(o, v) \
	CBigInt o; \
	o.sign = 1; \
	sl_uint32 __m__##o[2]; \
	__m__##o[0] = (sl_uint32)((sl_uint64)v); \
	__m__##o[1] = (sl_uint32)(((sl_uint64)v) >> 32); \
	o.setUserDataElements(__m__##o, 2);

SLIB_MATH_NAMESPACE_BEGIN

SLIB_INLINE static sl_int32 _cbigint_compare(const sl_uint32* a, const sl_uint32* b, sl_size n)
{
	for (sl_size i = n; i > 0; i--) {
		if (a[i - 1] > b[i - 1]) {
			return 1;
		}
		if (a[i - 1] < b[i - 1]) {
			return -1;
		}
	}
	return 0;
}

// returns 0, 1 (overflow)
SLIB_INLINE static sl_uint32 _cbigint_add(sl_uint32* c, const sl_uint32* a, const sl_uint32* b, sl_size n, sl_uint32 _of)
{
	sl_uint32 of = _of;
	for (sl_size i = 0; i < n; i++) {
		sl_uint32 sum = a[i] + of;
		of = sum < of ? 1 : 0;
		sl_uint32 t = b[i];
		sum += t;
		of += sum < t ? 1 : 0;
		c[i] = sum;
	}
	return of;
}

// returns 0, 1 (overflow)
SLIB_INLINE static sl_uint32 _cbigint_add_uint32(sl_uint32* c, const sl_uint32* a, sl_size n, sl_uint32 b)
{
	sl_uint32 of = b;
	if (c == a) {
		for (sl_size i = 0; i < n && of; i++) {
			sl_uint32 sum = a[i] + of;
			of = sum < of ? 1 : 0;
			c[i] = sum;
		}
	} else {
		for (sl_size i = 0; i < n; i++) {
			sl_uint32 sum = a[i] + of;
			of = sum < of ? 1 : 0;
			c[i] = sum;
		}
	}
	return of;
}

// returns 0, 1 (overflow)
SLIB_INLINE static sl_uint32 _cbigint_sub(sl_uint32* c, const sl_uint32* a, const sl_uint32* b, sl_size n, sl_uint32 _of)
{
	sl_uint32 of = _of;
	for (sl_size i = 0; i < n; i++) {
		sl_uint32 k1 = a[i];
		sl_uint32 k2 = b[i];
		sl_uint32 o = k1 < of ? 1 : 0;
		k1 -= of;
		of = o + (k1 < k2 ? 1 : 0);
		k1 -= k2;
		c[i] = k1;
	}
	return of;
}

// returns 0, 1 (overflow)
SLIB_INLINE static sl_uint32 _cbigint_sub_uint32(sl_uint32* c, const sl_uint32* a, sl_size n, sl_uint32 b)
{
	sl_uint32 of = b;
	if (c == a) {
		for (sl_size i = 0; i < n && of; i++) {
			sl_uint32 k = a[i];
			sl_uint32 o = k < of ? 1 : 0;
			k -= of;
			of = o;
			c[i] = k;
		}
	} else {
		for (sl_size i = 0; i < n; i++) {
			sl_uint32 k = a[i];
			sl_uint32 o = k < of ? 1 : 0;
			k -= of;
			of = o;
			c[i] = k;
		}
	}
	return of;
}

// returns overflow
SLIB_INLINE static sl_uint32 _cbigint_mul_uint32(sl_uint32* c, const sl_uint32* a, sl_size n, sl_uint32 b, sl_uint32 o)
{
	sl_uint32 of = o;
	for (sl_size i = 0; i < n; i++) {
		sl_uint64 k = a[i];
		k *= b;
		k += of;
		c[i] = (sl_uint32)k;
		of = (sl_uint32)(k >> 32);
	}
	return of;
}


// c = c + a * b
SLIB_INLINE static sl_uint32 _cbigint_muladd_uint32(sl_uint32* c, const sl_uint32* s, sl_size m, const sl_uint32* a, sl_size n, sl_uint32 b, sl_uint32 o)
{
	n = Math::min(m, n);
	sl_uint32 of = o;
	sl_size i;
	for (i = 0; i < n; i++) {
		sl_uint64 k = a[i];
		k *= b;
		k += of;
		k += s[i];
		c[i] = (sl_uint32)k;
		of = (sl_uint32)(k >> 32);
	}
	if (c == s) {
		for (i = n; i < m && of; i++) {
			sl_uint32 sum = s[i] + of;
			of = sum < of ? 1 : 0;
			c[i] = sum;
		}
	} else {
		for (i = n; i < m; i++) {
			sl_uint32 sum = s[i] + of;
			of = sum < of ? 1 : 0;
			c[i] = sum;
		}
	}
	return of;
}


// returns remainder
SLIB_INLINE static sl_uint32 _cbigint_div_uint32(sl_uint32* q, const sl_uint32* a, sl_size n, sl_uint32 b, sl_uint32 o)
{
	sl_size j = n - 1;
	if (q) {
		for (sl_size i = 0; i < n; i++) {
			sl_uint64 k = o;
			k <<= 32;
			k |= a[j];
			q[j] = (sl_uint32)(k / b);
			o = (sl_uint32)(k % b);
			j--;
		}
	} else {
		for (sl_size i = 0; i < n; i++) {
			sl_uint64 k = o;
			k <<= 32;
			k |= a[j];
			o = (sl_uint32)(k % b);
			j--;
		}
	}
	return o;
}

// shift 0~31 bits
// returns overflow
SLIB_INLINE static sl_uint32 _cbigint_shiftLeft(sl_uint32* c, const sl_uint32* a, sl_size n, sl_uint32 shift, sl_uint32 valueRight)
{
	sl_uint32 rs = 32 - shift;
	sl_uint32 of = valueRight >> rs;
	for (sl_size i = 0; i < n; i++) {
		sl_uint32 t = a[i];
		c[i] = (t << shift) | of;
		of = t >> rs;
	}
	return of;
}

// shift 0~31 bits
// returns overflow
SLIB_INLINE static sl_uint32 _cbigint_shiftRight(sl_uint32* c, const sl_uint32* a, sl_size n, sl_uint32 shift, sl_uint32 valueLeft)
{
	sl_uint32 rs = 32 - shift;
	sl_uint32 of = valueLeft << rs;
	for (sl_size i = n; i > 0; i--) {
		sl_uint32 t = a[i - 1];
		c[i - 1] = (t >> shift) | of;
		of = t << rs;
	}
	return of;
}

SLIB_INLINE static sl_size _cbigint_mse(const sl_uint32* a, sl_size n)
{
	for (sl_size ni = n; ni > 0; ni--) {
		if (a[ni - 1] != 0) {
			return ni;
		}
	}
	return 0;
}

SLIB_INLINE static sl_size _cbigint_lse(const sl_uint32* a, sl_size n)
{
	for (sl_size ni = 0; ni < n; ni++) {
		if (a[ni] != 0) {
			return ni + 1;
		}
	}
	return 0;
}

SLIB_INLINE static sl_size _cbigint_msbytes(const sl_uint32* a, sl_size n)
{
	for (sl_size ni = n; ni > 0; ni--) {
		sl_uint32 e = a[ni - 1];
		if (e != 0) {
			for (sl_uint32 nB = 4; nB > 0; nB--) {
				if (((e >> ((nB - 1) << 3)) & 255) != 0) {
					return (((ni - 1) << 2) + nB);
				}
			}
			break;
		}
	}
	return 0;
}

SLIB_INLINE static sl_size _cbigint_lsbytes(const sl_uint32* a, sl_size n)
{
	for (sl_size ni = 0; ni < n; ni++) {
		sl_uint32 e = a[ni];
		if (e != 0) {
			for (sl_uint32 nB = 0; nB < 4; nB++) {
				if (((e >> (nB << 3)) & 255) != 0) {
					return ((ni << 2) + nB + 1);
				}
			}
			break;
		}
	}
	return 0;
}

SLIB_INLINE static sl_size _cbigint_msbits(const sl_uint32* a, sl_size n)
{
	for (sl_size ni = n; ni > 0; ni--) {
		sl_uint32 e = a[ni - 1];
		if (e != 0) {
			for (sl_uint32 nb = 32; nb > 0; nb--) {
				if (((e >> (nb - 1)) & 1) != 0) {
					return (((ni - 1) << 5) + nb);
				}
			}
			break;
		}
	}
	return 0;
}

SLIB_INLINE static sl_size _cbigint_lsbits(const sl_uint32* a, sl_size n)
{
	for (sl_size ni = 0; ni < n; ni++) {
		sl_uint32 e = a[ni];
		if (e != 0) {
			for (sl_uint32 nb = 0; nb < 32; nb++) {
				if (((e >> nb) & 1) != 0) {
					return ((ni << 5) + nb + 1);
				}
			}
			break;
		}
	}
	return 0;
}


SLIB_DEFINE_ROOT_OBJECT(CBigInt)

SLIB_INLINE void CBigInt::_free()
{
	if (elements) {
		if (!m_flagUserData) {
			Base::freeMemory(elements);
		}
		elements = sl_null;
	}
	sign = 1;
	length = 0;
}


CBigInt::CBigInt()
{
	sign = 1;
	length = 0;
	elements = sl_null;
	m_flagUserData = sl_false;
}

CBigInt::~CBigInt()
{
	_free();
}

void CBigInt::setUserDataElements(sl_uint32* elements, sl_size n)
{
	_free();
	if (n > 0) {
		this->elements = elements;
		this->length = n;
		this->m_flagUserData = sl_true;
	}
}

sl_int32 CBigInt::makeNagative()
{
	sign = -sign;
	return sign;
}

sl_bool CBigInt::getBit(sl_size pos) const
{
	if (pos < (length << 5)) {
		return ((elements[pos >> 5] >> (pos & 0x1F)) & 1) != 0;
	} else {
		return sl_false;
	}
}

void CBigInt::setBit(sl_size pos, sl_bool bit)
{
	if (growLength((pos + 31) >> 5)) {
		sl_size ni = pos >> 5;
		sl_uint32 nb = (sl_uint32)(pos & 0x1F);
		if (bit) {
			elements[ni] |= (((sl_uint32)(1)) << nb);
		} else {
			elements[ni] &= ~(((sl_uint32)(1)) << nb);
		}
	}
}

sl_size CBigInt::getMostSignificantElements() const
{
	if (elements) {
		return _cbigint_mse(elements, length);
	}
	return 0;
}

sl_size CBigInt::getLeastSignificantElements() const
{
	if (elements) {
		return _cbigint_lse(elements, length);
	}
	return 0;
}

sl_size CBigInt::getMostSignificantBytes() const
{
	if (elements) {
		return _cbigint_msbytes(elements, length);
	}
	return 0;
}

sl_size CBigInt::getLeastSignificantBytes() const
{
	if (elements) {
		return _cbigint_lsbytes(elements, length);
	}
	return 0;
}

sl_size CBigInt::getMostSignificantBits() const
{
	if (elements) {
		return _cbigint_msbits(elements, length);
	}
	return 0;
}

sl_size CBigInt::getLeastSignificantBits() const
{
	if (elements) {
		return _cbigint_lsbits(elements, length);
	}
	return 0;
}

sl_bool CBigInt::isZero() const
{
	return getMostSignificantElements() == 0;
}

sl_bool CBigInt::isNotZero() const
{
	return getMostSignificantElements() != 0;
}

void CBigInt::setZero()
{
	if (elements) {
		Base::zeroMemory(elements, length * 4);
	}
}

CBigInt* CBigInt::allocate(sl_size length)
{
	CBigInt* newObject = new CBigInt;
	if (newObject) {
		if (length > 0) {
			sl_uint32* elements = (sl_uint32*)(Base::createMemory(length * 4));
			if (elements) {
				newObject->m_flagUserData = sl_false;
				newObject->length = length;
				newObject->elements = elements;
				Base::zeroMemory(elements, length * 4);
				return newObject;
			}
			delete newObject;
		} else {
			return newObject;
		}
	}
	return sl_null;
}

CBigInt* CBigInt::duplicate(sl_size newLength) const
{
	CBigInt* ret = allocate(length);
	if (ret) {
		sl_size n = Math::min(length, newLength);
		if (n > 0) {
			Base::copyMemory(ret->elements, elements, n * 4);
		}
		ret->sign = sign;
		ret->length = newLength;
		return ret;
	}
	return sl_null;
}

CBigInt* CBigInt::duplicate() const
{
	return duplicate(length);
}

CBigInt* CBigInt::duplicateCompact() const
{
	return duplicate(getMostSignificantElements());
}

sl_bool CBigInt::copyAbsFrom(const CBigInt& other)
{
	if (this == &other) {
		return sl_true;
	}
	sl_size n = other.getMostSignificantElements();
	if (growLength(n)) {
		if (other.elements) {
			Base::copyMemory(elements, other.elements, n * 4);
			Base::zeroMemory(elements + n, (length - n) * 4);
		} else {
			setZero();
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool CBigInt::copyFrom(const CBigInt& other)
{
	if (copyAbsFrom(other)) {
		sign = other.sign;
		return sl_true;
	}
	return sl_false;
}

sl_bool CBigInt::compact()
{
	return setLength(getMostSignificantElements());
}

sl_bool CBigInt::growLength(sl_size newLength)
{
	if (length >= newLength) {
		return sl_true;
	}
	sl_uint32* newData = (sl_uint32*)(Base::createMemory(newLength * 4));
	if (newData) {
		if (elements) {
			Base::copyMemory(newData, elements, length * 4);
			Base::zeroMemory(newData + length, (newLength - length) * 4);
			if (!m_flagUserData) {
				Base::freeMemory(elements);
			}
		} else {
			Base::zeroMemory(newData, newLength * 4);
		}
		m_flagUserData = sl_false;
		length = newLength;
		elements = newData;
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool CBigInt::setLength(sl_size newLength)
{
	if (length < newLength) {
		return growLength(newLength);
	} else if (length == newLength) {
		return sl_true;
	} else {
		if (newLength) {
			sl_uint32* newData = (sl_uint32*)(Base::createMemory(newLength * 4));
			if (newData) {
				if (elements) {
					Base::copyMemory(newData, elements, newLength * 4);
					if (!m_flagUserData) {
						Base::freeMemory(elements);
					}
				}
				m_flagUserData = sl_false;
				length = newLength;
				elements = newData;
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			if (elements) {
				if (!m_flagUserData) {
					Base::freeMemory(elements);
				}
			}
			length = 0;
			elements = sl_null;
			return sl_true;
		}
	}
}

sl_bool CBigInt::setValueFromElements(const sl_uint32* _data, sl_size n)
{
	sl_size nd = getMostSignificantElements();
	if (growLength(n)) {
		sl_size i;
		for (i = 0; i < n; i++) {
			elements[i] = _data[i];
		}
		for (; i < nd; i++) {
			elements[i] = 0;
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool CBigInt::setBytesLE(const void* _bytes, sl_size nBytes)
{
	sl_uint8* bytes = (sl_uint8*)_bytes;
	// remove zeros
	{
		sl_size n;
		for (n = nBytes; n > 0; n--) {
			if (bytes[n - 1]) {
				break;
			}
		}
		nBytes = n;
	}
	setZero();
	if (nBytes) {
		if (growLength((nBytes + 3) >> 2)) {
			for (sl_size i = 0; i < nBytes; i++) {
				elements[i >> 2] |= ((sl_uint32)(bytes[i])) << ((i & 3) << 3);
			}
			return sl_true;
		}
	}
	return sl_false;
}

void CBigInt::setBytesLE(const Memory& mem)
{
	setBytesLE(mem.getData(), mem.getSize());
}

CBigInt* CBigInt::fromBytesLE(const void* bytes, sl_size nBytes)
{
	CBigInt* ret = CBigInt::allocate((nBytes + 3) >> 2);
	if (ret) {
		if (ret->setBytesLE(bytes, nBytes)) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

CBigInt* CBigInt::fromBytesLE(const Memory& mem)
{
	return fromBytesLE(mem.getData(), mem.getSize());
}

sl_bool CBigInt::getBytesLE(void* _bytes, sl_size n) const
{
	sl_size size = getMostSignificantBytes();
	if (n < size) {
		return sl_false;
	}
	sl_uint8* bytes = (sl_uint8*)_bytes;
	sl_size i;
	for (i = 0; i < size; i++) {
		bytes[i] = (sl_uint8)(elements[i >> 2] >> ((i & 3) << 3));
	}
	for (; i < n; i++) {
		bytes[i] = 0;
	}
	return sl_true;
}

Memory CBigInt::getBytesLE() const
{
	sl_size size = getMostSignificantBytes();
	Memory mem = Memory::create(size);
	if (mem.isNotEmpty()) {
		sl_uint8* bytes = (sl_uint8*)(mem.getData());
		for (sl_size i = 0; i < size; i++) {
			bytes[i] = (sl_uint8)(elements[i >> 2] >> ((i & 3) << 3));
		}
	}
	return mem;
}

sl_bool CBigInt::setBytesBE(const void* _bytes, sl_size nBytes)
{
	sl_uint8* bytes = (sl_uint8*)_bytes;
	// remove zeros
	{
		sl_size n;
		for (n = 0; n < nBytes; n++) {
			if (bytes[n]) {
				break;
			}
		}
		nBytes -= n;
		bytes += n;
	}
	setZero();
	if (nBytes) {
		if (growLength((nBytes + 3) >> 2)) {
			sl_size m = nBytes - 1;
			for (sl_size i = 0; i < nBytes; i++) {
				elements[i >> 2] |= ((sl_uint32)(bytes[m])) << ((i & 3) << 3);
				m--;
			}
			return sl_true;
		}
	}
	return sl_false;
}

void CBigInt::setBytesBE(const Memory& mem)
{
	setBytesBE(mem.getData(), mem.getSize());
}

CBigInt* CBigInt::fromBytesBE(const void* bytes, sl_size nBytes)
{
	CBigInt* ret = CBigInt::allocate((nBytes + 3) >> 2);
	if (ret) {
		if (ret->setBytesBE(bytes, nBytes)) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

CBigInt* CBigInt::fromBytesBE(const Memory& mem)
{
	return fromBytesBE(mem.getData(), mem.getSize());
}

sl_bool CBigInt::getBytesBE(void* _bytes, sl_size n) const
{
	sl_size size = getMostSignificantBytes();
	if (n < size) {
		return sl_false;
	}
	sl_uint8* bytes = (sl_uint8*)_bytes;
	sl_size i;
	sl_size m = n - 1;
	for (i = 0; i < size; i++) {
		bytes[m] = (sl_uint8)(elements[i >> 2] >> ((i & 3) << 3));
		m--;
	}
	for (; i < n; i++) {
		bytes[m] = 0;
		m--;
	}
	return sl_true;
}

Memory CBigInt::getBytesBE() const
{
	sl_size size = getMostSignificantBytes();
	Memory mem = Memory::create(size);
	if (mem.isNotEmpty()) {
		sl_uint8* bytes = (sl_uint8*)(mem.getData());
		sl_size m = size - 1;
		for (sl_size i = 0; i < size; i++) {
			bytes[m] = (sl_uint8)(elements[i >> 2] >> ((i & 3) << 3));
			m--;
		}
	}
	return mem;
}

sl_bool CBigInt::setValue(sl_int32 v)
{
	if (growLength(1)) {
		if (v < 0) {
			elements[0] = -v;
			sign = -1;
		} else {
			elements[0] = v;
			sign = 1;
		}
		Base::zeroMemory(elements + 1, (length - 1) * 4);
		return sl_true;
	} else {
		return sl_false;
	}
}

CBigInt* CBigInt::fromInt32(sl_int32 v)
{
	CBigInt* ret = allocate(1);
	if (ret) {
		ret->setValue(v);
		return ret;
	}
	return sl_null;
}

sl_bool CBigInt::setValue(sl_uint32 v)
{
	if (growLength(1)) {
		sign = 1;
		elements[0] = v;
		Base::zeroMemory(elements + 1, (length - 1) * 4);
		return sl_true;
	} else {
		return sl_false;
	}
}

CBigInt* CBigInt::fromUint32(sl_uint32 v)
{
	CBigInt* ret = allocate(1);
	if (ret) {
		ret->setValue(v);
		return ret;
	}
	return sl_null;
}

sl_bool CBigInt::setValue(sl_int64 v)
{
	if (growLength(2)) {
		sl_uint64 _v;
		if (v < 0) {
			_v = v;
			sign = -1;
		} else {
			_v = v;
			sign = 1;
		}
		elements[0] = (sl_uint32)(_v);
		elements[1] = (sl_uint32)(_v >> 32);
		Base::zeroMemory(elements + 2, (length - 2) * 4);
		return sl_true;
	} else {
		return sl_false;
	}
}

CBigInt* CBigInt::fromInt64(sl_int64 v)
{
	CBigInt* ret = allocate(2);
	if (ret) {
		ret->setValue(v);
		return ret;
	}
	return sl_null;
}

sl_bool CBigInt::setValue(sl_uint64 v)
{
	if (growLength(2)) {
		sign = 1;
		elements[0] = (sl_uint32)(v);
		elements[1] = (sl_uint32)(v >> 32);
		Base::zeroMemory(elements + 2, (length - 2) * 4);
		return sl_true;
	} else {
		return sl_false;
	}
}

CBigInt* CBigInt::fromUint64(sl_uint64 v)
{
	CBigInt* ret = allocate(2);
	if (ret) {
		ret->setValue(v);
		return ret;
	}
	return sl_null;
}

CBigInt* CBigInt::fromString(const String& s, sl_uint32 radix)
{
	CBigInt* ret = new CBigInt;
	if (ret) {
		ret->parse(s, radix);
	}
	return ret;
}

String CBigInt::toString(sl_uint32 radix) const
{
	if (radix < 2 || radix > 64) {
		return sl_null;
	}
	sl_size nb = getMostSignificantBits();
	if (nb == 0) {
		SLIB_STATIC_STRING(s, "0");
		return s;
	}
	if (radix == 16) {
		sl_size nh = (nb + 3) >> 2;
		sl_size ns;
		if (sign < 0) {
			ns = nh + 1;
		} else {
			ns = nh;
		}
		String ret = String::allocate(ns);
		if (ret.isNotNull()) {
			sl_char8* buf = ret.getData();
			if (sign < 0) {
				buf[0] = '-';
				buf++;
			}
			sl_size ih = nh - 1;
			for (sl_size i = 0; i < nh; i++) {
				sl_size ie = ih >> 3;
				sl_uint32 ib = (sl_uint32)((ih << 2) & 31);
				sl_uint32 vh = (sl_uint32)((elements[ie] >> ib) & 15);
				if (vh < 10) {
					buf[i] = (sl_char8)(vh + 0x30);
				} else {
					buf[i] = (sl_char8)(vh + 0x37);
				}
				ih--;
			}
		}
		return ret;
	} else {
		sl_size ne = (nb + 31) >> 5;
		sl_size n = (sl_size)(Math::ceil((nb + 1) / Math::log2((double)radix))) + 1;
		SLIB_SCOPED_BUFFER(sl_uint32, STACK_BUFFER_SIZE, a, ne);
		if (!a) {
			return sl_null;
		}
		SLIB_SCOPED_BUFFER(sl_char8, STACK_BUFFER_SIZE, s, n + 2);
		if (!s) {
			return sl_null;
		}
		s = s + n;
		s[1] = 0;
		Base::copyMemory(a, elements, ne * 4);
		sl_size l = 0;
		for (; ne > 0;) {
			sl_uint32 v = _cbigint_div_uint32(a, a, ne, radix, 0);
			ne = _cbigint_mse(a, ne);
			if (v < radix) {
				*s = _StringConv_radixPatternUpper[v];
			} else {
				*s = '?';
			}
			s--;
			l++;
		}
		if (sign < 0) {
			*s = '-';
			s--;
			l++;
		}
		return String(s + 1, l);
	}
}

CBigInt* CBigInt::fromHexString(const String& str)
{
	return fromString(str, 16);
}

String CBigInt::toHexString() const
{
	return toString(16);
}

template <class CT>
static sl_reg _CBigInt_parseString(CBigInt* _out, const CT* sz, sl_size posBegin, sl_size len, sl_uint32 radix)
{
	if (radix < 2 || radix > 64) {
		return SLIB_PARSE_ERROR;;
	}
	sl_int32 sign;
	sl_size pos = posBegin;
	if (pos < len && sz[pos] == '-') {
		pos++;
		sign = -1;
	} else {
		sign = 1;
	}
	for (; pos < len; pos++) {
		sl_int32 c = (sl_uint32)(sz[pos]);
		if (c != '\t' && c != ' ') {
			break;
		}
	}
	sl_size end = pos;
	const sl_uint8* pattern = radix <= 36 ? _StringConv_radixInversePatternSmall : _StringConv_radixInversePatternBig;
	for (; end < len; end++) {
		sl_uint32 c = (sl_uint8)(sz[end]);
		sl_uint32 v = c < 128 ? pattern[c] : 255;
		if (v >= radix) {
			break;
		}
	}
	if (end <= pos) {
		return SLIB_PARSE_ERROR;
	}
	if (!_out) {
		return end;
	}
	_out->sign = sign;
	if (radix == 16) {
		_out->setZero();
		sl_size nh = end - pos;
		sl_size ne = ((nh << 2) + 31) >> 5;
		if (!(_out->growLength(ne))) {
			return SLIB_PARSE_ERROR;
		}
		sl_uint32* elements = _out->elements;
		sl_size ih = nh - 1;
		for (; pos < end; pos++) {
			sl_uint32 c = (sl_uint8)(sz[pos]);
			sl_uint32 v = c < 128 ? pattern[c] : 255;
			if (v >= radix) {
				break;
			}
			sl_size ie = ih >> 3;
			sl_uint32 ib = (sl_uint32)((ih << 2) & 31);
			elements[ie] |= (v << ib);
			ih--;
		}
		return pos;
	} else {
		sl_size nb = (sl_size)(Math::ceil(Math::log2((double)radix) * len));
		sl_size ne = (nb + 31) >> 5;
		SLIB_SCOPED_BUFFER(sl_uint32, STACK_BUFFER_SIZE, a, ne);
		if (!a) {
			return SLIB_PARSE_ERROR;
		}
		sl_size n = 0;
		for (; pos < end; pos++) {
			sl_uint32 c = (sl_uint8)(sz[pos]);
			sl_uint32 v = c < 128 ? pattern[c] : 255;
			if (v >= radix) {
				break;
			}
			sl_uint32 o = _cbigint_mul_uint32(a, a, n, radix, v);
			if (o) {
				a[n] = o;
				n++;
			}
		}
		if (!(_out->setValueFromElements(a, n))) {
			return SLIB_PARSE_ERROR;
		}
		return pos;
	}
}

template <>
sl_reg IntParser<CBigInt, sl_char8>::parse(CBigInt* _out, sl_uint32 radix, const sl_char8 *sz, sl_size posBegin, sl_size len)
{
	return _CBigInt_parseString(_out, sz, posBegin, len, radix);
}

template <>
sl_reg IntParser<CBigInt, sl_char16>::parse(CBigInt* _out, sl_uint32 radix, const sl_char16 *sz, sl_size posBegin, sl_size len)
{
	return _CBigInt_parseString(_out, sz, posBegin, len, radix);
}

sl_int32 CBigInt::compareAbs(const CBigInt& other) const
{
	const CBigInt& a = *this;
	const CBigInt& b = other;
	sl_size na = a.getMostSignificantElements();
	sl_size nb = b.getMostSignificantElements();
	if (na > nb) {
		return 1;
	} else if (na < nb) {
		return -1;
	}
	return _cbigint_compare(a.elements, b.elements, na);
}

sl_int32 CBigInt::compare(const CBigInt& other) const
{
	const CBigInt& a = *this;
	const CBigInt& b = other;
	sl_size na = a.getMostSignificantElements();
	sl_size nb = b.getMostSignificantElements();
	if (na == 0) {
		if (nb == 0) {
			return 0;
		} else {
			return -b.sign;
		}
	} else {
		if (nb == 0) {
			return a.sign;
		}
	}
	if (a.sign >= 0 && b.sign < 0) {
		return 1;
	} else if (a.sign < 0 && b.sign >= 0) {
		return -1;
	}
	if (na > nb) {
		return a.sign;
	} else if (na < nb) {
		return -a.sign;
	}
	return _cbigint_compare(a.elements, b.elements, na) * a.sign;
}

sl_int32 CBigInt::compare(sl_int32 v) const
{
	CBIGINT_INT32(o, v);
	return compare(o);
}

sl_int32 CBigInt::compare(sl_uint32 v) const
{
	CBIGINT_UINT32(o, v);
	return compare(o);
}

sl_int32 CBigInt::compare(sl_int64 v) const
{
	CBIGINT_INT64(o, v);
	return compare(o);
}

sl_int32 CBigInt::compare(sl_uint64 v) const
{
	CBIGINT_UINT64(o, v);
	return compare(o);
}

sl_bool CBigInt::addAbs(const CBigInt& a, const CBigInt& b)
{
	sl_size na = a.getMostSignificantElements();
	sl_size nb = b.getMostSignificantElements();
	if (na == 0) {
		if (nb) {
			if (this != &b) {
				return copyAbsFrom(b);
			} else {
				return sl_true;
			}
		}
		return sl_true;
	}
	if (nb == 0) {
		if (this != &a) {
			return copyAbsFrom(a);
		}
		return sl_true;
	}
	sl_size nd;
	if (&a == this) {
		nd = na;
	} else if (&b == this) {
		nd = nb;
	} else {
		nd = getMostSignificantElements();
	}
	const CBigInt* _p;
	const CBigInt* _q;
	sl_size np, nq;
	if (na > nb) {
		_p = &b;
		np = nb;
		_q = &a;
		nq = na;
	} else {
		_p = &a;
		np = na;
		_q = &b;
		nq = nb;
	}
	const CBigInt& p = *_p;
	const CBigInt& q = *_q;
	if (growLength(nq)) {
		sl_uint32 of = _cbigint_add(elements, q.elements, p.elements, np, 0);
		if (of) {
			of = _cbigint_add_uint32(elements + np, q.elements + np, nq - np, of);
			if (of) {
				if (growLength(nq + 1)) {
					elements[nq] = of;
					nq++;
				} else {
					return sl_false;
				}
			}
		} else {
			if (elements != q.elements) {
				Base::copyMemory(elements + np, q.elements + np, (nq - np) * 4);
			}
		}
		for (sl_size i = nq; i < nd; i++) {
			elements[i] = 0;
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool CBigInt::addAbs(const CBigInt& a, sl_uint32 v)
{
	CBIGINT_UINT32(o, v);
	return addAbs(a, o);
}

sl_bool CBigInt::addAbs(const CBigInt& a, sl_uint64 v)
{
	CBIGINT_UINT64(o, v);
	return addAbs(a, o);
}

sl_bool CBigInt::add(const CBigInt& a, const CBigInt& b)
{
	if (a.sign * b.sign < 0) {
		if (a.compareAbs(b) >= 0) {
			sign = a.sign;
			return subAbs(a, b);
		} else {
			sign = - a.sign;
			return subAbs(b, a);
		}
	} else {
		sign = a.sign;
		return addAbs(a, b);
	}
}

sl_bool CBigInt::add(const CBigInt& a, sl_int32 v)
{
	CBIGINT_INT32(o, v);
	return add(a, o);
}

sl_bool CBigInt::add(const CBigInt& a, sl_uint32 v)
{
	CBIGINT_UINT32(o, v);
	return add(a, o);
}

sl_bool CBigInt::add(const CBigInt& a, sl_int64 v)
{
	CBIGINT_INT64(o, v);
	return add(a, o);
}

sl_bool CBigInt::add(const CBigInt& a, sl_uint64 v)
{
	CBIGINT_UINT64(o, v);
	return add(a, o);
}

sl_bool CBigInt::add(const CBigInt& o)
{
	return add(*this, o);
}

sl_bool CBigInt::add(sl_int32 v)
{
	return add(*this, v);
}

sl_bool CBigInt::add(sl_uint32 v)
{
	return add(*this, v);
}

sl_bool CBigInt::add(sl_int64 v)
{
	return add(*this, v);
}

sl_bool CBigInt::add(sl_uint64 v)
{
	return add(*this, v);
}

sl_bool CBigInt::subAbs(const CBigInt& a, const CBigInt& b)
{
	sl_size na = a.getMostSignificantElements();
	sl_size nb = b.getMostSignificantElements();
	if (nb == 0) {
		if (this != &a) {
			return copyAbsFrom(a);
		}
		return sl_true;
	}
	if (na < nb) {
		return sl_false;
	}
	sl_size nd;
	if (&a == this) {
		nd = na;
	} else if (&b == this) {
		nd = nb;
		if (!growLength(na)) {
			return sl_false;
		}
	} else {
		nd = getMostSignificantElements();
		if (!growLength(na)) {
			return sl_false;
		}
	}
	sl_uint32 of = _cbigint_sub(elements, a.elements, b.elements, nb, 0);
	if (of) {
		of = _cbigint_sub_uint32(elements + nb, a.elements + nb, na - nb, of);
		if (of) {
			return sl_false;
		}
	} else {
		if (elements != a.elements) {
			Base::copyMemory(elements + nb, a.elements + nb, (na - nb) * 4);
		}
	}
	for (sl_size i = na; i < nd; i++) {
		elements[i] = 0;
	}
	return sl_true;
}

sl_bool CBigInt::subAbs(const CBigInt& a, sl_uint32 v)
{
	CBIGINT_UINT32(o, v);
	return addAbs(a, o);
}

sl_bool CBigInt::subAbs(const CBigInt& a, sl_uint64 v)
{
	CBIGINT_UINT64(o, v);
	return addAbs(a, o);
}

sl_bool CBigInt::sub(const CBigInt& a, const CBigInt& b)
{
	if (a.sign * b.sign > 0) {
		if (a.compareAbs(b) >= 0) {
			sign = a.sign;
			return subAbs(a, b);
		} else {
			sign = - a.sign;
			return subAbs(b, a);
		}
	} else {
		sign = a.sign;
		return addAbs(a, b);
	}
}

sl_bool CBigInt::sub(const CBigInt& a, sl_int32 v)
{
	CBIGINT_INT32(o, v);
	return sub(a, o);
}

sl_bool CBigInt::sub(const CBigInt& a, sl_uint32 v)
{
	CBIGINT_UINT32(o, v);
	return sub(a, o);
}

sl_bool CBigInt::sub(const CBigInt& a, sl_int64 v)
{
	CBIGINT_INT64(o, v);
	return sub(a, o);
}

sl_bool CBigInt::sub(const CBigInt& a, sl_uint64 v)
{
	CBIGINT_UINT64(o, v);
	return sub(a, o);
}

sl_bool CBigInt::sub(const CBigInt& o)
{
	return sub(*this, o);
}

sl_bool CBigInt::sub(sl_int32 v)
{
	return sub(*this, v);
}

sl_bool CBigInt::sub(sl_uint32 v)
{
	return sub(*this, v);
}

sl_bool CBigInt::sub(sl_int64 v)
{
	return sub(*this, v);
}

sl_bool CBigInt::sub(sl_uint64 v)
{
	return sub(*this, v);
}

sl_bool CBigInt::mulAbs(const CBigInt& a, const CBigInt& b)
{
	sl_size na = a.getMostSignificantElements();
	sl_size nb = b.getMostSignificantElements();
	if (na == 0 || nb == 0) {
		setZero();
		return sl_true;
	}
	sl_size nd;
	if (&a == this) {
		nd = na;
	} else if (&b == this) {
		nd = nb;
	} else {
		nd = getMostSignificantElements();
	}
	sl_size n = na + nb;
	SLIB_SCOPED_BUFFER(sl_uint64, STACK_BUFFER_SIZE, out, n);
	if (!out) {
		return sl_false;
	}
	Base::zeroMemory(out, 8 * n);
	for (sl_size ib = 0; ib < nb; ib++) {
		for (sl_size ia = 0; ia < na; ia++) {
			sl_uint64 c = a.elements[ia];
			c *= b.elements[ib];
			out[ia + ib] += (sl_uint32)c;
			out[ia + ib + 1] += (sl_uint32)(c >> 32);
		}
	}
	sl_uint32 o = 0;
	sl_size i;
	sl_size m = 0;
	for (i = 0; i < n; i++) {
		sl_uint64 c = out[i] + o;
		sl_uint32 t = (sl_uint32)c;
		out[i] = t;
		if (t) {
			m = i;
		}
		o = (sl_uint32)(c >> 32);
	}
	if (growLength(m + 1)) {
		for (i = 0; i <= m; i++) {
			elements[i] = (sl_uint32)(out[i]);
		}
		for (; i < nd; i++) {
			elements[i] = 0;
		}
		return sl_true;
	}
	return sl_false;
}

sl_bool CBigInt::mul(const CBigInt& a, const CBigInt& b)
{
	sign = a.sign * b.sign;
	return mulAbs(a, b);
}

sl_bool CBigInt::mulAbs(const CBigInt& a, sl_uint32 b)
{
	sl_size na = a.getMostSignificantElements();
	if (na == 0 || b == 0) {
		setZero();
		return sl_true;
	}
	sl_size n = na + 1;
	SLIB_SCOPED_BUFFER(sl_uint32, STACK_BUFFER_SIZE, out, n);
	if (!out) {
		return sl_false;
	}
	sl_uint32 o = _cbigint_mul_uint32(out, a.elements, na, b, 0);
	if (o == 0) {
		n = na;
	} else {
		out[n - 1] = o;
	}
	return setValueFromElements(out, n);
}

sl_bool CBigInt::mul(const CBigInt& a, sl_int32 v)
{
	if (v < 0) {
		sign = -a.sign;
		v = -v;
	} else {
		sign = a.sign;
	}
	return mulAbs(a, v);
}

sl_bool CBigInt::mul(const CBigInt& a, sl_uint32 v)
{
	return mulAbs(a, v);
}

sl_bool CBigInt::mul(const CBigInt& a, sl_int64 v)
{
	CBIGINT_INT64(o, v);
	return mul(a, o);
}

sl_bool CBigInt::mul(const CBigInt& a, sl_uint64 v)
{
	CBIGINT_UINT64(o, v);
	return mul(a, o);
}

sl_bool CBigInt::mul(const CBigInt& o)
{
	return mul(*this, o);
}

sl_bool CBigInt::mul(sl_int32 v)
{
	return mul(*this, v);
}

sl_bool CBigInt::mul(sl_uint32 v)
{
	return mul(*this, v);
}

sl_bool CBigInt::mul(sl_int64 v)
{
	return mul(*this, v);
}

sl_bool CBigInt::mul(sl_uint64 v)
{
	return mul(*this, v);
}

sl_bool CBigInt::divAbs(const CBigInt& a, const CBigInt& b, CBigInt* quotient, CBigInt* remainder)
{
	sl_size nba = a.getMostSignificantBits();
	sl_size nbb = b.getMostSignificantBits();
	if (nbb == 0) {
		return sl_false;
	}
	if (nba == 0) {
		if (remainder) {
			remainder->setZero();
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	if (nba < nbb) {
		if (remainder) {
			if (!remainder->copyAbsFrom(a)) {
				return sl_false;
			}
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	sl_size na = (nba + 31) >> 5;
	sl_size nb = (nbb + 31) >> 5;
	sl_size nbc = nba - nbb;
	SLIB_SCOPED_BUFFER(sl_uint32, STACK_BUFFER_SIZE, _tmem, (nb + 1) * 31);
	if (!_tmem) {
		return sl_false;
	}
	sl_uint32* tb[32];
	sl_size tl[32];
	{
		sl_uint32 n = (sl_uint32)(Math::min((sl_size)31, nbc));
		tb[0] = (sl_uint32*)(b.elements);
		tl[0] = nb;
		for (sl_uint32 i = 1; i <= n; i++) {
			tb[i] = _tmem + ((i - 1) * (nb + 1));
			tl[i] = (nbb + i + 31) >> 5;
			sl_uint32 o = _cbigint_shiftLeft(tb[i], b.elements, nb, i, 0);
			if (o) {
				tb[i][nb] = o;
			}
		}
	}
	SLIB_SCOPED_BUFFER(sl_uint32, STACK_BUFFER_SIZE, rem, na * 2);
	if (!rem) {
		return sl_false;
	}
	Base::copyMemory(rem, a.elements, na * 4);
	sl_uint32* q = rem + na;
	Base::zeroMemory(q, na * 4);
	sl_size nbr = nba;
	sl_size shift = nbc;
	sl_size nq = 0;
	for (sl_size i = 0; i <= nbc; i++) {
		sl_size se = shift >> 5;
		sl_size sb = shift & 31;
		sl_size nbs = nbb + shift;
		if (nbs < nbr || (nbs == nbr && _cbigint_compare(rem + se, tb[sb], tl[sb]) >= 0)) {
			if (_cbigint_sub(rem + se, rem + se, tb[sb], tl[sb], 0)) {
				rem[se + tl[sb]] = 0;
			}
			q[se] |= (1 << sb);
			if (nq == 0) {
				nq = se + 1;
			}
			nbr = _cbigint_msbits(rem, se + tl[sb]);
		}
		shift--;
	}
	if (quotient) {
		if (!quotient->setValueFromElements(q, nq)) {
			return sl_false;
		}
	}
	sl_size nr = (nbr + 31) >> 5;
	if (remainder) {
		if (! remainder->setValueFromElements(rem, nr)) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool CBigInt::divAbs(const CBigInt& a, sl_uint32 b, CBigInt* quotient, sl_uint32* remainder)
{
	if (b == 0) {
		return sl_false;
	}
	sl_size na = a.getMostSignificantElements();
	if (na == 0) {
		if (remainder) {
			*remainder = 0;
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	sl_uint32* q;
	if (quotient) {
		quotient->setZero();
		if (quotient->growLength(na)) {
			q = quotient->elements;
		} else {
			return sl_false;
		}
	} else {
		q = sl_null;
	}
	sl_uint32 r = _cbigint_div_uint32(q, a.elements, na, b, 0);
	if (remainder) {
		*remainder = r;
	}
	return sl_true;
}

sl_bool CBigInt::div(const CBigInt& a, const CBigInt& b, CBigInt* quotient, CBigInt* remainder)
{
	if (divAbs(a, b, quotient, remainder)) {
		if (quotient) {
			if (a.sign < 0) {
				if (quotient->addAbs(*quotient, (sl_uint32)1)) {
					quotient->sign = -b.sign;
				} else {
					return sl_false;
				}
			} else {
				quotient->sign = b.sign;
			}
		}
		if (remainder) {
			if (a.sign < 0) {
				if (remainder->subAbs(b, *remainder)) {
					remainder->sign = 1;
				} else {
					return sl_false;
				}
			} else {
				remainder->sign = 1;
			}
		}
		return sl_true;
	} else {
		return sl_true;
	}
}

sl_bool CBigInt::div(const CBigInt& a, sl_int32 b, CBigInt* quotient, sl_uint32* remainder)
{
	sl_int32 v;
	sl_int32 s;
	if (b > 0) {
		v = b;
		s = 1;
	} else {
		v = -b;
		s = -1;
	}
	if (divAbs(a, (sl_uint32)v, quotient, remainder)) {
		if (quotient) {
			if (a.sign < 0) {
				if (quotient->addAbs(*quotient, (sl_uint32)1)) {
					quotient->sign = -s;
				} else {
					return sl_false;
				}
			} else {
				quotient->sign = s;
			}
		}
		if (remainder) {
			if (a.sign < 0) {
				*remainder = b - *remainder;
			}
		}
		return sl_true;
	} else {
		return sl_true;
	}
}

sl_bool CBigInt::div(const CBigInt& a, sl_uint32 b, CBigInt* quotient, sl_uint32* remainder)
{
	if (divAbs(a, b, quotient, remainder)) {
		if (quotient) {
			if (a.sign < 0) {
				if (quotient->addAbs(*quotient, (sl_uint32)1)) {
					quotient->sign = -1;
				} else {
					return sl_false;
				}
			} else {
				quotient->sign = 1;
			}
		}
		if (remainder) {
			if (a.sign < 0) {
				*remainder = b - *remainder;
			}
		}
		return sl_true;
	} else {
		return sl_true;
	}
}

sl_bool CBigInt::div(const CBigInt& a, sl_int64 b, CBigInt* quotient, sl_uint64* remainder)
{
	CBIGINT_INT64(o, b);
	CBigInt* r;
	if (remainder) {
		r = new CBigInt;
		if (!r) {
			return sl_false;
		}
	} else {
		r = sl_null;
	}
	if (div(a, o, quotient, r)) {
		if (remainder) {
			sl_uint8 bytes[8];
			r->getBytesLE(bytes, 8);
			*remainder = MIO::readUint64(bytes);
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool CBigInt::div(const CBigInt& a, sl_uint64 b, CBigInt* quotient, sl_uint64* remainder)
{
	CBIGINT_UINT64(o, b);
	CBigInt* r;
	if (remainder) {
		r = new CBigInt;
		if (!r) {
			return sl_false;
		}
	} else {
		r = sl_null;
	}
	if (div(a, o, quotient, r)) {
		if (remainder) {
			sl_uint8 bytes[8] = {0};
			r->getBytesLE(bytes, 8);
			*remainder = MIO::readUint64(bytes);
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool CBigInt::shiftLeft(const CBigInt& a, sl_size shift)
{
	if (shift == 0) {
		return copyFrom(a);
	}
	sl_size nba = a.getMostSignificantBits();
	sl_size nd;
	if (&a == this) {
		nd = (nba + 31) >> 5;
	} else {
		sign = a.sign;
		nd = getMostSignificantElements();
	}
	sl_size nbt = nba + shift;
	sl_size nt = (nbt + 31) >> 5;
	if (growLength(nt)) {
		sl_size se = shift >> 5;
		sl_uint32 sb = (sl_uint32)(shift & 31);
		if (se > 0 || elements != a.elements) {
			sl_size i;
			for (i = nt; i > se; i--) {
				elements[i - 1] = a.elements[i - 1 - se];
			}
			for (; i > 0; i--) {
				elements[i - 1] = 0;
			}
		}
		if (sb > 0) {
			_cbigint_shiftLeft(elements, elements, nt, sb, 0);
		}
		for (sl_size i = nt; i < nd; i++) {
			elements[i] = 0;
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool CBigInt::shiftRight(const CBigInt& a, sl_size shift)
{
	if (shift == 0) {
		return copyFrom(a);
	}	
	sl_size nba = a.getMostSignificantBits();
	if (nba <= shift) {
		setZero();
		return sl_true;
	}
	sl_size nd;
	if (&a == this) {
		nd = (nba + 31) >> 5;
	} else {
		sign = a.sign;
		nd = getMostSignificantElements();
	}
	sl_size nbt = nba - shift;
	sl_size nt = (nbt + 31) >> 5;
	if (growLength(nt)) {
		sl_size se = shift >> 5;
		sl_uint32 sb = (sl_uint32)(shift & 31);
		if (se > 0 || elements != a.elements) {
			sl_size i;
			for (i = 0; i < nt; i++) {
				elements[i] = a.elements[i + se];
			}
		}
		if (sb > 0) {
			sl_uint32 l;
			if (nt + se < a.length) {
				l = a.elements[nt + se];
			} else {
				l = 0;
			}
			_cbigint_shiftRight(elements, elements, nt, sb, l);
		}
		for (sl_size i = nt; i < nd; i++) {
			elements[i] = 0;
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool CBigInt::shiftLeft(sl_size n)
{
	return shiftLeft(*this, n);
}

sl_bool CBigInt::shiftRight(sl_size n)
{
	return shiftRight(*this, n);
}

sl_bool CBigInt::pow(const CBigInt& A, const CBigInt& E, const CBigInt* pM)
{
	if (pM) {
		sl_size nM = pM->getMostSignificantElements();
		if (nM == 0) {
			return sl_false;
		}
	}
	sl_size nbE = E.getMostSignificantBits();
	if (nbE == 0) {
		if (!setValue((sl_uint32)1)) {
			return sl_false;
		}
		sign = 1;
		return sl_true;
	}
	if (E.sign < 0) {
		return sl_false;
	}
	sl_size nA = A.getMostSignificantElements();
	if (nA == 0) {
		setZero();
		return sl_true;
	}
	CBigInt T;
	if (!T.copyFrom(A)) {
		return sl_false;
	}
	const CBigInt* TE;
	CBigInt _TE;
	if (this == &E) {
		TE = &_TE;
		if (!_TE.copyFrom(E)) {
			return sl_false;
		}
	} else {
		TE = &E;
	}
	if (!setValue((sl_uint32)1)) {
		return sl_false;
	}
	for (sl_size ib = 0; ib < nbE; ib++) {
		sl_size ke = ib >> 5;
		sl_uint32 kb = (sl_uint32)(ib & 31);
		if (((TE->elements[ke]) >> kb) & 1) {
			if (!mul(*this, T)) {
				return sl_false;
			}
			if (pM) {
				if (!CBigInt::div(*this, *pM, sl_null, this)) {
					return sl_false;
				}
			}
		}
		if (!T.mul(T, T)) {
			return sl_false;
		}
		if (pM) {
			if (!CBigInt::div(T, *pM, sl_null, &T)) {
				return sl_false;
			}
		}
	}
	return sl_true;
}

sl_bool CBigInt::pow_mod(const CBigInt& A, const CBigInt& E, const CBigInt& M)
{
	return pow(A, E, &M);
}

sl_bool CBigInt::pow_mod(const CBigInt& E, const CBigInt& M)
{
	return pow(*this, E, &M);
}

sl_bool CBigInt::pow(const CBigInt& E)
{
	return pow(*this, E);
}

sl_bool CBigInt::pow(const CBigInt& A, sl_uint32 E, const CBigInt* pM)
{
	CBIGINT_UINT32(o, E);
	return pow(A, o, pM);
}

sl_bool CBigInt::pow_mod(const CBigInt& A, sl_uint32 E, const CBigInt& M)
{
	return pow(A, E, &M);
}

sl_bool CBigInt::pow_mod(sl_uint32 E, const CBigInt& M)
{
	return pow(*this, E, &M);
}

sl_bool CBigInt::pow(sl_uint32 E)
{
	return pow(*this, E);
}

/*
	Montgomery multiplication: A = A * B * R^-1 mod M
*/
static sl_bool _cbigint_mont_mul(CBigInt& A, const CBigInt& B, const CBigInt& M, sl_uint32 MI)
{
	sl_size nM = M.length;
	sl_size nB = Math::min(nM, B.length);
	
	sl_size nOut = nM * 2 + 1;
	SLIB_SCOPED_BUFFER(sl_uint32, STACK_BUFFER_SIZE, out, nOut);
	if (!out) {
		return sl_false;
	}
	Base::zeroMemory(out, nOut * 4);
	for (sl_size i = 0; i < nM; i++) {
		// T = (T + cB*B + cM*M) / 2^(32*nM)
		sl_uint32 cB = i < A.length ? A.elements[i] : 0;
		sl_uint32 cM = (out[0] + cB * B.elements[0]) * MI;
		_cbigint_muladd_uint32(out, out, nOut, B.elements, nB, cB, 0);
		_cbigint_muladd_uint32(out, out, nOut, M.elements, nM, cM, 0);
		*out = cB;
		nOut--;
		out++;
	}
	if (!A.setValueFromElements(out, nM + 1)) {
		return sl_false;
	}
	if (A.compareAbs(M) >= 0) {
		if (!A.subAbs(A, M)) {
			return sl_false;
		}
	}
	return sl_true;
}

/*
	Montgomery reduction: A = A * R^-1 mod M
*/
SLIB_INLINE static sl_bool _cbigint_mont_reduction(CBigInt& A, const CBigInt& M, sl_uint32 MI)
{
	CBIGINT_UINT32(o, 1);
	return _cbigint_mont_mul(A, o, M, MI);
}

sl_bool CBigInt::pow_montgomery(const CBigInt& A, const CBigInt& _E, const CBigInt& _M)
{
	CBigInt M;
	M.copyFrom(_M);
	if (!M.compact()) {
		return sl_false;
	}
	sl_size nM = M.getMostSignificantElements();
	if (nM == 0) {
		return sl_false;
	}
	if (M.sign < 0) {
		return sl_false;
	}
	const CBigInt* pE;
	CBigInt __E;
	if (&_E == this) {
		pE = &__E;
		if (!__E.copyFrom(_E)) {
			return sl_false;
		}
	} else {
		pE = &_E;
	}
	const CBigInt& E = *pE;
	sl_size nE = E.getMostSignificantElements();
	if (nE == 0) {
		if (!setValue((sl_uint32)1)) {
			return sl_false;
		}
		sign = 1;
		return sl_true;
	}
	if (E.sign < 0) {
		return sl_false;
	}
	sl_size nA = A.getMostSignificantElements();
	if (nA == 0) {
		setZero();
		return sl_true;
	}

	// MI = -(M0^-1) mod (2^32)
	sl_uint32 MI;
	// initialize montgomery
	{
		sl_uint32 M0 = M.elements[0];
		sl_uint32 K = M0;
		K += ((M0 + 2) & 4) << 1;
		for (sl_uint32 i = 32; i >= 8; i /= 2) {
			K *= (2 - (M0 * K));
		}
		MI = 0 - K;
	}

	// pre-compute R^2 mod M
	// R = 2^(nM*32)
	CBigInt R2;
	{
		if (!R2.setValue((sl_uint32)1)) {
			return sl_false;
		}
		if (!R2.shiftLeft(nM * 64)) {
			return sl_false;
		}
		if (!CBigInt::divAbs(R2, M, sl_null, &R2)) {
			return sl_false;
		}
	}

	sl_bool flagNegative = A.sign < 0;
	// T = A * R^2 * R^-1 mod M = A * R mod M
	CBigInt T;
	if (!CBigInt::divAbs(A, M, sl_null, &T)) {
		return sl_false;
	}
	if (!_cbigint_mont_mul(T, R2, M, MI)) {
		return sl_false;
	}

	// C = R^2 * R^-1 mod M = R mod M
	if (!copyFrom(R2)) {
		return sl_false;
	}
	if (!_cbigint_mont_reduction(*this, M, MI)) {
		return sl_false;
	}

	sl_size nbE = E.getMostSignificantBits();
	for (sl_size ib = 0; ib < nbE; ib++) {
		sl_size ke = ib >> 5;
		sl_uint32 kb = (sl_uint32)(ib & 31);
		if (((E.elements[ke]) >> kb) & 1) {
			// C = C * T * R^-1 mod M
			if (!_cbigint_mont_mul(*this, T, M, MI)) {
				return sl_false;
			}
		}
		// T = T * T * R^-1 mod M
		if (!_cbigint_mont_mul(T, T, M, MI)) {
			return sl_false;
		}
	}
	if (!_cbigint_mont_reduction(*this, M, MI)) {
		return sl_false;
	}
	if (flagNegative && (E.elements[0] & 1) != 0) {
		sign = -1;
		if (!add(M)) {
			return sl_false;
		}
	} else {
		sign = 1;
	}
	return sl_true;
}

sl_bool CBigInt::pow_montgomery(const CBigInt& E, const CBigInt& M)
{
	return pow_montgomery(*this, E, M);
}

sl_bool CBigInt::inverseMod(const CBigInt& A, const CBigInt& M)
{
	sl_size nM = M.getMostSignificantElements();
	if (nM == 0) {
		return sl_false;
	}
	if (M.sign < 0) {
		return sl_false;
	}
	sl_size nA = A.getMostSignificantElements();
	if (nA == 0) {
		return sl_false;
	}
	CBigInt G;
	if (!G.gcd(A, M)) {
		return sl_false;
	}
	if (G.compare((sl_uint32)1) != 0) {
		return sl_false;
	}
	CBigInt Xa;
	if (!CBigInt::div(A, M, sl_null, &Xa)) {
		return sl_false;
	}
	CBigInt Xb;
	if (!Xb.copyFrom(M)) {
		return sl_false;
	}
	CBigInt T1;
	if (!T1.copyFrom(Xa)) {
		return sl_false;
	}
	if (!(T1.elements)) {
		return sl_false;
	}
	CBIGINT_INT32(T1a, 1);
	CBIGINT_INT32(T1b, 0);
	CBigInt T2;
	if (!T2.copyFrom(M)) {
		return sl_false;
	}
	if (!(T2.elements)) {
		return sl_false;
	}
	CBIGINT_INT32(T2a, 0);
	CBIGINT_INT32(T2b, 1);

	for (;;) {
		while ((T1.elements[0] & 1) == 0) {
			if (!T1.shiftRight(1)) {
				return sl_false;
			}
			if ((T1a.elements[0] & 1) != 0 || (T1b.elements[0] & 1) != 0) {
				if (!T1a.add(Xb)) {
					return sl_false;
				}
				if (!T1b.sub(Xa)) {
					return sl_false;
				}
			}
			if (!T1a.shiftRight(1)) {
				return sl_false;
			}
			if (!T1b.shiftRight(1)) {
				return sl_false;
			}
		}
		while ((T2.elements[0] & 1) == 0) {
			if (!T2.shiftRight(1)) {
				return sl_false;
			}
			if ((T2a.elements[0] & 1) != 0 || (T2b.elements[0] & 1) != 0) {
				if (!T2a.add(Xb)) {
					return sl_false;
				}
				if (!T2b.sub(Xa)) {
					return sl_false;
				}
			}
			if (!T2a.shiftRight(1)) {
				return sl_false;
			}
			if (!T2b.shiftRight(1)) {
				return sl_false;
			}
		}
		if (T1.compare(T2) >= 0) {
			if (!T1.sub(T2)) {
				return sl_false;
			}
			if (!T1a.sub(T2a)) {
				return sl_false;
			}
			if (!T1b.sub(T2b)) {
				return sl_false;
			}
		} else {
			if (!T2.sub(T1)) {
				return sl_false;
			}
			if (!T2a.sub(T1a)) {
				return sl_false;
			}
			if (!T2b.sub(T1b)) {
				return sl_false;
			}
		}
		if (T1.isZero()) {
			break;
		}
	}
	while (T2a.compare((sl_uint32)0) < 0) {
		if (!T2a.add(M)) {
			return sl_false;
		}
	}
	while (T2a.compare(M) >= 0) {
		if (!T2a.sub(M)) {
			return sl_false;
		}
	}
	if (!copyFrom(T2a)) {
		return sl_false;
	}
	sign = A.sign;
	return sl_true;
}

sl_bool CBigInt::inverseMod(const CBigInt& M)
{
	return inverseMod(*this, M);
}

sl_bool CBigInt::gcd(const CBigInt& _A, const CBigInt& _B)
{
	if (&_A == &_B) {
		sign = 1;
		return copyAbsFrom(_A);
	}
	sl_size lbA = _A.getLeastSignificantBits();
	sl_size lbB = _B.getLeastSignificantBits();
	if (lbA == 0 || lbB == 0) {
		setZero();
		return sl_true;
	}
	sl_size min_p2 = Math::min(lbA - 1, lbB - 1);
	CBigInt A, B;
	if (!A.shiftRight(_A, min_p2)) {
		return sl_false;
	}
	if (!B.shiftRight(_B, min_p2)) {
		return sl_false;
	}
	for (;;) {
		lbA = A.getLeastSignificantBits();
		if (lbA == 0) {
			break;
		}
		if (!A.shiftRight(lbA - 1)) {
			return sl_false;
		}
		lbB = B.getLeastSignificantBits();
		if (lbB == 0) {
			break;
		}
		if (!B.shiftRight(lbB - 1)) {
			return sl_false;
		}
		if (A.compareAbs(B) >= 0) {
			if (!A.subAbs(A, B)) {
				return sl_false;
			}
			if (!A.shiftRight(1)) {
				return sl_false;
			}
		} else {
			if (!B.subAbs(B, A)) {
				return sl_false;
			}
			if (!B.shiftRight(1)) {
				return sl_false;
			}
		}
	}
	if (!shiftLeft(B, min_p2)) {
		return sl_false;
	}
	sign = 1;
	return sl_true;
}

sl_bool CBigInt::gcd(const CBigInt& B)
{
	return gcd(*this, B);
}

/*
	BigInt
*/

BigInt::BigInt(sl_int32 n) : ref(CBigInt::fromInt32(n))
{
}

BigInt::BigInt(sl_uint32 n) : ref(CBigInt::fromUint32(n))
{
}

BigInt::BigInt(sl_int64 n) : ref(CBigInt::fromInt64(n))
{
}

BigInt::BigInt(sl_uint64 n) : ref(CBigInt::fromUint64(n))
{
}

BigInt BigInt::fromInt32(sl_int32 v)
{
	return CBigInt::fromInt32(v);
}

BigInt BigInt::fromUint32(sl_uint32 v)
{
	return CBigInt::fromUint32(v);
}

BigInt BigInt::fromInt64(sl_int64 v)
{
	return CBigInt::fromInt64(v);
}

BigInt BigInt::fromUint64(sl_uint64 v)
{
	return CBigInt::fromUint64(v);
}

BigInt BigInt::fromBytesLE(const void* bytes, sl_size nBytes)
{
	return CBigInt::fromBytesLE(bytes, nBytes);
}

BigInt BigInt::fromBytesLE(const Memory& mem)
{
	return CBigInt::fromBytesLE(mem.getData(), mem.getSize());
}

BigInt BigInt::fromBytesBE(const void* bytes, sl_size nBytes)
{
	return CBigInt::fromBytesBE(bytes, nBytes);
}

BigInt BigInt::fromBytesBE(const Memory& mem)
{
	return CBigInt::fromBytesBE(mem.getData(), mem.getSize());
}

BigInt BigInt::fromString(const String& str, sl_uint32 radix)
{
	return CBigInt::fromString(str, radix);
}

BigInt BigInt::fromHexString(const String& str)
{
	return CBigInt::fromString(str, 16);
}

CBigInt& BigInt::instance() const
{
	return *ref;
}

BigInt BigInt::duplicate() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->duplicate();
	}
	return sl_null;
}

BigInt BigInt::compact() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->duplicateCompact();
	}
	return sl_null;
}

sl_size BigInt::getElementsCount() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->length;
	}
	return 0;
}

sl_uint32* BigInt::getElements() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->elements;
	}
	return sl_null;
}

sl_int32 BigInt::getSign() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->sign;
	}
	return 1;
}

sl_bool BigInt::getBit(sl_uint32 pos) const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getBit(pos);
	}
	return sl_false;
}

sl_size BigInt::getMostSignificantElements() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getMostSignificantElements();
	}
	return 0;
}

sl_size BigInt::getLeastSignificantElements() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getLeastSignificantElements();
	}
	return 0;
}

sl_size BigInt::getMostSignificantBytes() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getMostSignificantBytes();
	}
	return 0;
}

sl_size BigInt::getLeastSignificantBytes() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getLeastSignificantBytes();
	}
	return 0;
}

sl_size BigInt::getMostSignificantBits() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getMostSignificantBits();
	}
	return 0;
}

sl_size BigInt::getLeastSignificantBits() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getLeastSignificantBits();
	}
	return 0;
}

sl_bool BigInt::isZero() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->isZero();
	}
	return sl_true;
}

sl_bool BigInt::isNotZero() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->isNotZero();
	}
	return sl_false;
}

sl_bool BigInt::getBytesLE(void* buf, sl_size n) const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getBytesLE(buf, n);
	}
	return sl_false;
}

Memory BigInt::getBytesLE() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getBytesLE();
	}
	return sl_null;
}

sl_bool BigInt::getBytesBE(void* buf, sl_size n) const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getBytesBE(buf, n);
	}
	return sl_false;
}

Memory BigInt::getBytesBE() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->getBytesBE();
	}
	return sl_null;
}

String BigInt::toString(sl_uint32 radix) const
{
	CBigInt* o = ref._ptr;
	if (o) {
		return o->toString(radix);
	} else {
		SLIB_STATIC_STRING(s, "0");
		return s;
	}
}

String BigInt::toHexString() const
{
	return toString(16);
}

sl_int32 BigInt::compare(const BigInt& other) const
{
	CBigInt* a = ref._ptr;
	CBigInt* b = other.ref._ptr;
	if (a) {
		if (b) {
			return a->compare(*b);
		} else {
			return a->sign;
		}
	} else {
		if (b) {
			return -(b->sign);
		} else {
			return 0;
		}
	}
}

sl_int32 BigInt::compare(sl_int32 v) const
{
	CBigInt* a = ref._ptr;
	if (a) {
		return a->compare(v);
	} else {
		if (v > 0) {
			return -1;
		} else if (v < 0) {
			return 1;
		} else {
			return 0;
		}
	}
}

sl_int32 BigInt::compare(sl_uint32 v) const
{
	CBigInt* a = ref._ptr;
	if (a) {
		return a->compare(v);
	} else {
		return -1;
	}
}

sl_int32 BigInt::compare(sl_int64 v) const
{
	CBigInt* a = ref._ptr;
	if (a) {
		return a->compare(v);
	} else {
		if (v > 0) {
			return -1;
		} else if (v < 0) {
			return 1;
		} else {
			return 0;
		}
	}
}

sl_int32 BigInt::compare(sl_uint64 v) const
{
	CBigInt* a = ref._ptr;
	if (a) {
		return a->compare(v);
	} else {
		return -1;
	}
}

BigInt BigInt::add(const BigInt& A, const BigInt& B)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* b = B.ref._ptr;
	if (b) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->add(*a, *b)) {
					return r;
				}
				delete r;
			}
		} else {
			return b;
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::add(const BigInt& other)
{
	CBigInt* a = ref._ptr;
	CBigInt* b = other.ref._ptr;
	if (b) {
		if (a) {
			return a->add(*a, *b);
		} else {
			a = b->duplicate();
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::add(const BigInt& A, sl_int32 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->add(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromInt32(v);
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::add(sl_int32 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->add(*a, v);
		} else {
			a = CBigInt::fromInt32(v);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::add(const BigInt& A, sl_uint32 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->add(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromUint32(v);
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::add(sl_uint32 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->add(*a, v);
		} else {
			a = CBigInt::fromUint32(v);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::add(const BigInt& A, sl_int64 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->add(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromInt64(v);
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::add(sl_int64 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->add(*a, v);
		} else {
			a = CBigInt::fromInt64(v);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::add(const BigInt& A, sl_uint64 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->add(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromUint64(v);
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::add(sl_uint64 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->add(*a, v);
		} else {
			a = CBigInt::fromUint64(v);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

sl_bool BigInt::increase()
{
	return add((sl_int32)1);
}

BigInt BigInt::sub(const BigInt& A, const BigInt& B)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* b = B.ref._ptr;
	if (b) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->sub(*a, *b)) {
					return r;
				}
				delete r;
			}
		} else {
			CBigInt* r = b->duplicate();
			if (r) {
				r->makeNagative();
				return r;
			}
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::sub(const BigInt& other)
{
	CBigInt* a = ref._ptr;
	CBigInt* b = other.ref._ptr;
	if (b) {
		if (a) {
			return a->sub(*a, *b);
		} else {
			a = b->duplicate();
			if (a) {
				a->makeNagative();
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::sub(const BigInt& A, sl_int32 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->sub(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromInt32(-v);
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::sub(sl_int32 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->sub(*a, v);
		} else {
			a = CBigInt::fromInt32(-v);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::sub(const BigInt& A, sl_uint32 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->sub(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromInt64(-((sl_int64)v));
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::sub(sl_uint32 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->sub(*a, v);
		} else {
			a = CBigInt::fromUint32(v);
			if (a) {
				a->makeNagative();
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::sub(const BigInt& A, sl_int64 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->sub(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			return CBigInt::fromInt64(-v);
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::sub(sl_int64 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->sub(*a, v);
		} else {
			a = CBigInt::fromInt64(-v);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::sub(const BigInt& A, sl_uint64 v)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->sub(*a, v)) {
					return r;
				}
				delete r;
			}
		} else {
			CBigInt* r = CBigInt::fromUint64(v);
			if (r) {
				r->makeNagative();
				return r;
			}
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::sub(sl_uint64 v)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			return a->sub(*a, v);
		} else {
			a = CBigInt::fromUint64(v);
			if (a) {
				a->makeNagative();
				ref = a;
				return sl_true;
			}
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

sl_bool BigInt::decrease()
{
	return add((sl_int32)-1);
}

void BigInt::makeNegative() const
{
	CBigInt* o = ref._ptr;
	if (o) {
		o->makeNagative();
	}
}

BigInt BigInt::negative() const
{
	BigInt ret = duplicate();
	ret.makeNegative();
	return ret;
}

BigInt BigInt::mul(const BigInt& A, const BigInt& B)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		CBigInt* b = B.ref._ptr;
		if (b) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->mul(*a, *b)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::mul(const BigInt& other)
{
	CBigInt* a = ref._ptr;
	CBigInt* b = other.ref._ptr;
	if (a) {
		if (b) {
			return a->mul(*a, *b);
		} else {
			a->setZero();
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::mul(const BigInt& A, sl_int32 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		if (v) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->mul(*a, v)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::mul(sl_int32 v)
{
	CBigInt* a = ref._ptr;
	if (a) {
		if (v) {
			return a->mul(*a, v);
		} else {
			a->setZero();
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::mul(const BigInt& A, sl_uint32 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		if (v) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->mul(*a, v)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::mul(sl_uint32 v)
{
	CBigInt* a = ref._ptr;
	if (a) {
		if (v) {
			return a->mul(*a, v);
		} else {
			a->setZero();
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::mul(const BigInt& A, sl_int64 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		if (v) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->mul(*a, v)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::mul(sl_int64 v)
{
	CBigInt* a = ref._ptr;
	if (a) {
		if (v) {
			return a->mul(*a, v);
		} else {
			a->setZero();
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::mul(const BigInt& A, sl_uint64 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		if (v) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->mul(*a, v)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::mul(sl_uint64 v)
{
	CBigInt* a = ref._ptr;
	if (a) {
		if (v) {
			return a->mul(*a, v);
		} else {
			a->setZero();
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::div(const BigInt& A, const BigInt& B, BigInt* remainder)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* b = B.ref._ptr;
	if (b) {
		if (a) {
			CBigInt* q = new CBigInt;
			if (q) {
				if (remainder) {
					CBigInt* r = new CBigInt;
					if (r) {
						if (CBigInt::div(*a, *b, q, r)) {
							*remainder = r;
							return q;
						}
						delete r;
					}
				} else {
					if (CBigInt::div(*a, *b, q, sl_null)) {
						return q;
					}
				}
				delete q;
			}
		}
	}
	if (remainder) {
		remainder->setNull();
	}
	return sl_null;
}

sl_bool BigInt::div(const BigInt& other, BigInt* remainder)
{
	CBigInt* a = ref._ptr;
	CBigInt* b = other.ref._ptr;
	if (b) {
		if (a) {
			if (remainder) {
				CBigInt* r = new CBigInt;
				if (r) {
					if (CBigInt::div(*a, *b, a, r)) {
						*remainder = r;
						return sl_true;
					}
					delete r;
				}
			} else {
				if (CBigInt::div(*a, *b, a, sl_null)) {
					return sl_true;
				}
			}
		} else {
			if (remainder) {
				remainder->setNull();
			}
			return sl_true;
		}
	}
	return sl_false;
}

BigInt BigInt::div(const BigInt& A, sl_int32 v, sl_uint32* remainder)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* q = new CBigInt;
			if (q) {
				if (CBigInt::div(*a, v, q, remainder)) {
					return q;
				}
				delete q;
			}
		}
	}
	if (remainder) {
		*remainder = 0;
	}
	return sl_null;
}

sl_bool BigInt::div(sl_int32 v, sl_uint32* remainder)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			if (CBigInt::div(*a, v, a, remainder)) {
				return sl_true;
			}
		} else {
			if (remainder) {
				*remainder = 0;
			}
			return sl_true;
		}
	}
	return sl_false;
}

BigInt BigInt::div(const BigInt& A, sl_uint32 v, sl_uint32* remainder)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* q = new CBigInt;
			if (q) {
				if (CBigInt::div(*a, v, q, remainder)) {
					return q;
				}
				delete q;
			}
		}
	}
	if (remainder) {
		*remainder = 0;
	}
	return sl_null;
}

sl_bool BigInt::div(sl_uint32 v, sl_uint32* remainder)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			if (CBigInt::div(*a, v, a, remainder)) {
				return sl_true;
			}
		} else {
			if (remainder) {
				*remainder = 0;
			}
			return sl_true;
		}
	}
	return sl_false;
}

BigInt BigInt::div(const BigInt& A, sl_int64 v, sl_uint64* remainder)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* q = new CBigInt;
			if (q) {
				if (CBigInt::div(*a, v, q, remainder)) {
					return q;
				}
				delete q;
			}
		}
	}
	if (remainder) {
		*remainder = 0;
	}
	return sl_null;
}

sl_bool BigInt::div(sl_int64 v, sl_uint64* remainder)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			if (CBigInt::div(*a, v, a, remainder)) {
				return sl_true;
			}
		} else {
			if (remainder) {
				*remainder = 0;
			}
			return sl_true;
		}
	}
	return sl_false;
}

BigInt BigInt::div(const BigInt& A, sl_uint64 v, sl_uint64* remainder)
{
	CBigInt* a = A.ref._ptr;
	if (v) {
		if (a) {
			CBigInt* q = new CBigInt;
			if (q) {
				if (CBigInt::div(*a, v, q, remainder)) {
					return q;
				}
				delete q;
			}
		}
	}
	if (remainder) {
		*remainder = 0;
	}
	return sl_null;
}

sl_bool BigInt::div(sl_uint64 v, sl_uint64* remainder)
{
	CBigInt* a = ref._ptr;
	if (v) {
		if (a) {
			if (CBigInt::div(*a, v, a, remainder)) {
				return sl_true;
			}
		} else {
			if (remainder) {
				*remainder = 0;
			}
			return sl_true;
		}
	}
	return sl_false;
}

BigInt BigInt::mod(const BigInt& A, const BigInt& B)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* b = B.ref._ptr;
	if (b) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (CBigInt::div(*a, *b, sl_null, r)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::mod(const BigInt& other)
{
	CBigInt* a = ref._ptr;
	CBigInt* b = other.ref._ptr;
	if (b) {
		if (a) {
			return CBigInt::div(*a, *b, sl_null, a);
		} else {
			return sl_true;
		}
	}
	return sl_false;
}

sl_uint32 BigInt::mod(const BigInt& A, sl_int32 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		sl_uint32 r = 0;
		if (CBigInt::div(*a, v, sl_null, &r)) {
			return r;
		}
	}
	return 0;
}

sl_uint32 BigInt::mod(const BigInt& A, sl_uint32 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		sl_uint32 r = 0;
		if (CBigInt::div(*a, v, sl_null, &r)) {
			return r;
		}
	}
	return 0;
}

sl_uint64 BigInt::mod(const BigInt& A, sl_int64 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		sl_uint64 r;
		if (CBigInt::div(*a, v, sl_null, &r)) {
			return r;
		}
	}
	return 0;
}

sl_uint64 BigInt::mod(const BigInt& A, sl_uint64 v)
{
	CBigInt* a = A.ref._ptr;
	if (a) {
		sl_uint64 r;
		if (CBigInt::div(*a, v, sl_null, &r)) {
			return r;
		}
	}
	return 0;
}

BigInt BigInt::shiftLeft(const BigInt& A, sl_size n)
{
	CBigInt* a = A.ref._ptr;
	if (n) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->shiftLeft(*a, n)) {
					return r;
				}
				delete r;
			}
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::shiftLeft(sl_size n)
{
	CBigInt* a = ref._ptr;
	if (n) {
		if (a) {
			return a->shiftLeft(*a, n);
		} else {
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::shiftRight(const BigInt& A, sl_size n)
{
	CBigInt* a = A.ref._ptr;
	if (n) {
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->shiftRight(*a, n)) {
					return r;
				}
				delete r;
			}
		}
	} else {
		return a;
	}
	return sl_null;
}

sl_bool BigInt::shiftRight(sl_size n)
{
	CBigInt* a = ref._ptr;
	if (n) {
		if (a) {
			return a->shiftRight(*a, n);
		} else {
			return sl_true;
		}
	} else {
		return sl_true;
	}
	return sl_false;
}

BigInt BigInt::pow(const BigInt& A, const BigInt& E, const BigInt* pM)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* e = E.ref._ptr;
	if (!e || e->isZero()) {
		return fromInt32(1);
	}
	if (a) {
		CBigInt* r = new CBigInt;
		if (r) {
			if (pM) {
				if (r->pow(*a, *e, pM->ref._ptr)) {
					return r;
				}
			} else {
				if (r->pow(*a, *e, sl_null)) {
					return r;
				}
			}
			delete r;
		}
	}
	return sl_null;
}

sl_bool BigInt::pow(const BigInt& E, const BigInt* pM)
{
	CBigInt* a = ref._ptr;
	CBigInt* e = E.ref._ptr;
	if (!e || e->isZero()) {
		if (a) {
			if (a->setValue(1)) {
				return sl_true;
			}
		} else {
			a = CBigInt::fromInt32(1);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		if (a) {
			if (pM) {
				return a->pow(*a, *e, pM->ref._ptr);
			} else {
				return a->pow(*a, *e, sl_null);
			}
		} else {
			return sl_true;
		}
	}
	return sl_false;
}

BigInt BigInt::pow(const BigInt& A, sl_uint32 E, const BigInt* pM)
{
	CBigInt* a = A.ref._ptr;
	if (E) {
		if (E == 1) {
			return a;
		}
		if (a) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (pM) {
					if (r->pow(*a, E, pM->ref._ptr)) {
						return r;
					}
				} else {
					if (r->pow(*a, E, sl_null)) {
						return r;
					}
				}
				delete r;
			}
		}
	} else {
		return fromInt32(1);
	}
	return sl_null;
}

BigInt BigInt::pow_mod(const BigInt& A, const BigInt& E, const BigInt& M)
{
	return pow(A, E, &M);
}

sl_bool BigInt::pow_mod(const BigInt& E, const BigInt& M)
{
	return pow(E, &M);
}

sl_bool BigInt::pow(sl_uint32 E, const BigInt* pM)
{
	CBigInt* a = ref._ptr;
	if (E) {
		if (E == 1) {
			sl_true;
		}
		if (a) {
			if (pM) {
				return a->pow(*a, E, pM->ref._ptr);
			} else {
				return a->pow(*a, E, sl_null);
			}
		}
	} else {
		if (a) {
			if (a->setValue(1)) {
				return sl_true;
			}
		} else {
			a = CBigInt::fromInt32(1);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	}
	return sl_false;
}

BigInt BigInt::pow_mod(const BigInt& A, sl_uint32 E, const BigInt& M)
{
	return pow(A, E, &M);
}

sl_bool BigInt::pow_mod(sl_uint32 E, const BigInt& M)
{
	return pow(E, &M);
}

BigInt BigInt::pow_montgomery(const BigInt& A, const BigInt& E, const BigInt& M)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* e = E.ref._ptr;
	CBigInt* m = M.ref._ptr;
	if (!e || e->isZero()) {
		return fromInt32(1);
	} else {
		if (m) {
			if (a) {
				CBigInt* r = new CBigInt;
				if (r) {
					if (r->pow_montgomery(*a, *e, *m)) {
						return r;
					}
					delete r;
				}
			}
		}
	}
	return sl_null;
}

sl_bool BigInt::pow_montgomery(const BigInt& E, const BigInt& M)
{
	CBigInt* a = ref._ptr;
	CBigInt* e = E.ref._ptr;
	CBigInt* m = M.ref._ptr;
	if (!e || e->isZero()) {
		if (a) {
			if (a->setValue(1)) {
				return sl_true;
			}
		} else {
			a = CBigInt::fromInt32(1);
			if (a) {
				ref = a;
				return sl_true;
			}
		}
	} else {
		if (m) {
			if (a) {
				return a->pow_montgomery(*a, *e, *m);
			} else {
				return sl_true;
			}
		}
	}
	return sl_false;
}

BigInt BigInt::gcd(const BigInt& A, const BigInt& B)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* b = B.ref._ptr;
	if (a) {
		if (b) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->gcd(*a, *b)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

BigInt BigInt::inverseMod(const BigInt& A, const BigInt& M)
{
	CBigInt* a = A.ref._ptr;
	CBigInt* m = M.ref._ptr;
	if (a) {
		if (m) {
			CBigInt* r = new CBigInt;
			if (r) {
				if (r->inverseMod(*a, *m)) {
					return r;
				}
				delete r;
			}
		}
	}
	return sl_null;
}

BigInt& BigInt::operator=(sl_int32 n)
{
	ref = CBigInt::fromInt32(n);
	return *this;
}

BigInt& BigInt::operator=(sl_uint32 n)
{
	ref = CBigInt::fromUint32(n);
	return *this;
}

BigInt& BigInt::operator=(sl_int64 n)
{
	ref = CBigInt::fromInt64(n);
	return *this;
}

BigInt& BigInt::operator=(sl_uint64 n)
{
	ref = CBigInt::fromUint64(n);
	return *this;
}

BigInt& BigInt::operator+=(const BigInt& other)
{
	add(other);
	return *this;
}

BigInt& BigInt::operator+=(sl_int32 v)
{
	add(v);
	return *this;
}

BigInt& BigInt::operator+=(sl_uint32 v)
{
	add(v);
	return *this;
}

BigInt& BigInt::operator+=(sl_int64 v)
{
	add(v);
	return *this;
}

BigInt& BigInt::operator+=(sl_uint64 v)
{
	add(v);
	return *this;
}

BigInt& BigInt::operator++()
{
	increase();
	return *this;
}

BigInt BigInt::operator++(int)
{
	BigInt r = duplicate();
	increase();
	return r;
}

BigInt& BigInt::operator-=(const BigInt& other)
{
	sub(other);
	return *this;
}

BigInt& BigInt::operator-=(sl_int32 v)
{
	sub(v);
	return *this;
}

BigInt& BigInt::operator-=(sl_uint32 v)
{
	sub(v);
	return *this;
}

BigInt& BigInt::operator-=(sl_int64 v)
{
	sub(v);
	return *this;
}

BigInt& BigInt::operator-=(sl_uint64 v)
{
	sub(v);
	return *this;
}

BigInt& BigInt::operator--()
{
	decrease();
	return *this;
}

BigInt BigInt::operator--(int)
{
	BigInt r = duplicate();
	decrease();
	return r;
}

BigInt& BigInt::operator*=(const BigInt& other)
{
	mul(other);
	return *this;
}

BigInt& BigInt::operator*=(sl_int32 v)
{
	mul(v);
	return *this;
}

BigInt& BigInt::operator*=(sl_uint32 v)
{
	mul(v);
	return *this;
}

BigInt& BigInt::operator*=(sl_int64 v)
{
	mul(v);
	return *this;
}

BigInt& BigInt::operator*=(sl_uint64 v)
{
	mul(v);
	return *this;
}

BigInt& BigInt::operator/=(const BigInt& other)
{
	div(other);
	return *this;
}

BigInt& BigInt::operator/=(sl_int32 v)
{
	div(v);
	return *this;
}

BigInt& BigInt::operator/=(sl_uint32 v)
{
	div(v);
	return *this;
}

BigInt& BigInt::operator/=(sl_int64 v)
{
	div(v);
	return *this;
}

BigInt& BigInt::operator/=(sl_uint64 v)
{
	div(v);
	return *this;
}

BigInt& BigInt::operator%=(const BigInt& other)
{
	mod(other);
	return *this;
}

BigInt& BigInt::operator%=(sl_int32 v)
{
	mod(v);
	return *this;
}

BigInt& BigInt::operator%=(sl_uint32 v)
{
	mod(v);
	return *this;
}

BigInt& BigInt::operator%=(sl_int64 v)
{
	mod(v);
	return *this;
}

BigInt& BigInt::operator%=(sl_uint64 v)
{
	mod(v);
	return *this;
}

BigInt& BigInt::operator<<=(sl_uint32 n)
{
	shiftLeft(n);
	return *this;
}

BigInt& BigInt::operator>>=(sl_uint32 n)
{
	shiftRight(n);
	return *this;
}


Atomic<BigInt>::Atomic(sl_int32 n) : ref(CBigInt::fromInt32(n))
{
}

Atomic<BigInt>::Atomic(sl_uint32 n) : ref(CBigInt::fromUint32(n))
{
}

Atomic<BigInt>::Atomic(sl_int64 n) : ref(CBigInt::fromInt64(n))
{
}

Atomic<BigInt>::Atomic(sl_uint64 n) : ref(CBigInt::fromUint64(n))
{
}

BigInt Atomic<BigInt>::duplicate() const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->duplicate();
	}
	return sl_null;
}

BigInt Atomic<BigInt>::compact() const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->duplicateCompact();
	}
	return sl_null;
}

sl_bool Atomic<BigInt>::isZero() const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->isZero();
	}
	return sl_true;
}

sl_bool Atomic<BigInt>::isNotZero() const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->isNotZero();
	}
	return sl_false;
}

sl_bool Atomic<BigInt>::getBytesLE(void* buf, sl_size n) const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->getBytesLE(buf, n);
	}
	return sl_false;
}

Memory Atomic<BigInt>::getBytesLE() const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->getBytesLE();
	}
	return sl_null;
}

sl_bool Atomic<BigInt>::getBytesBE(void* buf, sl_size n) const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->getBytesBE(buf, n);
	}
	return sl_false;
}

Memory Atomic<BigInt>::getBytesBE() const
{
	Ref<CBigInt> o(ref);
	if (o.isNotNull()) {
		return o->getBytesBE();
	}
	return sl_null;
}

sl_int32 Atomic<BigInt>::compare(const BigInt& other) const
{
	BigInt o(*this);
	return o.compare(other);
}

sl_int32 Atomic<BigInt>::compare(sl_int32 v) const
{
	BigInt o(*this);
	return o.compare(v);
}

sl_int32 Atomic<BigInt>::compare(sl_uint32 v) const
{
	BigInt o(*this);
	return o.compare(v);
}

sl_int32 Atomic<BigInt>::compare(sl_int64 v) const
{
	BigInt o(*this);
	return o.compare(v);
}

sl_int32 Atomic<BigInt>::compare(sl_uint64 v) const
{
	BigInt o(*this);
	return o.compare(v);
}

BigInt Atomic<BigInt>::negative() const
{
	BigInt o(*this);
	return o.negative();
}

AtomicBigInt& Atomic<BigInt>::operator=(sl_int32 n)
{
	ref = CBigInt::fromInt32(n);
	return *this;
}

AtomicBigInt& Atomic<BigInt>::operator=(sl_uint32 n)
{
	ref = CBigInt::fromUint32(n);
	return *this;
}

AtomicBigInt& Atomic<BigInt>::operator=(sl_int64 n)
{
	ref = CBigInt::fromInt64(n);
	return *this;
}

AtomicBigInt& Atomic<BigInt>::operator=(sl_uint64 n)
{
	ref = CBigInt::fromUint64(n);
	return *this;
}


sl_bool operator==(const BigInt& a, const BigInt& b)
{
	return a.compare(b) == 0;
}

sl_bool operator==(const BigInt& a, sl_int32 v)
{
	return a.compare(v) == 0;
}

sl_bool operator==(sl_int32 v, const BigInt& b)
{
	return b.compare(v) == 0;
}

sl_bool operator==(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) == 0;
}

sl_bool operator==(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) == 0;
}

sl_bool operator==(const BigInt& a, sl_int64 v)
{
	return a.compare(v) == 0;
}

sl_bool operator==(sl_int64 v, const BigInt& b)
{
	return b.compare(v) == 0;
}

sl_bool operator==(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) == 0;
}

sl_bool operator==(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) == 0;
}


sl_bool operator!=(const BigInt& a, const BigInt& b)
{
	return a.compare(b) != 0;
}

sl_bool operator!=(const BigInt& a, sl_int32 v)
{
	return a.compare(v) != 0;
}

sl_bool operator!=(sl_int32 v, const BigInt& b)
{
	return b.compare(v) != 0;
}

sl_bool operator!=(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) != 0;
}

sl_bool operator!=(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) != 0;
}

sl_bool operator!=(const BigInt& a, sl_int64 v)
{
	return a.compare(v) != 0;
}

sl_bool operator!=(sl_int64 v, const BigInt& b)
{
	return b.compare(v) != 0;
}

sl_bool operator!=(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) != 0;
}

sl_bool operator!=(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) != 0;
}


sl_bool operator>=(const BigInt& a, const BigInt& b)
{
	return a.compare(b) >= 0;
}

sl_bool operator>=(const BigInt& a, sl_int32 v)
{
	return a.compare(v) >= 0;
}

sl_bool operator>=(sl_int32 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}

sl_bool operator>=(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) >= 0;
}

sl_bool operator>=(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}

sl_bool operator>=(const BigInt& a, sl_int64 v)
{
	return a.compare(v) >= 0;
}

sl_bool operator>=(sl_int64 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}

sl_bool operator>=(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) >= 0;
}

sl_bool operator>=(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}


sl_bool operator>(const BigInt& a, const BigInt& b)
{
	return a.compare(b) > 0;
}

sl_bool operator>(const BigInt& a, sl_int32 v)
{
	return a.compare(v) > 0;
}

sl_bool operator>(sl_int32 v, const BigInt& b)
{
	return b.compare(v) < 0;
}

sl_bool operator>(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) > 0;
}

sl_bool operator>(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) < 0;
}

sl_bool operator>(const BigInt& a, sl_int64 v)
{
	return a.compare(v) > 0;
}

sl_bool operator>(sl_int64 v, const BigInt& b)
{
	return b.compare(v) < 0;
}

sl_bool operator>(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) > 0;
}

sl_bool operator>(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) < 0;
}


sl_bool operator<=(const BigInt& a, const BigInt& b)
{
	return a.compare(b) <= 0;
}

sl_bool operator<=(const BigInt& a, sl_int32 v)
{
	return a.compare(v) <= 0;
}

sl_bool operator<=(sl_int32 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}

sl_bool operator<=(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) <= 0;
}

sl_bool operator<=(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}

sl_bool operator<=(const BigInt& a, sl_int64 v)
{
	return a.compare(v) <= 0;
}

sl_bool operator<=(sl_int64 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}

sl_bool operator<=(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) <= 0;
}

sl_bool operator<=(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}


sl_bool operator<(const BigInt& a, const BigInt& b)
{
	return a.compare(b) < 0;
}

sl_bool operator<(const BigInt& a, sl_int32 v)
{
	return a.compare(v) < 0;
}

sl_bool operator<(sl_int32 v, const BigInt& b)
{
	return b.compare(v) > 0;
}

sl_bool operator<(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) < 0;
}

sl_bool operator<(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) > 0;
}

sl_bool operator<(const BigInt& a, sl_int64 v)
{
	return a.compare(v) < 0;
}

sl_bool operator<(sl_int64 v, const BigInt& b)
{
	return b.compare(v) > 0;
}

sl_bool operator<(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) < 0;
}

sl_bool operator<(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) > 0;
}


BigInt operator+(const BigInt& a, const BigInt& b)
{
	return BigInt::add(a, b);
}

BigInt operator+(const BigInt& a, sl_int32 v)
{
	return BigInt::add(a, v);
}

BigInt operator+(sl_int32 v, const BigInt& b)
{
	return BigInt::add(b, v);
}

BigInt operator+(const BigInt& a, sl_uint32 v)
{
	return BigInt::add(a, v);
}

BigInt operator+(sl_uint32 v, const BigInt& b)
{
	return BigInt::add(b, v);
}

BigInt operator+(const BigInt& a, sl_int64 v)
{
	return BigInt::add(a, v);
}

BigInt operator+(sl_int64 v, const BigInt& b)
{
	return BigInt::add(b, v);
}

BigInt operator+(const BigInt& a, sl_uint64 v)
{
	return BigInt::add(a, v);
}

BigInt operator+(sl_uint64 v, const BigInt& b)
{
	return BigInt::add(b, v);
}


BigInt operator-(const BigInt& a, const BigInt& b)
{
	return BigInt::sub(a, b);
}

BigInt operator-(const BigInt& a, sl_int32 v)
{
	return BigInt::sub(a, v);
}

BigInt operator-(sl_int32 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

BigInt operator-(const BigInt& a, sl_uint32 v)
{
	return BigInt::sub(a, v);
}

BigInt operator-(sl_uint32 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

BigInt operator-(const BigInt& a, sl_int64 v)
{
	return BigInt::sub(a, v);
}

BigInt operator-(sl_int64 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

BigInt operator-(const BigInt& a, sl_uint64 v)
{
	return BigInt::sub(a, v);
}

BigInt operator-(sl_uint64 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

BigInt operator-(const BigInt& a)
{
	return a.negative();
}


BigInt operator*(const BigInt& a, const BigInt& b)
{
	return BigInt::mul(a, b);
}

BigInt operator*(const BigInt& a, sl_int32 v)
{
	return BigInt::mul(a, v);
}

BigInt operator*(sl_int32 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}

BigInt operator*(const BigInt& a, sl_uint32 v)
{
	return BigInt::mul(a, v);
}

BigInt operator*(sl_uint32 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}

BigInt operator*(const BigInt& a, sl_int64 v)
{
	return BigInt::mul(a, v);
}

BigInt operator*(sl_int64 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}

BigInt operator*(const BigInt& a, sl_uint64 v)
{
	return BigInt::mul(a, v);
}

BigInt operator*(sl_uint64 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}


BigInt operator/(const BigInt& a, const BigInt& b)
{
	return BigInt::div(a, b);
}

BigInt operator/(const BigInt& a, sl_int32 v)
{
	return BigInt::div(a, v);
}

BigInt operator/(sl_int32 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromInt32(v), b);
}

BigInt operator/(const BigInt& a, sl_uint32 v)
{
	return BigInt::div(a, v);
}

BigInt operator/(sl_uint32 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromUint32(v), b);
}

BigInt operator/(const BigInt& a, sl_int64 v)
{
	return BigInt::div(a, v);
}

BigInt operator/(sl_int64 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromInt64(v), b);
}

BigInt operator/(const BigInt& a, sl_uint64 v)
{
	return BigInt::div(a, v);
}

BigInt operator/(sl_uint64 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromUint64(v), b);
}


BigInt operator%(const BigInt& a, const BigInt& b)
{
	return BigInt::mod(a, b);
}

sl_uint32 operator%(const BigInt& a, sl_int32 v)
{
	return BigInt::mod(a, v);
}

BigInt operator%(sl_int32 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromInt32(v), b);
}

sl_uint32 operator%(const BigInt& a, sl_uint32 v)
{
	return BigInt::mod(a, v);
}

BigInt operator%(sl_uint32 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromUint32(v), b);
}

sl_uint64 operator%(const BigInt& a, sl_int64 v)
{
	return BigInt::mod(a, v);
}

BigInt operator%(sl_int64 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromInt64(v), b);
}

sl_uint64 operator%(const BigInt& a, sl_uint64 v)
{
	return BigInt::mod(a, v);
}

BigInt operator%(sl_uint64 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromUint64(v), b);
}


BigInt operator<<(const BigInt& a, sl_size n)
{
	return BigInt::shiftLeft(a, n);
}

BigInt operator>>(const BigInt& a, sl_size n)
{
	return BigInt::shiftRight(a, n);
}

SLIB_MATH_NAMESPACE_END
