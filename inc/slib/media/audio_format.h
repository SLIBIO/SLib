#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_FORMAT
#define CHECKHEADER_SLIB_MEDIA_AUDIO_FORMAT

#include "definition.h"

#include "../core/math.h"

SLIB_MEDIA_NAMESPACE_BEGIN

enum AudioSampleType
{
	audioSampleType_Int8 = 1,
	audioSampleType_Uint8 = 2,
	
	audioSampleType_Int16 = 8,
	audioSampleType_Uint16 = 9,
	audioSampleType_Int16LE = 12,
	audioSampleType_Uint16LE = 13,
	audioSampleType_Int16BE = 14,
	audioSampleType_Uint16BE = 15,
	
	audioSampleType_Float = 32,
	audioSampleType_FloatLE = 33,
	audioSampleType_FloatBE = 34
};

/*
	Format Bits Description
 
 0~7: audio sample type
 8~15: bits per sample
 16~23: channels count
 24: is non-interleaved
 
*/

#define SLIB_DEFINE_AUDIO_FORMAT(sampleType, bitsPerSample, nChannels, isNonInterleaved) \
	((sampleType) | (bitsPerSample << 8) | (nChannels << 16) | (isNonInterleaved << 24))

enum AudioFormatValue
{
	audioFormat_Null = 0,
	
	audioFormat_Int8_Mono =							SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int8, 8, 1, 0),
	audioFormat_Int8_Stereo =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int8, 8, 2, 0),
	audioFormat_Int8_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int8, 8, 2, 1),
	
	audioFormat_Uint8_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint8, 8, 1, 0),
	audioFormat_Uint8_Stereo =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint8, 8, 2, 0),
	audioFormat_Uint8_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint8, 8, 2, 1),
	
	audioFormat_Int16_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16, 16, 1, 0),
	audioFormat_Int16_Stereo =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16, 16, 2, 0),
	audioFormat_Int16_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16, 16, 2, 1),
	
	audioFormat_Uint16_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16, 16, 1, 0),
	audioFormat_Uint16_Stereo =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16, 16, 2, 0),
	audioFormat_Uint16_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16, 16, 2, 1),
	
	audioFormat_Int16LE_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16LE, 16, 1, 0),
	audioFormat_Int16LE_Stereo =					SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16LE, 16, 2, 0),
	audioFormat_Int16LE_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16LE, 16, 2, 1),
	
	audioFormat_Uint16LE_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16LE, 16, 1, 0),
	audioFormat_Uint16LE_Stereo =					SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16LE, 16, 2, 0),
	audioFormat_Uint16LE_Stereo_NonInterleaved =	SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16LE, 16, 2, 1),
	
	audioFormat_Int16BE_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16BE, 16, 1, 0),
	audioFormat_Int16BE_Stereo =					SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16BE, 16, 2, 0),
	audioFormat_Int16BE_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Int16BE, 16, 2, 1),
	
	audioFormat_Uint16BE_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16BE, 16, 1, 0),
	audioFormat_Uint16BE_Stereo =					SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16BE, 16, 2, 0),
	audioFormat_Uint16BE_Stereo_NonInterleaved =	SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Uint16BE, 16, 2, 1),
	
	audioFormat_Float_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Float, 32, 1, 0),
	audioFormat_Float_Stereo =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Float, 32, 2, 0),
	audioFormat_Float_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_Float, 32, 2, 1),
	
	audioFormat_FloatLE_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_FloatLE, 32, 1, 0),
	audioFormat_FloatLE_Stereo =					SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_FloatLE, 32, 2, 0),
	audioFormat_FloatLE_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_FloatLE, 32, 2, 1),
	
	audioFormat_FloatBE_Mono =						SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_FloatBE, 32, 1, 0),
	audioFormat_FloatBE_Stereo =					SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_FloatBE, 32, 2, 0),
	audioFormat_FloatBE_Stereo_NonInterleaved =		SLIB_DEFINE_AUDIO_FORMAT(audioSampleType_FloatBE, 32, 2, 1)

};


class SLIB_EXPORT AudioFormat
{
private:
	AudioFormatValue m_format;
	
public:
	SLIB_INLINE AudioFormat()
	{
		m_format = audioFormat_Null;
	}
	
	SLIB_INLINE AudioFormat(const AudioFormat& other)
	{
		m_format = other.m_format;
	}
	
	SLIB_INLINE AudioFormat(AudioFormatValue format)
	{
		m_format = format;
	}
	
private:
	static const AudioFormatValue _null;
	
public:
	SLIB_INLINE AudioFormatValue getValue() const
	{
		return m_format;
	}
	
	SLIB_INLINE static const AudioFormat& null()
	{
		return *((AudioFormat*)((void*)&_null));
	}
	
	SLIB_INLINE void setNull()
	{
		m_format = audioFormat_Null;
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_format == audioFormat_Null;
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_format != audioFormat_Null;
	}
	
public:
	SLIB_INLINE AudioFormat& operator=(const AudioFormat& other)
	{
		m_format = other.m_format;
		return *this;
	}
	
	SLIB_INLINE AudioFormat& operator=(AudioFormatValue other)
	{
		m_format = other;
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const AudioFormat& other) const
	{
		return m_format == other.m_format;
	}
	
	SLIB_INLINE sl_bool operator==(AudioFormatValue other) const
	{
		return m_format == other;
	}
	
	SLIB_INLINE sl_bool operator!=(const AudioFormat& other) const
	{
		return m_format != other.m_format;
	}
	
	SLIB_INLINE sl_bool operator!=(AudioFormatValue other) const
	{
		return m_format != other;
	}
	
public:
	AudioSampleType getSampleType() const;
	
	sl_uint32 getBitsPerSample() const;
	
	sl_uint32 getBytesPerSample() const;
	
	sl_uint32 getChannelsCount() const;
	
	sl_bool isNonInterleaved() const;
	
	sl_bool isFloat() const;
	
public:
	// S8 => S8
	SLIB_INLINE static void convertSample(sl_int8 _in, sl_int8& _out)
	{
		_out = _in;
	}
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_int8* _out);
	
	// S8 => U8
	SLIB_INLINE static void convertSample(sl_int8 _in, sl_uint8& _out)
	{
		_out = (sl_uint8)((sl_int32)_in + 128);
	}
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_uint8* _out);
	
	// S8 => S16
	SLIB_INLINE static void convertSample(sl_int8 _in, sl_int16& _out)
	{
		_out = (sl_int16)(_in) << 8;
	}
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_int16* _out);
	
	// S8 => U16
	SLIB_INLINE static void convertSample(sl_int8 _in, sl_uint16& _out)
	{
		_out = (sl_uint16)((sl_int32)_in + 128) << 8;
	}
	
	static void convertSamples(sl_size count, const sl_int8* _in, sl_uint16* _out);
	
	// S8 => Float
	SLIB_INLINE static void convertSample(sl_int8 _in, float& _out)
	{
		_out = (float)(_in) / 128.0f;
	}
	
	static void convertSamples(sl_size count, const sl_int8* _in, float* _out);
	


	// U8 => S8
	SLIB_INLINE static void convertSample(sl_uint8 _in, sl_int8& _out)
	{
		_out = (sl_int8)((sl_int32)_in - 128);
	}
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_int8* _out);
	
	// U8 => U8
	SLIB_INLINE static void convertSample(sl_uint8 _in, sl_uint8& _out)
	{
		_out = _in;
	}
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_uint8* _out);
	
	// U8 => S16
	SLIB_INLINE static void convertSample(sl_uint8 _in, sl_int16& _out)
	{
		_out = (sl_int16)(((sl_int32)(_in) << 8) - 0x8000);
	}
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_int16* _out);
	
	// U8 => U16
	SLIB_INLINE static void convertSample(sl_uint8 _in, sl_uint16& _out)
	{
		_out = (sl_uint16)(_in) << 8;
	}
	
	static void convertSamples(sl_size count, const sl_uint8* _in, sl_uint16* _out);
	
	// U8 => Float
	SLIB_INLINE static void convertSample(sl_uint8 _in, float& _out)
	{
		_out = (float)((sl_int8)((sl_int32)_in - 128)) / 128.0f;
	}
	
	static void convertSamples(sl_size count, const sl_uint8* _in, float* _out);
	
	

	// S16 => S8
	SLIB_INLINE static void convertSample(sl_int16 _in, sl_int8& _out)
	{
		_out = (sl_int8)(_in >> 8);
	}
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_int8* _out);
	
	// S16 => U8
	SLIB_INLINE static void convertSample(sl_int16 _in, sl_uint8& _out)
	{
		_out = (sl_uint8)(((sl_uint16)((sl_int32)_in + 0x8000)) >> 8);
	}
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_uint8* _out);
	
	// S16 => S16
	SLIB_INLINE static void convertSample(sl_int16 _in, sl_int16& _out)
	{
		_out = _in;
	}
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_int16* _out);
	
	// S16 => U16
	SLIB_INLINE static void convertSample(sl_int16 _in, sl_uint16& _out)
	{
		_out = (sl_uint16)((sl_int32)_in + 0x8000);
	}
	
	static void convertSamples(sl_size count, const sl_int16* _in, sl_uint16* _out);
	
	// S16 => Float
	SLIB_INLINE static void convertSample(sl_int16 _in, float& _out)
	{
		_out = (float)(_in) / 32768.0f;
	}
	
	static void convertSamples(sl_size count, const sl_int16* _in, float* _out);
	
	
	
	// U16 => S8
	SLIB_INLINE static void convertSample(sl_uint16 _in, sl_int8& _out)
	{
		_out = (sl_int8)((sl_int32)(_in >> 8) - 128);
	}
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_int8* _out);
	
	// U16 => U8
	SLIB_INLINE static void convertSample(sl_uint16 _in, sl_uint8& _out)
	{
		_out = (sl_uint8)(_in >> 8);
	}
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_uint8* _out);
	
	// U16 => S16
	SLIB_INLINE static void convertSample(sl_uint16 _in, sl_int16& _out)
	{
		_out = (sl_int16)((sl_int32)_in - 0x8000);
	}
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_int16* _out);
	
	// U16 => U16
	SLIB_INLINE static void convertSample(sl_uint16 _in, sl_uint16& _out)
	{
		_out = _in;
	}
	
	static void convertSamples(sl_size count, const sl_uint16* _in, sl_uint16* _out);
	
	// U16 => Float
	SLIB_INLINE static void convertSample(sl_uint16 _in, float& _out)
	{
		_out = (float)((sl_int16)((sl_int32)_in - 0x8000)) / 32768.0f;
	}
	
	static void convertSamples(sl_size count, const sl_uint16* _in, float* _out);
	
	
	
	// Float => S8
	SLIB_INLINE static void convertSample(float _in, sl_int8& _out)
	{
		_out = (sl_int8)(Math::clamp0_255((sl_int32)(_in * 128.0f) + 128) - 128);
	}
	
	static void convertSamples(sl_size count, const float* _in, sl_int8* _out);
	
	// Float => U8
	SLIB_INLINE static void convertSample(float _in, sl_uint8& _out)
	{
		_out = (sl_uint8)(Math::clamp0_255((sl_int32)(_in * 128.0f) + 128));
	}
	
	static void convertSamples(sl_size count, const float* _in, sl_uint8* _out);
	
	// Float => S16
	SLIB_INLINE static void convertSample(float _in, sl_int16& _out)
	{
		_out = (sl_int16)(Math::clamp0_65535((sl_int32)(_in * 32768.0f)) - 0x8000);
	}
	
	static void convertSamples(sl_size count, const float* _in, sl_int16* _out);
	
	// Float => U16
	SLIB_INLINE static void convertSample(float _in, sl_uint16& _out)
	{
		_out = (sl_int16)(Math::clamp0_65535((sl_int32)(_in * 32768.0f)));
	}
	
	static void convertSamples(sl_size count, const float* _in, sl_uint16* _out);
	
	// Float => Float
	SLIB_INLINE static void convertSample(float _in, float& _out)
	{
		_out = _in;
	}
	
	static void convertSamples(sl_size count, const float* _in, float* _out);

public:
	SLIB_INLINE static void mixSamples(sl_int8 in1, sl_int8 in2, sl_int8& _out)
	{
		_out = (sl_int8)(((sl_int32)in1 + (sl_int32)in2) >> 1);
	}
	
	SLIB_INLINE static void mixSamples(sl_uint8 in1, sl_uint8 in2, sl_uint8& _out)
	{
		_out = (sl_uint8)(((sl_uint32)in1 + (sl_uint32)in2) >> 1);
	}
	
	SLIB_INLINE static void mixSamples(sl_int16 in1, sl_int16 in2, sl_int16& _out)
	{
		_out = (sl_int16)(((sl_int32)in1 + (sl_int32)in2) >> 1);
	}
	
	SLIB_INLINE static void mixSamples(sl_uint16 in1, sl_uint16 in2, sl_uint16& _out)
	{
		_out = (sl_uint16)(((sl_int32)in1 + (sl_int32)in2) >> 1);
	}
	
	SLIB_INLINE static void mixSamples(float in1, float in2, float& _out)
	{
		_out = (in1 + in2) / 2;
	}

};

SLIB_MEDIA_NAMESPACE_END

#endif
