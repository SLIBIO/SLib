#include "../../../inc/slib/core/math.h"

#include "../../../inc/slib/core/time.h"
#include "../../../inc/slib/crypto/sha2.h"
#include "../../../inc/slib/core/system.h"

#include <math.h>
#include <stdlib.h>

#if defined(SLIB_PLATFORM_IS_WP8)
#include "float.h"
#endif

SLIB_NAMESPACE_BEGIN

float Math::pow(float x, float y)
{
	return ::powf(x, y);
}
double Math::pow(double x, double y)
{
	return ::pow(x, y);
}

float Math::sqrt(float f)
{
	return ::sqrtf(f);
}
double Math::sqrt(double f)
{
	return ::sqrt(f);
}

float Math::cbrt(float f)
{
	return ::cbrtf(f);
}
double Math::cbrt(double f)
{
	return ::cbrt(f);
}

float Math::sin(float f)
{
	return ::sinf(f);
}
double Math::sin(double f)
{
	return ::sin(f);
}

float Math::cos(float f)
{
	return ::cosf(f);
}
double Math::cos(double f)
{
	return ::cos(f);
}

float Math::tan(float f)
{
	return ::tanf(f);
}
double Math::tan(double f)
{
	return ::tan(f);
}

float Math::cot(float f)
{
	return 1.0f / tan(f);
}
double Math::cot(double f)
{
	return 1.0 / tan(f);
}

float Math::arccos(float f)
{
	return ::acosf(f);
}
double Math::arccos(double f)
{
	return ::acos(f);
}

float Math::arcsin(float f)
{
	return ::asinf(f);
}
double Math::arcsin(double f)
{
	return ::asin(f);
}

float Math::arctan(float f)
{
	return ::atanf(f);
}
double Math::arctan(double f)
{
	return ::atan(f);
}

float Math::arctan(float y, float x)
{
	return ::atan2f(y, x);
}
double Math::arctan(double y, double x)
{
	return ::atan2(y, x);
}

float Math::log(float f)
{
	return ::logf(f);
}
double Math::log(double f)
{
	return ::log(f);
}

float Math::log10(float f)
{
	return ::log10f(f);
}
double Math::log10(double f)
{
	return ::log10(f);
}

float Math::log2(float f)
{
	float log2f = 0.69314718055994530941723212145818f;
	return ::logf(f) / log2f;
}

double Math::log2(double f)
{
	double log2 = 0.69314718055994530941723212145818;
	return ::log(f) / log2;
}

float Math::exp(float f)
{
	return ::expf(f);
}
double Math::exp(double f)
{
	return ::exp(f);
}
float Math::round(float f)
{
	return ::floorf(f + 0.5f);
}
double Math::round(double f)
{
	return ::floor(f + 0.5);
}
float Math::floor(float f)
{
	return ::floorf(f);
}
double Math::floor(double f)
{
	return ::floor(f);
}

float Math::ceil(float f)
{
	return ::ceilf(f);
}
double Math::ceil(double f)
{
	return ::ceil(f);
}

#if defined(SLIB_PLATFORM_IS_WP8)

sl_bool Math::isNaN(float f)
{
	sl_int32 ret = _isnan(f);
	if (ret == 0) {
		return sl_false;
	}
	return sl_true;
}
sl_bool Math::isNaN(double f)
{
	sl_int32 ret = _isnan(f);
	if (ret == 0) {
		return sl_false;
	}
	return sl_true;
}

sl_bool Math::isInfinite(float f)
{
	sl_int32 ret = _finite(f);
	if (ret == 0) {
		return sl_false;
	}
	return sl_true;
}
sl_bool Math::isInfinite(double f)
{
	sl_int32 ret = _finite(f);
	if (ret == 0) {
		return sl_false;
	}
	return sl_true;
}
#else
sl_bool Math::isNaN(float f)
{
	return isnan(f) != 0;
}
sl_bool Math::isNaN(double f)
{
	return isnan(f) != 0;
}

sl_bool Math::isInfinite(float f)
{
	return isinf(f) != 0;
}
sl_bool Math::isInfinite(double f)
{
	return isinf(f) != 0;
}
#endif

template <class T>
SLIB_INLINE static T _Math_normalizeDegree(T v)
{
	if (Math::isNaN(v)) {
		return 0;
	}
	sl_int32 n = (sl_int32)v;
	T f = v - n;
	if (f < 0) {
		f = 1 + f;
		n--;
	}
	n = n % 360;
	if (n < 0) {
		n += 360;
	}
	v = (T)(n) + f;
	return v;
}

float Math::normalizeDegree(float v)
{
	return _Math_normalizeDegree(v);
}

double Math::normalizeDegree(double v)
{
	return _Math_normalizeDegree(v);
}

float Math::normalizeDegreeDistance(float v)
{
	return normalizeDegree(v + 180) - 180;
}

double Math::normalizeDegreeDistance(double v)
{
	return normalizeDegree(v + 180) - 180;
}

template <class T>
SLIB_INLINE static T _Math_convertAngleFromEllipseToCircle(T angle, T radiusX, T radiusY)
{
	T _cos = Math::cos(angle);
	if (Math::isNearZero(_cos) || Math::isNearZero(Math::sin(angle))) {
		return angle;
	}
	T PI;
	Math::getPI(&PI);
	T PI2 = PI * 2;
	T stretched = Math::atan2(Math::sin(angle) / Math::abs(radiusY), Math::cos(angle) / Math::abs(radiusX));
	T revs_off = Math::round(angle / PI2) - Math::round(stretched / PI2);
	stretched += revs_off * PI2;
	return stretched;
}

float Math::convertAngleFromEllipseToCircle(float angle, float radiusX, float radiusY)
{
	return _Math_convertAngleFromEllipseToCircle(angle, radiusX, radiusY);
}

double Math::convertAngleFromEllipseToCircle(double angle, double radiusX, double radiusY)
{
	return _Math_convertAngleFromEllipseToCircle(angle, radiusX, radiusY);
}

double Math::random()
{
	return (randomInt() % 10000) / 10000.0;
}

sl_uint32 Math::randomInt()
{
	sl_uint32 dw = System::getTickCount();
	sl_uint32 n = ::rand() ^ (dw >> 16) ^ (dw & 0xFFFF);
	return (n % 10000);
}

void Math::randomMemory(void* _mem, sl_size size)
{
#define RANDOM_BLOCK 64
	sl_size nSections = size / RANDOM_BLOCK;
	if (size % RANDOM_BLOCK) {
		nSections++;
	}

	sl_uint32 dw = System::getTickCount() + (::rand());
	dw = (dw >> 16) ^ (dw & 0xFFFF);
	::srand((unsigned int)(dw + 1000));

	sl_uint8* mem = (sl_uint8*)_mem;
	for (sl_size i = 0; i < nSections; i++) {
		sl_size n;
		if (i == nSections - 1) {
			n = size - i * RANDOM_BLOCK;
		} else {
			n = RANDOM_BLOCK;
		}
		char hashSeed[RANDOM_BLOCK];
		// preparing referencing data
		{
			char buf[72];
			// 0:8 Time
			sl_uint64 t = Time::now().toInt();
			Base::copyMemory(buf, &t, 8);
			// 8:4 Stack Address
			sl_uint32 sa = (sl_uint32)(sl_reg)((void*)(&size));
			Base::copyMemory(buf + 8, &sa, 4);
			// 12:4 Random
			sl_uint32 ra = ::rand();
			Base::copyMemory(buf + 12, &ra, 4);

			static sl_bool flagStatics = sl_false;
			static sl_uint32 pid = 0;
			static char hashAppPath[32] = { 0 };
			if (!flagStatics) {
				pid = System::getProcessId();
				String str = System::getApplicationPath();
				SHA256::hash(str, hashAppPath);
				flagStatics = sl_true;
			}
			// 16:4 process id
			Base::copyMemory(buf + 16, &pid, 4);
			// 20:4 thread id
			sl_uint32 tid = System::getThreadId();
			Base::copyMemory(buf + 20, &tid, 4);
			// 24:4 new address
			void* pna = Base::createMemory(1);
			sl_uint32 na = (sl_uint32)(sl_reg)(pna);
			if (pna) {
				Base::freeMemory(pna);
			}
			Base::copyMemory(buf + 24, &na, 4);
			// 28:4 system ticks
			sl_uint32 tick = System::getTickCount();
			Base::copyMemory(buf + 28, &tick, 4);
			// 32:32 app path hash
			Base::copyMemory(buf + 32, hashAppPath, 32);
			// 64:8 sequence
			static sl_uint64 se = 0;
			se++;
			Base::copyMemory(buf + 64, &se, 8);
			SHA512::hash(buf, 72, hashSeed);
		}
		sl_uint32 mm = 0;
		for (sl_size i = 0; i < n; i++) {
			mm ^= rand();
			mem[i] = (char)mm ^ hashSeed[i];
			mm >>= 8;
		}
		mem += RANDOM_BLOCK;
	}
}

sl_uint32 Math::roundUpToPowerOfTwo32(sl_uint32 num)
{
	num--;
	num |= (num >> 1);
	num |= (num >> 2);
	num |= (num >> 4);
	num |= (num >> 8);
	num |= (num >> 16);
	return num + 1;
}
sl_uint64 Math::roundUpToPowerOfTwo64(sl_uint64 num)
{
	num--;
	num |= (num >> 1);
	num |= (num >> 2);
	num |= (num >> 4);
	num |= (num >> 8);
	num |= (num >> 16);
	num |= (num >> 32);
	return num + 1;
}

sl_uint32 Math::getMostSignificantBits(sl_uint32 n)
{
	sl_uint32 ret = 0;
	while (n) {
		ret++;
		n >>= 1;
	}
	return ret;
}

sl_uint32 Math::getMostSignificantBits(sl_uint64 n)
{
	sl_uint32 ret = 0;
	while (n) {
		ret++;
		n >>= 1;
	}
	return ret;
}

sl_uint32 Math::getLeastSignificantBits(sl_uint32 n)
{
	if (n == 0) {
		return 0;
	}
	sl_uint32 ret = 0;
	while ((n & 1) == 0) {
		ret++;
		n >>= 1;
	}
	return ret;
}

sl_uint32 Math::getLeastSignificantBits(sl_uint64 n)
{
	if (n == 0) {
		return 0;
	}
	sl_uint32 ret = 0;
	while ((n & 1) == 0) {
		ret++;
		n >>= 1;
	}
	return ret;
}

SLIB_NAMESPACE_END

