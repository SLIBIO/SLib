#include "../../../inc/slib/media/audio_format.h"

SLIB_MEDIA_NAMESPACE_BEGIN

const AudioFormatValue AudioFormat::_null = audioFormat_Null;

AudioSampleType AudioFormat::getSampleType() const
{
	return (AudioSampleType)(m_format & 255);
}

sl_uint32 AudioFormat::getBitsPerSample() const
{
	return (m_format >> 8) & 255;
}

sl_uint32 AudioFormat::getBytesPerSample() const
{
	return (m_format >> 11) & 31;
}

sl_uint32 AudioFormat::getChannelsCount() const
{
	return (m_format >> 16) & 255;
}

sl_bool AudioFormat::isNonInterleaved() const
{
	return (m_format >> 24) & 1;
}

sl_bool AudioFormat::isFloat() const
{
	AudioSampleType type = getSampleType();
	return type == audioSampleType_Float || type == audioSampleType_FloatLE || type == audioSampleType_FloatBE;
}


#define _DEFINE_CONVERT_SAMPLES(TYPE_IN, TYPE_OUT) \
void AudioFormat::convertSamples(sl_size count, const TYPE_IN* in, TYPE_OUT* out) \
{ \
	for (sl_size i = 0; i < count; i++) { \
		AudioFormat::convertSample(in[i], out[i]); \
	} \
}

_DEFINE_CONVERT_SAMPLES(sl_int8, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_int8, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_int8, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_int8, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_int8, float)

_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_uint8, float)

_DEFINE_CONVERT_SAMPLES(sl_int16, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_int16, float)

_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_uint16, float)

_DEFINE_CONVERT_SAMPLES(float, sl_int8)
_DEFINE_CONVERT_SAMPLES(float, sl_uint8)
_DEFINE_CONVERT_SAMPLES(float, sl_int16)
_DEFINE_CONVERT_SAMPLES(float, sl_uint16)
_DEFINE_CONVERT_SAMPLES(float, float)

SLIB_MEDIA_NAMESPACE_END

