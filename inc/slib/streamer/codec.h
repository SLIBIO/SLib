#ifndef CHECKHEADER_SLIB_STREAMER_CODEC
#define CHECKHEADER_SLIB_STREAMER_CODEC

#include "definition.h"

#include "graph.h"
#include "../codec/audio_opus.h"

SLIB_STREAMER_NAMESPACE_BEGIN

class SLIB_EXPORT AudioOpusEncodeFilter : public Filter
{
public:
	SLIB_INLINE AudioOpusEncodeFilter(const Ref<AudioOpusEncoder>& encoder)
	{
		m_encoder = encoder;
	}

	List<Packet> filter(const Packet& input);

private:
	Ref<AudioOpusEncoder> m_encoder;
};

class SLIB_EXPORT AudioOpusDecodeFilter : public Filter
{
public:
	SLIB_INLINE AudioOpusDecodeFilter(const Ref<AudioOpusDecoder>& decoder)
	{
		m_decoder = decoder;
		setMaxSamplesPerFrame(1600);
	}

	List<Packet> filter(const Packet& input);

public:
	SLIB_PROPERTY_INLINE(sl_uint32, MaxSamplesPerFrame)

private:
	Ref<AudioOpusDecoder> m_decoder;
};

SLIB_STREAMER_NAMESPACE_END

#endif
