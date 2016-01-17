#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_VPX
#define CHECKHEADER_SLIB_MEDIA_CODEC_VPX

#include "definition.h"

#include "video_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

enum VPXBitrateMode {
	vpxBitrateMode_Variable,
	vpxBitrateMode_Constant,
	vpxBitrateMode_ConstrainedQuality,
	vpxBitrateMode_ConstantQuality
};

class SLIB_EXPORT VP8EncoderParam
{
public:
	sl_uint32 width;
	sl_uint32 height;
	sl_uint32 framesPerSecond;
	sl_uint32 keyFrameInterval;
	sl_uint32 bitrate;
	sl_uint32 threadsCount;
	sl_uint32 cpuUsage;
	VPXBitrateMode bitrateMode;

public:
	VP8EncoderParam();
};

class SLIB_EXPORT VP8DecoderParam
{
public:
	sl_uint32 width;
	sl_uint32 height;
	
public:
	VP8DecoderParam();
};

class SLIB_EXPORT VP8Encoder : public VideoEncoder
{
public:
	static Ref<VP8Encoder> create(const VP8EncoderParam& param);
};

class SLIB_EXPORT VP8Decoder : public VideoDecoder
{
public:
	static Ref<VP8Decoder> create(const VP8DecoderParam& param);
};

SLIB_MEDIA_NAMESPACE_END

#endif
