#include "../../../inc/slib/media/video_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

VideoEncoder::VideoEncoder()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nKeyFrameInterval = 0;
	m_bitrate = 0;
}

sl_uint32 VideoEncoder::getBitrate()
{
	return m_bitrate;
}

void VideoEncoder::setBitrate(sl_uint32 bitrate)
{
	m_bitrate = bitrate;
}

VideoDecoder::VideoDecoder()
{
	m_nWidth = 0;
	m_nHeight = 0;
}

SLIB_MEDIA_NAMESPACE_END

