#ifndef CHECKHEADER_SLIB_MATH_BIGINT
#define CHECKHEADER_SLIB_MATH_BIGINT

#include "definition.h"

#include "../core/reference.h"
#include "../core/memory.h"
#include "../core/string.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	Notice:
		
		CBigInt, BigInt is not thread-safe on modification operations
*/

class SLIB_EXPORT CBigInt : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(CBigInt)
public:
	CBigInt();
	
	~CBigInt();

public:
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
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return getMostSignificantElements() != 0;
	}
	
	void setZero();
	
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
	
	sl_bool setValueFromElements(const sl_uint32* data, sl_uint32 n);
	
public:
	
	// set/get data from/to bytes buffer (Little Endian), the sign is not changed
	sl_bool setBytesLE(const void* bytes, sl_uint32 nBytes);
	
	SLIB_INLINE void setBytesLE(const Memory& mem)
	{
		setBytesLE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	
	static CBigInt* fromBytesLE(const void* bytes, sl_uint32 nBytes);
	
	SLIB_INLINE static CBigInt* fromBytesLE(const Memory& mem)
	{
		return fromBytesLE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	
	sl_bool getBytesLE(void* buf, sl_uint32 n) const;
	
	Memory getBytesLE() const;
	
	
	// set/get data from/to bytes buffer (Big Endian), the sign is not changed
	sl_bool setBytesBE(const void* bytes, sl_uint32 nBytes);
	
	SLIB_INLINE void setBytesBE(const Memory& mem)
	{
		setBytesBE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	
	static CBigInt* fromBytesBE(const void* bytes, sl_uint32 nBytes);
	
	SLIB_INLINE static CBigInt* fromBytesBE(const Memory& mem)
	{
		return fromBytesBE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	
	sl_bool getBytesBE(void* buf, sl_uint32 n) const;
	
	Memory getBytesBE() const;
	
	
	sl_bool setValue(sl_int32 v);
	
	static CBigInt* fromInt32(sl_int32 v);
	
	sl_bool setValue(sl_uint32 v);
	
	static CBigInt* fromUint32(sl_uint32 v);
	
	sl_bool setValue(sl_int64 v);
	
	static CBigInt* fromInt64(sl_int64 v);
	
	sl_bool setValue(sl_uint64 v);
	
	static CBigInt* fromUint64(sl_uint64 v);
	
public:
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
	sl_bool pow(const CBigInt& A, const CBigInt& E, const CBigInt* pM = sl_null);
	
	SLIB_INLINE sl_bool pow_mod(const CBigInt& A, const CBigInt& E, const CBigInt& M)
	{
		return pow(A, E, &M);
	}
	
	SLIB_INLINE sl_bool pow_mod(const CBigInt& E, const CBigInt& M)
	{
		return pow(*this, E, &M);
	}
	
	SLIB_INLINE sl_bool pow(const CBigInt& E)
	{
		return pow(*this, E);
	}
	
	sl_bool pow(const CBigInt& A, sl_uint32 E, const CBigInt* pM = sl_null);

	SLIB_INLINE sl_bool pow_mod(const CBigInt& A, sl_uint32 E, const CBigInt& M)
	{
		return pow(A, E, &M);
	}
	
	SLIB_INLINE sl_bool pow_mod(sl_uint32 E, const CBigInt& M)
	{
		return pow(*this, E, &M);
	}
	
	SLIB_INLINE sl_bool pow(sl_uint32 E)
	{
		return pow(*this, E);
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

public:
	sl_int32 sign;
	sl_uint32 length;
	sl_uint32* data;

private:
	sl_bool m_flagUserData;

private:
	void _free();
};

class SafeBigInt;

class SLIB_EXPORT BigInt
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(BigInt, CBigInt)
	SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(BigInt, BigInt, CBigInt, Ref<CBigInt>)

public:
	BigInt(const SafeBigInt& other);
	
	SLIB_INLINE BigInt(sl_int32 n) : m_object(CBigInt::fromInt32(n))
	{
	}
	
	SLIB_INLINE BigInt(sl_uint32 n) : m_object(CBigInt::fromUint32(n))
	{
	}
	
	SLIB_INLINE BigInt(sl_int64 n) : m_object(CBigInt::fromInt64(n))
	{
	}
	
	SLIB_INLINE BigInt(sl_uint64 n) : m_object(CBigInt::fromUint64(n))
	{
	}

public:
	SLIB_INLINE BigInt& operator=(const BigInt& other)
	{
		m_object = other.m_object;
		return *this;
	}
	
	SLIB_INLINE BigInt& operator=(const CBigInt* other)
	{
		m_object = other;
		return *this;
	}
	
	BigInt& operator=(const SafeBigInt& other);
	
	SLIB_INLINE BigInt& operator=(sl_int32 n)
	{
		m_object = CBigInt::fromInt32(n);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator=(sl_uint32 n)
	{
		m_object = CBigInt::fromUint32(n);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator=(sl_int64 n)
	{
		m_object = CBigInt::fromInt64(n);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator=(sl_uint64 n)
	{
		m_object = CBigInt::fromUint64(n);
		return *this;
	}
	
public:
	SLIB_INLINE static BigInt fromInt32(sl_int32 v)
	{
		return CBigInt::fromInt32(v);
	}
	
	SLIB_INLINE static BigInt fromUint32(sl_uint32 v)
	{
		return CBigInt::fromUint32(v);
	}
	
	SLIB_INLINE static BigInt fromInt64(sl_int64 v)
	{
		return CBigInt::fromInt64(v);
	}
	
	SLIB_INLINE static BigInt fromUint64(sl_uint64 v)
	{
		return CBigInt::fromUint64(v);
	}

	SLIB_INLINE static BigInt fromBytesLE(const void* bytes, sl_uint32 nBytes)
	{
		return CBigInt::fromBytesLE(bytes, nBytes);
	}
	
	SLIB_INLINE static BigInt fromBytesLE(const Memory& mem)
	{
		return CBigInt::fromBytesLE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	
	SLIB_INLINE static BigInt fromBytesBE(const void* bytes, sl_uint32 nBytes)
	{
		return CBigInt::fromBytesBE(bytes, nBytes);
	}
	
	SLIB_INLINE static BigInt fromBytesBE(const Memory& mem)
	{
		return CBigInt::fromBytesBE(mem.getBuf(), (sl_uint32)(mem.getSize()));
	}
	
	SLIB_INLINE static BigInt fromString(const String& str, sl_uint32 radix = 10)
	{
		return CBigInt::fromString(str, radix);
	}
	
	SLIB_INLINE static BigInt fromHexString(const String& str)
	{
		return CBigInt::fromString(str, 16);
	}
	
public:
	SLIB_INLINE const CBigInt& instance() const
	{
		return *m_object;
	}
	
	SLIB_INLINE CBigInt& instance()
	{
		return *m_object;
	}
	
public:
	SLIB_INLINE BigInt duplicate() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->duplicate();
		}
		return BigInt::null();
	}
	
	SLIB_INLINE BigInt compact() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->duplicateCompact();
		}
		return BigInt::null();
	}
	
public:
	SLIB_INLINE sl_uint32 getElementsCount() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getElementsCount();
		}
		return 0;
	}
	
	SLIB_INLINE sl_uint32* getElements() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getElements();
		}
		return sl_null;
	}
	
	SLIB_INLINE sl_int32 getSign() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getSign();
		}
		return 1;
	}
	
	SLIB_INLINE sl_bool getBit(sl_uint32 pos) const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getBit(pos);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_uint32 getMostSignificantElements() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getMostSignificantElements();
		}
		return 0;
	}
	
	SLIB_INLINE sl_uint32 getLeastSignificantElements() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getLeastSignificantElements();
		}
		return 0;
	}
	
	SLIB_INLINE sl_uint32 getMostSignificantBytes() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getMostSignificantBytes();
		}
		return 0;
	}
	
	SLIB_INLINE sl_uint32 getLeastSignificantBytes() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getLeastSignificantBytes();
		}
		return 0;
	}
	
	SLIB_INLINE sl_uint32 getMostSignificantBits() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getMostSignificantBits();
		}
		return 0;
	}
	
	SLIB_INLINE sl_uint32 getLeastSignificantBits() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getLeastSignificantBits();
		}
		return 0;
	}
	
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
		CBigInt* o = m_object.get();
		if (o) {
			return o->isZero();
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool isNotZero() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->isNotZero();
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool getBytesLE(void* buf, sl_uint32 n) const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getBytesLE(buf, n);
		}
		return sl_false;
	}
	
	SLIB_INLINE Memory getBytesLE() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getBytesLE();
		}
		return Memory::null();
	}
	
	SLIB_INLINE sl_bool getBytesBE(void* buf, sl_uint32 n) const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getBytesBE(buf, n);
		}
		return sl_false;
	}
	
	SLIB_INLINE Memory getBytesBE() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			return o->getBytesBE();
		}
		return Memory::null();
	}
	
	String toString(sl_uint32 radix = 10) const;
	
	SLIB_INLINE String toHexString() const
	{
		return toString(16);
	}
	
public:
	// compare returns
	//  0: equal,  negative: less than, positive: greater than
	sl_int32 compare(const BigInt& other) const;
	
	sl_int32 compare(sl_int32 v) const;
	
	sl_int32 compare(sl_uint32 v) const;
	
	sl_int32 compare(sl_int64 v) const;
	
	sl_int32 compare(sl_uint64 v) const;
	

	static BigInt add(const BigInt& A, const BigInt& B);
	
	sl_bool add(const BigInt& other);
	
	static BigInt add(const BigInt& A, sl_int32 v);
	
	sl_bool add(sl_int32 v);
	
	static BigInt add(const BigInt& A, sl_uint32 v);
	
	sl_bool add(sl_uint32 v);
	
	static BigInt add(const BigInt& A, sl_int64 v);

	sl_bool add(sl_int64 v);
	
	static BigInt add(const BigInt& A, sl_uint64 v);

	sl_bool add(sl_uint64 v);
	
	SLIB_INLINE sl_bool increase()
	{
		return add((sl_int32)1);
	}
	
	
	static BigInt sub(const BigInt& A, const BigInt& B);

	sl_bool sub(const BigInt& other);
	
	static BigInt sub(const BigInt& A, sl_int32 v);
	
	sl_bool sub(sl_int32 v);
	
	static BigInt sub(const BigInt& A, sl_uint32 v);
	
	sl_bool sub(sl_uint32 v);
	
	static BigInt sub(const BigInt& A, sl_int64 v);

	sl_bool sub(sl_int64 v);
	
	static BigInt sub(const BigInt& A, sl_uint64 v);

	sl_bool sub(sl_uint64 v);
	
	SLIB_INLINE sl_bool decrease()
	{
		return add((sl_int32)-1);
	}

	
	SLIB_INLINE void makeNegative() const
	{
		CBigInt* o = m_object.get();
		if (o) {
			o->makeNagative();
		}
	}
	
	SLIB_INLINE BigInt negative() const
	{
		BigInt ret = duplicate();
		ret.makeNegative();
		return ret;
	}

	static BigInt mul(const BigInt& A, const BigInt& B);
	
	sl_bool mul(const BigInt& other);

	static BigInt mul(const BigInt& A, sl_int32 v);
	
	sl_bool mul(sl_int32 v);
	
	static BigInt mul(const BigInt& A, sl_uint32 v);
	
	sl_bool mul(sl_uint32 v);
	
	static BigInt mul(const BigInt& A, sl_int64 v);
	
	sl_bool mul(sl_int64 v);
	
	static BigInt mul(const BigInt& A, sl_uint64 v);
	
	sl_bool mul(sl_uint64 v);
	
	
	static BigInt div(const BigInt& A, const BigInt& B, BigInt* remainder = sl_null);
	
	sl_bool div(const BigInt& other, BigInt* remainder = sl_null);
	
	static BigInt div(const BigInt& A, sl_int32 v, sl_uint32* remainder = sl_null);
	
	sl_bool div(sl_int32 v, sl_uint32* remainder = sl_null);
	
	static BigInt div(const BigInt& A, sl_uint32 v, sl_uint32* remainder = sl_null);
	
	sl_bool div(sl_uint32 v, sl_uint32* remainder = sl_null);
	
	static BigInt div(const BigInt& A, sl_int64 v, sl_uint64* remainder = sl_null);
	
	sl_bool div(sl_int64 v, sl_uint64* remainder = sl_null);
	
	static BigInt div(const BigInt& A, sl_uint64 v, sl_uint64* remainder = sl_null);
	
	sl_bool div(sl_uint64 v, sl_uint64* remainder = sl_null);
	
	
	static BigInt mod(const BigInt& A, const BigInt& B);

	sl_bool mod(const BigInt& other);
	
	static sl_uint32 mod(const BigInt& A, sl_int32 v);
	
	static sl_uint32 mod(const BigInt& A, sl_uint32 v);
	
	static sl_uint64 mod(const BigInt& A, sl_int64 v);
	
	static sl_uint64 mod(const BigInt& A, sl_uint64 v);

	
	static BigInt shiftLeft(const BigInt& A, sl_uint32 n);
	
	sl_bool shiftLeft(sl_uint32 n);
	
	static BigInt shiftRight(const BigInt& A, sl_uint32 n);
	
	sl_bool shiftRight(sl_uint32 n);

	
	/*
		E > 0
		M is not null => C = A^E mod M
		M is null => C = A^E
	*/
	static BigInt pow(const BigInt& A, const BigInt& E, const BigInt* pM = sl_null);
	
	sl_bool pow(const BigInt& E, const BigInt* pM = sl_null);
	
	SLIB_INLINE static BigInt pow_mod(const BigInt& A, const BigInt& E, const BigInt& M)
	{
		return pow(A, E, &M);
	}

	SLIB_INLINE sl_bool pow_mod(const BigInt& E, const BigInt& M)
	{
		return pow(E, &M);
	}
	
	static BigInt pow(const BigInt& A, sl_uint32 E, const BigInt* pM = sl_null);
	
	sl_bool pow(sl_uint32 E, const BigInt* pM = sl_null);
	
	SLIB_INLINE static BigInt pow_mod(const BigInt& A, sl_uint32 E, const BigInt& M)
	{
		return pow(A, E, &M);
	}
	
	SLIB_INLINE sl_bool pow_mod(sl_uint32 E, const BigInt& M)
	{
		return pow(E, &M);
	}
	
	
	/*
		Exponentiation based on Montgomery Reduction
			C = A^E mod M
		Available Input:
			M - an odd value (M%2=1), M>0
			E > 0
	*/
	static BigInt pow_montgomery(const BigInt& A, const BigInt& E, const BigInt& M);
	
	sl_bool pow_montgomery(const BigInt& E, const BigInt& M);

	
	/*
		C = A^-1 mod M

		Available Input:
			A != 0
			M > 0
			gcd(A, M) = 1
	*/
	static BigInt inverseMod(const BigInt& A, const BigInt& M);

	/*
		gcd - greatest common divisor
	*/
	static BigInt gcd(const BigInt& A, const BigInt& B);

	
public:
	SLIB_INLINE BigInt& operator+=(const BigInt& other)
	{
		add(other);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator+=(sl_int32 v)
	{
		add(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator+=(sl_uint32 v)
	{
		add(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator+=(sl_int64 v)
	{
		add(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator+=(sl_uint64 v)
	{
		add(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator++()
	{
		increase();
		return *this;
	}
	
	SLIB_INLINE BigInt operator++(int)
	{
		BigInt r = duplicate();
		increase();
		return r;
	}

	
	SLIB_INLINE BigInt& operator-=(const BigInt& other)
	{
		sub(other);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator-=(sl_int32 v)
	{
		sub(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator-=(sl_uint32 v)
	{
		sub(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator-=(sl_int64 v)
	{
		sub(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator-=(sl_uint64 v)
	{
		sub(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator--()
	{
		decrease();
		return *this;
	}
	
	SLIB_INLINE BigInt operator--(int)
	{
		BigInt r = duplicate();
		decrease();
		return r;
	}
	
	
	SLIB_INLINE BigInt& operator*=(const BigInt& other)
	{
		mul(other);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator*=(sl_int32 v)
	{
		mul(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator*=(sl_uint32 v)
	{
		mul(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator*=(sl_int64 v)
	{
		mul(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator*=(sl_uint64 v)
	{
		mul(v);
		return *this;
	}
	
	
	SLIB_INLINE BigInt& operator/=(const BigInt& other)
	{
		div(other);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator/=(sl_int32 v)
	{
		div(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator/=(sl_uint32 v)
	{
		div(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator/=(sl_int64 v)
	{
		div(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator/=(sl_uint64 v)
	{
		div(v);
		return *this;
	}
	
	
	SLIB_INLINE BigInt& operator%=(const BigInt& other)
	{
		mod(other);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator%=(sl_int32 v)
	{
		mod(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator%=(sl_uint32 v)
	{
		mod(v);
		return *this;
	}

	SLIB_INLINE BigInt& operator%=(sl_int64 v)
	{
		mod(v);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator%=(sl_uint64 v)
	{
		mod(v);
		return *this;
	}

	
	SLIB_INLINE BigInt& operator<<=(sl_uint32 n)
	{
		shiftLeft(n);
		return *this;
	}
	
	SLIB_INLINE BigInt& operator>>=(sl_uint32 n)
	{
		shiftRight(n);
		return *this;
	}

};

class SLIB_EXPORT SafeBigInt
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(SafeBigInt, CBigInt)
	SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(SafeBigInt, SafeBigInt, CBigInt, SafeRef<CBigInt>)
	
public:
	SLIB_INLINE SafeBigInt(const BigInt& other) : m_object(other.getRef())
	{
	}
	
	SLIB_INLINE SafeBigInt(sl_int32 n) : m_object(CBigInt::fromInt32(n))
	{
	}
	
	SLIB_INLINE SafeBigInt(sl_uint32 n) : m_object(CBigInt::fromUint32(n))
	{
	}
	
	SLIB_INLINE SafeBigInt(sl_int64 n) : m_object(CBigInt::fromInt64(n))
	{
	}
	
	SLIB_INLINE SafeBigInt(sl_uint64 n) : m_object(CBigInt::fromUint64(n))
	{
	}
	
public:
	SLIB_INLINE SafeBigInt& operator=(const SafeBigInt& other)
	{
		m_object = other.m_object;
		return *this;
	}
	
	SLIB_INLINE SafeBigInt& operator=(const CBigInt* other)
	{
		m_object = other;
		return *this;
	}
	
	SLIB_INLINE SafeBigInt& operator=(const BigInt& other)
	{
		m_object = other.getRef();
		return *this;
	}
	
	SLIB_INLINE SafeBigInt& operator=(sl_int32 n)
	{
		m_object = CBigInt::fromInt32(n);
		return *this;
	}
	
	SLIB_INLINE SafeBigInt& operator=(sl_uint32 n)
	{
		m_object = CBigInt::fromUint32(n);
		return *this;
	}
	
	SLIB_INLINE SafeBigInt& operator=(sl_int64 n)
	{
		m_object = CBigInt::fromInt64(n);
		return *this;
	}
	
	SLIB_INLINE SafeBigInt& operator=(sl_uint64 n)
	{
		m_object = CBigInt::fromUint64(n);
		return *this;
	}
	
public:
	SLIB_INLINE BigInt duplicate() const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->duplicate();
		}
		return BigInt::null();
	}
	
	SLIB_INLINE BigInt compact() const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->duplicateCompact();
		}
		return BigInt::null();
	}
	
public:
	SLIB_INLINE sl_bool isZero() const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->isZero();
		}
		return sl_true;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->isNotZero();
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool getBytesLE(void* buf, sl_uint32 n) const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->getBytesLE(buf, n);
		}
		return sl_false;
	}
	
	SLIB_INLINE Memory getBytesLE() const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->getBytesLE();
		}
		return Memory::null();
	}
	
	SLIB_INLINE sl_bool getBytesBE(void* buf, sl_uint32 n) const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->getBytesBE(buf, n);
		}
		return sl_false;
	}
	
	SLIB_INLINE Memory getBytesBE() const
	{
		Ref<CBigInt> o(m_object);
		if (o.isNotNull()) {
			return o->getBytesBE();
		}
		return Memory::null();
	}
	
	SLIB_INLINE String toString(sl_uint32 radix = 10) const
	{
		BigInt o(*this);
		return o.toString();
	}
	
	SLIB_INLINE String toHexString() const
	{
		return toString(16);
	}
	
public:
	// compare returns
	//  0: equal,  negative: less than, positive: greater than
	SLIB_INLINE sl_int32 compare(const BigInt& other) const
	{
		BigInt o(*this);
		return o.compare(other);
	}
	
	SLIB_INLINE sl_int32 compare(sl_int32 v) const
	{
		BigInt o(*this);
		return o.compare(v);
	}
	
	SLIB_INLINE sl_int32 compare(sl_uint32 v) const
	{
		BigInt o(*this);
		return o.compare(v);
	}
	
	SLIB_INLINE sl_int32 compare(sl_int64 v) const
	{
		BigInt o(*this);
		return o.compare(v);
	}
	
	SLIB_INLINE sl_int32 compare(sl_uint64 v) const
	{
		BigInt o(*this);
		return o.compare(v);
	}
	
	SLIB_INLINE BigInt negative() const
	{
		BigInt o(*this);
		return o.negative();
	}
	
};

SLIB_INLINE BigInt::BigInt(const SafeBigInt& other) : m_object(other.getRef())
{
}

SLIB_INLINE BigInt& BigInt::operator=(const SafeBigInt& other)
{
	m_object = other.getRef();
	return *this;
}


SLIB_INLINE sl_bool operator==(const BigInt& a, const BigInt& b)
{
	return a.compare(b) == 0;
}

SLIB_INLINE sl_bool operator==(const BigInt& a, sl_int32 v)
{
	return a.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(sl_int32 v, const BigInt& b)
{
	return b.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(const BigInt& a, sl_int64 v)
{
	return a.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(sl_int64 v, const BigInt& b)
{
	return b.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) == 0;
}

SLIB_INLINE sl_bool operator==(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) == 0;
}


SLIB_INLINE sl_bool operator!=(const BigInt& a, const BigInt& b)
{
	return a.compare(b) != 0;
}

SLIB_INLINE sl_bool operator!=(const BigInt& a, sl_int32 v)
{
	return a.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(sl_int32 v, const BigInt& b)
{
	return b.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(const BigInt& a, sl_int64 v)
{
	return a.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(sl_int64 v, const BigInt& b)
{
	return b.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) != 0;
}

SLIB_INLINE sl_bool operator!=(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) != 0;
}


SLIB_INLINE sl_bool operator>=(const BigInt& a, const BigInt& b)
{
	return a.compare(b) >= 0;
}

SLIB_INLINE sl_bool operator>=(const BigInt& a, sl_int32 v)
{
	return a.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator>=(sl_int32 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator>=(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator>=(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator>=(const BigInt& a, sl_int64 v)
{
	return a.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator>=(sl_int64 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator>=(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator>=(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) <= 0;
}


SLIB_INLINE sl_bool operator>(const BigInt& a, const BigInt& b)
{
	return a.compare(b) > 0;
}

SLIB_INLINE sl_bool operator>(const BigInt& a, sl_int32 v)
{
	return a.compare(v) > 0;
}

SLIB_INLINE sl_bool operator>(sl_int32 v, const BigInt& b)
{
	return b.compare(v) < 0;
}

SLIB_INLINE sl_bool operator>(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) > 0;
}

SLIB_INLINE sl_bool operator>(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) < 0;
}

SLIB_INLINE sl_bool operator>(const BigInt& a, sl_int64 v)
{
	return a.compare(v) > 0;
}

SLIB_INLINE sl_bool operator>(sl_int64 v, const BigInt& b)
{
	return b.compare(v) < 0;
}

SLIB_INLINE sl_bool operator>(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) > 0;
}

SLIB_INLINE sl_bool operator>(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) < 0;
}


SLIB_INLINE sl_bool operator<=(const BigInt& a, const BigInt& b)
{
	return a.compare(b) <= 0;
}

SLIB_INLINE sl_bool operator<=(const BigInt& a, sl_int32 v)
{
	return a.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator<=(sl_int32 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator<=(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator<=(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator<=(const BigInt& a, sl_int64 v)
{
	return a.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator<=(sl_int64 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}

SLIB_INLINE sl_bool operator<=(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) <= 0;
}

SLIB_INLINE sl_bool operator<=(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) >= 0;
}


SLIB_INLINE sl_bool operator<(const BigInt& a, const BigInt& b)
{
	return a.compare(b) < 0;
}

SLIB_INLINE sl_bool operator<(const BigInt& a, sl_int32 v)
{
	return a.compare(v) < 0;
}

SLIB_INLINE sl_bool operator<(sl_int32 v, const BigInt& b)
{
	return b.compare(v) > 0;
}

SLIB_INLINE sl_bool operator<(const BigInt& a, sl_uint32 v)
{
	return a.compare(v) < 0;
}

SLIB_INLINE sl_bool operator<(sl_uint32 v, const BigInt& b)
{
	return b.compare(v) > 0;
}

SLIB_INLINE sl_bool operator<(const BigInt& a, sl_int64 v)
{
	return a.compare(v) < 0;
}

SLIB_INLINE sl_bool operator<(sl_int64 v, const BigInt& b)
{
	return b.compare(v) > 0;
}

SLIB_INLINE sl_bool operator<(const BigInt& a, sl_uint64 v)
{
	return a.compare(v) < 0;
}

SLIB_INLINE sl_bool operator<(sl_uint64 v, const BigInt& b)
{
	return b.compare(v) > 0;
}


SLIB_INLINE BigInt operator+(const BigInt& a, const BigInt& b)
{
	return BigInt::add(a, b);
}

SLIB_INLINE BigInt operator+(const BigInt& a, sl_int32 v)
{
	return BigInt::add(a, v);
}

SLIB_INLINE BigInt operator+(sl_int32 v, const BigInt& b)
{
	return BigInt::add(b, v);
}

SLIB_INLINE BigInt operator+(const BigInt& a, sl_uint32 v)
{
	return BigInt::add(a, v);
}

SLIB_INLINE BigInt operator+(sl_uint32 v, const BigInt& b)
{
	return BigInt::add(b, v);
}

SLIB_INLINE BigInt operator+(const BigInt& a, sl_int64 v)
{
	return BigInt::add(a, v);
}

SLIB_INLINE BigInt operator+(sl_int64 v, const BigInt& b)
{
	return BigInt::add(b, v);
}

SLIB_INLINE BigInt operator+(const BigInt& a, sl_uint64 v)
{
	return BigInt::add(a, v);
}

SLIB_INLINE BigInt operator+(sl_uint64 v, const BigInt& b)
{
	return BigInt::add(b, v);
}


SLIB_INLINE BigInt operator-(const BigInt& a, const BigInt& b)
{
	return BigInt::sub(a, b);
}

SLIB_INLINE BigInt operator-(const BigInt& a, sl_int32 v)
{
	return BigInt::sub(a, v);
}

SLIB_INLINE BigInt operator-(sl_int32 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

SLIB_INLINE BigInt operator-(const BigInt& a, sl_uint32 v)
{
	return BigInt::sub(a, v);
}

SLIB_INLINE BigInt operator-(sl_uint32 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

SLIB_INLINE BigInt operator-(const BigInt& a, sl_int64 v)
{
	return BigInt::sub(a, v);
}

SLIB_INLINE BigInt operator-(sl_int64 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

SLIB_INLINE BigInt operator-(const BigInt& a, sl_uint64 v)
{
	return BigInt::sub(a, v);
}

SLIB_INLINE BigInt operator-(sl_uint64 v, const BigInt& b)
{
	BigInt ret = BigInt::sub(b, v);
	ret.makeNegative();
	return ret;
}

SLIB_INLINE BigInt operator-(const BigInt& a)
{
	return a.negative();
}


SLIB_INLINE BigInt operator*(const BigInt& a, const BigInt& b)
{
	return BigInt::mul(a, b);
}

SLIB_INLINE BigInt operator*(const BigInt& a, sl_int32 v)
{
	return BigInt::mul(a, v);
}

SLIB_INLINE BigInt operator*(sl_int32 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}

SLIB_INLINE BigInt operator*(const BigInt& a, sl_uint32 v)
{
	return BigInt::mul(a, v);
}

SLIB_INLINE BigInt operator*(sl_uint32 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}

SLIB_INLINE BigInt operator*(const BigInt& a, sl_int64 v)
{
	return BigInt::mul(a, v);
}

SLIB_INLINE BigInt operator*(sl_int64 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}

SLIB_INLINE BigInt operator*(const BigInt& a, sl_uint64 v)
{
	return BigInt::mul(a, v);
}

SLIB_INLINE BigInt operator*(sl_uint64 v, const BigInt& b)
{
	return BigInt::mul(b, v);
}


SLIB_INLINE BigInt operator/(const BigInt& a, const BigInt& b)
{
	return BigInt::div(a, b);
}

SLIB_INLINE BigInt operator/(const BigInt& a, sl_int32 v)
{
	return BigInt::div(a, v);
}

SLIB_INLINE BigInt operator/(sl_int32 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromInt32(v), b);
}

SLIB_INLINE BigInt operator/(const BigInt& a, sl_uint32 v)
{
	return BigInt::div(a, v);
}

SLIB_INLINE BigInt operator/(sl_uint32 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromUint32(v), b);
}

SLIB_INLINE BigInt operator/(const BigInt& a, sl_int64 v)
{
	return BigInt::div(a, v);
}

SLIB_INLINE BigInt operator/(sl_int64 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromInt64(v), b);
}

SLIB_INLINE BigInt operator/(const BigInt& a, sl_uint64 v)
{
	return BigInt::div(a, v);
}

SLIB_INLINE BigInt operator/(sl_uint64 v, const BigInt& b)
{
	return BigInt::div(BigInt::fromUint64(v), b);
}


SLIB_INLINE BigInt operator%(const BigInt& a, const BigInt& b)
{
	return BigInt::mod(a, b);
}

SLIB_INLINE sl_uint32 operator%(const BigInt& a, sl_int32 v)
{
	return BigInt::mod(a, v);
}

SLIB_INLINE BigInt operator%(sl_int32 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromInt32(v), b);
}

SLIB_INLINE sl_uint32 operator%(const BigInt& a, sl_uint32 v)
{
	return BigInt::mod(a, v);
}

SLIB_INLINE BigInt operator%(sl_uint32 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromUint32(v), b);
}

SLIB_INLINE sl_uint64 operator%(const BigInt& a, sl_int64 v)
{
	return BigInt::mod(a, v);
}

SLIB_INLINE BigInt operator%(sl_int64 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromInt64(v), b);
}

SLIB_INLINE sl_uint64 operator%(const BigInt& a, sl_uint64 v)
{
	return BigInt::mod(a, v);
}

SLIB_INLINE BigInt operator%(sl_uint64 v, const BigInt& b)
{
	return BigInt::mod(BigInt::fromUint64(v), b);
}


SLIB_INLINE BigInt operator<<(const BigInt& a, sl_uint32 n)
{
	return BigInt::shiftLeft(a, n);
}

SLIB_INLINE BigInt operator>>(const BigInt& a, sl_uint32 n)
{
	return BigInt::shiftRight(a, n);
}

SLIB_MATH_NAMESPACE_END

#endif

