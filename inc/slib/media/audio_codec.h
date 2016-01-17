#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_CODEC
#define CHECKHEADER_SLIB_MEDIA_AUDIO_CODEC

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/memory.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT AudioEncoder : public Object
{
	SLIB_DECLARE_OBJECT(AudioEncoder, Object)
public:
	AudioEncoder();

public:
	virtual Memory encode(const AudioData& input) = 0;
	
public:
	SLIB_PROPERTY(sl_uint32, Bitrate)

public:
	SLIB_INLINE sl_uint32 getSamplesCountPerSecond() const
	{
		return m_nSamplesPerSecond;
	}
	
	SLIB_INLINE sl_uint32 getChannelsCount() const
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

public:
	// returns the count of samples (per channel) actually decoded
	virtual sl_uint32 decode(const void* input, sl_uint32 sizeInput, const AudioData& output) = 0;

public:
	SLIB_INLINE sl_uint32 getSamplesCountPerSecond() const
	{
		return m_nSamplesPerSecond;
	}
	
	SLIB_INLINE sl_uint32 getChannelsCount() const
	{
		return m_nChannels;
	}

protected:
	sl_uint32 m_nSamplesPerSecond;
	sl_uint32 m_nChannels;
};

SLIB_MEDIA_NAMESPACE_END

#endif
