#ifndef CHECKHEADER_SLIB_MATH_BIGINT
#define CHECKHEADER_SLIB_MATH_BIGINT

#include "definition.h"
#include "../core/reference.h"
#include "../core/memory.h"
#include "../core/string.h"

/*************************************
	
	CBigInt - Not Thread Safe, But Fast
	BigInt - Thread Safe, But Slow

*************************************/

SLIB_MATH_NAMESPACE_BEGIN
class SLIB_EXPORT CBigInt : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(CBigInt)
public:
	CBigInt();
	~CBigInt();

	SLIB_INLINE sl_uint32 getElementsCount() const
	{
		return length;
	}

	SLIB_INLINE sl_uint32* getElements() const
	{
		return data;
	}
	SLIB_INLINE void setUserDataElements(sl_uint32* data, sl_uint32 n)
	{
		_free();
		if (n > 0) {
			this->data = data;
			this->length = n;
			this->m_flagUserData = sl_true;
		}
	}

	SLIB_INLINE sl_int32 getSign() const
	{
		return sign;
	}
	SLIB_INLINE void setSign(sl_int32 sign)
	{
		this->sign = sign;
	}
	SLIB_INLINE sl_int32 makeNagative()
	{
		sign = -sign;
		return sign;
	}

	sl_bool getBit(sl_uint32 pos) const;
	void setBit(sl_uint32 pos, sl_bool bit);

	sl_uint32 getMostSignificantElements() const;
	sl_uint32 getLeastSignificantElements() const;
	sl_uint32 getMostSignificantBytes() const;
	sl_uint32 getLeastSignificantBytes() const;
	sl_uint32 getMostSignificantBits() const;
	sl_uint32 getLeastSignificantBits() const;

	SLIB_INLINE sl_uint32 getSizeInBits() const
	{
		return getMostSignificantBits();
	}
	SLIB_INLINE sl_uint32 getSizeInBytes() const
	{
		return getMostSignificantBytes();
	}
	SLIB_INLINE sl_uint32 getSizeInElements() const
	{
		return getMostSignificantElements();
	}

	SLIB_INLINE sl_bool isZero() const
	{
		return getMostSignificantElements() == 0;
	}
	// compare returns
	//  0: equal,  negative: less than, positive: greater than
	sl_int32 compareAbs(const CBigInt& other) const;
	sl_int32 compare(const CBigInt& other) const;
	sl_int32 compare(sl_int32 v) const;
	sl_int32 compare(sl_uint32 v) const;
	sl_int32 compare(sl_int64 v) const;
	sl_int32 compare(sl_uint64 v) const;

	sl_bool addAbs(const CBigInt& a, const CBigInt& b);
	sl_bool addAbs(const CBigInt& a, sl_uint32 v);
	sl_bool addAbs(const CBigInt& a, sl_uint64 v);
	sl_bool add(const CBigInt& a, const CBigInt& b);
	sl_bool add(const CBigInt& a, sl_int32 v);
	sl_bool add(const CBigInt& a, sl_uint32 v);
	sl_bool add(const CBigInt& a, sl_int64 v);
	sl_bool add(const CBigInt& a, sl_uint64 v);
	SLIB_INLINE sl_bool add(const CBigInt& o)
	{
		return add(*this, o);
	}
	SLIB_INLINE sl_bool add(sl_int32 v)
	{
		return add(*this, v);
	}
	SLIB_INLINE sl_bool add(sl_uint32 v)
	{
		return add(*this, v);
	}
	SLIB_INLINE sl_bool add(sl_int64 v)
	{
		return add(*this, v);
	}
	SLIB_INLINE sl_bool add(sl_uint64 v)
	{
		return add(*this, v);
	}

	sl_bool subAbs(const CBigInt& a, const CBigInt& b);
	sl_bool subAbs(const CBigInt& a, sl_uint32 v);
	sl_bool subAbs(const CBigInt& a, sl_uint64 v);
	sl_bool sub(const CBigInt& a, const CBigInt& b);
	sl_bool sub(const CBigInt& a, sl_int32 v);
	sl_bool sub(const CBigInt& a, sl_uint32 v);
	sl_bool sub(const CBigInt& a, sl_int64 v);
	sl_bool sub(const CBigInt& a, sl_uint64 v);
	SLIB_INLINE sl_bool sub(const CBigInt& o)
	{
		return sub(*this, o);
	}
	SLIB_INLINE sl_bool sub(sl_int32 v)
	{
		return sub(*this, v);
	}
	SLIB_INLINE sl_bool sub(sl_uint32 v)
	{
		return sub(*this, v);
	}
	SLIB_INLINE sl_bool sub(sl_int64 v)
	{
		return sub(*this, v);
	}
	SLIB_INLINE sl_bool sub(sl_uint64 v)
	{
		return sub(*this, v);
	}

	sl_bool mulAbs(const CBigInt& a, const CBigInt& b);
	sl_bool mulAbs(const CBigInt& a, sl_uint32 v);
	sl_bool mul(const CBigInt& a, const CBigInt& b);
	sl_bool mul(const CBigInt& a, sl_int32 v);
	sl_bool mul(const CBigInt& a, sl_uint32 v);
	sl_bool mul(const CBigInt& a, sl_int64 v);
	sl_bool mul(const CBigInt& a, sl_uint64 v);
	SLIB_INLINE sl_bool mul(const CBigInt& o)
	{
		return mul(*this, o);
	}
	SLIB_INLINE sl_bool mul(sl_int32 v)
	{
		return mul(*this, v);
	}
	SLIB_INLINE sl_bool mul(sl_uint32 v)
	{
		return mul(*this, v);
	}
	SLIB_INLINE sl_bool mul(sl_int64 v)
	{
		return mul(*this, v);
	}
	SLIB_INLINE sl_bool mul(sl_uint64 v)
	{
		return mul(*this, v);
	}

	static sl_bool divAbs(const CBigInt& a, const CBigInt& b, CBigInt* quotient = sl_null, CBigInt* remainder = sl_null);
	static sl_bool divAbs(const CBigInt& a, sl_uint32 b, CBigInt* quotient = sl_null, sl_uint32* remainder = sl_null);
	static sl_bool div(const CBigInt& a, const CBigInt& b, CBigInt* quotient = sl_null, CBigInt* remainder = sl_null);
	static sl_bool div(const CBigInt& a, sl_int32 b, CBigInt* quotient = sl_null, sl_uint32* remainder = sl_null);
	static sl_bool div(const CBigInt& a, sl_uint32 b, CBigInt* quotient = sl_null, sl_uint32* remainder = sl_null);
	static sl_bool div(const CBigInt& a, sl_int64 b, CBigInt* quotient = sl_null, sl_uint64* remainder = sl_null);
	static sl_bool div(const CBigInt& a, sl_uint64 b, CBigInt* quotient = sl_null, sl_uint64* remainder = sl_null);

	sl_bool shiftLeft(const CBigInt& other, sl_uint32 n);
	sl_bool shiftRight(const CBigInt& other, sl_uint32 n);
	SLIB_INLINE sl_bool shiftLeft(sl_uint32 n)
	{
		return shiftLeft(*this, n);
	}
	SLIB_INLINE sl_bool shiftRight(sl_uint32 n)
	{
		return shiftRight(*this, n);
	}

	/*
		E > 0
		M is not null => C = A^E mod M
		M is null => C = A^E
		*/
	sl_bool _pow(const CBigInt& A, const CBigInt& E, const CBigInt* pM = sl_null);
	sl_bool _pow(const CBigInt& A, sl_uint32 E, const CBigInt* pM = sl_null);
	SLIB_INLINE sl_bool powMod(const CBigInt& A, const CBigInt& E, const CBigInt& M)
	{
		return _pow(A, E, &M);
	}
	SLIB_INLINE sl_bool powMod(const CBigInt& E, const CBigInt& M)
	{
		return _pow(*this, E, &M);
	}
	SLIB_INLINE sl_bool pow(const CBigInt& A, const CBigInt& E)
	{
		return _pow(A, E);
	}
	SLIB_INLINE sl_bool pow(const CBigInt& E)
	{
		return _pow(*this, E);
	}
	SLIB_INLINE sl_bool powMod(const CBigInt& A, sl_uint32 E, const CBigInt& M)
	{
		return _pow(A, E, &M);
	}
	SLIB_INLINE sl_bool powMod(sl_uint32 E, const CBigInt& M)
	{
		return _pow(*this, E, &M);
	}
	SLIB_INLINE sl_bool pow(const CBigInt& A, sl_uint32 E)
	{
		return _pow(A, E);
	}
	SLIB_INLINE sl_bool pow(sl_uint32 E)
	{
		return _pow(*this, E);
	}

	/*
		Exponentiation based on Montgomery Reduction
		C = A^E mod M
		Available Input:
		M - an odd value (M%2=1), M>0
		E > 0
		*/
	sl_bool pow_montgomery(const CBigInt& A, const CBigInt& E, const CBigInt& M);
	SLIB_INLINE sl_bool pow_montgomery(const CBigInt& E, const CBigInt& M)
	{
		return pow_montgomery(*this, E, M);
	}

	/*
		C = A^-1 mod M

		Available Input:
			A != 0
			M > 0
			gcd(A, M) = 1
	*/
	sl_bool inverseMod(const CBigInt& A, const CBigInt& M);
	SLIB_INLINE sl_bool inverseMod(const CBigInt& M)
	{
		return inverseMod(*this, M);
	}

	/*
		gcd - greatest common divisor
	*/
	sl_bool gcd(const CBigInt& A, const CBigInt& B);
	SLIB_INLINE sl_bool gcd(const CBigInt& B)
	{
		return gcd(*this, B);
	}

	// set/get data from/to bytes buffer (Little Endian), the sign is not changed
	sl_bool setBytesLE(const void* bytes, sl_uint32 nBytes);
	SLIB_INLINE void setBytesLE(const Memory& _mem)
	{
		Memory mem = _mem;
		setBytesLE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	sl_bool getBytesLE(void* buf, sl_uint32 n) const;
	Memory getBytesLE() const;

	// set/get data from/to bytes buffer (Big Endian), the sign is not changed
	sl_bool setBytesBE(const void* bytes, sl_uint32 nBytes);
	SLIB_INLINE void setBytesBE(const Memory& _mem)
	{
		Memory mem = _mem;
		setBytesBE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	sl_bool getBytesBE(void* buf, sl_uint32 n) const;
	Memory getBytesBE() const;

	sl_bool setValueFromElements(const sl_uint32* data, sl_uint32 n);
	void makeZero();

	SLIB_INLINE void setZero()
	{
		makeZero();
	}
	sl_bool setValue(sl_int32 v);
	sl_bool setValue(sl_uint32 v);
	sl_bool setValue(sl_int64 v);
	sl_bool setValue(sl_uint64 v);
	static CBigInt* fromInt32(sl_int32 v);
	static CBigInt* fromUint32(sl_uint32 v);
	static CBigInt* fromInt64(sl_int64 v);
	static CBigInt* fromUint64(sl_uint64 v);

	static sl_int32 parseString(CBigInt* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_uint32 radix = 10);
	static sl_int32 parseString(CBigInt* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_uint32 radix = 10);
	sl_bool parseString(const String& str, sl_uint32 radix = 10);
	static CBigInt* fromString(const String& str, sl_uint32 radix = 10);
	String toString(sl_uint32 radix = 10) const;

	SLIB_INLINE static CBigInt* fromHexString(const String& str)
	{
		return fromString(str, 16);
	}
	SLIB_INLINE String toHexString() const
	{
		return toString(16);
	}

public:
	static CBigInt* allocate(sl_uint32 length);
	CBigInt* duplicate(sl_uint32 newLength) const;
	SLIB_INLINE CBigInt* duplicate() const
	{
		return duplicate(length);
	}
	SLIB_INLINE CBigInt* duplicateCompact() const
	{
		return duplicate(getSizeInElements());
	}
	sl_bool copyAbsFrom(const CBigInt& other);
	SLIB_INLINE sl_bool copyFrom(const CBigInt& other)
	{
		if (copyAbsFrom(other)) {
			sign = other.sign;
			return sl_true;
		}
		return sl_false;
	}
	SLIB_INLINE sl_bool compact()
	{
		return setLength(getMostSignificantElements());
	}

	sl_bool growLength(sl_uint32 newLength);
	sl_bool setLength(sl_uint32 newLength);

public:
	sl_int32 sign;
	sl_uint32 length;
	sl_uint32* data;

private:
	sl_bool m_flagUserData;

private:
	void _free();
};

class SLIB_EXPORT BigInt
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(BigInt, CBigInt)
	SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(BigInt, BigInt, CBigInt, Ref<CBigInt>)

public:
	BigInt(sl_int32 n);
	BigInt(sl_uint32 n);
	BigInt(sl_int64 n);
	BigInt(sl_uint64 n);

	BigInt& operator=(sl_int32 n);
	BigInt& operator=(sl_uint32 n);
	BigInt& operator=(sl_int64 n);
	BigInt& operator=(sl_uint64 n);

public:
	SLIB_INLINE const CBigInt& instance() const
	{
		return *m_object;
	}
	SLIB_INLINE CBigInt& instance()
	{
		return *m_object;
	}

	sl_uint32 getElementsCount() const;
	sl_uint32* getElements() const;
	sl_int32 getSign() const;
	BigInt negative() const;

	sl_bool getBit(sl_uint32 pos) const;

	sl_uint32 getMostSignificantElements() const;
	sl_uint32 getLeastSignificantElements() const;
	sl_uint32 getMostSignificantBytes() const;
	sl_uint32 getLeastSignificantBytes() const;
	sl_uint32 getMostSignificantBits() const;
	sl_uint32 getLeastSignificantBits() const;
	SLIB_INLINE sl_uint32 getSizeInBits() const
	{
		return getMostSignificantBits();
	}
	SLIB_INLINE sl_uint32 getSizeInBytes() const
	{
		return getMostSignificantBytes();
	}
	SLIB_INLINE sl_uint32 getSizeInElements() const
	{
		return getMostSignificantElements();
	}

	sl_bool isZero() const;
	// compare returns
	//  0: equal,  negative: less than, positive: greater than
	sl_int32 compare(const BigInt& other) const;
	sl_int32 compare(sl_int32 v) const;
	sl_int32 compare(sl_uint32 v) const;
	sl_int32 compare(sl_int64 v) const;
	sl_int32 compare(sl_uint64 v) const;

	BigInt add(const BigInt& other) const;
	BigInt add(sl_int32 v) const;
	BigInt add(sl_uint32 v) const;
	BigInt add(sl_int64 v) const;
	BigInt add(sl_uint64 v) const;
	BigInt sub(const BigInt& other) const;
	BigInt sub(sl_int32 v) const;
	BigInt sub(sl_uint32 v) const;
	BigInt sub(sl_int64 v) const;
	BigInt sub(sl_uint64 v) const;
	BigInt mul(const BigInt& other) const;
	BigInt mul(sl_int32 v) const;
	BigInt mul(sl_uint32 v) const;
	BigInt mul(sl_int64 v) const;
	BigInt mul(sl_uint64 v) const;
	BigInt div(const BigInt& other, BigInt* remainder = sl_null) const;
	BigInt div(sl_int32 v, sl_uint32* remainder = sl_null) const;
	BigInt div(sl_uint32 v, sl_uint32* remainder = sl_null) const;
	BigInt div(sl_int64 v, sl_uint64* remainder = sl_null) const;
	BigInt div(sl_uint64 v, sl_uint64* remainder = sl_null) const;
	BigInt mod(const BigInt& other) const;
	sl_uint32 mod(sl_int32 v) const;
	sl_uint32 mod(sl_uint32 v) const;
	sl_uint64 mod(sl_int64 v) const;
	sl_uint64 mod(sl_uint64 v) const;

	BigInt shiftLeft(sl_uint32 n) const;
	BigInt shiftRight(sl_uint32 n) const;

	/*
		E > 0
		M is not null => C = A^E mod M
		M is null => C = A^E
		*/
	BigInt _pow(const BigInt& E, const BigInt* pM = sl_null);
	BigInt _pow(sl_uint32 E, const BigInt* pM = sl_null);
	SLIB_INLINE BigInt pow(const BigInt& E, const BigInt& M)
	{
		return _pow(E, &M);
	}
	SLIB_INLINE BigInt pow(sl_uint32 E, const BigInt& M)
	{
		return _pow(E, &M);
	}
	SLIB_INLINE BigInt pow(const BigInt& E)
	{
		return _pow(E);
	}
	SLIB_INLINE BigInt pow(sl_uint32 E)
	{
		return _pow(E);
	}

	/*
		Exponentiation based on Montgomery Reduction
		C = A^E mod M
		Available Input:
		M - an odd value (M%2=1), M>0
		E > 0
		*/
	BigInt pow_montgomery(const BigInt& E, const BigInt& M);

	/*
		C = A^-1 mod M

		Available Input:
		A != 0
		M > 0
		gcd(A, M) = 1
		*/
	BigInt inverseMod(const BigInt& M);

	/*
		gcd - greatest common divisor
		*/
	BigInt gcd(const BigInt& other);

	// set/get data from/to bytes buffer (Little Endian), the sign is not changed
	static BigInt fromBytesLE(const void* bytes, sl_uint32 nBytes);
	SLIB_INLINE static BigInt fromBytesLE(const Memory& _mem)
	{
		Memory mem = _mem;
		return fromBytesLE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	sl_bool getBytesLE(void* buf, sl_uint32 n) const;
	Memory getBytesLE() const;

	// set/get data from/to bytes buffer (Big Endian), the sign is not changed
	static BigInt fromBytesBE(const void* bytes, sl_uint32 nBytes);
	SLIB_INLINE static BigInt fromBytesBE(const Memory& _mem)
	{
		Memory mem = _mem;
		return fromBytesBE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	sl_bool getBytesBE(void* buf, sl_uint32 n) const;
	Memory getBytesBE() const;

public:
	BigInt duplicate() const;
	BigInt compact() const;

	static BigInt fromInt32(sl_int32 n);
	static BigInt fromUint32(sl_uint32 n);
	static BigInt fromInt64(sl_int64 n);
	static BigInt fromUint64(sl_uint64 n);

public:
	static sl_int32 parseString(BigInt* out, const char* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_uint32 radix = 10);
	static sl_int32 parseString(BigInt* out, const sl_char16* sz, sl_uint32 posBegin = 0, sl_uint32 len = SLIB_INT32_MAX, sl_uint32 radix = 10);
	static BigInt fromString(const String& str, sl_uint32 radix = 10);
	String toString(sl_uint32 radix = 10) const;

	SLIB_INLINE static BigInt fromHexString(const String& str)
	{
		return fromString(str, 16);
	}
	SLIB_INLINE String toHexString() const
	{
		return toString(16);
	}

public:
	SLIB_INLINE sl_bool operator==(const BigInt& other) const
	{
		return compare(other) == 0;
	}
	SLIB_INLINE sl_bool operator==(sl_int32 v) const
	{
		return compare(v) == 0;
	}
	SLIB_INLINE friend sl_bool operator==(sl_int32 v, const BigInt& bi)
	{
		return bi.compare(v) == 0;
	}
	SLIB_INLINE sl_bool operator==(sl_uint32 v) const
	{
		return compare(v) == 0;
	}
	SLIB_INLINE friend sl_bool operator==(sl_uint32 v, const BigInt& bi)
	{
		return bi.compare(v) == 0;
	}
	SLIB_INLINE sl_bool operator==(sl_int64 v) const
	{
		return compare(v) == 0;
	}
	SLIB_INLINE friend sl_bool operator==(sl_int64 v, const BigInt& bi)
	{
		return bi.compare(v) == 0;
	}
	SLIB_INLINE sl_bool operator==(sl_uint64 v) const
	{
		return compare(v) == 0;
	}
	SLIB_INLINE friend sl_bool operator==(sl_uint64 v, const BigInt& bi)
	{
		return bi.compare(v) == 0;
	}

	SLIB_INLINE sl_bool operator!=(const BigInt& other) const
	{
		return compare(other) != 0;
	}
	SLIB_INLINE sl_bool operator!=(sl_int32 v) const
	{
		return compare(v) != 0;
	}
	SLIB_INLINE friend sl_bool operator!=(sl_int32 v, const BigInt& bi)
	{
		return bi.compare(v) != 0;
	}
	SLIB_INLINE sl_bool operator!=(sl_uint32 v) const
	{
		return compare(v) != 0;
	}
	SLIB_INLINE friend sl_bool operator!=(sl_uint32 v, const BigInt& bi)
	{
		return bi.compare(v) != 0;
	}
	SLIB_INLINE sl_bool operator!=(sl_int64 v) const
	{
		return compare(v) != 0;
	}
	SLIB_INLINE friend sl_bool operator!=(sl_int64 v, const BigInt& bi)
	{
		return bi.compare(v) != 0;
	}
	SLIB_INLINE sl_bool operator!=(sl_uint64 v) const
	{
		return compare(v) != 0;
	}
	SLIB_INLINE friend sl_bool operator!=(sl_uint64 v, const BigInt& bi)
	{
		return bi.compare(v) != 0;
	}

	SLIB_INLINE sl_bool operator>=(const BigInt& other) const
	{
		return compare(other) >= 0;
	}
	SLIB_INLINE sl_bool operator>=(sl_int32 v) const
	{
		return compare(v) >= 0;
	}
	SLIB_INLINE friend sl_bool operator>=(sl_int32 v, const BigInt& bi)
	{
		return bi.compare(v) <= 0;
	}
	SLIB_INLINE sl_bool operator>=(sl_uint32 v) const
	{
		return compare(v) >= 0;
	}
	SLIB_INLINE friend sl_bool operator>=(sl_uint32 v, const BigInt& bi)
	{
		return bi.compare(v) <= 0;
	}
	SLIB_INLINE sl_bool operator>=(sl_int64 v) const
	{
		return compare(v) >= 0;
	}
	SLIB_INLINE friend sl_bool operator>=(sl_int64 v, const BigInt& bi)
	{
		return bi.compare(v) <= 0;
	}
	SLIB_INLINE sl_bool operator>=(sl_uint64 v) const
	{
		return compare(v) >= 0;
	}
	SLIB_INLINE friend sl_bool operator>=(sl_uint64 v, const BigInt& bi)
	{
		return bi.compare(v) <= 0;
	}

	SLIB_INLINE sl_bool operator>(const BigInt& other) const
	{
		return compare(other) > 0;
	}
	SLIB_INLINE sl_bool operator>(sl_int32 v) const
	{
		return compare(v) > 0;
	}
	SLIB_INLINE friend sl_bool operator>(sl_int32 v, const BigInt& bi)
	{
		return bi.compare(v) < 0;
	}
	SLIB_INLINE sl_bool operator>(sl_uint32 v) const
	{
		return compare(v) > 0;
	}
	SLIB_INLINE friend sl_bool operator>(sl_uint32 v, const BigInt& bi)
	{
		return bi.compare(v) < 0;
	}
	SLIB_INLINE sl_bool operator>(sl_int64 v) const
	{
		return compare(v) > 0;
	}
	SLIB_INLINE friend sl_bool operator>(sl_int64 v, const BigInt& bi)
	{
		return bi.compare(v) < 0;
	}
	SLIB_INLINE sl_bool operator>(sl_uint64 v) const
	{
		return compare(v) > 0;
	}
	SLIB_INLINE friend sl_bool operator>(sl_uint64 v, const BigInt& bi)
	{
		return bi.compare(v) < 0;
	}

	SLIB_INLINE sl_bool operator<=(const BigInt& other) const
	{
		return compare(other) <= 0;
	}
	SLIB_INLINE sl_bool operator<=(sl_int32 v) const
	{
		return compare(v) <= 0;
	}
	SLIB_INLINE friend sl_bool operator<=(sl_int32 v, const BigInt& bi)
	{
		return bi.compare(v) >= 0;
	}
	SLIB_INLINE sl_bool operator<=(sl_uint32 v) const
	{
		return compare(v) <= 0;
	}
	SLIB_INLINE friend sl_bool operator<=(sl_uint32 v, const BigInt& bi)
	{
		return bi.compare(v) >= 0;
	}
	SLIB_INLINE sl_bool operator<=(sl_int64 v) const
	{
		return compare(v) <= 0;
	}
	SLIB_INLINE friend sl_bool operator<=(sl_int64 v, const BigInt& bi)
	{
		return bi.compare(v) >= 0;
	}
	SLIB_INLINE sl_bool operator<=(sl_uint64 v) const
	{
		return compare(v) <= 0;
	}
	SLIB_INLINE friend sl_bool operator<=(sl_uint64 v, const BigInt& bi)
	{
		return bi.compare(v) >= 0;
	}

	SLIB_INLINE sl_bool operator<(const BigInt& other) const
	{
		return compare(other) < 0;
	}
	SLIB_INLINE sl_bool operator<(sl_int32 v) const
	{
		return compare(v) < 0;
	}
	SLIB_INLINE friend sl_bool operator<(sl_int32 v, const BigInt& bi)
	{
		return bi.compare(v) > 0;
	}
	SLIB_INLINE sl_bool operator<(sl_uint32 v) const
	{
		return compare(v) < 0;
	}
	SLIB_INLINE friend sl_bool operator<(sl_uint32 v, const BigInt& bi)
	{
		return bi.compare(v) > 0;
	}
	SLIB_INLINE sl_bool operator<(sl_int64 v) const
	{
		return compare(v) < 0;
	}
	SLIB_INLINE friend sl_bool operator<(sl_int64 v, const BigInt& bi)
	{
		return bi.compare(v) > 0;
	}
	SLIB_INLINE sl_bool operator<(sl_uint64 v) const
	{
		return compare(v) < 0;
	}
	SLIB_INLINE friend sl_bool operator<(sl_uint64 v, const BigInt& bi)
	{
		return bi.compare(v) > 0;
	}

	SLIB_INLINE BigInt operator+(const BigInt& other) const
	{
		return add(other);
	}
	SLIB_INLINE BigInt operator+(sl_int32 v) const
	{
		return add(v);
	}
	SLIB_INLINE friend BigInt operator+(sl_int32 v, const BigInt& bi)
	{
		return bi.add(v);
	}
	SLIB_INLINE BigInt operator+(sl_uint32 v) const
	{
		return add(v);
	}
	SLIB_INLINE friend BigInt operator+(sl_uint32 v, const BigInt& bi)
	{
		return bi.add(v);
	}
	SLIB_INLINE BigInt operator+(sl_int64 v) const
	{
		return add(v);
	}
	SLIB_INLINE friend BigInt operator+(sl_int64 v, const BigInt& bi)
	{
		return bi.add(v);
	}
	SLIB_INLINE BigInt operator+(sl_uint64 v) const
	{
		return add(v);
	}
	SLIB_INLINE friend BigInt operator+(sl_uint64 v, const BigInt& bi)
	{
		return bi.add(v);
	}

	SLIB_INLINE BigInt& operator+=(const BigInt& other)
	{
		*this = add(other);
		return *this;
	}
	SLIB_INLINE BigInt& operator+=(sl_int32 v)
	{
		*this = add(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator+=(sl_uint32 v)
	{
		*this = add(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator+(sl_int64 v)
	{
		*this = add(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator+(sl_uint64 v)
	{
		*this = add(v);
		return *this;
	}

	SLIB_INLINE BigInt& operator++()
	{
		*this = add(1);
		return *this;
	}
	SLIB_INLINE BigInt operator++(int)
	{
		BigInt ret = *this;
		*this = add(1);
		return ret;
	}

	SLIB_INLINE BigInt operator-(const BigInt& other) const
	{
		return sub(other);
	}
	SLIB_INLINE BigInt operator-(sl_int32 v) const
	{
		return sub(v);
	}
	SLIB_INLINE friend BigInt operator-(sl_int32 v, const BigInt& bi)
	{
		return bi.negative().add(v);
	}
	SLIB_INLINE BigInt operator-(sl_uint32 v) const
	{
		return sub(v);
	}
	SLIB_INLINE friend BigInt operator-(sl_uint32 v, const BigInt& bi)
	{
		return bi.negative().add(v);
	}
	SLIB_INLINE BigInt operator-(sl_int64 v) const
	{
		return sub(v);
	}
	SLIB_INLINE friend BigInt operator-(sl_int64 v, const BigInt& bi)
	{
		return bi.negative().add(v);
	}
	SLIB_INLINE BigInt operator-(sl_uint64 v) const
	{
		return sub(v);
	}
	SLIB_INLINE friend BigInt operator-(sl_uint64 v, const BigInt& bi)
	{
		return bi.negative().add(v);
	}

	SLIB_INLINE BigInt& operator-=(const BigInt& other)
	{
		*this = sub(other);
		return *this;
	}
	SLIB_INLINE BigInt& operator-=(sl_int32 v)
	{
		*this = sub(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator-=(sl_uint32 v)
	{
		*this = sub(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator-=(sl_int64 v)
	{
		*this = sub(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator-=(sl_uint64 v)
	{
		*this = sub(v);
		return *this;
	}

	SLIB_INLINE BigInt& operator--()
	{
		*this = sub(1);
		return *this;
	}
	SLIB_INLINE BigInt operator--(int)
	{
		BigInt ret = *this;
		*this = sub(1);
		return ret;
	}

	SLIB_INLINE BigInt operator*(const BigInt& other) const
	{
		return mul(other);
	}
	SLIB_INLINE BigInt operator*(sl_int32 v) const
	{
		return mul(v);
	}
	SLIB_INLINE friend BigInt operator*(sl_int32 v, const BigInt& bi)
	{
		return bi.mul(v);
	}
	SLIB_INLINE BigInt operator*(sl_uint32 v) const
	{
		return mul(v);
	}
	SLIB_INLINE friend BigInt operator*(sl_uint32 v, const BigInt& bi)
	{
		return bi.mul(v);
	}
	SLIB_INLINE BigInt operator*(sl_int64 v) const
	{
		return mul(v);
	}
	SLIB_INLINE friend BigInt operator*(sl_int64 v, const BigInt& bi)
	{
		return bi.mul(v);
	}
	SLIB_INLINE BigInt operator*(sl_uint64 v) const
	{
		return mul(v);
	}
	SLIB_INLINE friend BigInt operator*(sl_uint64 v, const BigInt& bi)
	{
		return bi.mul(v);
	}

	SLIB_INLINE BigInt& operator*=(const BigInt& other)
	{
		*this = mul(other);
		return *this;
	}
	SLIB_INLINE BigInt& operator*=(sl_int32 v)
	{
		*this = mul(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator*=(sl_uint32 v)
	{
		*this = mul(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator*=(sl_int64 v)
	{
		*this = mul(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator*=(sl_uint64 v)
	{
		*this = mul(v);
		return *this;
	}

	SLIB_INLINE BigInt operator/(const BigInt& other) const
	{
		return div(other);
	}
	SLIB_INLINE BigInt operator/(sl_int32 v) const
	{
		return div(v);
	}
	SLIB_INLINE friend BigInt operator/(sl_int32 v, const BigInt& other)
	{
		return BigInt::fromInt32(v).div(other);
	}
	SLIB_INLINE BigInt operator/(sl_uint32 v) const
	{
		return div(v);
	}
	SLIB_INLINE friend BigInt operator/(sl_uint32 v, const BigInt& other)
	{
		return BigInt::fromUint32(v).div(other);
	}
	SLIB_INLINE BigInt operator/(sl_int64 v) const
	{
		return div(v);
	}
	SLIB_INLINE friend BigInt operator/(sl_int64 v, const BigInt& other)
	{
		return BigInt::fromInt64(v).div(other);
	}
	SLIB_INLINE BigInt operator/(sl_uint64 v) const
	{
		return div(v);
	}
	SLIB_INLINE friend BigInt operator/(sl_uint64 v, const BigInt& other)
	{
		return BigInt::fromUint64(v).div(other);
	}

	SLIB_INLINE BigInt& operator/=(const BigInt& other)
	{
		*this = div(other);
		return *this;
	}
	SLIB_INLINE BigInt& operator/=(sl_int32 v)
	{
		*this = div(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator/=(sl_uint32 v)
	{
		*this = div(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator/=(sl_int64 v)
	{
		*this = div(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator/=(sl_uint64 v)
	{
		*this = div(v);
		return *this;
	}

	SLIB_INLINE BigInt operator%(const BigInt& other) const
	{
		return mod(other);
	}
	SLIB_INLINE sl_uint32 operator%(sl_int32 v) const
	{
		return mod(v);
	}
	SLIB_INLINE friend BigInt operator%(sl_int32 v, const BigInt& other)
	{
		return BigInt::fromInt32(v).mod(other);
	}
	SLIB_INLINE sl_uint32 operator%(sl_uint32 v) const
	{
		return mod(v);
	}
	SLIB_INLINE friend BigInt operator%(sl_uint32 v, const BigInt& other)
	{
		return BigInt::fromUint32(v).mod(other);
	}
	SLIB_INLINE sl_uint64 operator%(sl_int64 v) const
	{
		return mod(v);
	}
	SLIB_INLINE friend BigInt operator%(sl_int64 v, const BigInt& other)
	{
		return BigInt::fromInt64(v).mod(other);
	}
	SLIB_INLINE sl_uint64 operator%(sl_uint64 v) const
	{
		return mod(v);
	}
	SLIB_INLINE friend BigInt operator%(sl_uint64 v, const BigInt& other)
	{
		return BigInt::fromUint64(v).mod(other);
	}

	SLIB_INLINE BigInt& operator%=(const BigInt& other)
	{
		*this = mod(other);
		return *this;
	}
	SLIB_INLINE BigInt& operator%=(sl_int32 v)
	{
		*this = mod(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator%=(sl_uint32 v)
	{
		*this = mod(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator%=(sl_int64 v)
	{
		*this = mod(v);
		return *this;
	}
	SLIB_INLINE BigInt& operator%=(sl_uint64 v)
	{
		*this = mod(v);
		return *this;
	}

	SLIB_INLINE BigInt operator-() const
	{
		return negative();
	}

	SLIB_INLINE BigInt operator<<(sl_uint32 n) const
	{
		return shiftLeft(n);
	}
	SLIB_INLINE BigInt& operator<<=(sl_uint32 n)
	{
		*this = shiftLeft(n);
		return *this;
	}

	SLIB_INLINE BigInt operator>>(sl_uint32 n) const
	{
		return shiftRight(n);
	}
	SLIB_INLINE BigInt& operator>>=(sl_uint32 n)
	{
		*this = shiftRight(n);
		return *this;
	}

};

SLIB_MATH_NAMESPACE_END

#endif

