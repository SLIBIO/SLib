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

namespace slib
{
	
	template <class T>
	class MathContants;
	
	template <>
	class MathContants<float>
	{
	public:
		constexpr static float PI = SLIB_PI;
		constexpr static float PI_DUAL = SLIB_PI_DUAL;
		constexpr static float PI_HALF = SLIB_PI_HALF;
		constexpr static float PI_QUARTER = SLIB_PI_QUARTER;
		constexpr static float EPSILON = SLIB_EPSILON;
	};
	
	template <>
	class MathContants<double>
	{
	public:
		constexpr static double PI = SLIB_PI_LONG;
		constexpr static double PI_DUAL = SLIB_PI_DUAL_LONG;
		constexpr static double PI_HALF = SLIB_PI_HALF_LONG;
		constexpr static double PI_QUARTER = SLIB_PI_QUARTER_LONG;
		constexpr static double EPSILON = SLIB_EPSILON_LONG;
	};
	
	class Math
	{
	public:
		template <typename T>
		constexpr static T max(T a, T b)
		{
			return (a > b) ? a : b;
		}

		template <typename T>
		constexpr static T min(T a, T b)
		{
			return (a < b) ? a : b;
		}

		template <typename T>
		constexpr static T abs(T v)
		{
			return (v > 0) ? (v) : (-v);
		}

		constexpr static sl_int32 sign(float v)
		{
			return (v >= 0) ? 1 : -1;
		}

		constexpr static sl_int32 sign(double v)
		{
			return (v >= 0) ? 1 : -1;
		}
	
		static float pow(float x, float y);

		static double pow(double x, double y);

		constexpr static float square(float x)
		{
			return x * x;
		}

		constexpr static double square(double x)
		{
			return x * x;
		}

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
	

		constexpr static float saturate(float f)
		{
			return (f<0.0f) ? 0.0f : ((f>1.0f) ? 1.0f : f);
		}

		constexpr static double saturate(double f)
		{
			return (f<0.0) ? 0.0 : ((f>1.0) ? 1.0 : f);
		}

		template <typename T>
		constexpr static T clamp(T v, T vMin, T vMax)
		{
			return (v<vMin) ? vMin : ((v>vMax) ? vMax : v);
		}

		constexpr static sl_int32 clamp0_255(sl_int32 v)
		{
			// ((-v) >> 31) & v <=> arithmetic sign shift, clamp to >=0
			return (((0xFF - (((-v) >> 31) & v)) >> 31) | (((-v) >> 31) & v)) & 0xFF; // clamp to < 256
		}

		constexpr static sl_int32 clamp0_65535(sl_int32 v)
		{
			// ((-v) >> 31) & v <=> arithmetic sign shift, clamp to >=0
			return (((0xFFFF - (((-v) >> 31) & v)) >> 31) | (((-v) >> 31) & v)) & 0xFFFF; // clamp to < 65536
		}
	

		constexpr static sl_bool isAlmostZero(float f)
		{
			return f > -SLIB_EPSILON && f < SLIB_EPSILON;
		}

		constexpr static sl_bool isAlmostZero(double f)
		{
			return f > -SLIB_EPSILON_LONG && f < SLIB_EPSILON_LONG;
		}

		constexpr static sl_bool isAlmostZero(sl_int32 v)
		{
			return v == 0;
		}

		constexpr static sl_bool isAlmostZero(sl_uint32 v)
		{
			return v == 0;
		}

		constexpr static sl_bool isAlmostZero(sl_int64 v)
		{
			return v == 0;
		}

		constexpr static sl_bool isAlmostZero(sl_uint64 v)
		{
			return v == 0;
		}
	
		constexpr static sl_bool isLessThanEpsilon(float f)
		{
			return f < SLIB_EPSILON;
		}

		constexpr static sl_bool isLessThanEpsilon(double f)
		{
			return f < SLIB_EPSILON_LONG;
		}

		constexpr static sl_bool isLessThanEpsilon(sl_int32 v)
		{
			return v <= 0;
		}

		constexpr static sl_bool isLessThanEpsilon(sl_uint32 v)
		{
			return v <= 0;
		}

		constexpr static sl_bool isLessThanEpsilon(sl_int64 v)
		{
			return v <= 0;
		}

		constexpr static sl_bool isLessThanEpsilon(sl_uint64 v)
		{
			return v <= 0;
		}
	

		constexpr static float getRadianFromDegrees(float f)
		{
			return f * SLIB_PI / 180.0f;
		}

		constexpr static double getRadianFromDegrees(double f)
		{
			return f * SLIB_PI_LONG / 180.0;
		}

		constexpr static float getDegreesFromRadian(float f)
		{
			return f * 180.0f / SLIB_PI;
		}

		constexpr static double getDegreesFromRadian(double f)
		{
			return f * 180.0 / SLIB_PI_LONG;
		}


		template <class T>
		constexpr static T PI()
		{
			return MathContants<T>::PI;
		}

		template <class T>
		constexpr static T DualPI()
		{
			return MathContants<T>::PI_DUAL;
		}

		template <class T>
		constexpr static T HalfPI()
		{
			return MathContants<T>::PI_HALF;
		}

		template <class T>
		constexpr static T QuarterPI()
		{
			return MathContants<T>::PI_QUARTER;
		}

		template <class T>
		constexpr static T Epsilon()
		{
			return MathContants<T>::EPSILON;
		}


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
	

		// random value between 0~1
		static double random();

		// random value between 0~10000
		static sl_uint32 randomInt();

		static void randomMemory(void* mem, sl_size size);


		static sl_uint32 roundUpToPowerOfTwo32(sl_uint32 num);

		static sl_uint64 roundUpToPowerOfTwo64(sl_uint64 num);
	

		constexpr static sl_uint32 rotateLeft32(sl_uint32 x, sl_uint32 n)
		{
			return (x << n) | (x >> (32 - n));
		}

		constexpr static sl_uint64 rotateLeft64(sl_uint64 x, sl_uint32 n)
		{
			return (x << n) | (x >> (64 - n));
		}

		constexpr static sl_uint32 rotateRight32(sl_uint32 x, sl_uint32 n)
		{
			return (x >> n) | (x << (32 - n));
		}

		constexpr static sl_uint64 rotateRight64(sl_uint64 x, sl_uint32 n)
		{
			return (x >> n) | (x << (64 - n));
		}


		static sl_uint32 getMostSignificantBits(sl_uint32 n);

		static sl_uint32 getMostSignificantBits(sl_uint64 n);

		static sl_uint32 getLeastSignificantBits(sl_uint32 n);

		static sl_uint32 getLeastSignificantBits(sl_uint64 n);
	

		static void mul32(sl_uint32 a, sl_uint32 b, sl_uint32& o_high, sl_uint32& o_low);

		static void mul64(sl_uint64 a, sl_uint64 b, sl_uint64& o_high, sl_uint64& o_low);

	};

}

#include "detail/math.h"

#endif
