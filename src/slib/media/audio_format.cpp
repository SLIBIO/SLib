#include "../../../inc/slib/media/audio_format.h"

SLIB_MEDIA_NAMESPACE_BEGIN

AudioSampleType AudioFormats::getSampleType(AudioFormat format)
{
	return (AudioSampleType)((sl_uint32)(format) & 255);
}

sl_uint32 AudioFormats::getBitsPerSample(AudioFormat format)
{
	return ((sl_uint32)(format) >> 8) & 255;
}

sl_uint32 AudioFormats::getBytesPerSample(AudioFormat format)
{
	return ((sl_uint32)(format) >> 11) & 31;
}

sl_uint32 AudioFormats::getChannelsCount(AudioFormat format)
{
	return ((sl_uint32)(format) >> 16) & 255;
}

sl_bool AudioFormats::isNonInterleaved(AudioFormat format)
{
	return ((sl_uint32)(format) >> 24) & 1;
}

sl_bool AudioFormats::isFloat(AudioFormat format)
{
	AudioSampleType type = getSampleType(format);
	return type == AudioSampleType::Float || type == AudioSampleType::FloatLE || type == AudioSampleType::FloatBE;
}

SLIB_MEDIA_NAMESPACE_END

