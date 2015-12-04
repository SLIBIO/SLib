#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_VPX
#define CHECKHEADER_SLIB_MEDIA_CODEC_VPX

#include "definition.h"

#include "video_codec.h"

SLIB_MEDIA_NAMESPACE_BEGIN

struct SLIB_EXPORT VideoVpxEncoderParam
{
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
	sl_uint32 nWidth;
	sl_uint32 nHeight;
	sl_uint32 nFramePerSecond;
	sl_uint32 nKeyFrameInterval;
	sl_uint32 nBitrate;
	sl_uint32 nThreadCount;
	sl_uint32 nCpuUsage;
	BitRateControlMode mode;
	Type type;

	VideoVpxEncoderParam();
};

struct SLIB_EXPORT VideoVpxDecoderParam
{
	enum Type {
		VP8 = 0,
		VP9 = 1
	};

	Type type;
	sl_uint32 nWidth;
	sl_uint32 nHeight;
	
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
