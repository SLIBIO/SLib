#include "../../../inc/slib/media/audio_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN
AudioEncoder::AudioEncoder()
{
	m_nSamplesPerSecond = 0;
	m_nChannels = 0;
	AudioEncoder::setBitrate(0);
}

AudioDecoder::AudioDecoder()
{
	m_nSamplesPerSecond = 0;
	m_nChannels = 0;
}
SLIB_MEDIA_NAMESPACE_END

