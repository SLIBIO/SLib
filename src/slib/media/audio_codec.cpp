#include "../../../inc/slib/media/audio_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

AudioEncoder::AudioEncoder()
{
	m_nSamplesPerSecond = 0;
	m_nChannels = 0;
	m_bitrate = 0;
}

sl_uint32 AudioEncoder::getBitrate()
{
	return m_bitrate;
}

void AudioEncoder::setBitrate(sl_uint32 bitrate)
{
	m_bitrate = bitrate;
}

AudioDecoder::AudioDecoder()
{
	m_nSamplesPerSecond = 0;
	m_nChannels = 0;
}

SLIB_MEDIA_NAMESPACE_END

