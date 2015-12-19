#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_VPX
#define CHECKHEADER_SLIB_MEDIA_CODEC_VPX

#include "definition.h"

#include "video_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT VideoVpxEncoderParam
{
public:
	enum Type {
		VP8 = 0,
		VP9 = 1
	};

	enum BitRateControlMode {
		VPX_VBR,
		VPX_CBR,
		VPX_CQ,
		VPX_Q,
	};
	sl_uint32 width;
	sl_uint32 height;
	sl_uint32 framesPerSecond;
	sl_uint32 keyFrameInterval;
	sl_uint32 bitrate;
	sl_uint32 threadsCount;
	sl_uint32 cpuUsage;
	BitRateControlMode mode;
	Type type;

public:
	VideoVpxEncoderParam();
};

class SLIB_EXPORT VideoVpxDecoderParam
{
public:
	enum Type {
		VP8 = 0,
		VP9 = 1
	};

	Type type;
	sl_uint32 width;
	sl_uint32 height;
	
public:
	VideoVpxDecoderParam();
};

class SLIB_EXPORT VideoVpxEncoder : public VideoEncoder
{
public:
	VideoVpxEncoder();
	
public:
	static Ref<VideoVpxEncoder> create(const VideoVpxEncoderParam& param);
};

class SLIB_EXPORT VideoVpxDecoder : public VideoDecoder
{
public:
	VideoVpxDecoder();
	
public:
	static Ref<VideoVpxDecoder> create(const VideoVpxDecoderParam& param);
};
SLIB_MEDIA_NAMESPACE_END

#endif
