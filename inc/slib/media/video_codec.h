#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_CODEC
#define CHECKHEADER_SLIB_MEDIA_VIDEO_CODEC

#include "definition.h"

#include "video_frame.h"

#include "../core/object.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT VideoEncoder : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	VideoEncoder();
	
public:
	virtual Memory encode(const VideoFrame& input) = 0;
	
public:
	sl_uint32 getBitrate();
	
	virtual void setBitrate(sl_uint32 bitrate);
	
protected:
	sl_uint32 m_nWidth;
	sl_uint32 m_nHeight;
	sl_uint32 m_nKeyFrameInterval;
	sl_uint32 m_bitrate;
	
};

class SLIB_EXPORT VideoDecoder : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	VideoDecoder();
	
public:
	virtual sl_bool decode(const void* input, const sl_uint32& inputSize, VideoFrame& output) = 0;
	
protected:
	sl_uint32 m_nWidth;
	sl_uint32 m_nHeight;
	
};

SLIB_MEDIA_NAMESPACE_END

#endif
