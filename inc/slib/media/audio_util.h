#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_UTIL
#define CHECKHEADER_SLIB_MEDIA_AUDIO_UTIL

#include "definition.h"

#include "../core/math.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT AudioUtil
{
public:
	// S8 => S8
	static void convertSample(sl_int8 _in, sl_int8& _out);
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_int8* _out);
	
	// S8 => U8
	static void convertSample(sl_int8 _in, sl_uint8& _out);
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_uint8* _out);
	
	// S8 => S16
	static void convertSample(sl_int8 _in, sl_int16& _out);
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_int16* _out);
	
	// S8 => U16
	static void convertSample(sl_int8 _in, sl_uint16& _out);
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_uint16* _out);
	
	// S8 => Float
	static void convertSample(sl_int8 _in, float& _out);
	
	static void convertSamples(sl_size count, const sl_int8* _in, float* _out);
	
	
	// U8 => S8
	static void convertSample(sl_uint8 _in, sl_int8& _out);
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_int8* _out);
	
	// U8 => U8
	static void convertSample(sl_uint8 _in, sl_uint8& _out);
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_uint8* _out);
	
	// U8 => S16
	static void convertSample(sl_uint8 _in, sl_int16& _out);
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_int16* _out);
	
	// U8 => U16
	static void convertSample(sl_uint8 _in, sl_uint16& _out);
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_uint16* _out);
	
	// U8 => Float
	static void convertSample(sl_uint8 _in, float& _out);
	
	static void convertSamples(sl_size count, const sl_uint8* _in, float* _out);
	
	
	// S16 => S8
	static void convertSample(sl_int16 _in, sl_int8& _out);
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_int8* _out);
	
	// S16 => U8
	static void convertSample(sl_int16 _in, sl_uint8& _out);
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_uint8* _out);
	
	// S16 => S16
	static void convertSample(sl_int16 _in, sl_int16& _out);
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_int16* _out);
	
	// S16 => U16
	static void convertSample(sl_int16 _in, sl_uint16& _out);
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_uint16* _out);
	
	// S16 => Float
	static void convertSample(sl_int16 _in, float& _out);
	
	static void convertSamples(sl_size count, const sl_int16* _in, float* _out);
	
	
	
	// U16 => S8
	static void convertSample(sl_uint16 _in, sl_int8& _out);
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_int8* _out);
	
	// U16 => U8
	static void convertSample(sl_uint16 _in, sl_uint8& _out);
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_uint8* _out);
	
	// U16 => S16
	static void convertSample(sl_uint16 _in, sl_int16& _out);
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_int16* _out);
	
	// U16 => U16
	static void convertSample(sl_uint16 _in, sl_uint16& _out);
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_uint16* _out);
	
	// U16 => Float
	static void convertSample(sl_uint16 _in, float& _out);
	
	static void convertSamples(sl_size count, const sl_uint16* _in, float* _out);
	
	
	// Float => S8
	static void convertSample(float _in, sl_int8& _out);
	
	static void convertSamples(sl_size count, const float* _in, sl_int8* _out);
	
	// Float => U8
	static void convertSample(float _in, sl_uint8& _out);
	
	static void convertSamples(sl_size count, const float* _in, sl_uint8* _out);
	
	// Float => S16
	static void convertSample(float _in, sl_int16& _out);
	
	static void convertSamples(sl_size count, const float* _in, sl_int16* _out);
	
	// Float => U16
	static void convertSample(float _in, sl_uint16& _out);
	
	static void convertSamples(sl_size count, const float* _in, sl_uint16* _out);
	
	// Float => Float
	static void convertSample(float _in, float& _out);
	
	static void convertSamples(sl_size count, const float* _in, float* _out);
	
	
	static void mixSamples(sl_int8 in1, sl_int8 in2, sl_int8& _out);
	
	static void mixSamples(sl_uint8 in1, sl_uint8 in2, sl_uint8& _out);
	
	static void mixSamples(sl_int16 in1, sl_int16 in2, sl_int16& _out);
	
	static void mixSamples(sl_uint16 in1, sl_uint16 in2, sl_uint16& _out);
	
	static void mixSamples(float in1, float in2, float& _out);

};

SLIB_MEDIA_NAMESPACE_END


SLIB_MEDIA_NAMESPACE_BEGIN

SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_int8& _out)
{
	_out = _in;
}

SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_uint8& _out)
{
	_out = (sl_uint8)((sl_int32)_in + 128);
}

SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_int16& _out)
{
	_out = (sl_int16)(_in) << 8;
}

SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_uint16& _out)
{
	_out = (sl_uint16)((sl_int32)_in + 128) << 8;
}

SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, float& _out)
{
	_out = (float)(_in) / 128.0f;
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_int8& _out)
{
	_out = (sl_int8)((sl_int32)_in - 128);
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_uint8& _out)
{
	_out = _in;
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_int16& _out)
{
	_out = (sl_int16)(((sl_int32)(_in) << 8) - 0x8000);
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_uint16& _out)
{
	_out = (sl_uint16)(_in) << 8;
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, float& _out)
{
	_out = (float)((sl_int8)((sl_int32)_in - 128)) / 128.0f;
}

SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_int8& _out)
{
	_out = (sl_int8)(_in >> 8);
}

SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_uint8& _out)
{
	_out = (sl_uint8)(((sl_uint16)((sl_int32)_in + 0x8000)) >> 8);
}

SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_int16& _out)
{
	_out = _in;
}

SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_uint16& _out)
{
	_out = (sl_uint16)((sl_int32)_in + 0x8000);
}

SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, float& _out)
{
	_out = (float)(_in) / 32768.0f;
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_int8& _out)
{
	_out = (sl_int8)((sl_int32)(_in >> 8) - 128);
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_uint8& _out)
{
	_out = (sl_uint8)(_in >> 8);
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_int16& _out)
{
	_out = (sl_int16)((sl_int32)_in - 0x8000);
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_uint16& _out)
{
	_out = _in;
}

SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, float& _out)
{
	_out = (float)((sl_int16)((sl_int32)_in - 0x8000)) / 32768.0f;
}

SLIB_INLINE void AudioUtil::convertSample(float _in, sl_int8& _out)
{
	_out = (sl_int8)(Math::clamp0_255((sl_int32)(_in * 128.0f) + 128) - 128);
}

SLIB_INLINE void AudioUtil::convertSample(float _in, sl_uint8& _out)
{
	_out = (sl_uint8)(Math::clamp0_255((sl_int32)(_in * 128.0f) + 128));
}

SLIB_INLINE void AudioUtil::convertSample(float _in, sl_int16& _out)
{
	_out = (sl_int16)(Math::clamp0_65535((sl_int32)(_in * 32768.0f)) - 0x8000);
}

SLIB_INLINE void AudioUtil::convertSample(float _in, sl_uint16& _out)
{
	_out = (sl_int16)(Math::clamp0_65535((sl_int32)(_in * 32768.0f)));
}

SLIB_INLINE void AudioUtil::convertSample(float _in, float& _out)
{
	_out = _in;
}

SLIB_INLINE void AudioUtil::mixSamples(sl_int8 in1, sl_int8 in2, sl_int8& _out)
{
	_out = (sl_int8)(((sl_int32)in1 + (sl_int32)in2) >> 1);
}

SLIB_INLINE void AudioUtil::mixSamples(sl_uint8 in1, sl_uint8 in2, sl_uint8& _out)
{
	_out = (sl_uint8)(((sl_uint32)in1 + (sl_uint32)in2) >> 1);
}

SLIB_INLINE void AudioUtil::mixSamples(sl_int16 in1, sl_int16 in2, sl_int16& _out)
{
	_out = (sl_int16)(((sl_int32)in1 + (sl_int32)in2) >> 1);
}

SLIB_INLINE void AudioUtil::mixSamples(sl_uint16 in1, sl_uint16 in2, sl_uint16& _out)
{
	_out = (sl_uint16)(((sl_int32)in1 + (sl_int32)in2) >> 1);
}

SLIB_INLINE void AudioUtil::mixSamples(float in1, float in2, float& _out)
{
	_out = (in1 + in2) / 2;
}

SLIB_MEDIA_NAMESPACE_END

#endif
