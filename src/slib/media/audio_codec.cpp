#include "../../../inc/slib/media/audio_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(AudioEncoder, Object)

AudioEncoder::AudioEncoder()
{
	m_nSamplesPerSecond = 0;
	m_nChannels = 0;
	m_bitrate = 0;
}

AudioEncoder::~AudioEncoder()
{
}

sl_uint32 AudioEncoder::getSamplesCountPerSecond() const
{
	return m_nSamplesPerSecond;
}

sl_uint32 AudioEncoder::getChannelsCount() const
{
	return m_nChannels;
}

sl_uint32 AudioEncoder::getBitrate()
{
	return m_bitrate;
}

void AudioEncoder::setBitrate(sl_uint32 bitrate)
{
	m_bitrate = bitrate;
}


SLIB_DEFINE_OBJECT(AudioDecoder, Object)

AudioDecoder::AudioDecoder()
{
	m_nSamplesPerSecond = 0;
	m_nChannels = 0;
}

AudioDecoder::~AudioDecoder()
{
}

sl_uint32 AudioDecoder::getSamplesCountPerSecond() const
{
	return m_nSamplesPerSecond;
}

sl_uint32 AudioDecoder::getChannelsCount() const
{
	return m_nChannels;
}

SLIB_MEDIA_NAMESPACE_END

