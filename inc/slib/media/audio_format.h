#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_FORMAT
#define CHECKHEADER_SLIB_MEDIA_AUDIO_FORMAT

#include "definition.h"

#include "../core/math.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT AudioFormat
{
public:
	// U8 => S16
	SLIB_INLINE static void convertSample(sl_uint8 _in, sl_int16& _out)
	{
		sl_uint16 v = (sl_uint16)(_in);
		v <<= 8;
		v -= 0x8000;
		_out = v;
	}
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_int16* _out);
	
	// U8 => Float
	SLIB_INLINE static void convertSample(sl_uint8 _in, float& _out)
	{
		float f = (float)((sl_int8)(_in - 0x80));
		_out = f / 128.0f;
	}
	static void convertSamples(sl_size count, const sl_uint8* _in, float* _out);

	// S16 => U8
	SLIB_INLINE static void convertSample(sl_int16 _in, sl_uint8& _out)
	{
		sl_uint16 v = (sl_uint16)(_in + 0x8000);
		v >>= 8;
		_out = (sl_uint8)(v);
	}
	static void convertSamples(sl_size count, const sl_int16* _in, sl_uint8* _out);
	
	// S16 => Float
	SLIB_INLINE static void convertSample(sl_int16 _in, float& _out)
	{
		float f = (float)(_in);
		_out = f / 32768.0f;
	}
	static void convertSamples(sl_size count, const sl_int16* _in, float* _out);
	
	// Float => U8
	SLIB_INLINE static void convertSample(float _in, sl_uint8& _out)
	{
		sl_int32 v = (sl_int32)(_in * 128.0f);
		v = Math::clamp0_255(v + 0x80);
		_out = (sl_uint8)v;
	}
	static void convertSamples(sl_size count, const float* _in, sl_uint8* _out);
	
	// Float => S16
	SLIB_INLINE static void convertSample(float _in, sl_int16& _out)
	{
		sl_int32 v = (sl_int32)(_in * 32768.0f);
		v = Math::clamp0_65535(v + 0x8000);
		_out = (sl_int16)(v - 0x8000);
	}
	static void convertSamples(sl_size count, const float* _in, sl_int16* _out);
};

SLIB_MEDIA_NAMESPACE_END

#endif
