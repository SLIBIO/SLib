#include "../../../inc/slib/codec/video.h"

SLIB_CODEC_NAMESPACE_BEGIN
VideoEncoder::VideoEncoder()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nKeyFrameInterval = 0;
	VideoEncoder::setBitrate(0);
}

VideoDecoder::VideoDecoder()
{
	m_nWidth = 0;
	m_nHeight = 0;
}
SLIB_CODEC_NAMESPACE_END

