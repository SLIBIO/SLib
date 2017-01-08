#ifndef CHECKHEADER_SLIB_CORE_MATH
#define CHECKHEADER_SLIB_CORE_MATH

#include "definition.h"

#define SLIB_PI					3.141592653589793f
#define SLIB_PI_LONG			3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define SLIB_PI_DUAL			6.283185307179586f
#define SLIB_PI_DUAL_LONG		6.283185307179586476925286766559
#define SLIB_PI_HALF			1.570796326795f
#define SLIB_PI_HALF_LONG		1.5707963267948966192313216916398
#define SLIB_PI_QUARTER			0.7853981633975f
#define SLIB_PI_QUARTER_LONG	0.7853981633974483096156608458199
#define SLIB_EPSILON			1.192092896e-10f
#define SLIB_EPSILON_LONG		1.192092896e-20

#undef max
#undef min

SLIB_NAMESPACE_BEGIN

namespace Math
{
	template <typename T>
	SLIB_CONSTEXPR T max(T a, T b)
	{
		return (a > b) ? a : b;
	}
	
	template <typename T>
	SLIB_CONSTEXPR T min(T a, T b)
	{
		return (a < b) ? a : b;
	}
	
	template <typename T>
	SLIB_CONSTEXPR T abs(T v)
	{
		return (v > 0) ? (v) : (-v);
	}
	
	SLIB_CONSTEXPR sl_int32 sign(float v)
	{
		return (v >= 0) ? 1 : -1;
	}
	
	SLIB_CONSTEXPR sl_int32 sign(double v)
	{
		return (v >= 0) ? 1 : -1;
	}

	float pow(float x, float y);
	
	double pow(double x, double y);
	
	SLIB_CONSTEXPR float square(float x)
	{
		return x * x;
	}
	
	SLIB_CONSTEXPR double square(double x)
	{
		return x * x;
	}
	
	float sqrt(float f);
	
	double sqrt(double f);
	
	float cbrt(float f);
	
	double cbrt(double f);

	
	float sin(float f);
	
	double sin(double f);
	
	float cos(float f);
	
	double cos(double f);
	
	float tan(float f);
	
	double tan(double f);
	
	float cot(float f);
	
	double cot(double f);

	float arcsin(float f);
	
	double arcsin(double f);
	
	float arccos(float f);
	
	double arccos(double f);
	
	float arctan(float f);
	
	double arctan(double f);
	
	float arctan2(float y, float x);
	
	double arctan2(double y, double x);
	
	
	float log(float f);
	
	double log(double f);
	
	float log2(float f);
	
	double log2(double f);
	
	float log10(float f);
	
	double log10(double f);

	float exp(float f);
	
	double exp(double f);

	
	float ceil(float f);
	
	double ceil(double f);

	float floor(float f);
	
	double floor(double f);
	
	float round(float f);
	
	double round(double f);

	
	// check "Not a Number"  such as (sqrt(-1.0))
	sl_bool isNaN(float f);
	
	// check "Not a Number"  such as (sqrt(-1.0))
	sl_bool isNaN(double f);

	// check "Infinite"  such as (1.0 / 0.0)
	sl_bool isInfinite(float f);
	
	// check "Infinite"  such as (1.0 / 0.0)
	sl_bool isInfinite(double f);

	
	SLIB_CONSTEXPR float saturate(float f)
	{
		return (f<0.0f) ? 0.0f : ((f>1.0f) ? 1.0f : f);
	}
	
	SLIB_CONSTEXPR double saturate(double f)
	{
		return (f<0.0) ? 0.0 : ((f>1.0) ? 1.0 : f);
	}
	
	template <typename T>
	SLIB_CONSTEXPR T clamp(T v, T vMin, T vMax)
	{
		return (v<vMin) ? vMin : ((v>vMax) ? vMax : v);
	}
	
	SLIB_CONSTEXPR sl_int32 clamp0_255(sl_int32 v)
	{
		// ((-v) >> 31) & v <=> arithmetic sign shift, clamp to >=0
		return (((0xFF - (((-v) >> 31) & v)) >> 31) | (((-v) >> 31) & v)) & 0xFF; // clamp to < 256
	}
	
	SLIB_CONSTEXPR sl_int32 clamp0_65535(sl_int32 v)
	{
		// ((-v) >> 31) & v <=> arithmetic sign shift, clamp to >=0
		return (((0xFFFF - (((-v) >> 31) & v)) >> 31) | (((-v) >> 31) & v)) & 0xFFFF; // clamp to < 65536
	}

	
	SLIB_CONSTEXPR sl_bool isAlmostZero(float f)
	{
		return f > -SLIB_EPSILON && f < SLIB_EPSILON;
	}
	
	SLIB_CONSTEXPR sl_bool isAlmostZero(double f)
	{
		return f > -SLIB_EPSILON_LONG && f < SLIB_EPSILON_LONG;
	}
	
	SLIB_CONSTEXPR sl_bool isAlmostZero(sl_int32 v)
	{
		return v == 0;
	}
	
	SLIB_CONSTEXPR sl_bool isAlmostZero(sl_uint32 v)
	{
		return v == 0;
	}
	
	SLIB_CONSTEXPR sl_bool isAlmostZero(sl_int64 v)
	{
		return v == 0;
	}
	
	SLIB_CONSTEXPR sl_bool isAlmostZero(sl_uint64 v)
	{
		return v == 0;
	}

	SLIB_CONSTEXPR sl_bool isLessThanEpsilon(float f)
	{
		return f < SLIB_EPSILON;
	}
	
	SLIB_CONSTEXPR sl_bool isLessThanEpsilon(double f)
	{
		return f < SLIB_EPSILON_LONG;
	}
	
	SLIB_CONSTEXPR sl_bool isLessThanEpsilon(sl_int32 v)
	{
		return v <= 0;
	}
	
	SLIB_CONSTEXPR sl_bool isLessThanEpsilon(sl_uint32 v)
	{
		return v <= 0;
	}
	
	SLIB_CONSTEXPR sl_bool isLessThanEpsilon(sl_int64 v)
	{
		return v <= 0;
	}
	
	SLIB_CONSTEXPR sl_bool isLessThanEpsilon(sl_uint64 v)
	{
		return v <= 0;
	}

	
	SLIB_CONSTEXPR float getRadianFromDegrees(float f)
	{
		return f * SLIB_PI / 180.0f;
	}
	
	SLIB_CONSTEXPR double getRadianFromDegrees(double f)
	{
		return f * SLIB_PI_LONG / 180.0;
	}
	
	SLIB_CONSTEXPR float getDegreesFromRadian(float f)
	{
		return f * 180.0f / SLIB_PI;
	}
	
	SLIB_CONSTEXPR double getDegreesFromRadian(double f)
	{
		return f * 180.0 / SLIB_PI_LONG;
	}
	
	
	template <class T>
	SLIB_CONSTEXPR T PI();
	
	template <>
	SLIB_CONSTEXPR float PI<float>()
	{
		return SLIB_PI;
	}
	
	template <>
	SLIB_CONSTEXPR double PI<double>()
	{
		return SLIB_PI_LONG;
	}
	
	template <class T>
	SLIB_CONSTEXPR T DualPI();
	
	template <>
	SLIB_CONSTEXPR float DualPI<float>()
	{
		return SLIB_PI_DUAL;
	}
	
	template <>
	SLIB_CONSTEXPR double DualPI<double>()
	{
		return SLIB_PI_DUAL_LONG;
	}
	
	template <class T>
	SLIB_CONSTEXPR T HalfPI();
	
	template <>
	SLIB_CONSTEXPR float HalfPI<float>()
	{
		return SLIB_PI_HALF;
	}
	
	template <>
	SLIB_CONSTEXPR double HalfPI<double>()
	{
		return SLIB_PI_HALF_LONG;
	}
	
	template <class T>
	SLIB_CONSTEXPR T QuarterPI();
	
	template <>
	SLIB_CONSTEXPR float QuarterPI<float>()
	{
		return SLIB_PI_QUARTER;
	}
	
	template <>
	SLIB_CONSTEXPR double QuarterPI<double>()
	{
		return SLIB_PI_QUARTER_LONG;
	}
	
	template <class T>
	SLIB_CONSTEXPR T Epsilon();
	
	template <>
	SLIB_CONSTEXPR float Epsilon<float>()
	{
		return SLIB_EPSILON;
	}
	
	template <>
	SLIB_CONSTEXPR double Epsilon<double>()
	{
		return SLIB_EPSILON_LONG;
	}

	
	// normalize degree to [0, 360]
	float normalizeDegree(float f);
	
	// normalize degree to [0, 360]
	double normalizeDegree(double f);
	
	// normalize degree to [-180, 180]
	float normalizeDegreeDistance(float f);
	
	// normalize degree to [-180, 180]
	double normalizeDegreeDistance(double f);

	
	float convertAngleFromEllipseToCircle(float f, float radiusX, float radiusY);
	
	double convertAngleFromEllipseToCircle(double f, double radiusX, double radiusY);

	
	// random value between 0~1
	double random();
	
	// random value between 0~10000
	sl_uint32 randomInt();
	
	void randomMemory(void* mem, sl_size size);
	
	
	sl_uint32 roundUpToPowerOfTwo32(sl_uint32 num);
	
	sl_uint64 roundUpToPowerOfTwo64(sl_uint64 num);

	
	SLIB_CONSTEXPR sl_uint32 rotateLeft32(sl_uint32 x, sl_uint32 n)
	{
		return (x << n) | (x >> (32 - n));
	}
	
	SLIB_CONSTEXPR sl_uint64 rotateLeft64(sl_uint64 x, sl_uint32 n)
	{
		return (x << n) | (x >> (64 - n));
	}
	
	SLIB_CONSTEXPR sl_uint32 rotateRight32(sl_uint32 x, sl_uint32 n)
	{
		return (x >> n) | (x << (32 - n));
	}
	
	SLIB_CONSTEXPR sl_uint64 rotateRight64(sl_uint64 x, sl_uint32 n)
	{
		return (x >> n) | (x << (64 - n));
	}
	
	
	sl_uint32 getMostSignificantBits(sl_uint32 n);
	
	sl_uint32 getMostSignificantBits(sl_uint64 n);
	
	sl_uint32 getLeastSignificantBits(sl_uint32 n);
	
	sl_uint32 getLeastSignificantBits(sl_uint64 n);

}

SLIB_NAMESPACE_END

#if defined(SLIB_COMPILER_IS_VC)
#include <intrin.h>
#pragma intrinsic(__emulu)
#	if defined(SLIB_ARCH_IS_64BIT)
#pragma intrinsic(_umul128)
#	endif
#endif

SLIB_NAMESPACE_BEGIN

namespace Math {

	SLIB_INLINE void mul32(sl_uint32 a, sl_uint32 b, sl_uint32& o_high, sl_uint32& o_low)
	{
#if defined(SLIB_COMPILER_IS_VC)
		sl_uint64 m = __emulu(a, b);
		o_high = (sl_uint32)(m >> 32);
		o_low = (sl_uint32)(m);
#else
		sl_uint64 m = a;
		m *= b;
		o_high = (sl_uint32)(m >> 32);
		o_low = (sl_uint32)(m);
#endif
	}
	
	SLIB_INLINE void mul64(sl_uint64 a, sl_uint64 b, sl_uint64& o_high, sl_uint64& o_low)
	{
#if defined(SLIB_COMPILER_IS_VC) && defined(SLIB_ARCH_IS_64BIT)
		o_low = _umul128(a, b, &o_high);
#elif defined(SLIB_COMPILER_IS_GCC) && defined(__SIZEOF_INT128__)
		unsigned __int128 m = ((unsigned __int128)a) * ((unsigned __int128)b);
		o_high = (sl_uint64)(m >> 64);
		o_low = (sl_uint64)m;
#else
		sl_uint64 al = (sl_uint64)((sl_uint32)a);
		sl_uint64 ah = a >> 32;
		sl_uint64 bl = (sl_uint64)((sl_uint32)b);
		sl_uint64 bh = b >> 32;
		sl_uint64 m0 = al * bl;
		sl_uint64 m1 = al * bh + (m0 >> 32);
		sl_uint64 m2 = bh * al + (sl_uint32)(m1);
		o_low = (((sl_uint64)((sl_uint32)m2)) << 32) + ((sl_uint32)m0);
		o_high = ah * bh + (m1 >> 32) + (m2 >> 32);
#endif
	}
	
}

SLIB_NAMESPACE_END

#endif
