#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_OPUS
#define CHECKHEADER_SLIB_MEDIA_CODEC_OPUS

#include "definition.h"

#include "audio_codec.h"

/*
	Opus audio codec (RFC 6716)
 ------------------------------------
 
 one frame is (2.5, 5, 10, 20, 40 or 60 ms) of audio data
 
*/

namespace slib
{
	enum class OpusEncoderType
	{
		Voice = 0,
		Music = 1
	};
	
	class SLIB_EXPORT OpusEncoderParam
	{
	public:
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		sl_uint32 bitsPerSecond;
		
		OpusEncoderType type;
		
	public:
		OpusEncoderParam();
		
		~OpusEncoderParam();
		
	};
	
	class SLIB_EXPORT OpusEncoder : public AudioEncoder
	{
	public:
		OpusEncoder();
		
		~OpusEncoder();
		
	public:
		static Ref<OpusEncoder> create(const OpusEncoderParam& param);
		
	public:
		static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
		
	};
	
	class SLIB_EXPORT OpusDecoderParam
	{
	public:
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		
	public:
		OpusDecoderParam();
		
		~OpusDecoderParam();
		
	};
	
	class SLIB_EXPORT OpusDecoder : public AudioDecoder
	{
	public:
		OpusDecoder();
		
		~OpusDecoder();
		
	public:
		static Ref<OpusDecoder> create(const OpusDecoderParam& param);
		
	public:
		static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
		
	};	
}

#endif
