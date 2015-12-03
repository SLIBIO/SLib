#ifndef CHECKHEADER_SLIB_CODEC_VIDEO
#define CHECKHEADER_SLIB_CODEC_VIDEO

#include "definition.h"

#include "../core/object.h"
#include "../graphics/color.h"
#include "../graphics/bitmap.h"

SLIB_CODEC_NAMESPACE_BEGIN

class SLIB_EXPORT VideoEncoder : public Object
{
	SLIB_DECLARE_OBJECT(VideoEncoder, Object)
public:
	VideoEncoder();
	
	virtual sl_bool encode(const BitmapDesc& input, void* output, sl_uint32& sizeOutput, sl_uint32& encodingTime) = 0;
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
	
	virtual sl_bool decode(const void* input, const sl_uint32& inputSize, BitmapDesc& output) = 0;

protected:
	sl_uint32 m_nWidth;
	sl_uint32 m_nHeight;
};

SLIB_CODEC_NAMESPACE_END

#endif
