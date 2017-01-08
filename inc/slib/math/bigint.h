#ifndef CHECKHEADER_SLIB_MATH_BIGINT
#define CHECKHEADER_SLIB_MATH_BIGINT

#include "definition.h"

#include "../core/ref.h"
#include "../core/memory.h"
#include "../core/string.h"
#include "../core/parse.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	Notice:
		
		CBigInt, BigInt is not thread-safe on modification operations
*/

class SLIB_EXPORT CBigInt : public Referable
{
	SLIB_DECLARE_OBJECT
	
public:
	sl_uint32* elements;
	
	// count of elements
	sl_size length;
	
	// 1 : positive,  -1 : negative
	sl_int32 sign;
	
private:
	sl_bool m_flagUserData;
	
public:
	CBigInt();
	
	~CBigInt();

public:
	void setUserDataElements(sl_uint32* data, sl_size n);
	
	sl_int32 makeNagative();

	sl_bool getBit(sl_size pos) const;
	
	void setBit(sl_size pos, sl_bool bit);

	// get size in elements
	sl_size getMostSignificantElements() const;
	
	sl_size getLeastSignificantElements() const;
	
	// get size in bytes
	sl_size getMostSignificantBytes() const;
	
	sl_size getLeastSignificantBytes() const;
	
	// get size in bits
	sl_size getMostSignificantBits() const;
	
	sl_size getLeastSignificantBits() const;

	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	void setZero();
	
	
	static CBigInt* allocate(sl_size length);
	
	CBigInt* duplicate(sl_size newLength) const;
	
	CBigInt* duplicate() const;
	
	CBigInt* duplicateCompact() const;
	
	sl_bool copyAbsFrom(const CBigInt& other);
	
	sl_bool copyFrom(const CBigInt& other);
	
	sl_bool compact();
	
	sl_bool growLength(sl_size newLength);
	
	sl_bool setLength(sl_size newLength);
	
	sl_bool setValueFromElements(const sl_uint32* data, sl_size n);
	
	// set/get data from/to bytes buffer (Little Endian), the sign is not changed
	sl_bool setBytesLE(const void* bytes, sl_size nBytes);
	
	void setBytesLE(const Memory& mem);
	
	static CBigInt* fromBytesLE(const void* bytes, sl_size nBytes);
	
	static CBigInt* fromBytesLE(const Memory& mem);
	
	sl_bool getBytesLE(void* buf, sl_size n) const;
	
	Memory getBytesLE() const;
	
	// set/get data from/to bytes buffer (Big Endian), the sign is not changed
	sl_bool setBytesBE(const void* bytes, sl_size nBytes);
	
	void setBytesBE(const Memory& mem);
	
	static CBigInt* fromBytesBE(const void* bytes, sl_size nBytes);
	
	static CBigInt* fromBytesBE(const Memory& mem);
	
	sl_bool getBytesBE(void* buf, sl_size n) const;
	
	Memory getBytesBE() const;
	
	
	sl_bool setValue(sl_int32 v);
	
	static CBigInt* fromInt32(sl_int32 v);
	
	sl_bool setValue(sl_uint32 v);
	
	static CBigInt* fromUint32(sl_uint32 v);
	
	sl_bool setValue(sl_int64 v);
	
	static CBigInt* fromInt64(sl_int64 v);
	
	sl_bool setValue(sl_uint64 v);
	
	static CBigInt* fromUint64(sl_uint64 v);
	
	static CBigInt* fromString(const String& str, sl_uint32 radix = 10);
	
	String toString(sl_uint32 radix = 10) const;
	
	static CBigInt* fromHexString(const String& str);
	
	String toHexString() const;
	
	
	template <class ST>
	static sl_bool parse(const ST& str, CBigInt* _out, sl_uint32 radix = 10)
	{
		return ParseInt(str, _out, radix);
	}
	
	template <class ST>
	sl_bool parse(const ST& str, sl_uint32 radix = 10)
	{
		return ParseInt(str, this, radix);
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
	
	sl_bool add(const CBigInt& o);
	
	sl_bool add(sl_int32 v);
	
	sl_bool add(sl_uint32 v);
	
	sl_bool add(sl_int64 v);
	
	sl_bool add(sl_uint64 v);

	
	sl_bool subAbs(const CBigInt& a, const CBigInt& b);
	
	sl_bool subAbs(const CBigInt& a, sl_uint32 v);
	
	sl_bool subAbs(const CBigInt& a, sl_uint64 v);
	
	sl_bool sub(const CBigInt& a, const CBigInt& b);
	
	sl_bool sub(const CBigInt& a, sl_int32 v);
	
	sl_bool sub(const CBigInt& a, sl_uint32 v);
	
	sl_bool sub(const CBigInt& a, sl_int64 v);
	
	sl_bool sub(const CBigInt& a, sl_uint64 v);
	
	sl_bool sub(const CBigInt& o);
	
	sl_bool sub(sl_int32 v);
	
	sl_bool sub(sl_uint32 v);
	
	sl_bool sub(sl_int64 v);
	
	sl_bool sub(sl_uint64 v);
	

	sl_bool mulAbs(const CBigInt& a, const CBigInt& b);
	
	sl_bool mulAbs(const CBigInt& a, sl_uint32 v);
	
	sl_bool mul(const CBigInt& a, const CBigInt& b);
	
	sl_bool mul(const CBigInt& a, sl_int32 v);
	
	sl_bool mul(const CBigInt& a, sl_uint32 v);
	
	sl_bool mul(const CBigInt& a, sl_int64 v);
	
	sl_bool mul(const CBigInt& a, sl_uint64 v);
	
	sl_bool mul(const CBigInt& o);
	
	sl_bool mul(sl_int32 v);
	
	sl_bool mul(sl_uint32 v);
	
	sl_bool mul(sl_int64 v);
	
	sl_bool mul(sl_uint64 v);

	
	static sl_bool divAbs(const CBigInt& a, const CBigInt& b, CBigInt* quotient = sl_null, CBigInt* remainder = sl_null);
	
	static sl_bool divAbs(const CBigInt& a, sl_uint32 b, CBigInt* quotient = sl_null, sl_uint32* remainder = sl_null);
	
	static sl_bool div(const CBigInt& a, const CBigInt& b, CBigInt* quotient = sl_null, CBigInt* remainder = sl_null);
	
	static sl_bool div(const CBigInt& a, sl_int32 b, CBigInt* quotient = sl_null, sl_uint32* remainder = sl_null);
	
	static sl_bool div(const CBigInt& a, sl_uint32 b, CBigInt* quotient = sl_null, sl_uint32* remainder = sl_null);
	
	static sl_bool div(const CBigInt& a, sl_int64 b, CBigInt* quotient = sl_null, sl_uint64* remainder = sl_null);
	
	static sl_bool div(const CBigInt& a, sl_uint64 b, CBigInt* quotient = sl_null, sl_uint64* remainder = sl_null);

	
	sl_bool shiftLeft(const CBigInt& other, sl_size n);
	
	sl_bool shiftRight(const CBigInt& other, sl_size n);
	
	sl_bool shiftLeft(sl_size n);
	
	sl_bool shiftRight(sl_size n);

	/*
		E > 0
		M is not null => C = A^E mod M
		M is null => C = A^E
	*/
	sl_bool pow(const CBigInt& A, const CBigInt& E, const CBigInt* pM = sl_null);
	
	sl_bool pow_mod(const CBigInt& A, const CBigInt& E, const CBigInt& M);
	
	sl_bool pow_mod(const CBigInt& E, const CBigInt& M);
	
	sl_bool pow(const CBigInt& E);
	
	sl_bool pow(const CBigInt& A, sl_uint32 E, const CBigInt* pM = sl_null);

	sl_bool pow_mod(const CBigInt& A, sl_uint32 E, const CBigInt& M);
	
	sl_bool pow_mod(sl_uint32 E, const CBigInt& M);
	
	sl_bool pow(sl_uint32 E);

	
	/*
		Exponentiation based on Montgomery Reduction
			C = A^E mod M
		Available Input:
			M - an odd value (M%2=1), M>0
			E > 0
	*/
	sl_bool pow_montgomery(const CBigInt& A, const CBigInt& E, const CBigInt& M);
	
	sl_bool pow_montgomery(const CBigInt& E, const CBigInt& M);

	/*
		C = A^-1 mod M

		Available Input:
			A != 0
			M > 0
			gcd(A, M) = 1
	*/
	sl_bool inverseMod(const CBigInt& A, const CBigInt& M);
	
	sl_bool inverseMod(const CBigInt& M);

	/*
		gcd - greatest common divisor
	*/
	sl_bool gcd(const CBigInt& A, const CBigInt& B);
	
	sl_bool gcd(const CBigInt& B);

private:
	void _free();
};

template <>
sl_reg IntParser<CBigInt, sl_char8>::parse(CBigInt* _out, sl_uint32 radix, const sl_char8 *sz, sl_size posBegin, sl_size len);

template <>
sl_reg IntParser<CBigInt, sl_char16>::parse(CBigInt* _out, sl_uint32 radix, const sl_char16 *sz, sl_size posBegin, sl_size len);


class BigInt;

template <>
class SLIB_EXPORT Atomic<BigInt>
{
public:
	AtomicRef<CBigInt> ref;
	SLIB_ATOMIC_REF_WRAPPER_NO_OP(CBigInt)
	
public:
	Atomic(sl_int32 n);
	
	Atomic(sl_uint32 n);
	
	Atomic(sl_int64 n);
	
	Atomic(sl_uint64 n);
	
public:
	BigInt duplicate() const;
	
	BigInt compact() const;
	
	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	sl_bool getBytesLE(void* buf, sl_size n) const;
	
	Memory getBytesLE() const;
	
	sl_bool getBytesBE(void* buf, sl_size n) const;
	
	Memory getBytesBE() const;
	
	String toString(sl_uint32 radix = 10) const;
	
	String toHexString() const;
	
	// compare returns
	//  0: equal,  negative: less than, positive: greater than
	sl_int32 compare(const BigInt& other) const;
	
	sl_int32 compare(sl_int32 v) const;
	
	sl_int32 compare(sl_uint32 v) const;
	
	sl_int32 compare(sl_int64 v) const;
	
	sl_int32 compare(sl_uint64 v) const;
	
	BigInt negative() const;
	
public:
	Atomic& operator=(sl_int32 n);
	
	Atomic& operator=(sl_uint32 n);
	
	Atomic& operator=(sl_int64 n);
	
	Atomic& operator=(sl_uint64 n);
	
};

typedef Atomic<BigInt> AtomicBigInt;


class SLIB_EXPORT BigInt
{
public:
	Ref<CBigInt> ref;
	SLIB_REF_WRAPPER_NO_OP(BigInt, CBigInt)

public:
	BigInt(sl_int32 n);
	
	BigInt(sl_uint32 n);
	
	BigInt(sl_int64 n);
	
	BigInt(sl_uint64 n);
	
public:
	static BigInt fromInt32(sl_int32 v);
	
	static BigInt fromUint32(sl_uint32 v);
	
	static BigInt fromInt64(sl_int64 v);
	
	static BigInt fromUint64(sl_uint64 v);
	
	static BigInt fromBytesLE(const void* bytes, sl_size nBytes);
	
	static BigInt fromBytesLE(const Memory& mem);
	
	static BigInt fromBytesBE(const void* bytes, sl_size nBytes);
	
	static BigInt fromBytesBE(const Memory& mem);
	
	static BigInt fromString(const String& str, sl_uint32 radix = 10);
	
	static BigInt fromHexString(const String& str);
	
	CBigInt& instance() const;
	
	BigInt duplicate() const;
	
	BigInt compact() const;
	
	sl_size getElementsCount() const;
	
	sl_uint32* getElements() const;
	
	sl_int32 getSign() const;
	
	sl_bool getBit(sl_uint32 pos) const;
	
	sl_size getMostSignificantElements() const;
	
	sl_size getLeastSignificantElements() const;
	
	sl_size getMostSignificantBytes() const;
	
	sl_size getLeastSignificantBytes() const;
	
	sl_size getMostSignificantBits() const;
	
	sl_size getLeastSignificantBits() const;
		
	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	sl_bool getBytesLE(void* buf, sl_size n) const;
	
	Memory getBytesLE() const;
	
	sl_bool getBytesBE(void* buf, sl_size n) const;
	
	Memory getBytesBE() const;
	
	String toString(sl_uint32 radix = 10) const;
	
	String toHexString() const;
	
	
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
	
	sl_bool increase();
	
	
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
	
	sl_bool decrease();
	
	
	void makeNegative() const;
	
	BigInt negative() const;
	

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

	
	static BigInt shiftLeft(const BigInt& A, sl_size n);
	
	sl_bool shiftLeft(sl_size n);
	
	static BigInt shiftRight(const BigInt& A, sl_size n);
	
	sl_bool shiftRight(sl_size n);

	
	/*
		E > 0
		M is not null => C = A^E mod M
		M is null => C = A^E
	*/
	static BigInt pow(const BigInt& A, const BigInt& E, const BigInt* pM = sl_null);
	
	sl_bool pow(const BigInt& E, const BigInt* pM = sl_null);
	
	static BigInt pow_mod(const BigInt& A, const BigInt& E, const BigInt& M);

	sl_bool pow_mod(const BigInt& E, const BigInt& M);
	
	static BigInt pow(const BigInt& A, sl_uint32 E, const BigInt* pM = sl_null);
	
	sl_bool pow(sl_uint32 E, const BigInt* pM = sl_null);
	
	BigInt pow_mod(const BigInt& A, sl_uint32 E, const BigInt& M);
	
	sl_bool pow_mod(sl_uint32 E, const BigInt& M);
	
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
	BigInt& operator=(sl_int32 n);
	
	BigInt& operator=(sl_uint32 n);
	
	BigInt& operator=(sl_int64 n);
	
	BigInt& operator=(sl_uint64 n);
	
	BigInt& operator+=(const BigInt& other);
	
	BigInt& operator+=(sl_int32 v);
	
	BigInt& operator+=(sl_uint32 v);
	
	BigInt& operator+=(sl_int64 v);
	
	BigInt& operator+=(sl_uint64 v);
	
	BigInt& operator++();
	
	BigInt operator++(int);
	
	BigInt& operator-=(const BigInt& other);
	
	BigInt& operator-=(sl_int32 v);
	
	BigInt& operator-=(sl_uint32 v);
	
	BigInt& operator-=(sl_int64 v);
	
	BigInt& operator-=(sl_uint64 v);
	
	BigInt& operator--();
	
	BigInt operator--(int);
	
	BigInt& operator*=(const BigInt& other);
	
	BigInt& operator*=(sl_int32 v);
	
	BigInt& operator*=(sl_uint32 v);
	
	BigInt& operator*=(sl_int64 v);
	
	BigInt& operator*=(sl_uint64 v);
	
	BigInt& operator/=(const BigInt& other);
	
	BigInt& operator/=(sl_int32 v);
	
	BigInt& operator/=(sl_uint32 v);
	
	BigInt& operator/=(sl_int64 v);
	
	BigInt& operator/=(sl_uint64 v);
	
	BigInt& operator%=(const BigInt& other);
	
	BigInt& operator%=(sl_int32 v);
	
	BigInt& operator%=(sl_uint32 v);

	BigInt& operator%=(sl_int64 v);
	
	BigInt& operator%=(sl_uint64 v);
	
	BigInt& operator<<=(sl_uint32 n);
	
	BigInt& operator>>=(sl_uint32 n);

};


sl_bool operator==(const BigInt& a, const BigInt& b);

sl_bool operator==(const BigInt& a, sl_int32 v);

sl_bool operator==(sl_int32 v, const BigInt& b);

sl_bool operator==(const BigInt& a, sl_uint32 v);

sl_bool operator==(sl_uint32 v, const BigInt& b);

sl_bool operator==(const BigInt& a, sl_int64 v);

sl_bool operator==(sl_int64 v, const BigInt& b);

sl_bool operator==(const BigInt& a, sl_uint64 v);

sl_bool operator==(sl_uint64 v, const BigInt& b);


sl_bool operator!=(const BigInt& a, const BigInt& b);

sl_bool operator!=(const BigInt& a, sl_int32 v);

sl_bool operator!=(sl_int32 v, const BigInt& b);

sl_bool operator!=(const BigInt& a, sl_uint32 v);

sl_bool operator!=(sl_uint32 v, const BigInt& b);

sl_bool operator!=(const BigInt& a, sl_int64 v);

sl_bool operator!=(sl_int64 v, const BigInt& b);

sl_bool operator!=(const BigInt& a, sl_uint64 v);

sl_bool operator!=(sl_uint64 v, const BigInt& b);


sl_bool operator>=(const BigInt& a, const BigInt& b);

sl_bool operator>=(const BigInt& a, sl_int32 v);

sl_bool operator>=(sl_int32 v, const BigInt& b);

sl_bool operator>=(const BigInt& a, sl_uint32 v);

sl_bool operator>=(sl_uint32 v, const BigInt& b);

sl_bool operator>=(const BigInt& a, sl_int64 v);

sl_bool operator>=(sl_int64 v, const BigInt& b);

sl_bool operator>=(const BigInt& a, sl_uint64 v);

sl_bool operator>=(sl_uint64 v, const BigInt& b);


sl_bool operator>(const BigInt& a, const BigInt& b);

sl_bool operator>(const BigInt& a, sl_int32 v);

sl_bool operator>(sl_int32 v, const BigInt& b);

sl_bool operator>(const BigInt& a, sl_uint32 v);

sl_bool operator>(sl_uint32 v, const BigInt& b);

sl_bool operator>(const BigInt& a, sl_int64 v);

sl_bool operator>(sl_int64 v, const BigInt& b);

sl_bool operator>(const BigInt& a, sl_uint64 v);

sl_bool operator>(sl_uint64 v, const BigInt& b);


sl_bool operator<=(const BigInt& a, const BigInt& b);

sl_bool operator<=(const BigInt& a, sl_int32 v);

sl_bool operator<=(sl_int32 v, const BigInt& b);

sl_bool operator<=(const BigInt& a, sl_uint32 v);

sl_bool operator<=(sl_uint32 v, const BigInt& b);

sl_bool operator<=(const BigInt& a, sl_int64 v);

sl_bool operator<=(sl_int64 v, const BigInt& b);

sl_bool operator<=(const BigInt& a, sl_uint64 v);

sl_bool operator<=(sl_uint64 v, const BigInt& b);


sl_bool operator<(const BigInt& a, const BigInt& b);

sl_bool operator<(const BigInt& a, sl_int32 v);

sl_bool operator<(sl_int32 v, const BigInt& b);

sl_bool operator<(const BigInt& a, sl_uint32 v);

sl_bool operator<(sl_uint32 v, const BigInt& b);

sl_bool operator<(const BigInt& a, sl_int64 v);

sl_bool operator<(sl_int64 v, const BigInt& b);

sl_bool operator<(const BigInt& a, sl_uint64 v);

sl_bool operator<(sl_uint64 v, const BigInt& b);


BigInt operator+(const BigInt& a, const BigInt& b);

BigInt operator+(const BigInt& a, sl_int32 v);

BigInt operator+(sl_int32 v, const BigInt& b);

BigInt operator+(const BigInt& a, sl_uint32 v);

BigInt operator+(sl_uint32 v, const BigInt& b);

BigInt operator+(const BigInt& a, sl_int64 v);

BigInt operator+(sl_int64 v, const BigInt& b);

BigInt operator+(const BigInt& a, sl_uint64 v);

BigInt operator+(sl_uint64 v, const BigInt& b);


BigInt operator-(const BigInt& a, const BigInt& b);

BigInt operator-(const BigInt& a, sl_int32 v);

BigInt operator-(sl_int32 v, const BigInt& b);

BigInt operator-(const BigInt& a, sl_uint32 v);

BigInt operator-(sl_uint32 v, const BigInt& b);

BigInt operator-(const BigInt& a, sl_int64 v);

BigInt operator-(sl_int64 v, const BigInt& b);

BigInt operator-(const BigInt& a, sl_uint64 v);

BigInt operator-(sl_uint64 v, const BigInt& b);

BigInt operator-(const BigInt& a);


BigInt operator*(const BigInt& a, const BigInt& b);

BigInt operator*(const BigInt& a, sl_int32 v);

BigInt operator*(sl_int32 v, const BigInt& b);

BigInt operator*(const BigInt& a, sl_uint32 v);

BigInt operator*(sl_uint32 v, const BigInt& b);

BigInt operator*(const BigInt& a, sl_int64 v);

BigInt operator*(sl_int64 v, const BigInt& b);

BigInt operator*(const BigInt& a, sl_uint64 v);

BigInt operator*(sl_uint64 v, const BigInt& b);


BigInt operator/(const BigInt& a, const BigInt& b);

BigInt operator/(const BigInt& a, sl_int32 v);

BigInt operator/(sl_int32 v, const BigInt& b);

BigInt operator/(const BigInt& a, sl_uint32 v);

BigInt operator/(sl_uint32 v, const BigInt& b);

BigInt operator/(const BigInt& a, sl_int64 v);

BigInt operator/(sl_int64 v, const BigInt& b);

BigInt operator/(const BigInt& a, sl_uint64 v);

BigInt operator/(sl_uint64 v, const BigInt& b);


BigInt operator%(const BigInt& a, const BigInt& b);

sl_uint32 operator%(const BigInt& a, sl_int32 v);

BigInt operator%(sl_int32 v, const BigInt& b);

sl_uint32 operator%(const BigInt& a, sl_uint32 v);

BigInt operator%(sl_uint32 v, const BigInt& b);

sl_uint64 operator%(const BigInt& a, sl_int64 v);

BigInt operator%(sl_int64 v, const BigInt& b);

sl_uint64 operator%(const BigInt& a, sl_uint64 v);

BigInt operator%(sl_uint64 v, const BigInt& b);


BigInt operator<<(const BigInt& a, sl_size n);

BigInt operator>>(const BigInt& a, sl_size n);


SLIB_MATH_NAMESPACE_END

#endif

