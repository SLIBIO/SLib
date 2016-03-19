#ifndef CHECKHEADER_SLIB_CORE_MATH
#define CHECKHEADER_SLIB_CORE_MATH

#include "definition.h"

#define SLIB_PI					3.141592653589793f
#define SLIB_PI_LONG			3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define SLIB_2_PI				6.283185307179586f
#define SLIB_2_PI_LONG			6.283185307179586476925286766559
#define SLIB_PI_2				1.570796326795f
#define SLIB_PI_2_LONG			1.5707963267948966192313216916398
#define SLIB_PI_4				0.7853981633975f
#define SLIB_PI_4_LONG			0.7853981633974483096156608458199
#define SLIB_EPSILON			1.192092896e-10f
#define SLIB_EPSILON_LONG		1.192092896e-20

#undef max
#undef min

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Math
{
public:
	template <typename T>
	static T max(T a, T b);
	
	template <typename T>
	static T min(T a, T b);
	
	template <typename T>
	static T abs(T v);
	
	
	static sl_int32 sign(float v);
	
	static sl_int32 sign(double v);
	
	static float pow(float x, float y);
	
	static double pow(double x, double y);
	
	static float square(float x);
	
	static double square(double x);
	
	static float sqrt(float f);
	
	static double sqrt(double f);
		
	static float cbrt(float f);
	
	static double cbrt(double f);

	
	static float sin(float f);
	
	static double sin(double f);
	
	static float cos(float f);
	
	static double cos(double f);
	
	static float tan(float f);
	
	static double tan(double f);
	
	static float cot(float f);
	
	static double cot(double f);

	static float arcsin(float f);
	
	static double arcsin(double f);
	
	static float arccos(float f);
	
	static double arccos(double f);
	
	static float arctan(float f);
	
	static double arctan(double f);
	
	static float arctan2(float y, float x);
	
	static double arctan2(double y, double x);
	
	
	static float log(float f);
	
	static double log(double f);
	
	static float log2(float f);
	
	static double log2(double f);
	
	static float log10(float f);
	
	static double log10(double f);

	static float exp(float f);
	
	static double exp(double f);

	
	static float ceil(float f);
	
	static double ceil(double f);

	static float floor(float f);
	
	static double floor(double f);
	
	static float round(float f);
	
	static double round(double f);

	
	// check "Not a Number"  such as (sqrt(-1.0))
	static sl_bool isNaN(float f);
	
	// check "Not a Number"  such as (sqrt(-1.0))
	static sl_bool isNaN(double f);

	// check "Infinite"  such as (1.0 / 0.0)
	static sl_bool isInfinite(float f);
	
	// check "Infinite"  such as (1.0 / 0.0)
	static sl_bool isInfinite(double f);

	
	static float saturate(float f);
	
	static double saturate(double f);

	template <typename T>
	static T clamp(T v, T vMin, T vMax);
	
	static sl_int32 clamp0_255(sl_int32 v);
	
	static sl_int32 clamp0_65535(sl_int32 v);

	static sl_bool isNearZero(float f);
	
	static sl_bool isNearZero(double f);

	static sl_bool isLessThanEpsilon(float f);
	
	static sl_bool isLessThanEpsilon(double f);
	

	static float getRadianFromDegrees(float f);
	
	static double getRadianFromDegrees(double f);

	static float getDegreesFromRadian(float f);
	
	static double getDegreesFromRadian(double f);

	// normalize degree to [0, 360]
	static float normalizeDegree(float f);
	
	// normalize degree to [0, 360]
	static double normalizeDegree(double f);
	
	// normalize degree to [-180, 180]
	static float normalizeDegreeDistance(float f);
	
	// normalize degree to [-180, 180]
	static double normalizeDegreeDistance(double f);

	
	static float convertAngleFromEllipseToCircle(float f, float radiusX, float radiusY);
	
	static double convertAngleFromEllipseToCircle(double f, double radiusX, double radiusY);

	
	static void getPI(float* v);
	
	static void getPI(double* v);

	// 2 * PI
	static void get2PI(float* v);
	
	// 2 * PI
	static void get2PI(double* v);

	// PI / 2
	static void getPI2(float* v);
	
	// PI / 2
	static void getPI2(double* v);

	// PI / 4
	static void getPI4(float* v);
	
	// PI / 4
	static void getPI4(double* v);

	static void getEpsilon(float* v);
	
	static void getEpsilon(double* v);

	
	// random value between 0~1
	static double random();
	
	// random value between 0~10000
	static sl_uint32 randomInt();
	
	static void randomMemory(void* mem, sl_size size);

	
	static sl_uint32 roundUpToPowerOfTwo32(sl_uint32 num);
	
	static sl_uint64 roundUpToPowerOfTwo64(sl_uint64 num);

	static sl_uint32 rotateLeft32(sl_uint32 x, sl_uint32 n);
	
	static sl_uint64 rotateLeft64(sl_uint64 x, sl_uint32 n);

	static sl_uint32 rotateRight32(sl_uint32 x, sl_uint32 n);
	
	static sl_uint64 rotateRight64(sl_uint64 x, sl_uint32 n);

	static sl_uint32 getMostSignificantBits(sl_uint32 n);
	
	static sl_uint32 getMostSignificantBits(sl_uint64 n);
	
	static sl_uint32 getLeastSignificantBits(sl_uint32 n);
	
	static sl_uint32 getLeastSignificantBits(sl_uint64 n);
	
	static void mul32(sl_uint32 a, sl_uint32 b, sl_uint32& o_high, sl_uint32& o_low);
	
	static void mul64(sl_uint64 a, sl_uint64 b, sl_uint64& o_high, sl_uint64& o_low);

};

SLIB_NAMESPACE_END


#if defined(SLIB_COMPILER_IS_VISUALSTUDIO)
#include <intrin.h>
#pragma intrinsic(__emulu)
#	if defined(SLIB_ARCH_IS_64BIT)
#pragma intrinsic(_umul128)
#	endif
#endif

SLIB_NAMESPACE_BEGIN

template <typename T>
SLIB_INLINE T Math::max(T a, T b)
{
	return (a > b) ? a : b;
}

template <typename T>
SLIB_INLINE T Math::min(T a, T b)
{
	return (a < b) ? a : b;
}

template <typename T>
SLIB_INLINE T Math::abs(T v)
{
	return (v > 0) ? (v) : (-v);
}

SLIB_INLINE sl_int32 Math::sign(float v)
{
	return (v >= 0) ? 1 : -1;
}

SLIB_INLINE sl_int32 Math::sign(double v)
{
	return (v >= 0) ? 1 : -1;
}

SLIB_INLINE float Math::square(float x)
{
	return x * x;
}

SLIB_INLINE double Math::square(double x)
{
	return x * x;
}

SLIB_INLINE float Math::saturate(float f)
{
	return (f<0.0f) ? 0.0f : ((f>1.0f) ? 1.0f : f);
}

SLIB_INLINE double Math::saturate(double f)
{
	return (f<0.0) ? 0.0 : ((f>1.0) ? 1.0 : f);
}

template <typename T>
SLIB_INLINE T Math::clamp(T v, T vMin, T vMax)
{
	return (v<vMin) ? vMin : ((v>vMax) ? vMax : v);
}

SLIB_INLINE sl_int32 Math::clamp0_255(sl_int32 v)
{
	v = ((-v) >> 31) & v; // arithmetic sign shift, clamp to >=0
	v = (((0xFF - v) >> 31) | v) & 0xFF; // clamp to < 256
	return v;
}

SLIB_INLINE sl_int32 Math::clamp0_65535(sl_int32 v)
{
	v = ((-v) >> 31) & v; // arithmetic sign shift, clamp to >=0
	v = (((0xFFFF - v) >> 31) | v) & 0xFFFF; // clamp to < 65536
	return v;
}

SLIB_INLINE sl_bool Math::isNearZero(float f)
{
	return f > -SLIB_EPSILON && f < SLIB_EPSILON;
}

SLIB_INLINE sl_bool Math::isNearZero(double f)
{
	return f > -SLIB_EPSILON_LONG && f < SLIB_EPSILON_LONG;
}

SLIB_INLINE sl_bool Math::isLessThanEpsilon(float f)
{
	return f < SLIB_EPSILON;
}

SLIB_INLINE sl_bool Math::isLessThanEpsilon(double f)
{
	return f < SLIB_EPSILON_LONG;
}

SLIB_INLINE float Math::getRadianFromDegrees(float f)
{
	return f * SLIB_PI / 180.0f;
}

SLIB_INLINE double Math::getRadianFromDegrees(double f)
{
	return f * SLIB_PI_LONG / 180.0;
}

SLIB_INLINE float Math::getDegreesFromRadian(float f)
{
	return f * 180.0f / SLIB_PI;
}

SLIB_INLINE double Math::getDegreesFromRadian(double f)
{
	return f * 180.0 / SLIB_PI_LONG;
}

SLIB_INLINE void Math::getPI(float* v)
{
	*v = SLIB_PI;
}

SLIB_INLINE void Math::getPI(double* v)
{
	*v = SLIB_PI_LONG;
}

SLIB_INLINE void Math::get2PI(float* v)
{
	*v = SLIB_2_PI;
}

SLIB_INLINE void Math::get2PI(double* v)
{
	*v = SLIB_2_PI_LONG;
}

SLIB_INLINE void Math::getPI2(float* v)
{
	*v = SLIB_PI_2;
}

SLIB_INLINE void Math::getPI2(double* v)
{
	*v = SLIB_PI_2_LONG;
}

SLIB_INLINE void Math::getPI4(float* v)
{
	*v = SLIB_PI_4;
}

SLIB_INLINE void Math::getPI4(double* v)
{
	*v = SLIB_PI_4_LONG;
}

SLIB_INLINE void Math::getEpsilon(float* v)
{
	*v = SLIB_EPSILON;
}

SLIB_INLINE void Math::getEpsilon(double* v)
{
	*v = SLIB_EPSILON_LONG;
}

SLIB_INLINE sl_uint32 Math::rotateLeft32(sl_uint32 x, sl_uint32 n)
{
	return (x << n) | (x >> (32 - n));
}

SLIB_INLINE sl_uint64 Math::rotateLeft64(sl_uint64 x, sl_uint32 n)
{
	return (x << n) | (x >> (64 - n));
}

SLIB_INLINE sl_uint32 Math::rotateRight32(sl_uint32 x, sl_uint32 n)
{
	return (x >> n) | (x << (32 - n));
}

SLIB_INLINE sl_uint64 Math::rotateRight64(sl_uint64 x, sl_uint32 n)
{
	return (x >> n) | (x << (64 - n));
}

SLIB_INLINE void Math::mul32(sl_uint32 a, sl_uint32 b, sl_uint32& o_high, sl_uint32& o_low)
{
#if defined(SLIB_COMPILER_IS_VISUALSTUDIO)
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

SLIB_INLINE void Math::mul64(sl_uint64 a, sl_uint64 b, sl_uint64& o_high, sl_uint64& o_low)
{
#if defined(SLIB_COMPILER_IS_VISUALSTUDIO) && defined(SLIB_ARCH_IS_64BIT)
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

SLIB_NAMESPACE_END

#endif
