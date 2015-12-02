
#ifndef CHECKHEADER_SLIB_CODEC_OPUS
#define CHECKHEADER_SLIB_CODEC_OPUS

#include "definition.h"
#include "object.h"

SLIB_CODEC_NAMESPACE_BEGIN

struct SLIB_EXPORT AudioOpusEncoderParam
{
	enum Type {
		typeVoice = 0,
		typeMusic = 1
	};
	sl_uint32 nSamplesPerSecond;
	sl_uint32 nChannels;
	sl_uint32 nBitsPerSecond;
	Type type;
	sl_uint32 nMillisecondsRestart;

	AudioOpusEncoderParam()
	{
		nSamplesPerSecond = 16000;
		nChannels = 1;
		nBitsPerSecond = 8000;
		type = typeVoice;
		nMillisecondsRestart = 10000;
	}
};

class SLIB_EXPORT AudioOpusEncoder : public AudioEncoder
{
public:
	AudioOpusEncoder()
	{
	}

public:
	static Ref<AudioOpusEncoder> create(const AudioOpusEncoderParam& param);
	static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
};

struct SLIB_EXPORT AudioOpusDecoderParam
{
	sl_uint32 nSamplesPerSecond;
	sl_uint32 nChannels;

	AudioOpusDecoderParam()
	{
		nSamplesPerSecond = 16000;
		nChannels = 1;
	}
};

class SLIB_EXPORT AudioOpusDecoder : public AudioDecoder
{
public:
	AudioOpusDecoder()
	{
	}

public:
	static Ref<AudioOpusDecoder> create(const AudioOpusDecoderParam& param);
	static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
};
SLIB_CODEC_NAMESPACE_END

#endif
