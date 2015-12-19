#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_OPUS
#define CHECKHEADER_SLIB_MEDIA_CODEC_OPUS

#include "definition.h"
#include "audio_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

struct SLIB_EXPORT AudioOpusEncoderParam
{
	enum Type {
		typeVoice = 0,
		typeMusic = 1
	};
	sl_uint32 samplesPerSecond;
	sl_uint32 channelsCount;
	sl_uint32 bitsPerSecond;
	Type type;

	AudioOpusEncoderParam();
};

class SLIB_EXPORT AudioOpusEncoder : public AudioEncoder
{
public:
	AudioOpusEncoder();
	
public:
	static Ref<AudioOpusEncoder> create(const AudioOpusEncoderParam& param);
	static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
};

struct SLIB_EXPORT AudioOpusDecoderParam
{
	sl_uint32 samplesPerSecond;
	sl_uint32 channelsCount;

	AudioOpusDecoderParam();
};

class SLIB_EXPORT AudioOpusDecoder : public AudioDecoder
{
public:
	AudioOpusDecoder();
	
public:
	static Ref<AudioOpusDecoder> create(const AudioOpusDecoderParam& param);
	static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
};
SLIB_MEDIA_NAMESPACE_END

#endif
