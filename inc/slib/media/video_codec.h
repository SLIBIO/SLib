#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_CODEC
#define CHECKHEADER_SLIB_MEDIA_VIDEO_CODEC

#include "definition.h"

#include "video_format.h"

#include "../core/object.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT VideoEncoder : public Object
{
	SLIB_DECLARE_OBJECT(VideoEncoder, Object)
public:
	VideoEncoder();
	
	virtual sl_bool encode(const VideoFrame& input, void* output, sl_uint32& sizeOutput, sl_uint32& encodingTime) = 0;
public:
	SLIB_PROPERTY(sl_uint32, Bitrate)
	
protected:
	sl_uint32 m_nWidth;
	sl_uint32 m_nHeight;
	sl_uint32 m_nKeyFrameInterval;
};

class SLIB_EXPORT VideoDecoder : public Object
{
	SLIB_DECLARE_OBJECT(VideoDecoder, Object)
public:
	VideoDecoder();
	
	virtual sl_bool decode(const void* input, const sl_uint32& inputSize, VideoFrame& output) = 0;

protected:
	sl_uint32 m_nWidth;
	sl_uint32 m_nHeight;
};

SLIB_MEDIA_NAMESPACE_END

#endif
