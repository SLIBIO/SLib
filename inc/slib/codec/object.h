#ifndef CHECKHEADER_SLIB_CODEC_OBJECT
#define CHECKHEADER_SLIB_CODEC_OBJECT

#include "definition.h"

#include "../core/object.h"
#include "../graphics/color.h"
#include "../graphics/bitmap.h"

SLIB_CODEC_NAMESPACE_BEGIN
class SLIB_EXPORT VideoEncoder : public Object
{
	SLIB_DECLARE_OBJECT(VideoEncoder, Object)
public:
	SLIB_INLINE VideoEncoder() {}
	/* the count means "Count of Samples per Channel in a frame" */
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
	SLIB_INLINE VideoDecoder() {}
	/* the count means "Count of Samples per Channel in a frame" */
	virtual sl_bool decode(const void* input, const sl_uint32& inputSize, BitmapDesc& output) = 0;
protected:
	sl_uint32 m_nWidth;
	sl_uint32 m_nHeight;
};

class SLIB_EXPORT AudioEncoder : public Object
{
	SLIB_DECLARE_OBJECT(AudioEncoder, Object)
public:
	SLIB_INLINE AudioEncoder() {}

	/* the count means "Count of Samples per Channel in a frame" */
	virtual sl_bool encode(const sl_int16* input, sl_uint32& countInput, void* output, sl_uint32& sizeOutput) = 0;
	
public:
	SLIB_PROPERTY(sl_uint32, Bitrate)

public:
	SLIB_INLINE sl_uint32 getSamplesCountPerSecond()
	{
		return m_nSamplesPerSecond;
	}
	SLIB_INLINE sl_uint32 getChannelsCount()
	{
		return m_nChannels;
	}

protected:
	sl_uint32 m_nSamplesPerSecond;
	sl_uint32 m_nChannels;
};

class SLIB_EXPORT AudioDecoder : public Object
{
	SLIB_DECLARE_OBJECT(AudioDecoder, Object)
public:
	SLIB_INLINE AudioDecoder() {}

	/* the count means "Count of Samples per Channel in a frame" */
	virtual sl_bool decode(const void* input, sl_uint32& sizeInput, sl_int16* output, sl_uint32& countOutput) = 0;

public:
	SLIB_INLINE sl_uint32 getSamplesCountPerSecond()
	{
		return m_nSamplesPerSecond;
	}
	SLIB_INLINE sl_uint32 getChannelsCount()
	{
		return m_nChannels;
	}

protected:
	sl_uint32 m_nSamplesPerSecond;
	sl_uint32 m_nChannels;
};
SLIB_CODEC_NAMESPACE_END

#endif
