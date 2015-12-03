#ifndef CHECKHEADER_SLIB_CODEC_AUDIO
#define CHECKHEADER_SLIB_CODEC_AUDIO

#include "definition.h"

#include "../core/object.h"

SLIB_CODEC_NAMESPACE_BEGIN

class SLIB_EXPORT AudioEncoder : public Object
{
	SLIB_DECLARE_OBJECT(AudioEncoder, Object)
public:
	AudioEncoder();

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
	AudioDecoder();

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
