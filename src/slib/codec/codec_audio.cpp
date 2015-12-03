#include "../../../inc/slib/codec/audio.h"

SLIB_CODEC_NAMESPACE_BEGIN
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
SLIB_CODEC_NAMESPACE_END

