#include "../../../inc/slib/media/video_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(VideoEncoder, Object)

VideoEncoder::VideoEncoder()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nKeyFrameInterval = 0;
	m_bitrate = 0;
}

VideoEncoder::~VideoEncoder()
{
}

sl_uint32 VideoEncoder::getBitrate()
{
	return m_bitrate;
}

void VideoEncoder::setBitrate(sl_uint32 bitrate)
{
	m_bitrate = bitrate;
}


SLIB_DEFINE_OBJECT(VideoDecoder, Object)

VideoDecoder::VideoDecoder()
{
	m_nWidth = 0;
	m_nHeight = 0;
}

VideoDecoder::~VideoDecoder()
{
}

SLIB_MEDIA_NAMESPACE_END

