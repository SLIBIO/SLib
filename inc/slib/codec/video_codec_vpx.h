
#ifndef CHECKHEADER_SLIB_CODEC_LIBVPX
#define CHECKHEADER_SLIB_CODEC_LIBVPX

#include "definition.h"
#include "object.h"

SLIB_CODEC_NAMESPACE_BEGIN

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

	VideoVpxEncoderParam()
	{
		type = VP8;
		mode = VPX_CBR;
		nWidth = nHeight = 192;
		nFramePerSecond = 25;
		nBitrate = 150;
		nKeyFrameInterval = 5;
		nCpuUsage = 3;
		nThreadCount = 1;
	}
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
	VideoVpxDecoderParam()
	{
		type = VP8;
		nWidth = nHeight = 192;
	}
};

class SLIB_EXPORT VideoVpxEncoder : public VideoEncoder
{
public:
	VideoVpxEncoder()
	{
	}

public:
	static Ref<VideoVpxEncoder> create(const VideoVpxEncoderParam& param);
};

class SLIB_EXPORT VideoVpxDecoder : public VideoDecoder
{
public:
	VideoVpxDecoder()
	{
	}

public:
	static Ref<VideoVpxDecoder> create(const VideoVpxDecoderParam& param);
};
SLIB_CODEC_NAMESPACE_END

#endif
